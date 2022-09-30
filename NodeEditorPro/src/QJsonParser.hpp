#pragma once
#include <QColor>
#include <QJsonObject>
#include <QJsonArray>
#include <QVector>
#include "halconcpp/HalconCpp.h"
#include "NodeStyle.hpp"
#include "FlowViewStyle.hpp"
#include "ConnectionStyle.hpp"
#include "ShapeDrawView.hpp"

namespace QJsonParser
{

	QJsonObject readJsonObj(const QString& fileName);
	bool writeJsonObj(QString const& fileName, QJsonObject& json);

	QJsonObject convertToJson(const QPoint& _point);
	void convertFromJson(QJsonObject const& _obj, QPoint& _point);

	QJsonObject convertToJson(const QPointF& _point);
	void convertFromJson(QJsonObject const& _obj, QPointF& _point);

	QJsonObject convertToJson(const QColor& _color);
	void convertFromJson(QJsonObject const& _obj, QColor& _color);

	QJsonObject convertToJson(const QPolygonF& _obj);
	void convertFromJson(QJsonObject const& _json, QPolygonF& _obj);

	QJsonObject convertToJson(const QtNodes::NodeStyle& _obj);
	void convertFromJson(QJsonObject const& _json, QtNodes::NodeStyle& _obj);

	QJsonObject convertToJson(const QtNodes::FlowViewStyle& _obj);
	void convertFromJson(QJsonObject const& _json, QtNodes::FlowViewStyle& _obj);

	QJsonObject convertToJson(const QtNodes::ConnectionStyle& _obj);
	void convertFromJson(QJsonObject const& _json, QtNodes::ConnectionStyle& _obj);

	QJsonObject convertToJson(const RegionPixmapData& _obj);
	void convertFromJson(QJsonObject const& _json, RegionPixmapData& _obj);

	template<typename DataType>
	inline QJsonObject convertToJson(std::vector<DataType> const& obj)
	{
		QJsonObject result;
		QJsonArray arr;
		for (auto& elem : obj)
		{
			arr.append(convertToJson(elem));
		}
		result.insert("data", arr);
		return result;
	}

	template<typename DataType>
	inline void convertFromJson(QJsonObject const& json, std::vector<DataType>& obj)
	{
		auto jarr = json["data"].toArray();
		for (auto elem : jarr)
		{
			DataType tmp_data;
			QJsonParser::convertFromJson(elem.toObject(), tmp_data);
			obj.push_back(tmp_data);
		}
	}
};

