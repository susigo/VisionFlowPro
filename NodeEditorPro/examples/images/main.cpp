
#include <QtWidgets/QApplication>
#include "VisionFlowWidget.hpp"

#include"ShapeDrawView.hpp"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	app.setWindowIcon(QIcon(":/logo.png"));
	shapeDrawer = new ShapeDrawView();
	VisionFlowWidget* mainWidget = new VisionFlowWidget();
	mainWidget->show();

	return app.exec();
}

