﻿#include "HRegionConnectModel.hpp"
#include <QtCore/QEvent>

#include "halconcpp/HalconCpp.h"

using namespace HalconCpp;

HRegionConnectModel::HRegionConnectModel()
{
	m_InRegion = std::make_shared<HRegionData>();
	m_result = std::make_shared<HRegionData>();

}

bool HRegionConnectModel::RunTask()
{
	PortIndex const outPortIndex = 0;
	if (m_InRegion->hRegion() == nullptr)
	{
		return false;
	}
	try
	{
		m_result->setHRegion(m_InRegion->hRegion()->Connection());

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

bool HRegionConnectModel::eventFilter(QObject* watched, QEvent* event)
{

	return false;
}

unsigned int HRegionConnectModel::
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

NodeValidationState HRegionConnectModel::validationState() const
{
	return modelValidationState;
}

QString HRegionConnectModel::validationMessage() const
{
	return modelValidationError;
}

NodeDataType
HRegionConnectModel::dataType(PortType, PortIndex) const
{
	return HImageData().type();
}

void HRegionConnectModel::
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
HRegionConnectModel::
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
