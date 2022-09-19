#include "HImageShowModel.hpp"
#include <QtCore/QEvent>
#include <QtCore/QDir>
#include <QtWidgets/QFileDialog>

HImageShowModel::HImageShowModel() :
	m_label(new QLabel(u8"点击加载图片"))
{
	m_image_view = new HImageViewWidget();
	m_image_view->installEventFilter(this);
	m_image_view->resize(200, 200);
}

bool HImageShowModel::RunTask()
{
	PortIndex const outPortIndex = 0;
	auto img1 = m_hImage.lock();
	try
	{
		if (img1)
		{
			m_image_view->showImage(img1->hImage());
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
	//m_hwindow->DispImage(m_hImage);
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
		m_hImage = hImageData;
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
	return std::static_pointer_cast<NodeData>(m_result);
}
