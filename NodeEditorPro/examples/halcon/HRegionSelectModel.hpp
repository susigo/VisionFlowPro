#pragma once

#include <iostream>
#include <QObject>
#include <QWidget>
#include <QSlider>
#include <QComboBox>
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
class HRegionSelectModel :public NodeDataModel
{
	Q_OBJECT
public:
	HRegionSelectModel();
	virtual ~HRegionSelectModel() {}

public:
	QString caption() const override
	{
		return QString(u8"选择区域");
	}
	QString name() const override
	{
		return QString(u8"选择区域");
	}
	virtual QString modelName() const
	{
		return QString(u8"选择区域");
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
	QString modelValidationError = QString(u8"区域输入未连接!");
private:
	std::shared_ptr<HRegionData> m_InRegion;
	std::shared_ptr<HRegionData> m_result;
	QComboBox* combo_feature;
	QComboBox* combo_operation;
	QLineEdit* m_minValue;
	QLineEdit* m_maxValue;
	QWidget* m_widget;
};
