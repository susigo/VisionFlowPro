#include "HImageReduceDomainModel.hpp"

#include <QApplication>
#include <QtCore/QEvent>
#include "DrawShapeView.hpp"
#include "halconcpp/HalconCpp.h"
#include "QJsonParser.hpp"
using namespace HalconCpp;


HImageReduceDomainModel::HImageReduceDomainModel()
{
	m_hImage = std::make_shared<HImageData>();
	m_result = std::make_shared<HImageData>();
	btn_drawReg = new QPushButton(QStringLiteral("绘制区域"));
	m_region_data = std::make_shared<ShapeDataStruct>();
	m_domain.GenEmptyRegion();

	connect(DrawShapeView::getInst(), SIGNAL(RegionComform(ShapeDataStruct)),
		this, SLOT(OnNewRegionData(ShapeDataStruct)));

	connect(btn_drawReg, &QPushButton::clicked, [=]()
		{
			QPixmap tmpPix;
			HImageViewWidget::HImageToQPixmap(*m_hImage->hImage(), tmpPix);
			DrawShapeView::getInst()->FitShowImage(tmpPix, *m_region_data);
		});
}

bool HImageReduceDomainModel::RunTask()
{
	//Q_EMIT computingStarted();
	PortIndex const outPortIndex = 0;
	try
	{
		if ((int)m_region_data->shapePolygon.size() > 0)
		{
			HImage tmpImage;
			HalconCpp::ReduceDomain(*m_hImage->hImage(), m_domain, &tmpImage);
			m_result->setHImage(tmpImage);
		}
		else
		{
			m_result->setHImage(*m_hImage->hImage());
		}
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

void HImageReduceDomainModel::OnNewRegionData(ShapeDataStruct _data)
{
	*m_region_data = std::move(_data);
	m_domain = DrawShapeView::GetHRegionFromData(*m_region_data);

	RunTask();
}

unsigned int HImageReduceDomainModel::
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

NodeValidationState HImageReduceDomainModel::validationState() const
{
	return modelValidationState;
}

QString HImageReduceDomainModel::validationMessage() const
{
	return modelValidationError;
}

QJsonObject HImageReduceDomainModel::save() const
{
	QJsonObject result = NodeDataModel::save();
	result.insert("m_region_data", QJsonConvert::convertToJson(*m_region_data));
	return result;
}

void HImageReduceDomainModel::restore(QJsonObject const& _json)
{
	QJsonConvert::convertFromJson(_json.value("m_region_data").toObject(), *m_region_data);
	m_domain = DrawShapeView::GetHRegionFromData(*m_region_data);
}

NodeDataType
HImageReduceDomainModel::dataType(PortType, PortIndex) const
{
	return HImageData().type();
}

void HImageReduceDomainModel::
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
HImageReduceDomainModel::
outData(PortIndex)
{
	return std::dynamic_pointer_cast<HImageData>(m_result);
}
