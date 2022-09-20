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

