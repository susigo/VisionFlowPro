#pragma once

#include <iostream>
#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "DataModelRegistry.hpp"
#include "NodeDataModel.hpp"
#include "halconcpp/HalconCpp.h"
#include "HImageData.hpp"
#include "HRegionData.hpp"
#include "HImageViewWidget.hpp"
#include "DrawShapeView.hpp"

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
class HImageDLSegmentModel :public NodeDataModel
{
	Q_OBJECT
public:
	HImageDLSegmentModel();
	virtual ~HImageDLSegmentModel() {}

public:
	QString caption() const override
	{
		return QStringLiteral("语义分割");
	}
	QString name() const override
	{
		return QStringLiteral("语义分割");
	}
	virtual QString modelName() const
	{
		return QStringLiteral("语义分割");
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
		embeddedWidget() override { return btn_select_model; }

	bool
		resizable() const override { return false; }
	NodeValidationState
		validationState() const override;
	QString
		validationMessage() const override;
	QJsonObject save() const override;
	void restore(QJsonObject const&) override;
	void readDlModel(QString modelFileName);
protected:
	bool RunTask();

public slots:
	void OnNewRegionData(ShapeDataStruct _data);
public:
	NodeValidationState modelValidationState = NodeValidationState::Warning;
	QString modelValidationError = QStringLiteral("图片输入未连接!");
private:
	QString dl_path;
	QPushButton* btn_select_model;
	HDlModel* m_dl_model;
	HTuple image_dimensions;
	HTuple class_ids;
	HTuple valid_thres = 0.7;
	std::shared_ptr<HImageData> m_hImage;
	std::shared_ptr<HRegionData> m_result;
};
