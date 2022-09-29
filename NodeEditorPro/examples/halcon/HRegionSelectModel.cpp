#include "HRegionSelectModel.hpp"
#include <QtCore/QEvent>

#include "halconcpp/HalconCpp.h"

using namespace HalconCpp;

HRegionSelectModel::HRegionSelectModel()
{
	m_widget = new QWidget();
	m_widget->setAttribute(Qt::WA_NoSystemBackground);
	m_widget->setFixedSize(150, 140);
	m_minValue = new QLineEdit(m_widget);
	m_maxValue = new QLineEdit(m_widget);
	combo_feature = new QComboBox(m_widget);
	combo_operation = new QComboBox(m_widget);
	m_minValue->resize(120, 25);
	m_maxValue->resize(120, 25);
	combo_feature->move(0, 0);
	combo_operation->move(0, 35);
	m_minValue->move(0, 70);
	m_maxValue->move(0, 105);

	m_minValue->setText("0");
	m_maxValue->setText("99999");

	combo_feature->addItem("area");
	combo_feature->addItem("row");
	combo_feature->addItem("column");
	combo_feature->addItem("width");
	combo_feature->addItem("height");
	combo_feature->addItem("circularity");
	combo_feature->addItem("compactness");
	combo_feature->addItem("contlength");
	combo_feature->addItem("convexity");
	combo_feature->addItem("rectangularity");
	combo_feature->addItem("inner_width");
	combo_feature->addItem("inner_height");
	combo_feature->addItem("roundness");

	combo_operation->addItem("and");
	combo_operation->addItem("or");

	m_InRegion = std::make_shared<HRegionData>();
	m_result = std::make_shared<HRegionData>();

	connect(m_minValue, &QLineEdit::textEdited, [=]()
		{
			RunTask();
		});
	connect(m_maxValue, &QLineEdit::textEdited, [=]()
		{
			RunTask();
		});
	connect(combo_feature, &QComboBox::currentTextChanged, [=]()
		{
			RunTask();
		});
	connect(combo_operation, &QComboBox::currentTextChanged, [=]()
		{
			RunTask();
		});
}

bool HRegionSelectModel::RunTask()
{
	PortIndex const outPortIndex = 0;
	if (m_InRegion->hRegion() == nullptr)
	{
		return false;
	}
	try
	{
		minVal = m_minValue->text().toDouble();
		maxVal = m_maxValue->text().toDouble();
		m_cur_feature = combo_feature->currentText();
		m_cur_operation = combo_operation->currentText();
		m_result->setHRegion(m_InRegion->hRegion()->SelectShape(
			combo_feature->currentText().toStdString().c_str(),
			combo_operation->currentText().toStdString().c_str(),
			minVal, maxVal
		));
		m_result->setSize(m_InRegion->getSize());

		modelValidationState = NodeValidationState::Valid;
		modelValidationError = QString();
	}
	catch (...)
	{
		modelValidationState = NodeValidationState::Warning;
		modelValidationError = QStringLiteral("缺失或运行失败!");
	}

	Q_EMIT dataUpdated(outPortIndex);

	return true;
}

bool HRegionSelectModel::eventFilter(QObject* watched, QEvent* event)
{

	return false;
}

unsigned int HRegionSelectModel::
nPorts(PortType portType) const
{
	unsigned int result = 1;

	switch (portType)
	{
	case PortType::In:
		result = 1;
		break;

	case PortType::Out:
		result = 1;

	default:
		break;
	}

	return result;
}

NodeValidationState HRegionSelectModel::validationState() const
{
	return modelValidationState;
}

QString HRegionSelectModel::validationMessage() const
{
	return modelValidationError;
}

QJsonObject HRegionSelectModel::save() const
{
	QJsonObject result = NodeDataModel::save();
	result.insert("m_minValue", m_minValue->text());
	result.insert("m_maxValue", m_maxValue->text());
	return result;
}

void HRegionSelectModel::restore(QJsonObject const& p)
{
	m_minValue->setText(p["m_minValue"].toString("0"));
	m_maxValue->setText(p["m_maxValue"].toString("99999"));
}

NodeDataType
HRegionSelectModel::dataType(PortType, PortIndex) const
{
	return HRegionData().type();
}

void HRegionSelectModel::
setInData(std::shared_ptr<NodeData> data, int portIndex)
{
	auto hRegionData =
		std::dynamic_pointer_cast<HRegionData>(data);
	if (hRegionData == nullptr)
	{
		return;
	}
	switch (portIndex)
	{
	case 0:
		m_InRegion->setHRegion(*hRegionData->hRegion());
		m_InRegion->setSize(hRegionData->getSize());
		break;
	default:
		break;
	}
	RunTask();
}

std::shared_ptr<NodeData>
HRegionSelectModel::
outData(PortIndex index)
{
	switch (index)
	{
	case 0:
		return std::dynamic_pointer_cast<HRegionData>(m_result);
		break;
	case 1:
		break;
	default:
		break;
	}
	return std::dynamic_pointer_cast<HRegionData>(m_result);
}
