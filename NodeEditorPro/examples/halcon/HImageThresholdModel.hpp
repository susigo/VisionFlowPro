#pragma once

#include <iostream>
#include <QObject>
#include <QWidget>
#include <QSlider>
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
class HImageThresholdModel :public NodeDataModel
{
	Q_OBJECT
public:
	HImageThresholdModel();
	virtual ~HImageThresholdModel() {}

public:
	QString caption() const override
	{
		return QString(u8"Threshold");
	}
	QString name() const override
	{
		return QString(u8"二值化节点");
	}
	virtual QString modelName() const
	{
		return QString(u8"二值化");
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
		embeddedWidget() override { return m_widget; }

	bool
		resizable() const override { return true; }
	NodeValidationState
		validationState() const override;
	QString
		validationMessage() const override;

	QJsonObject save() const override;

	void restore(QJsonObject const&) override;

protected:
	bool RunTask();
	bool eventFilter(QObject* watched, QEvent* event) override;
public:
	NodeValidationState modelValidationState = NodeValidationState::Warning;
	QString modelValidationError = QString(u8"图片输入未连接!");
private:
	std::shared_ptr<HImageData> m_hImage;
	std::shared_ptr<HImageData> m_result;
	QSlider* m_minGraySlider;
	QSlider* m_maxGraySlider;
	QWidget* m_widget;
};
