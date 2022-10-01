#pragma once
#include <QObject>
#include <QGraphicsItem>
#include <QPixmap>
#include <QGraphicsObject>
#include <QVector>

struct RegionPixmapData
{
public:
	int width;
	int height;
	double w_ratio;
	double h_ratio;
	//绘制
	QVector<QPolygonF> comformPolygon;
	QVector<int> comformOp;
};
