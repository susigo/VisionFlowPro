#include "HRegionSelectShapeStdModel.hpp"
#include <QtCore/QEvent>
#include "halconcpp/HalconCpp.h"

using namespace HalconCpp;

HRegionSelectShapeStdModel::HRegionSelectShapeStdModel()
{
	m_widget = new QWidget();
	m_widget->setAttribute(Qt::WA_NoSystemBackground);
	m_widget->setFixedSize(120, 80);
	m_cur_feature = "rectangle1";
	m_percent = 90;
	combo_feature = new QComboBox(m_widget);
	m_percentEdit = new QLineEdit(m_widget);
	combo_feature->move(0, 10);
	m_percentEdit->move(0, 60);
	combo_feature->addItem("max_area");
	combo_feature->addItem("rectangle1");
	combo_feature->addItem("rectangle2");
	m_percentEdit->setText("90.0");
	m_InRegion = std::make_shared<HRegionData>();
	m_result = std::make_shared<HRegionData>();

	connect(combo_feature, &QComboBox::currentTextChanged, [=]()
		{
			m_cur_feature = combo_feature->currentText();
			RunTask();
		});
	connect(m_percentEdit, &QLineEdit::textEdited, this, [=]()
		{
			m_percent = m_percentEdit->text().toDouble();
			RunTask();
		});
}

bool HRegionSelectShapeStdModel::RunTask()
{
	PortIndex const outPortIndex = 0;
	if (m_InRegion->hRegion() == nullptr)
	{
		return false;
	}
	try
	{
		HalconCpp::SelectShapeStd(
			*m_InRegion->hRegion(),
			m_result->hRegion(),
			m_cur_feature.toStdString().c_str(),
			m_percent);

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

bool HRegionSelectShapeStdModel::eventFilter(QObject* watched, QEvent* event)
{

	return false;
}

unsigned int HRegionSelectShapeStdModel::
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

NodeValidationState HRegionSelectShapeStdModel::validationState() const
{
	return modelValidationState;
}

QString HRegionSelectShapeStdModel::validationMessage() const
{
	return modelValidationError;
}

QJsonObject HRegionSelectShapeStdModel::save() const
{
	QJsonObject result = NodeDataModel::save();
	result.insert("m_cur_feature", combo_feature->currentText());
	result.insert("m_percent", m_percent);
	return result;
}

void HRegionSelectShapeStdModel::restore(QJsonObject const& p)
{
	combo_feature->setCurrentText(p["m_cur_feature"].toString("rectangle1"));
	m_percent = p.value("m_percent").toDouble(90);
}

NodeDataType
HRegionSelectShapeStdModel::dataType(PortType, PortIndex) const
{
	return HRegionData().type();
}

void HRegionSelectShapeStdModel::
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
HRegionSelectShapeStdModel::
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
