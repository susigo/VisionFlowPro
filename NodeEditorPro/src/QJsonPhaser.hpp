#pragma once
#include <QColor>
#include <QJsonObject>
#include "halconcpp/HalconCpp.h"
#include "NodeStyle.hpp"
#include "FlowViewStyle.hpp"
#include "ConnectionStyle.hpp"

namespace QJsonPhaser
{

	QJsonObject readJsonObj(const QString& fileName);
	bool writeJsonObj(const QString& fileName, const QJsonObject& json);

	QJsonObject convertToJson(const QPoint& _point);
	void convertFromJson(const QJsonObject& _obj, QPoint& _point);

	QJsonObject convertToJson(const QPointF& _point);
	void convertFromJson(const QJsonObject& _obj, QPointF& _point);

	QJsonObject convertToJson(const QColor& _color);
	void convertFromJson(const QJsonObject& _obj, QColor& _color);

	QJsonObject convertToJson(const QtNodes::NodeStyle& _obj);
	void convertFromJson(const QJsonObject& _json, QtNodes::NodeStyle& _obj);

	QJsonObject convertToJson(const QtNodes::FlowViewStyle& _obj);
	void convertFromJson(const QJsonObject& _json, QtNodes::FlowViewStyle& _obj);

	QJsonObject convertToJson(const QtNodes::ConnectionStyle& _obj);
	void convertFromJson(const QJsonObject& _json, QtNodes::ConnectionStyle& _obj);

	template<typename ElemType>
	inline QJsonObject convertToJson(const QVector<ElemType>& obj)
	{
		QJsonObject result;
		QJsonArray arr;

		for (auto& elem : obj)
		{
			arr.append(convertToJson(elem));
		}

		result.insert("vector", arr);

		return result;
	}
	template<typename ElemType>
	inline void convertFromJson(const QJsonObject& json, QVector<ElemType>& obj)
	{
		QJsonArray arr = json.value("vector").toArray();
		obj.clear();

		for (auto& elem : arr)
		{
			ElemType temp_elem;

			convertFromJson(elem.toObject(), temp_elem);
			obj.push_back(std::move(temp_elem));
		}
	}
};
