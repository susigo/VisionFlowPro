#include "HImageSplitChanelModel.hpp"

#include <QtCore/QEvent>

#include "halconcpp/HalconCpp.h"

using namespace HalconCpp;

HImageSplitChanelModel::HImageSplitChanelModel()
{
	m_hImage = std::make_shared<HImageData>();
}

bool HImageSplitChanelModel::RunTask()
{

	auto img1 = m_hImage.lock();
	try
	{
		if (img1)
		{
			HTuple imgChanels = img1->hImage().CountChannels();
			if (imgChanels == 3)
			{
				HImage chanR, chanG, chanB;
				Decompose3(img1->hImage(), &chanR, &chanG, &chanB);
				if (m_resultR == nullptr)
				{
					m_resultR = std::make_shared<HImageData>(chanR);
				}
				if (m_resultG == nullptr)
				{
					m_resultG = std::make_shared<HImageData>(chanG);
				}
				if (m_resultB == nullptr)
				{
					m_resultB = std::make_shared<HImageData>(chanB);
				}
				m_resultR->setHImage(chanR);
				m_resultG->setHImage(chanG);
				m_resultB->setHImage(chanB);
			}
			else if (imgChanels == 1)
			{
				m_resultR = std::shared_ptr<HImageData>(m_hImage);
				m_resultG = std::shared_ptr<HImageData>(m_hImage);
				m_resultB = std::shared_ptr<HImageData>(m_hImage);
			}
			modelValidationState = NodeValidationState::Valid;
			modelValidationError = QString();
		}
	}
	catch (...)
	{
		modelValidationState = NodeValidationState::Warning;
		modelValidationError = QStringLiteral("缺失或运行失败!");
		m_resultR.reset();
		m_resultG.reset();
		m_resultB.reset();
	}

	Q_EMIT dataUpdated((PortIndex)0);
	Q_EMIT dataUpdated((PortIndex)1);
	Q_EMIT dataUpdated((PortIndex)2);

	return true;
}

unsigned int HImageSplitChanelModel::
nPorts(PortType portType) const
{
	unsigned int result = 1;

	switch (portType)
	{
	case PortType::In:
		result = 1;
		break;

	case PortType::Out:
		result = 3;

	default:
		break;
	}

	return result;
}

NodeValidationState HImageSplitChanelModel::validationState() const
{
	return modelValidationState;
}

QString HImageSplitChanelModel::validationMessage() const
{
	return modelValidationError;
}

NodeDataType
HImageSplitChanelModel::dataType(PortType, PortIndex) const
{
	return HImageData().type();
}

void HImageSplitChanelModel::
setInData(std::shared_ptr<NodeData> data, int portIndex)
{
	auto hImageData =
		std::dynamic_pointer_cast<HImageData>(data);

	switch (portIndex)
	{
	case 0:
		m_hImage = hImageData;
		break;
	default:
		break;
	}
	RunTask();
}

std::shared_ptr<NodeData>
HImageSplitChanelModel::
outData(PortIndex index)
{

	if (index == 0)
	{
		return std::static_pointer_cast<NodeData>(m_resultR);
	}
	else if (index == 1)
	{
		return std::static_pointer_cast<NodeData>(m_resultG);
	}
	else
	{
		return std::static_pointer_cast<NodeData>(m_resultB);
	}
}
