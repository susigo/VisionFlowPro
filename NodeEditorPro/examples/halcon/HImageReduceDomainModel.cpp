#include "HImageReduceDomainModel.hpp"
#include <QtCore/QEvent>
#include "RegionDrawer.hpp"
#include "halconcpp/HalconCpp.h"

using namespace HalconCpp;

HImageReduceDomainModel::HImageReduceDomainModel()
{
	m_hImage = std::make_shared<HImageData>();
	m_result = std::make_shared<HImageData>();
	btn_drawReg = new QPushButton(u8"绘制区域");

	connect(btn_drawReg, &QPushButton::clicked, [=]()
		{
			RegionDrawer::GetInst()->ShowHImage(*m_hImage->hImage());
			RegionDrawer::GetInst()->show();
			if (RegionDrawer::GetInst()->viewResult)
			{
				HImageViewWidget::QPixmapToHRegion(RegionDrawer::GetInst()->GetPixmap(), m_domain);
				RunTask();
			}
		});
}

bool HImageReduceDomainModel::RunTask()
{
	Q_EMIT computingStarted();
	PortIndex const outPortIndex = 0;
	try
	{
		if (m_domain.IsInitialized())
		{
			m_result->setHImage(m_hImage->hImage()->ReduceDomain(m_domain));
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
	Q_EMIT computingFinished();
	return true;
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

	return result;
}

void HImageReduceDomainModel::restore(QJsonObject const&)
{
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
	return std::dynamic_pointer_cast<NodeData>(m_result);
}
