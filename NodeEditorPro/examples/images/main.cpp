
#include <QtWidgets/QApplication>
#include "VisionFlowWidget.hpp"
#include "RegionDrawer.hpp"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	app.setWindowIcon(QIcon(":/logo.png"));
	//VisionFlowWidget* mainWidget = new VisionFlowWidget();
	//mainWidget->show();

	RegionDrawer* dView = new RegionDrawer();
	dView->show();
	return app.exec();
}

