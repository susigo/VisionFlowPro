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
#ifdef SHOWHALCON_OBJ
	h_window = new HWindow(0, 0, 512, 512, nullptr, "visible", "");
#endif

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
		result = 2;
		break;

	case PortType::Out:
		result = 2;

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
HImageShowModel::dataType(PortType, PortIndex index) const
{
	switch (index)
	{
	case 0:
		return HImageData().type();
		break;
	case 1:
		return HRegionData().type();
		break;
	}
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
#ifdef SHOWHALCON_OBJ
		h_window->ClearWindow();
		h_window->SetPart(HTuple(0), HTuple(0), m_hImage->hImage()->Height(), m_hImage->hImage()->Width());
		HTuple chanels = m_hImage->hImage()->CountChannels();
		if (chanels == 1)
		{
			h_window->DispImage(*m_hImage->hImage());
		}
		else
		{
			h_window->DispColor(*m_hImage->hImage());
		}
#endif
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
outData(PortIndex index)
{
	switch (index)
	{
	case 0:
		return std::dynamic_pointer_cast<HImageData>(m_hImage);
		break;
	case 1:
		return std::dynamic_pointer_cast<HRegionData>(m_hRegion);
		break;
	}
	return std::dynamic_pointer_cast<HImageData>(m_hImage);
}
