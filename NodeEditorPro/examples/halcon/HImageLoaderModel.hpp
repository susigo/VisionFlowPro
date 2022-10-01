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
 * \brief halcon 图像输入节点
 */
class HImageLoaderModel :public NodeDataModel
{
	Q_OBJECT
public:
	HImageLoaderModel();
	virtual ~HImageLoaderModel() {}
public:
	QString caption() const override
	{
		return QStringLiteral("图像输入");
	}
	QString name() const override
	{
		return QStringLiteral("图像输入");
	}
	virtual QString modelName() const
	{
		return QStringLiteral("图像输入");
	}
	unsigned int
		nPorts(PortType portType) const override;

	NodeDataType
		dataType(PortType portType, PortIndex portIndex) const override;

	std::shared_ptr<NodeData>
		outData(PortIndex port) override;

	void
		setInData(std::shared_ptr<NodeData>, int) override
	{ }

	QWidget*
		embeddedWidget() override { return m_image_view; }

	bool
		resizable() const override { return false; }
	QJsonObject save() const override;
	void restore(QJsonObject const&) override;
	void loadImage(QString fileName);
protected:
	bool eventFilter(QObject* watched, QEvent* event) override;
private:
	QString imageName;
	std::shared_ptr< HImageData> m_hImageData;
	HImageViewWidget* m_image_view;

};
