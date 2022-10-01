#pragma once

#include <iostream>
#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include "DataModelRegistry.hpp"
#include "NodeDataModel.hpp"
#include "halconcpp/HalconCpp.h"
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
class HRegionFillUpShapeModel :public NodeDataModel
{
	Q_OBJECT
public:
	HRegionFillUpShapeModel();
	virtual ~HRegionFillUpShapeModel() {}

public:
	QString caption() const override
	{
		return QStringLiteral("填充区域");
	}
	QString name() const override
	{
		return QStringLiteral("填充区域");
	}
	virtual QString modelName() const
	{
		return QStringLiteral("填充区域");
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
		resizable() const override { return false; }
	NodeValidationState
		validationState() const override;
	QString
		validationMessage() const override;
	QJsonObject save() const override;

	void restore(QJsonObject const&) override;
protected:
	bool RunTask();

public:
	NodeValidationState modelValidationState = NodeValidationState::Warning;
	QString modelValidationError = QStringLiteral("图片输入未连接!");
private:
	QString m_feature;
	double m_minval = 0.0;
	double m_maxval = 100.0;
	std::shared_ptr<HRegionData> m_hRegion;
	std::shared_ptr<HRegionData> m_result;
	QWidget* m_widget;
	QVBoxLayout* m_host;
	QComboBox* m_combo_feature;
	QLineEdit* m_minvalEdit;
	QLineEdit* m_maxvalEdit;
};
