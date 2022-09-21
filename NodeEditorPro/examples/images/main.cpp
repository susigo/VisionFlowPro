
#include <QtWidgets/QApplication>
#include "VisionFlowWidget.hpp"

int
main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	app.setWindowIcon(QIcon(":/logo.png"));
	VisionFlowWidget* mainWidget = new VisionFlowWidget();
	mainWidget->show();
	return app.exec();
}

