#pragma once

#include "NodeData.hpp"
#include "FlowScene.hpp"
#include "FlowView.hpp"

#include <QtWidgets/QApplication>
#include <QIcon>
#include <QImage>

#include "ImageShowModel.hpp"
#include "ImageLoaderModel.hpp"

#include "halcon/HalconNodes.hpp"
#include "calculator/MathNodes.hpp"

using QtNodes::DataModelRegistry;
using QtNodes::FlowScene;
using QtNodes::FlowView;

static std::shared_ptr<DataModelRegistry>
registerDataModels()
{
	QString numberNodeType = u8"数学操作";
	QString imageNodeType = u8"图像操作";
	QString getImageNodeType = u8"获取图像";
	QString dlNodeType = u8"深度学习";
	QString matchNodeType = u8"模板匹配";
	auto ret = std::make_shared<DataModelRegistry>();

	//数学节点
	ret->registerModel<AdditionModel>(numberNodeType);
	ret->registerModel<DivisionModel>(numberNodeType);
	ret->registerModel<MultiplicationModel>(numberNodeType);
	ret->registerModel<SubtractionModel>(numberNodeType);
	ret->registerModel<NumberSourceDataModel>(numberNodeType);
	ret->registerModel<NumberDisplayDataModel>(numberNodeType);
	//图像获取
	ret->registerModel<HImageLoaderModel>(getImageNodeType);
	ret->registerModel<HImageFolderModel>(getImageNodeType);
	//图像操作
	ret->registerModel<HImageShowModel>(imageNodeType);
	ret->registerModel<HImageRGB2GrayModel>(imageNodeType);
	ret->registerModel<HImageSplitChanelModel>(imageNodeType);
	ret->registerModel<HImageThresholdModel>(imageNodeType);
	ret->registerModel<HRegionSelectModel>(imageNodeType);
	ret->registerModel<HRegionConnectModel>(imageNodeType);
	ret->registerModel<HRegionShapeTransModel>(imageNodeType);
	ret->registerModel<HImageReduceDomainModel>(imageNodeType);
	ret->registerModel<HRegionFillUpShapeModel>(imageNodeType);
	ret->registerModel<HRegionOpenCircleModel>(imageNodeType);
	ret->registerModel<HRegionUnionModel>(imageNodeType);
	ret->registerModel<HRegionDifferenceModel>(imageNodeType);
	ret->registerModel<HRegionSelectShapeStdModel>(imageNodeType);

	return ret;
}

class VisionFlowWidget :public QWidget
{
public:
	VisionFlowWidget(QWidget* parent = Q_NULLPTR);
	virtual ~VisionFlowWidget();
private:
	QVBoxLayout* main_layout;
	QHBoxLayout* header_layout;
	QPushButton* btn_load_scheme;
	QPushButton* btn_save_scheme;
	QPushButton* btn_clear_scene;
	FlowScene* m_scene;
	FlowView* m_view;

private:
	void setConnection();
};
