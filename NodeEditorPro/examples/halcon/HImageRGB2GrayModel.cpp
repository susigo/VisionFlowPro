#include "HImageRGB2GrayModel.hpp"
#include <QtCore/QEvent>

#include "halconcpp/HalconCpp.h"

using namespace HalconCpp;

HImageRGB2GrayModel::HImageRGB2GrayModel()
{

}

bool HImageRGB2GrayModel::RunTask()
{
	PortIndex const outPortIndex = 0;
	auto img1 = m_hImage.lock();
	try
	{
		if (img1)
		{

			HTuple imgChanels = img1->hImage().CountChannels();
			if (imgChanels == 3)
			{
				HImage tmp_img = img1->hImage().Rgb3ToGray(img1->hImage(), img1->hImage());
				img1->setHImage(tmp_img);
				tmp_img.Clear();

			}
			modelValidationState = NodeValidationState::Valid;
			modelValidationError = QString();
			m_result = std::shared_ptr<HImageData>(m_hImage);
		}
	}
	catch (...)
	{
		modelValidationState = NodeValidationState::Warning;
		modelValidationError = QStringLiteral("缺失或运行失败!");
		m_result.reset();

	}

	Q_EMIT dataUpdated(outPortIndex);

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
HImageRGB2GrayModel::
outData(PortIndex)
{
	return std::static_pointer_cast<NodeData>(m_result);
}
