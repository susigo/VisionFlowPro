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
public:
	RegionPixmapData()
	{
		width = 512;
		height = 512;
		w_ratio = 1.0;
		h_ratio = 1.0;
	}
	~RegionPixmapData()
	{

	}
};
