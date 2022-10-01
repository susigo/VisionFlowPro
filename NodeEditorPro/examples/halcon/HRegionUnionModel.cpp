#include "HRegionUnionModel.hpp"
#include <QtCore/QEvent>

#include "halconcpp/HalconCpp.h"

using namespace HalconCpp;

HRegionUnionModel::HRegionUnionModel()
{
	m_InRegion = std::make_shared<HRegionData>();
	m_result = std::make_shared<HRegionData>();

}

QString HRegionUnionModel::portCaption(PortType port, PortIndex port_index) const
{
	if (port == PortType::In)
	{
		switch (port_index)
		{
		case 0:
			return "Ori";
			break;
		case 1:
			return "Add";
			break;
		}
	}
	else if (port == PortType::Out)
	{
		switch (port_index)
		{
		case 0:
			return "Res";
			break;
		}
	}

	return NodeDataModel::portCaption(port, port_index);
}

bool HRegionUnionModel::RunTask()
{
	PortIndex const outPortIndex = 0;
	if (m_InRegion->hRegion() == nullptr)
	{
		return false;
	}
	try
	{
		HalconCpp::Union2(*m_InRegion->hRegion(), *m_InRegionAdd->hRegion(), m_result->hRegion());

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

bool HRegionUnionModel::eventFilter(QObject* watched, QEvent* event)
{

	return false;
}

unsigned int HRegionUnionModel::
nPorts(PortType portType) const
{
	unsigned int result = 1;

	switch (portType)
	{
	case PortType::In:
		result = 2;
		break;

	case PortType::Out:
		result = 1;

	default:
		break;
	}

	return result;
}

NodeValidationState HRegionUnionModel::validationState() const
{
	return modelValidationState;
}

QString HRegionUnionModel::validationMessage() const
{
	return modelValidationError;
}

NodeDataType
HRegionUnionModel::dataType(PortType, PortIndex) const
{
	return HRegionData().type();
}

void HRegionUnionModel::
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
	case 1:
		m_InRegionAdd->setHRegion(*hRegionData->hRegion());
		m_InRegionAdd->setSize(hRegionData->getSize());
		break;
	default:
		break;
	}
	RunTask();
}

std::shared_ptr<NodeData>
HRegionUnionModel::
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
