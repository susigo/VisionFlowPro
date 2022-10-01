#include "NodeStyle.hpp"

#include <iostream>

#include <QtCore/QFile>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValueRef>
#include <QtCore/QJsonArray>
#include <QDebug>
#include "StyleCollection.hpp"
#include "QJsonParser.hpp"

using QtNodes::NodeStyle;

inline void initResources() { Q_INIT_RESOURCE(resources); }

NodeStyle::
NodeStyle() :
	NormalBoundaryColor(255, 255, 255, 255),
	SelectedBoundaryColor(255, 165, 0, 255),
	BackgroundColor(34, 34, 34, 255),
	TitleColor(253, 204, 82, 255),
	GradientColor0(60, 60, 60, 255),
	GradientColor1(80, 80, 80, 255),
	GradientColor2(64, 64, 64, 255),
	GradientColor3(58, 58, 58, 255),
	ShadowColor(20, 20, 20, 255),
	FontColor(255, 255, 255, 255),
	FontColorFaded(120, 120, 120, 255),
	ConnectionPointColor(169, 169, 169, 255),
	FilledConnectionPointColor(0, 255, 255, 255),
	WarningColor(128, 128, 0, 255),
	ErrorColor(255, 0, 0, 255),
	PenWidth(1.0),
	HoveredPenWidth(1.5),
	ConnectionPointDiameter(8.0),
	Opacity(0.8)
{
	// This configuration is stored inside the compiled unit and is loaded statically
	loadJsonFile("DefaultStyle.json");
}

NodeStyle::
NodeStyle(QString fileName)
{
	loadJsonFile(fileName);
}

void
NodeStyle::
setNodeStyle(QString fileName)
{
	NodeStyle style(fileName);
	StyleCollection::setNodeStyle(style);
}

void
NodeStyle::
loadJsonFile(QString styleFile)
{
	QJsonObject obj = QJsonConvert::readJsonObj(styleFile);
	QJsonObject styleObj = obj["NodeStyle"].toObject();
	if (styleObj.isEmpty())
	{
		saveJsonFile(styleFile);
	}
	else
	{
		QJsonConvert::convertFromJson(obj["NodeStyle"].toObject(), *this);
	}
}

void QtNodes::NodeStyle::saveJsonFile(QString fileName)
{
	QJsonObject obj = QJsonConvert::readJsonObj(fileName);
	obj.insert("NodeStyle", QJsonConvert::convertToJson(*this));
	QJsonConvert::writeJsonObj(fileName, obj);
}
