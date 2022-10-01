#pragma once

#include <iostream>
#include <QObject>
#include <QWidget>
#include <QSlider>
#include <QLineEdit>
#include "DataModelRegistry.hpp"
#include "NodeDataModel.hpp"
#include "halconcpp/HalconCpp.h"
#include "HImageData.hpp"
#include "HRegionData.hpp"
#include "HImageViewWidget.hpp"

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;
using namespace HalconCpp;
/**
 * \brief halcon 图像rgb2gray节点
 */
class HRegionConnectModel :public NodeDataModel
{
	Q_OBJECT
public:
	HRegionConnectModel();
	virtual ~HRegionConnectModel() {}

public:
	QString caption() const override
	{
		return QStringLiteral("非联通区域");
	}
	QString name() const override
	{
		return QStringLiteral("非联通区域");
	}
	virtual QString modelName() const
	{
		return QStringLiteral("非联通区域");
	}
	unsigned int
		nPorts(PortType portType) const override;

	NodeDataType
		dataType(PortType portType, PortIndex portIndex) const override;

	std::shared_ptr<NodeData>
		outData(PortIndex port) override;

	void
		setInData(std::shared_ptr<NodeData>, int) override;

	QWidget*
		embeddedWidget() override { return nullptr; }

	bool
		resizable() const override { return false; }
	NodeValidationState
		validationState() const override;
	QString
		validationMessage() const override;

protected:
	bool RunTask();
	bool eventFilter(QObject* watched, QEvent* event) override;
public:
	NodeValidationState modelValidationState = NodeValidationState::Warning;
	QString modelValidationError = QStringLiteral("区域输入未连接!");
private:
	std::shared_ptr<HRegionData> m_InRegion;
	std::shared_ptr<HRegionData> m_result;
};
