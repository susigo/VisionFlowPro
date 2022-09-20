#pragma once
#include <QColor>
#include <QJsonObject>
#include "halconcpp/HalconCpp.h"

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


	template<typename ElemType>
	inline QJsonObject converToJson(const QVector<ElemType>& obj)
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
