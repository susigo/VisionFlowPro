#include "HImageRGB2GrayModel.hpp"
#include <QtCore/QEvent>

#include "halconcpp/HalconCpp.h"

using namespace HalconCpp;

HImageRGB2GrayModel::HImageRGB2GrayModel()
{
	m_hImage = std::make_shared<HImageData>();
}

bool HImageRGB2GrayModel::RunTask()
{
	Q_EMIT computingStarted();
	PortIndex const outPortIndex = 0;
	try
	{
		HTuple imgChanels = m_hImage->hImage()->CountChannels();
		if (imgChanels == 3)
		{
			HImage tmp_img = m_hImage->hImage()->Rgb3ToGray(*m_hImage->hImage(), *m_hImage->hImage());
			m_hImage->setHImage(tmp_img);
			tmp_img.Clear();
		}
		modelValidationState = NodeValidationState::Valid;
		modelValidationError = QString();
		m_hImage = std::shared_ptr<HImageData>(m_hImage);

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

unsigned int HImageRGB2GrayModel::
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

NodeValidationState HImageRGB2GrayModel::validationState() const
{
	return modelValidationState;
}

QString HImageRGB2GrayModel::validationMessage() const
{
	return modelValidationError;
}

NodeDataType
HImageRGB2GrayModel::dataType(PortType, PortIndex) const
{
	return HImageData().type();
}

void HImageRGB2GrayModel::
setInData(std::shared_ptr<NodeData> data, int portIndex)
{
	auto hImageData =
		std::dynamic_pointer_cast<HImageData>(data);
	if (hImageData == nullptr)
	{
		return;
	}
	switch (portIndex)
	{
	case 0:
		m_hImage->setHImage(*hImageData->hImage());
		break;
	default:
		break;
	}
	RunTask();
}

std::shared_ptr<NodeData>
HImageRGB2GrayModel::
outData(PortIndex)
{
	return std::dynamic_pointer_cast<NodeData>(m_hImage);
}
