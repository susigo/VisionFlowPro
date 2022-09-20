#include "NodeData.hpp"
#include "FlowScene.hpp"
#include "FlowView.hpp"

#include <QtWidgets/QApplication>
#include <QIcon>
#include <QImage>

#include "ImageShowModel.hpp"
#include "ImageLoaderModel.hpp"
#include "calculator/AdditionModel.hpp"
#include "calculator/DivisionModel.hpp"
#include "calculator/MultiplicationModel.hpp"
#include "calculator/SubtractionModel.hpp"
#include "calculator/NumberSourceDataModel.hpp"
#include "calculator/NumberDisplayDataModel.hpp"
#include "halcon/HImageLoaderModel.hpp"
#include "halcon/HImageFolderModel.hpp"
#include "halcon/HImageShowModel.hpp"
#include "halcon/HImageRGB2GrayModel.hpp"
#include "halcon/HImageThresholdModel.hpp"
#include "halcon/HImageSplitChanelModel.hpp"

using QtNodes::DataModelRegistry;
using QtNodes::FlowScene;
using QtNodes::FlowView;

static std::shared_ptr<DataModelRegistry>
registerDataModels()
{
	QString numberNodeType = u8"数学操作";
	QString imageNodeType = u8"图像操作";
	QString getImageNodeType = u8"获取图像";
	auto ret = std::make_shared<DataModelRegistry>();
	ret->registerModel<AdditionModel>(numberNodeType);
	ret->registerModel<DivisionModel>(numberNodeType);
	ret->registerModel<MultiplicationModel>(numberNodeType);
	ret->registerModel<SubtractionModel>(numberNodeType);
	ret->registerModel<NumberSourceDataModel>(numberNodeType);
	ret->registerModel<NumberDisplayDataModel>(numberNodeType);

	ret->registerModel<HImageLoaderModel>(getImageNodeType);
	ret->registerModel<HImageFolderModel>(getImageNodeType);

	ret->registerModel<HImageShowModel>(imageNodeType);
	ret->registerModel<HImageRGB2GrayModel>(imageNodeType);
	ret->registerModel<HImageSplitChanelModel>(imageNodeType);
	ret->registerModel<HImageThresholdModel>(imageNodeType);
	//ret->registerModel<ImageShowModel>(imageNodeType);
	//ret->registerModel<ImageLoaderModel>(imageNodeType);

	return ret;
}

int
main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	app.setWindowIcon(QIcon(":/logo.png"));
	FlowScene scene(registerDataModels());

	FlowView view(&scene);

	view.setWindowTitle(u8"节点编辑器");
	view.resize(1280, 720);
	view.show();

	return app.exec();
}
