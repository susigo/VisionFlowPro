#include "ConnectionStyle.hpp"

#include "StyleCollection.hpp"

#include <QtCore/QFile>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValueRef>
#include <QtCore/QJsonArray>

#include <QDebug>

#include <random>
#include "QJsonParser.hpp"

using QtNodes::ConnectionStyle;

inline void initResources() { Q_INIT_RESOURCE(resources); }

ConnectionStyle::
ConnectionStyle() :
	ConstructionColor(20, 20, 20, 255),
	NormalColor(0, 139, 139, 255),
	SelectedColor(100, 100, 100, 255),
	SelectedHaloColor(255, 165, 0, 255),
	HoveredColor(225, 255, 255, 255),
	LineWidth(3.0),
	ConstructionLineWidth(2.0),
	PointDiameter(10.0),
	UseDataDefinedColors(false)
{
	// This configuration is stored inside the compiled unit and is loaded statically
	loadJsonFile("DefaultStyle.json");
}


ConnectionStyle::
ConnectionStyle(QString fileName)
{
	loadJsonFile(fileName);
}


void
ConnectionStyle::
setConnectionStyle(QString jsonText)
{
	ConnectionStyle style(jsonText);

	StyleCollection::setConnectionStyle(style);
}

void
ConnectionStyle::
loadJsonFile(QString styleFile)
{
	QJsonObject obj = QJsonConvert::readJsonObj(styleFile);
	QJsonObject styleObj = obj["ConnectionStyle"].toObject();
	if (styleObj.isEmpty())
	{
		saveJsonFile(styleFile);
	}
	else
	{
		QJsonConvert::convertFromJson(obj["ConnectionStyle"].toObject(), *this);
	}
}

void ConnectionStyle::saveJsonFile(QString fileName)
{
	QJsonObject obj = QJsonConvert::readJsonObj(fileName);
	obj.insert("ConnectionStyle", QJsonConvert::convertToJson(*this));
	QJsonConvert::writeJsonObj(fileName, obj);
}

QColor
ConnectionStyle::
constructionColor() const
{
	return ConstructionColor;
}


QColor
ConnectionStyle::
normalColor() const
{
	return NormalColor;
}


QColor
ConnectionStyle::
normalColor(QString typeId) const
{
	std::size_t hash = qHash(typeId);

	std::size_t const hue_range = 0xFF;

	std::mt19937 gen(static_cast<unsigned int>(hash));
	std::uniform_int_distribution<int> distrib(0, hue_range);

	int hue = distrib(gen);
	int sat = 120 + hash % 129;

	return QColor::fromHsl(hue,
		sat,
		160);
}


QColor
ConnectionStyle::
selectedColor() const
{
	return SelectedColor;
}


QColor
ConnectionStyle::
selectedHaloColor() const
{
	return SelectedHaloColor;
}


QColor
ConnectionStyle::
hoveredColor() const
{
	return HoveredColor;
}


float
ConnectionStyle::
lineWidth() const
{
	return LineWidth;
}


float
ConnectionStyle::
constructionLineWidth() const
{
	return ConstructionLineWidth;
}


float
ConnectionStyle::
pointDiameter() const
{
	return PointDiameter;
}


bool
ConnectionStyle::
useDataDefinedColors() const
{
	return UseDataDefinedColors;
}
