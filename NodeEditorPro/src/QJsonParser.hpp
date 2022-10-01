﻿#pragma once
#include <QColor>
#include <QJsonObject>
#include <QJsonArray>
#include <QVector>
#include <QString>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QColor>
#include <QPolygonF>
#include "halconcpp/HalconCpp.h"
#include "NodeStyle.hpp"
#include "FlowViewStyle.hpp"
#include "ConnectionStyle.hpp"
#include "ShapeDrawView.hpp"

namespace QJsonParser
{

	template<typename DataType>
	QJsonObject convertToJson(const QVector<DataType>& obj);

	template<typename DataType>
	void convertFromJson(const QJsonObject& json, QVector<DataType>& obj);

	QJsonObject readJsonObj(const QString& fileName);
	bool writeJsonObj(const QString& fileName, QJsonObject& json);

	//QJsonObject convertToJson(const QPoint& _point);
	//void convertFromJson(const QJsonObject& _obj, QPoint& _point);

	QJsonObject convertToJson(const QPointF& _point);
	void convertFromJson(const QJsonObject& _obj, QPointF& _point);

	QJsonObject convertToJson(const QColor& _color);
	void convertFromJson(const QJsonObject& _obj, QColor& _color);

	QJsonObject convertToJson(const QPolygonF& _obj);
	void convertFromJson(const QJsonObject& _json, QPolygonF& _obj);

	QJsonObject convertToJson(const QtNodes::NodeStyle& _obj);
	void convertFromJson(const QJsonObject& _json, QtNodes::NodeStyle& _obj);

	QJsonObject convertToJson(const QtNodes::FlowViewStyle& _obj);
	void convertFromJson(const QJsonObject& _json, QtNodes::FlowViewStyle& _obj);

	QJsonObject convertToJson(const QtNodes::ConnectionStyle& _obj);
	void convertFromJson(const QJsonObject& _json, QtNodes::ConnectionStyle& _obj);

	QJsonObject convertToJson(const RegionPixmapData& _obj);
	void convertFromJson(const QJsonObject& _json, RegionPixmapData& _obj);


};

