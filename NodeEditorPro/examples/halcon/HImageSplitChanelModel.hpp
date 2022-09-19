#pragma once

#include <iostream>
#include <QObject>
#include <QWidget>
#include <QLabel>
#include "DataModelRegistry.hpp"
#include "NodeDataModel.hpp"
#include "halconcpp/HalconCpp.h"
#include "HImageData.hpp"
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
class HImageSplitChanelModel :public NodeDataModel
{
	Q_OBJECT
public:
	HImageSplitChanelModel();
	virtual ~HImageSplitChanelModel() {}

public:
	QString caption() const override
	{
		return QString(u8"SplitChanel");
	}
	QString name() const override
	{
		return QString(u8"图像通道拆分");
	}
	virtual QString modelName() const
	{
		return QString(u8"通道拆分");
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
		embeddedWidget() override { return Q_NULLPTR; }

	bool
		resizable() const override { return true; }
	NodeValidationState
		validationState() const override;
	QString
		validationMessage() const override;
protected:
	bool RunTask();

public:
	NodeValidationState modelValidationState = NodeValidationState::Warning;
	QString modelValidationError = QString(u8"图片输入未连接!");
private:
	std::weak_ptr<HImageData> m_hImage;
	std::shared_ptr<HImageData> m_resultR;
	std::shared_ptr<HImageData> m_resultG;
	std::shared_ptr<HImageData> m_resultB;
};
