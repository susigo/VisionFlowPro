#include "HImageThresholdModel.hpp"
#include <QtCore/QEvent>

#include "halconcpp/HalconCpp.h"

using namespace HalconCpp;

HImageThresholdModel::HImageThresholdModel()
{
	m_widget = new QWidget();
	m_widget->setAttribute(Qt::WA_NoSystemBackground);
	m_widget->setFixedSize(130, 70);
	m_minGraySlider = new QSlider(m_widget);
	m_maxGraySlider = new QSlider(m_widget);
	m_minGraySlider->setOrientation(Qt::Horizontal);
	m_maxGraySlider->setOrientation(Qt::Horizontal);
	m_minGraySlider->resize(120, 25);
	m_maxGraySlider->resize(120, 25);
	m_minGraySlider->move(0, 0);
	m_maxGraySlider->move(0, 35);
	m_minGraySlider->setMinimum(0);
	m_minGraySlider->setMaximum(255);
	m_maxGraySlider->setMinimum(0);
	m_maxGraySlider->setMaximum(255);

	m_maxGraySlider->setValue(255);

	m_hImage = std::make_shared<HImageData>();
	m_result = std::make_shared<HImageData>();

	connect(m_minGraySlider, &QSlider::valueChanged, [=]()
		{
			RunTask();
		});
	connect(m_maxGraySlider, &QSlider::valueChanged, [=]()
		{
			RunTask();
		});
}

bool HImageThresholdModel::RunTask()
{
	PortIndex const outPortIndex = 0;
	if (m_hImage->hImage() == nullptr)
	{
		return false;
	}
	try
	{
		int imgChanels = m_hImage->hImage()->CountChannels();
		HImage tmp_img;
		if (imgChanels == 3)
		{
			tmp_img = m_hImage->hImage()->Rgb3ToGray(*m_hImage->hImage(), *m_hImage->hImage());
		}
		else if (imgChanels == 1)
		{
			tmp_img = *m_hImage->hImage();
		}

		double minVal = m_minGraySlider->value();
		double maxVal = m_maxGraySlider->value();

		HRegion tmpReg = m_hImage->hImage()->Threshold(minVal, maxVal);
		m_result->setHImage(tmpReg.RegionToBin(255, 0, m_hImage->hImage()->Width(), m_hImage->hImage()->Height()));
		tmpReg.Clear();
		tmp_img.Clear();
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

bool HImageThresholdModel::eventFilter(QObject* watched, QEvent* event)
{

	return false;
}

unsigned int HImageThresholdModel::
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

NodeValidationState HImageThresholdModel::validationState() const
{
	return modelValidationState;
}

QString HImageThresholdModel::validationMessage() const
{
	return modelValidationError;
}

NodeDataType
HImageThresholdModel::dataType(PortType, PortIndex) const
{
	return HImageData().type();
}

void HImageThresholdModel::
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
HImageThresholdModel::
outData(PortIndex)
{
	return std::dynamic_pointer_cast<NodeData>(m_result);
}
