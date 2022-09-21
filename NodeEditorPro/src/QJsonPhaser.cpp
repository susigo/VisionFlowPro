#include "QJsonPhaser.hpp"
#include <QString>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>

QJsonObject QJsonPhaser::readJsonObj(const QString& fileName)
{
	QFile file;

	file.setFileName(fileName);

	if (!file.open(QIODevice::Text | QIODevice::ReadOnly))
		return QJsonObject();

	QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
	if (doc.isEmpty() || !doc.isObject())
		return QJsonObject();

	return doc.object();
}

bool QJsonPhaser::writeJsonObj(const QString& fileName, const QJsonObject& json)
{
	QJsonDocument doc;
	QFile file;

	file.setFileName(fileName);

	if (!file.open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Truncate))
		return false;

	doc.setObject(json);
	auto bytes = doc.toJson();

	return (
		!bytes.isEmpty() &&
		file.write(bytes) == bytes.size());
}

QJsonObject QJsonPhaser::convertToJson(const QPoint& _point)
{
	QJsonObject result;
	result.insert("x", _point.x());
	result.insert("y", _point.y());
	return result;
}

void QJsonPhaser::convertFromJson(const QJsonObject& _obj, QPoint& _point)
{
	_point.setX(_obj.value("x").toInt());
	_point.setY(_obj.value("y").toInt());
}

QJsonObject QJsonPhaser::convertToJson(const QPointF& _point)
{
	QJsonObject result;
	result.insert("x", _point.x());
	result.insert("y", _point.y());
	return result;
}

void QJsonPhaser::convertFromJson(const QJsonObject& _obj, QPointF& _point)
{
	_point.setX(_obj.value("x").toDouble());
	_point.setY(_obj.value("y").toDouble());
}

QJsonObject QJsonPhaser::convertToJson(const QColor& _color)
{
	QJsonObject result;
	result.insert("red", _color.red());
	result.insert("green", _color.green());
	result.insert("blue", _color.blue());
	result.insert("alpha", _color.alpha());
	return result;
}

void QJsonPhaser::convertFromJson(const QJsonObject& _obj, QColor& _color)
{
	_color = QColor(
		_obj.value("red").toInt(),
		_obj.value("green").toInt(),
		_obj.value("blue").toInt(),
		_obj.value("alpha").toInt());
}

QJsonObject QJsonPhaser::convertToJson(const QtNodes::NodeStyle& _obj)
{
	QJsonObject result;
	result.insert("NormalBoundaryColor", convertToJson(_obj.NormalBoundaryColor));
	result.insert("SelectedBoundaryColor", convertToJson(_obj.SelectedBoundaryColor));
	result.insert("BackgroundColor", convertToJson(_obj.BackgroundColor));
	result.insert("TitileColor", convertToJson(_obj.TitileColor));
	result.insert("GradientColor0", convertToJson(_obj.GradientColor0));
	result.insert("GradientColor1", convertToJson(_obj.GradientColor1));
	result.insert("GradientColor2", convertToJson(_obj.GradientColor2));
	result.insert("GradientColor3", convertToJson(_obj.GradientColor3));
	result.insert("ShadowColor", convertToJson(_obj.ShadowColor));
	result.insert("FontColorFaded", convertToJson(_obj.FontColorFaded));
	result.insert("ConnectionPointColor", convertToJson(_obj.ConnectionPointColor));
	result.insert("FilledConnectionPointColor", convertToJson(_obj.FilledConnectionPointColor));
	result.insert("WarningColor", convertToJson(_obj.WarningColor));
	result.insert("ErrorColor", convertToJson(_obj.ErrorColor));
	result.insert("PenWidth", _obj.PenWidth);
	result.insert("HoveredPenWidth", _obj.HoveredPenWidth);
	result.insert("ConnectionPointDiameter", _obj.ConnectionPointDiameter);
	result.insert("Opacity", _obj.Opacity);
	return result;
}

void QJsonPhaser::convertFromJson(const QJsonObject& _json, QtNodes::NodeStyle& _obj)
{
	convertFromJson(_json["NormalBoundaryColor"].toObject(), _obj.NormalBoundaryColor);
	convertFromJson(_json["SelectedBoundaryColor"].toObject(), _obj.SelectedBoundaryColor);
	convertFromJson(_json["BackgroundColor"].toObject(), _obj.BackgroundColor);
	convertFromJson(_json["TitileColor"].toObject(), _obj.TitileColor);
	convertFromJson(_json["GradientColor0"].toObject(), _obj.GradientColor0);
	convertFromJson(_json["GradientColor1"].toObject(), _obj.GradientColor1);
	convertFromJson(_json["GradientColor2"].toObject(), _obj.GradientColor2);
	convertFromJson(_json["GradientColor3"].toObject(), _obj.GradientColor3);
	convertFromJson(_json["ShadowColor"].toObject(), _obj.ShadowColor);
	convertFromJson(_json["FontColor"].toObject(), _obj.FontColor);
	convertFromJson(_json["FontColorFaded"].toObject(), _obj.FontColorFaded);
	convertFromJson(_json["ConnectionPointColor"].toObject(), _obj.ConnectionPointColor);
	convertFromJson(_json["FilledConnectionPointColor"].toObject(), _obj.FilledConnectionPointColor);
	convertFromJson(_json["WarningColor"].toObject(), _obj.WarningColor);
	convertFromJson(_json["ErrorColor"].toObject(), _obj.ErrorColor);

	_obj.PenWidth = _json["PenWidth"].toDouble(0);
	_obj.HoveredPenWidth = _json["HoveredPenWidth"].toDouble(0);
	_obj.ConnectionPointDiameter = _json["ConnectionPointDiameter"].toDouble(0);
	_obj.Opacity = _json["Opacity"].toDouble(0);

}

QJsonObject QJsonPhaser::convertToJson(const QtNodes::FlowViewStyle& _obj)
{
	QJsonObject result;
	result.insert("BackgroundColor", convertToJson(_obj.BackgroundColor));
	result.insert("CoarseGridColor", convertToJson(_obj.CoarseGridColor));
	result.insert("FineGridColor", convertToJson(_obj.FineGridColor));
	return result;
}

void QJsonPhaser::convertFromJson(const QJsonObject& _json, QtNodes::FlowViewStyle& _obj)
{
	convertFromJson(_json["BackgroundColor"].toObject(), _obj.BackgroundColor);
	convertFromJson(_json["CoarseGridColor"].toObject(), _obj.CoarseGridColor);
	convertFromJson(_json["FineGridColor"].toObject(), _obj.FineGridColor);
}

QJsonObject QJsonPhaser::convertToJson(const QtNodes::ConnectionStyle& _obj)
{
	QJsonObject result;
	result.insert("BackgroundColor", convertToJson(_obj.ConstructionColor));
	result.insert("NormalColor", convertToJson(_obj.NormalColor));
	result.insert("SelectedColor", convertToJson(_obj.SelectedColor));
	result.insert("SelectedHaloColor", convertToJson(_obj.SelectedHaloColor));
	result.insert("HoveredColor", convertToJson(_obj.HoveredColor));

	result.insert("LineWidth", _obj.LineWidth);
	result.insert("ConstructionLineWidth", _obj.ConstructionLineWidth);
	result.insert("PointDiameter", _obj.PointDiameter);
	result.insert("UseDataDefinedColors", _obj.UseDataDefinedColors);

	return result;
}

void QJsonPhaser::convertFromJson(const QJsonObject& _json, QtNodes::ConnectionStyle& _obj)
{
}

