#ifndef DEFINEDATATYPE_H
#define DEFINEDATATYPE_H

#endif // DEFINEDATATYPE_H
#include <QPoint>
#include <QVector>



//矩形 
struct DefRect
{
	DefRect() :X(0), Y(0), Width(0), Height(0)
	{

	}
	DefRect(float x, float y, float width, float height)
	{
		X = x;
		Y = y;
		Width = width;
		Height = height;
	}
	DefRect(QPointF TopLeftP, float width, float height)
	{
		X = TopLeftP.x();
		Y = TopLeftP.y();
		Width = width;
		Height = height;
	}

	float X;
	float Y;
	float Width;
	float Height;
};

//旋转矩形  
struct DefRotateRect
{
	DefRotateRect() :X(0), Y(0), Width(0), Height(0), ang(0)
	{

	}
	DefRotateRect(float x, float y, float Length1, float Length2, float angDeg)
	{
		X = x;
		Y = y;
		Width = Length1;
		Height = Length2;
		ang = angDeg;
	}
	DefRotateRect(QPointF Center, float Length1, float Length2, float angDeg)
	{
		X = Center.x();
		Y = Center.y();
		Width = Length1;
		Height = Length2;
		ang = angDeg;
	}

	float X;
	float Y;
	float Width;
	float Height;
	float ang;

	QVector<QPointF> CornerP = QVector<QPointF>(4);//四个角点坐标
};

//圆形 
struct DefCircle
{
	DefCircle() :X(0), Y(0), R(0)
	{

	}
	DefCircle(float x, float y, float R)
	{
		X = x;
		Y = y;
		this->R = R;
	}
	DefCircle(QPointF Center, float R)
	{
		X = Center.x();
		Y = Center.y();
		this->R = R;
	}

	float X;
	float Y;
	float R;
};

//旋转椭圆
struct DefRotateEllipse
{
	DefRotateEllipse() :X(0), Y(0), Width(0), Height(0), ang(0)
	{

	}
	DefRotateEllipse(float x, float y, float Length1, float Length2, float angDeg)
	{
		X = x;
		Y = y;
		Width = Length1;
		Height = Length2;
		ang = angDeg;
	}
	DefRotateEllipse(QPointF Center, float Length1, float Length2, float angDeg)
	{
		X = Center.x();
		Y = Center.y();
		Width = Length1;
		Height = Length2;
		ang = angDeg;
	}

	float X;
	float Y;
	float Width;
	float Height;
	float ang;

	QVector<QPointF> CornerP = QVector<QPointF>(4);//四个角点坐标
};

//同心圆 
struct DefConcentricCircle
{
	DefConcentricCircle() :X(0), Y(0), RMax(0), RMin(0)
	{

	}
	DefConcentricCircle(float x, float y, float RMax, float RMin)
	{
		X = x;
		Y = y;
		this->RMax = RMax;
		this->RMin = RMin;
	}
	DefConcentricCircle(QPointF Center, float RMax, float RMin)
	{
		X = Center.x();
		Y = Center.y();
		this->RMax = RMax;
		this->RMin = RMin;
	}
	float X;
	float Y;
	float RMax;
	float RMin;
};

//饼图 
struct DefPie
{
	DefPie() :X(0), Y(0), R(0), AngStart(0), AngEnd(0)
	{

	}
	DefPie(float x, float y, float r, float angStart, float angEnd)
	{
		X = x;
		Y = y;
		R = r;
		AngStart = angStart;
		AngEnd = angEnd;
	}
	DefPie(QPointF Center, float r, float angStart, float angEnd)
	{
		X = Center.x();
		Y = Center.y();
		R = r;
		AngStart = angStart;
		AngEnd = angEnd;
	}

	float X;
	float Y;
	float R;
	float AngStart;
	float AngEnd;
};

//圆角矩形  
struct DefRoundRect
{
	DefRoundRect() :Width(0), Height(0), R(0), X(0), Y(0)
	{}
	DefRoundRect(float x, float y, float width, float height, float r)
	{
		R = r;
		X = x;
		Y = y;
		Width = width;
		Height = height;
	}
	DefRoundRect(QPointF TopLeftP, float width, float height, float r)
	{
		R = r;
		X = TopLeftP.x();
		Y = TopLeftP.y();
		Width = width;
		Height = height;
	}
	float Width;
	float Height;
	float R;
	float X;
	float Y;
};
//直线  
struct DefLine
{
	DefLine() :X1(0), Y1(0), X2(0), Y2(0)
	{}
	DefLine(float x1, float y1, float x2, float y2)
	{
		X1 = x1;
		X2 = x2;
		Y1 = y1;
		Y2 = y2;
	}
	DefLine(QPointF p1, QPointF p2)
	{
		X1 = p1.x();
		X2 = p2.x();
		Y1 = p1.y();
		Y2 = p2.y();
	}
	float X1;
	float Y1;
	float X2;
	float Y2;
};


//卡尺
struct DefCaliper
{
	DefCaliper() :X1(0), Y1(0), X2(0), Y2(0), Width(0)
	{}
	DefCaliper(float x1, float y1, float x2, float y2, float width)
	{
		X1 = x1;
		X2 = x2;
		Y1 = y1;
		Y2 = y2;
		Width = width;
	}
	DefCaliper(QPointF p1, QPointF p2, float width)
	{
		X1 = p1.x();
		X2 = p2.x();
		Y1 = p1.y();
		Y2 = p2.y();
		Width = width;
	}
	float X1;
	float Y1;
	float X2;
	float Y2;
	float Width;

};
//锚点
struct DefPoint
{
	DefPoint() :X1(0), Y1(0)
	{}
	DefPoint(float x1, float y1)
	{
		X1 = x1;
		Y1 = y1;
	}
	DefPoint(QPointF p)
	{
		X1 = p.x();
		Y1 = p.y();
	}
	float X1;
	float Y1;
};
//多边形
struct DefPolygon
{

	DefPolygon() {}
	DefPolygon(QVector<QPointF> pList)
	{
		PList = pList;
	}
	QVector<QPointF> PList;
};


struct DefDispItem
{

};











