#pragma once
#include <QString>

enum class ViewMode
{
	tNone,
	tTranslate,
	tZoom,
	tDrawing
};

enum class EShapeType
{
	sNone,
	sLine,
	sRectangle1Add,
	sRectangle2Add,
	sPolygonAdd,
	sFreeDrawAdd,
	sRectangle1Div,
	sRectangle2Div,
	sPolygonDiv,
	sFreeDrawDiv
};

enum class ShapeMode
{
	mNone,
	mAdd,
	mDiv
};

struct  ShapeDataStruct
{
	QString name;
	QVector<QPolygonF> shapePolygon;
	QVector<EShapeType> shapeType;
	QVector<ShapeMode> shapeMode;
};


