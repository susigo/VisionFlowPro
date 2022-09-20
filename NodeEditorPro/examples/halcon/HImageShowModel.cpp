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
}

bool HImageShowModel::RunTask()
{
	PortIndex const outPortIndex = 0;

	try
	{
		m_image_view->showImage(m_hImage->hImage());
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
	auto hImageData =
		std::dynamic_pointer_cast<HImageData>(data);

	switch (portIndex)
	{
	case 0:
		if (hImageData == nullptr)
		{
			break;
		}
		m_hImage->setHImage(hImageData->hImage());
		break;
	default:
		break;
	}
	RunTask();
}

std::shared_ptr<NodeData>
HImageShowModel::
outData(PortIndex)
{
	return std::dynamic_pointer_cast<NodeData>(m_hImage);
}
