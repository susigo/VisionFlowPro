#include "HImageShowModel.hpp"
#include <QtCore/QEvent>
#include <QtCore/QDir>
#include <QtWidgets/QFileDialog>

HImageShowModel::HImageShowModel()
{
	m_image_view = new HImageViewWidget();
	m_image_view->installEventFilter(this);
	m_image_view->resize(200, 200);
	m_hImage = std::make_shared<HImageData>();
	m_hRegion = std::make_shared<HRegionData>();
}

bool HImageShowModel::RunTask()
{
	PortIndex const outPortIndex = 0;

	try
	{
		m_image_view->showImage(*m_hImage->hImage());
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

unsigned int HImageShowModel::
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

NodeValidationState HImageShowModel::validationState() const
{
	return modelValidationState;
}

QString HImageShowModel::validationMessage() const
{
	return modelValidationError;
}

bool HImageShowModel::eventFilter(QObject* object, QEvent* event)
{

	return false;
}

NodeDataType
HImageShowModel::dataType(PortType, PortIndex) const
{
	return HImageData().type();
}

void HImageShowModel::
setInData(std::shared_ptr<NodeData> data, int portIndex)
{
	if (data == nullptr)
	{
		return;
	}
	if (data->type() == m_hImage->type())
	{
		auto dataPtr = std::dynamic_pointer_cast<HImageData>(data);
		if (!dataPtr->hImage()->IsInitialized())
		{
			return;
		}
		m_hImage->setHImage(*dataPtr->hImage());

	}
	else if (data->type() == m_hRegion->type())
	{
		auto dataPtr = std::dynamic_pointer_cast<HRegionData>(data);
		if (!dataPtr->hRegion()->IsInitialized())
		{
			return;
		}
		m_hRegion->setHRegion(*dataPtr->hRegion());
		m_hRegion->setSize(dataPtr->getSize());
		HImage tmpImg = m_hRegion->hRegion()->RegionToBin(255, 0,
			m_hRegion->getSize().width(), m_hRegion->getSize().height());
		m_hImage->setHImage(tmpImg);
	}
	RunTask();
}

std::shared_ptr<NodeData>
HImageShowModel::
outData(PortIndex)
{
	return std::dynamic_pointer_cast<NodeData>(m_hImage);
}
