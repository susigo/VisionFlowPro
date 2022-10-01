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
 * \brief halcon shapeTrans节点
 */
class HRegionShapeTransModel :public NodeDataModel
{
	Q_OBJECT
public:
	HRegionShapeTransModel();
	virtual ~HRegionShapeTransModel() {}

public:
	QString caption() const override
	{
		return QStringLiteral("仿形变换");
	}
	QString name() const override
	{
		return QStringLiteral("仿形变换");
	}
	virtual QString modelName() const
	{
		return QStringLiteral("仿形变换");
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
	QString modelValidationError = QStringLiteral("区域输入未连接!");
private:
	std::shared_ptr<HRegionData> m_InRegion;
	std::shared_ptr<HRegionData> m_result;
	QComboBox* combo_feature;
	QWidget* m_widget;
	QString m_cur_feature;

};
