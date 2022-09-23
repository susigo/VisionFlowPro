#include "HRegionShapeTransModel.hpp"
#include <QtCore/QEvent>
#include "halconcpp/HalconCpp.h"

using namespace HalconCpp;

HRegionShapeTransModel::HRegionShapeTransModel()
{
	m_widget = new QWidget();
	m_widget->setAttribute(Qt::WA_NoSystemBackground);
	m_widget->setFixedSize(150, 50);
	combo_feature = new QComboBox(m_widget);

	combo_feature->move(0, 10);

	combo_feature->addItem("convex");
	combo_feature->addItem("ellipse");
	combo_feature->addItem("outer_circle");
	combo_feature->addItem("inner_circle");
	combo_feature->addItem("rectangle1");
	combo_feature->addItem("rectangle2");
	combo_feature->addItem("inner_rectangle1");
	combo_feature->addItem("inner_rectangle2");

	m_InRegion = std::make_shared<HRegionData>();
	m_result = std::make_shared<HRegionData>();

	connect(combo_feature, &QComboBox::currentTextChanged, [=]()
		{
			RunTask();
		});
}

bool HRegionShapeTransModel::RunTask()
{
	PortIndex const outPortIndex = 0;
	if (m_InRegion->hRegion() == nullptr)
	{
		return false;
	}
	try
	{
		m_cur_feature = combo_feature->currentText();
		m_result->setHRegion(m_InRegion->hRegion()->ShapeTrans(
			combo_feature->currentText().toStdString().c_str()
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

bool HRegionShapeTransModel::eventFilter(QObject* watched, QEvent* event)
{

	return false;
}

unsigned int HRegionShapeTransModel::
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

NodeValidationState HRegionShapeTransModel::validationState() const
{
	return modelValidationState;
}

QString HRegionShapeTransModel::validationMessage() const
{
	return modelValidationError;
}

QJsonObject HRegionShapeTransModel::save() const
{
	QJsonObject result = NodeDataModel::save();
	result.insert("m_cur_feature", combo_feature->currentText());
	return result;
}

void HRegionShapeTransModel::restore(QJsonObject const& p)
{
	combo_feature->setCurrentText(p["m_cur_feature"].toString("rectangle1"));
}

NodeDataType
HRegionShapeTransModel::dataType(PortType, PortIndex) const
{
	return HImageData().type();
}

void HRegionShapeTransModel::
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
HRegionShapeTransModel::
outData(PortIndex index)
{
	switch (index)
	{
	case 0:
		return std::dynamic_pointer_cast<NodeData>(m_result);
		break;
	case 1:
		break;
	default:
		break;
	}
	return std::dynamic_pointer_cast<NodeData>(m_result);
}
