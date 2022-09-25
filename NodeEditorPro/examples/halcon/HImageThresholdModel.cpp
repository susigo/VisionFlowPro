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
	m_domain = std::make_shared<HRegionData>();
	m_result = std::make_shared<HRegionData>();

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
		if (m_domain->hRegion()->IsInitialized())
		{
			m_hImage->hImage()->ReduceDomain(*m_domain->hRegion());
		}
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

		m_result->setHRegion(m_hImage->hImage()->Threshold(minVal, maxVal));
		m_result->setSize(QSize(m_hImage->hImage()->Width().D(), m_hImage->hImage()->Height().D()));
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
		result = 2;
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

QJsonObject HImageThresholdModel::save() const
{
	QJsonObject result = NodeDataModel::save();
	result.insert("m_minGraySlider", m_minGraySlider->value());
	result.insert("m_maxGraySlider", m_maxGraySlider->value());
	return result;
}

void HImageThresholdModel::restore(QJsonObject const& p)
{
	m_minGraySlider->setValue(p["m_minGraySlider"].toInt(0));
	m_maxGraySlider->setValue(p["m_maxGraySlider"].toInt(255));
}

NodeDataType
HImageThresholdModel::dataType(PortType portType, PortIndex portIndex) const
{
	if (portType == PortType::In)
	{
		switch (portIndex)
		{
		case 0:
			return HImageData().type();
			break;
		case 1:
			return HRegionData().type();
			break;
		}
	}
	else
	{
		switch (portIndex)
		{
		case 0:
			return HRegionData().type();
			break;
		}
	}
	return HImageData().type();
}

void HImageThresholdModel::
setInData(std::shared_ptr<NodeData> data, int portIndex)
{
	if (portIndex == 0)
	{
		auto hImageData =
			std::dynamic_pointer_cast<HImageData>(data);
		if (hImageData == nullptr)
		{
			return;
		}
		m_hImage->setHImage(*hImageData->hImage());
	}
	else if (portIndex == 1)
	{
		auto hImageData =
			std::dynamic_pointer_cast<HRegionData>(data);
		if (hImageData != nullptr)
		{
			m_domain->setHRegion(*hImageData->hRegion());
		}
	}
	RunTask();
}

std::shared_ptr<NodeData>
HImageThresholdModel::
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
