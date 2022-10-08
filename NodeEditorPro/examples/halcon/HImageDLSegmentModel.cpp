#include "HImageDLSegmentModel.hpp"

#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QtCore/QEvent>

#include "HRegionData.hpp"
#include "DrawShapeView.hpp"
#include "halconcpp/HalconCpp.h"
#include "QJsonParser.hpp"
using namespace HalconCpp;


HImageDLSegmentModel::HImageDLSegmentModel()
{
	m_hImage = std::make_shared<HImageData>();
	m_result = std::make_shared<HRegionData>();
	btn_select_model = new QPushButton(QStringLiteral("选择模型"));

	connect(DrawShapeView::getInst(), SIGNAL(RegionFinished(RegionPixmapData)),
		this, SLOT(OnNewRegionData(RegionPixmapData)));

	connect(btn_select_model, &QPushButton::clicked, [=]()
		{
			dl_path.clear();
			dl_path =
				QFileDialog::getOpenFileName(nullptr,
					tr("Select Dl Model"),
					QDir::homePath(),
					tr("File (*.hdl)"));
			if (dl_path == "")
			{
				return;
			}
			readDlModel(dl_path);
		});
}

bool HImageDLSegmentModel::RunTask()
{
	PortIndex const outPortIndex = 0;
	try
	{

		modelValidationState = NodeValidationState::Valid;
		modelValidationError = QString();
	}
	catch (...)
	{
		modelValidationState = NodeValidationState::Warning;
		modelValidationError = QStringLiteral("缺失或运行失败!");
	}

	return true;
}

void HImageDLSegmentModel::OnNewRegionData(ShapeDataStruct _data)
{
	//if (!DrawShapeView::getInst()->getDrawFlag())
	//{
	//	return;
	//}
	RunTask();
}

unsigned int HImageDLSegmentModel::
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

NodeValidationState HImageDLSegmentModel::validationState() const
{
	return modelValidationState;
}

QString HImageDLSegmentModel::validationMessage() const
{
	return modelValidationError;
}

QJsonObject HImageDLSegmentModel::save() const
{
	QJsonObject result = NodeDataModel::save();
	result.insert("dl_path", dl_path);
	return result;
}

void HImageDLSegmentModel::restore(QJsonObject const& _json)
{
	dl_path = _json.value("dl_path").toString();
}

void HImageDLSegmentModel::readDlModel(QString modelFileName)
{
	m_dl_model = new HDlModel();
	m_dl_model->ReadDlModel(modelFileName.toStdString().c_str());
	image_dimensions = m_dl_model->GetDlModelParam("image_dimensions");
	class_ids = m_dl_model->GetDlModelParam("class_ids");
	m_dl_model->SetDlModelParam("batch_size", 1);
}

NodeDataType
HImageDLSegmentModel::dataType(PortType port_type, PortIndex port_index) const
{
	if (port_type == PortType::In)
	{
		switch (port_index)
		{
		case 0:
			return HImageData().type();
			break;
		}
	}
	else
	{
		switch (port_index)
		{
		case 0:
			return HRegionData().type();
			break;
		}
	}
	return HImageData().type();
}

void HImageDLSegmentModel::
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
HImageDLSegmentModel::
outData(PortIndex)
{
	return std::dynamic_pointer_cast<HRegionData>(m_result);
}
