#pragma once

#include <iostream>
#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
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
class HImageFolderModel :public NodeDataModel
{
	Q_OBJECT
public:
	HImageFolderModel();
	virtual ~HImageFolderModel() {}
public:
	QString caption() const override
	{
		return QString(u8"图像目录输入");
	}
	QString name() const override
	{
		return QString(u8"图像目录输入");
	}
	virtual QString modelName() const
	{
		return QString(u8"图像目录输入");
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
		embeddedWidget() override { return m_paraWidget; }

	bool
		resizable() const override { return false; }
	QJsonObject save() const override;
	void restore(QJsonObject const&) override;
	void loadImageFolder(QString path, int index = 0);
protected:
	bool eventFilter(QObject* watched, QEvent* event) override;
private:
	QString folderPath;
	std::shared_ptr< HImageData> m_hImageData;
	QWidget* m_paraWidget;
	QVBoxLayout* m_host;
	QHBoxLayout* m_h_host;
	QPushButton* btn_selectFolder;
	QPushButton* btn_last;
	QPushButton* btn_next;
	HImageViewWidget* m_image_view;
	HTuple fileListStr;
	HTuple imgListStr;
	HImage tmpImg;
	int curIndex = 0;
	int imageCounst = 0;
};
