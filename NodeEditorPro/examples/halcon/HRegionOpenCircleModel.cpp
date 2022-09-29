#include "HRegionOpenCircleModel.hpp"
#include <QtCore/QEvent>

#include "halconcpp/HalconCpp.h"

using namespace HalconCpp;

HRegionOpenCircleModel::HRegionOpenCircleModel()
{
	m_hRegion = std::make_shared<HRegionData>();
	m_result = std::make_shared<HRegionData>();

	m_maxvalEdit = new QLineEdit();
	m_maxvalEdit->setFixedWidth(80);
	m_maxvalEdit->setText("9.0");

	connect(m_maxvalEdit, &QLineEdit::textChanged, [=]()
		{
			m_maxval = m_maxvalEdit->text().toDouble();
			RunTask();
		});
}

bool HRegionOpenCircleModel::RunTask()
{
	Q_EMIT computingStarted();
	PortIndex const outPortIndex = 0;
	try
	{
		HalconCpp::OpeningCircle(
			*m_hRegion->hRegion(),
			m_result->hRegion(),
			m_maxval);
		modelValidationState = NodeValidationState::Valid;
		modelValidationError = QString();
	}
	catch (...)
	{
		modelValidationState = NodeValidationState::Warning;
		modelValidationError = QStringLiteral("缺失或运行失败!");
	}

	Q_EMIT dataUpdated(outPortIndex);
	Q_EMIT computingFinished();
	return true;
}

unsigned int HRegionOpenCircleModel::
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

NodeValidationState HRegionOpenCircleModel::validationState() const
{
	return modelValidationState;
}

QString HRegionOpenCircleModel::validationMessage() const
{
	return modelValidationError;
}

QJsonObject HRegionOpenCircleModel::save() const
{
	QJsonObject result = NodeDataModel::save();

	result.insert("m_maxval", m_maxval);
	return result;
}

void HRegionOpenCircleModel::restore(QJsonObject const& json_values)
{
	NodeDataModel::restore(json_values);
	m_maxval = json_values.value("m_maxval").toDouble();
	m_maxvalEdit->setText(QString::number(m_maxval));
}

NodeDataType
HRegionOpenCircleModel::dataType(PortType, PortIndex) const
{
	return HRegionData().type();
}

void HRegionOpenCircleModel::
setInData(std::shared_ptr<NodeData> data, int portIndex)
{
	auto hImageData =
		std::dynamic_pointer_cast<HRegionData>(data);
	if (hImageData == nullptr)
	{
		return;
	}
	switch (portIndex)
	{
	case 0:
		m_hRegion->setHRegion(*hImageData->hRegion());
		m_result->setSize(hImageData->getSize());
		break;
	default:
		break;
	}
	RunTask();
}

std::shared_ptr<NodeData>
HRegionOpenCircleModel::
outData(PortIndex)
{
	return std::static_pointer_cast<HRegionData>(m_result);
}
