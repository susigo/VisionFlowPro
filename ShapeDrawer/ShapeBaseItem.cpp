#include "ShapeBaseItem.h"
#include <math.h>
#include <QGraphicsView>
#include <QtDebug>

#define PI 3.141592653


template <typename T>    //T 为QPointF 或QPoint
double Distance(T p1, T p2)
{
	return sqrt(pow(p1.x() - p2.x(), 2) + pow(p1.y() - p2.y(), 2));
}
void CalculateCornerPt(QPointF ptMove, QPointF ptNoMove, double dAngle, QPointF  point[2])
{
	double dist = 0, tempDist = 0;
	double x1 = 0, y1 = 0;
	double sita = 0;
	dist = Distance(ptMove, ptNoMove);
	sita = atan2(ptMove.y() - ptNoMove.y(), ptMove.x() - ptNoMove.x());
	sita -= dAngle;
	tempDist = dist * cos(sita);
	x1 = ptNoMove.x() + tempDist * cos(dAngle);
	y1 = ptNoMove.y() + tempDist * sin(dAngle);
	point[0] = QPointF(x1, y1);
	point[1] = ptMove + ptNoMove - point[0];
}
void AfterRotate(QPointF ptMove, double& dAngle, QPolygonF& polygon)
{
	QPolygonF orignalPoly = polygon;
	QPointF ptOrignal = (polygon[1] + polygon[2]) / 2;
	QPointF ptCenter = (polygon[0] + polygon[2]) / 2;
	double sita1 = atan2(ptOrignal.y() - ptCenter.y(), ptOrignal.x() - ptCenter.x());
	double sita2 = atan2(ptMove.y() - ptCenter.y(), ptMove.x() - ptCenter.x());
	double deltaSita = sita2 - sita1;
	dAngle += deltaSita;
	double sita, distance;
	for (int i = 0; i < polygon.size(); i++)
	{
		sita = atan2(orignalPoly[i].y() - ptCenter.y(), orignalPoly[i].x() - ptCenter.x());
		sita += deltaSita;
		distance = Distance(orignalPoly[i], ptCenter);
		polygon[i].setX(distance * cos(sita) + ptCenter.x());
		polygon[i].setY(distance * sin(sita) + ptCenter.y());
	}
}
void AfterRotate(double dAngle, QPolygonF& polygon)
{
	QPolygonF orignalPoly = polygon;
	QPointF ptCenter = (polygon[0] + polygon[2]) / 2;

	double distance;
	for (int i = 0; i < polygon.size(); i++)
	{
		double sita = atan2(orignalPoly[i].y() - ptCenter.y(), orignalPoly[i].x() - ptCenter.x());
		sita += dAngle;
		distance = Distance(orignalPoly[i], ptCenter);
		polygon[i].setX(distance * cos(sita) + ptCenter.x());
		polygon[i].setY(distance * sin(sita) + ptCenter.y());
	}
}

void BaseItem::SetIndex(int num)
{
	if (ControlList.length() > 0)
	{
		ControlList[0]->index = num;
	}
}

BaseItem::BaseItem(ItemType type) :types(type)
{
	Stroker.setWidth(6);
	setHandlesChildEvents(false);//设置后才能将事件传递到子元素
	if (type != AutoItem)       //模式选择 自定义模式用于显示亚像素轮廓和Region  不设定任何属性
	{
		this->setFlags(QGraphicsItem::ItemIsSelectable |
			QGraphicsItem::ItemIsMovable |
			QGraphicsItem::ItemIsFocusable);
	}
	this->setCursor(Qt::ArrowCursor);
}

BaseItem::~BaseItem()
{
	// delete ;
}
void BaseItem::focusInEvent(QFocusEvent* event)
{
	Q_UNUSED(event);
	ItemColor = QColor(0, 180, 0);
	Background = QColor(0, 240, 0, 50);
	for (int i = 1; i < ControlList.length(); i++)
	{
		ControlList[i]->setVisible(true);
	}
}
void BaseItem::focusOutEvent(QFocusEvent* event)
{
	Q_UNUSED(event);
	ItemColor = QColor(0, 100, 200);
	Background = QColor(0, 160, 230, 50);
	for (int i = 1; i < ControlList.length(); i++)
	{
		ControlList[i]->setVisible(false);
	}
}
QRectF BaseItem::boundingRect() const
{
	return ItemPath.boundingRect().united(PathVir.boundingRect());
}
QPainterPath BaseItem::shape() const
{
	return ItemPath.united(PathVir);
}
void BaseItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option)
		Q_UNUSED(widget)

		//缩放控制点尺寸
		for (int i = 0; i < ControlList.size(); i++)
		{
			if (ControlList[i]->scale() != scaler)//无判断时会进入死循环
			{
				ControlList[i]->setScale(scaler);
			}
		}
	//绘制ROI的文本描述
	painter->setBrush(Qt::NoBrush);
	QFont font;
	font.setPointSizeF(fmax((12 * scaler), 1.0));//字体大小
	painter->setFont(font);
	painter->setPen(Qt::black);
	painter->drawText(ControlList[0]->GetPoint(), ItemDiscrib);

	painter->setBrush(QBrush(Background));
	auto pen = painter->pen();
	pen.setWidthF(2 * scaler);
	pen.setColor(ItemColor);
	painter->setPen(pen);
	painter->drawPath(ItemPath);

	pen.setWidthF(0);
	pen.setColor(Background);
	painter->setPen(pen);
	painter->drawPath(PathVir);
}

//**************************************矩形*******************************************************
RectangleItem::RectangleItem(qreal x, qreal y, qreal width, qreal height) :BaseItem(Rectangle)
{
	Create(DefRect(x, y, width, height));
}
RectangleItem::RectangleItem(DefRect rec) :BaseItem(Rectangle)
{
	Create(rec);
}
void RectangleItem::SetObject(DefRect rec)
{
	QPointF set = this->mapFromScene(rec.X, rec.X);
	polygon[0] = set;
	polygon[1] = set + QPointF(rec.Width, 0);
	polygon[2] = set + QPointF(rec.Width, rec.Height);
	polygon[3] = set + QPointF(0, rec.Height);
	UpDate();
}
DefRect RectangleItem::GetObject()
{
	double xmin = 9999999;
	double ymin = 9999999;
	for (auto item : polygon)
	{
		xmin = qMin(xmin, item.x());
		ymin = qMin(ymin, item.y());
	}
	auto tPoint = this->mapToScene(xmin, ymin);
	return DefRect(tPoint, abs(polygon[1].x() - polygon[0].x()), abs(polygon[0].y() - polygon[3].y()));
}
void RectangleItem::Create(DefRect rec)
{

	auto tPoint = QPointF(rec.X, rec.Y);
	polygon << tPoint << tPoint + QPointF(rec.Width, 0) << tPoint + QPointF(rec.Width, rec.Height) << tPoint + QPointF(0, rec.Height);
	ControlList << new ControlItem(this, QPointF(), 0);
	ControlList << new ControlItem(this, QPointF(), 1);
	ControlList << new ControlItem(this, QPointF(), 2);
	ControlList << new ControlItem(this, QPointF(), 3);
	ControlList << new ControlItem(this, QPointF(), 4);
	UpDate();
}
bool RectangleItem::UpDate(int index)
{
	if (index > 0)
	{
		//需要更新角点的情况
		QPointF Pf = ControlList[index]->GetPoint();
		if (index - 1 < 4)
		{
			polygon[index - 1] = Pf;
		}

		//角点分情况 变更
		switch (index)
		{
		case 1:
			polygon[1].setY(Pf.y());
			polygon[3].setX(Pf.x());
			break;
		case 2:
			polygon[0].setY(Pf.y());
			polygon[2].setX(Pf.x());
			break;
		case 3:
			polygon[3].setY(Pf.y());
			polygon[1].setX(Pf.x());
			break;
		case 4:
			polygon[2].setY(Pf.y());
			polygon[0].setX(Pf.x());
			break;
		default:
			break;
		}
	}
	//中心点变更
	ControlList[0]->SetPoint((polygon[0] + polygon[2]) / 2);
	ControlList[1]->SetPoint(polygon[0]);
	ControlList[2]->SetPoint(polygon[1]);
	ControlList[3]->SetPoint(polygon[2]);
	ControlList[4]->SetPoint(polygon[3]);
	ItemPath.clear();
	ItemPath.addPolygon(polygon);
	ItemPath.closeSubpath();
	return true;
}

//**************************************矩形 旋转*******************************************************
RectangleRItem::RectangleRItem(qreal x, qreal y, qreal Lenth1, qreal Lenth2, qreal Pi) :BaseItem(RectangleR)
{
	Create(DefRotateRect(x, y, Lenth1, Lenth2, Pi));
}
RectangleRItem::RectangleRItem(DefRotateRect rec) :BaseItem(RectangleR)
{
	Create(rec);
}
void RectangleRItem::SetObject(DefRotateRect rec)
{
	angle = rec.ang;
	float Lenth1 = rec.Width / 2;
	float Lenth2 = rec.Height / 2;
	QPointF set = this->mapFromScene(rec.X, rec.X);
	polygon[0] = set + QPointF(-Lenth1, Lenth2);
	polygon[1] = set + QPointF(Lenth1, Lenth2);
	polygon[2] = set + QPointF(Lenth1, -Lenth2);
	polygon[3] = set + QPointF(-Lenth1, -Lenth2);
	UpDate();
}
DefRotateRect RectangleRItem::GetObject()
{
	auto tPoint = this->mapToScene((polygon[0] + polygon[2]) / 2);
	return DefRotateRect(tPoint, Distance(polygon[0], polygon[1]), Distance(polygon[0], polygon[3]), angle);
}
void RectangleRItem::Create(DefRotateRect rec)
{
	angle = rec.ang;
	float Lenth1 = rec.Width / 2;
	float Lenth2 = rec.Height / 2;
	polygon << QPointF(-Lenth1, Lenth2) << QPointF(Lenth1, Lenth2) << QPointF(Lenth1, -Lenth2) << QPointF(-Lenth1, -Lenth2);

	ControlList << new ControlItem(this, QPointF(), 0);
	ControlList << new ControlItem(this, QPointF(), 1);
	ControlList << new ControlItem(this, QPointF(), 2);
	ControlList << new ControlItem(this, QPointF(), 3);
	ControlList << new ControlItem(this, QPointF(), 4);
	ControlList << new ControlItem(this, QPointF(), 5);

	UpDate();
}
bool RectangleRItem::UpDate(int index)
{
	if (index > 0)
	{
		QPointF Pf = ControlList[index]->GetPoint();
		QPointF pt2[2];
		switch (index)
		{
		case 1:
			polygon[0] = Pf;
			CalculateCornerPt(Pf, polygon[2], angle, pt2);
			polygon[1] = pt2[1];
			polygon[3] = pt2[0];
			break;
		case 2:
			polygon[1] = Pf;
			CalculateCornerPt(Pf, polygon[3], angle, pt2);
			polygon[0] = pt2[1];
			polygon[2] = pt2[0];
			break;
		case 3:
			polygon[2] = Pf;
			CalculateCornerPt(Pf, polygon[0], angle, pt2);
			polygon[1] = pt2[0];
			polygon[3] = pt2[1];
			break;
		case 4:
			polygon[3] = Pf;
			CalculateCornerPt(Pf, polygon[1], angle, pt2);
			polygon[0] = pt2[0];
			polygon[2] = pt2[1];
			break;
		case 5:
			AfterRotate(Pf, angle, polygon);
			break;
		}
	}
	else
	{
		AfterRotate(angle, polygon);
	}

	//prepareGeometryChange();

	ControlList[0]->SetPoint((polygon[0] + polygon[2]) / 2);
	ControlList[1]->SetPoint(polygon[0]);
	ControlList[2]->SetPoint(polygon[1]);
	ControlList[3]->SetPoint(polygon[2]);
	ControlList[4]->SetPoint(polygon[3]);
	ControlList[5]->SetPoint((polygon[1] + polygon[2]) / 2);
	ItemPath.clear();
	ItemPath.addPolygon(polygon);
	ItemPath.closeSubpath();

	return true;
}
//**************************************椭圆*******************************************************
EllipseItem::EllipseItem(qreal x, qreal y, qreal Lenth1, qreal Lenth2, qreal Pi) :BaseItem(RectangleR)
{
	Create(DefRotateEllipse(x, y, Lenth1, Lenth2, Pi));
}
EllipseItem::EllipseItem(DefRotateEllipse rec) :BaseItem(RectangleR)
{
	Create(rec);
}
void EllipseItem::SetObject(DefRotateEllipse rec)
{
	angle = rec.ang;
	float Lenth1 = rec.Width / 2;
	float Lenth2 = rec.Height / 2;
	QPointF set = this->mapFromScene(rec.X, rec.X);
	polygon[0] = set + QPointF(-Lenth1, Lenth2);
	polygon[1] = set + QPointF(Lenth1, Lenth2);
	polygon[2] = set + QPointF(Lenth1, -Lenth2);
	polygon[3] = set + QPointF(-Lenth1, -Lenth2);
	UpDate();
}
DefRotateEllipse EllipseItem::GetObject()
{
	auto tPoint = this->mapToScene((polygon[0] + polygon[2]) / 2);
	return DefRotateEllipse(tPoint, Distance(polygon[0], polygon[1]), Distance(polygon[0], polygon[3]), angle);
}
void EllipseItem::Create(DefRotateEllipse rec)
{
	angle = rec.ang;
	float Lenth1 = rec.Width / 2;
	float Lenth2 = rec.Height / 2;
	polygon << QPointF(-Lenth1, Lenth2) << QPointF(Lenth1, Lenth2) << QPointF(Lenth1, -Lenth2) << QPointF(-Lenth1, -Lenth2);

	ControlList << new ControlItem(this, QPointF(), 0);
	ControlList << new ControlItem(this, QPointF(), 1);
	ControlList << new ControlItem(this, QPointF(), 2);
	ControlList << new ControlItem(this, QPointF(), 3);
	ControlList << new ControlItem(this, QPointF(), 4);
	ControlList << new ControlItem(this, QPointF(), 5);

	UpDate();
}
bool EllipseItem::UpDate(int index)
{
	if (index > 0)
	{
		QPointF Pf = ControlList[index]->GetPoint();
		QPointF pt2[2];
		switch (index)
		{
		case 1:
			polygon[0] = Pf;
			CalculateCornerPt(Pf, polygon[2], angle, pt2);
			polygon[1] = pt2[1];
			polygon[3] = pt2[0];
			break;
		case 2:
			polygon[1] = Pf;
			CalculateCornerPt(Pf, polygon[3], angle, pt2);
			polygon[0] = pt2[1];
			polygon[2] = pt2[0];
			break;
		case 3:
			polygon[2] = Pf;
			CalculateCornerPt(Pf, polygon[0], angle, pt2);
			polygon[1] = pt2[0];
			polygon[3] = pt2[1];
			break;
		case 4:
			polygon[3] = Pf;
			CalculateCornerPt(Pf, polygon[1], angle, pt2);
			polygon[0] = pt2[0];
			polygon[2] = pt2[1];
			break;
		case 5:
			AfterRotate(Pf, angle, polygon);
			//计算弧度、旋转工具需要使用弧度
			angleDEG = angle / 3.14159265358979 * 180;
			break;
		}
	}
	else
	{
		AfterRotate(angle, polygon);
		angleDEG = angle / 3.14159265358979 * 180;
	}

	//prepareGeometryChange(); 

	ControlList[0]->SetPoint((polygon[0] + polygon[2]) / 2);
	ControlList[1]->SetPoint(polygon[0]);
	ControlList[2]->SetPoint(polygon[1]);
	ControlList[3]->SetPoint(polygon[2]);
	ControlList[4]->SetPoint(polygon[3]);
	ControlList[5]->SetPoint((polygon[1] + polygon[2]) / 2);
	ItemPath.clear();

	QTransform trans;
	trans.rotate(-angleDEG);
	//旋转到水平、画椭圆只能水平状态
	ItemPath.addEllipse(trans.map(polygon).boundingRect());
	//旋转到正常角度
	trans.rotate(2 * angleDEG);
	//将画好的椭圆旋转到正常位置
	ItemPath = trans.map(ItemPath);
	return true;
}

//**************************************圆*******************************************************
CircleItem::CircleItem(qreal x, qreal y, qreal R) :BaseItem(Circle)
{
	CircleItem::Create(DefCircle(x, y, R));
}
CircleItem::CircleItem(DefCircle circle) :BaseItem(Circle)
{
	CircleItem::Create(circle);
}
void CircleItem::SetObject(DefCircle circle)
{
	Center = this->mapFromScene(circle.X, circle.X);
	Radius = circle.R;
	UpDate();
}
DefCircle CircleItem::GetObject()
{
	auto tPoint = this->mapToScene(Center);
	return DefCircle(tPoint, Radius);
}
void CircleItem::Create(DefCircle circle)
{
	Center = QPointF(circle.X, circle.Y);
	Radius = circle.R;
	ControlList << new ControlItem(this, Center, 0);
	ControlList << new ControlItem(this, Center + QPointF(Radius, 0), 1);
	UpDate();
}
bool CircleItem::UpDate(int index)
{
	if (index > 0)
	{
		QPointF Pf = ControlList[index]->GetPoint();
		QPointF tmp = Pf - Center;
		Radius = sqrt(tmp.x() * tmp.x() + tmp.y() * tmp.y());
	}
	else
	{
		ControlList[0]->SetPoint(Center);
		ControlList[1]->SetPoint(Center + QPointF(Radius, 0));
	}

	ItemPath.clear();
	ItemPath.addEllipse(Center, Radius, Radius);
	return true;
}
//**************************************同心圆*********************************************************
ConcentricCircleItem::ConcentricCircleItem(qreal x, qreal y, qreal radiusMin, qreal radiusMax) :BaseItem(Concentric_Circle)
{

	Create(DefConcentricCircle(x, y, radiusMax, radiusMin));
}
ConcentricCircleItem::ConcentricCircleItem(DefConcentricCircle concentricCircle) :BaseItem(Concentric_Circle)
{
	Create(concentricCircle);
}
void ConcentricCircleItem::SetObject(DefConcentricCircle concentricCircle)
{
	Center = this->mapFromScene(concentricCircle.X, concentricCircle.X);
	RadiusMax = concentricCircle.RMax;
	RadiusMin = concentricCircle.RMin > RadiusMax ? RadiusMax : concentricCircle.RMin;
	UpDate();
}
DefConcentricCircle ConcentricCircleItem::GetObject()
{
	auto tPoint = this->mapToScene(Center);
	return DefConcentricCircle(tPoint, RadiusMax, RadiusMin);
}
void ConcentricCircleItem::Create(DefConcentricCircle concentricCircle)
{
	Center = QPointF(concentricCircle.X, concentricCircle.Y);
	RadiusMax = concentricCircle.RMax;
	RadiusMin = concentricCircle.RMin > RadiusMax ? RadiusMax : concentricCircle.RMin;
	ControlList << new ControlItem(this, Center, 0);
	ControlList << new ControlItem(this, Center + QPointF(RadiusMin, 0), 1);
	ControlList << new ControlItem(this, Center + QPointF(RadiusMax, 0), 2);
	UpDate();
}
bool ConcentricCircleItem::UpDate(int index)
{
	if (index > 0)
	{
		QPointF Pf = ControlList[index]->GetPoint();
		QPointF tmp = Pf - Center;
		qreal R = sqrt(tmp.x() * tmp.x() + tmp.y() * tmp.y());
		if (index == 1)
		{
			if (R > RadiusMax)
				return false;
			RadiusMin = R;
		}
		else if (index == 2)
		{
			if (R < RadiusMin)
				return false;
			RadiusMax = R;
		}
	}
	else
	{
		ControlList[0]->SetPoint(Center);
		ControlList[1]->SetPoint(Center + QPointF(RadiusMin, 0));
		ControlList[2]->SetPoint(Center + QPointF(RadiusMax, 0));
	}


	ItemPath.clear();
	ItemPath.addEllipse(Center, RadiusMax, RadiusMax);
	ItemPath.addEllipse(Center, RadiusMin, RadiusMin);
	return true;
}

//**************************************饼图********细节需要优化*************************************************
PieItem::PieItem(qreal x, qreal y, qreal radius, qreal startAng, qreal endAng) :BaseItem(Pie)
{
	Create(DefPie(x, y, radius, startAng, endAng));
}
PieItem::PieItem(DefPie defPie) :BaseItem(Pie)
{
	Create(defPie);
}
void PieItem::SetObject(DefPie defPie)
{
	Center = this->mapFromScene(defPie.X, defPie.Y);
	Radius = defPie.R;
	StartAng = defPie.AngStart;
	EndAng = defPie.AngEnd;
	UpDate();
}
DefPie PieItem::GetObject()
{
	return DefPie(this->mapToScene(Center), Radius, StartAng, EndAng);
}
void PieItem::Create(DefPie defPie)
{
	Center = QPointF(defPie.X, defPie.Y);
	Radius = defPie.R;
	StartAng = defPie.AngStart;
	EndAng = defPie.AngEnd; //endAng>StartAng?StartAng:
	ControlList << new ControlItem(this, QPointF(), 0);
	ControlList << new ControlItem(this, QPointF(), 1);
	ControlList << new ControlItem(this, QPointF(), 2);

	UpDate();
}
bool PieItem::UpDate(int index)
{

	if (index > 0)
	{
		QPointF Pf = ControlList[index]->GetPoint();
		qreal dx = Pf.x() - Center.x();
		qreal dy = Pf.y() - Center.y();
		Radius = sqrt(dx * dx + dy * dy);
		if (index == 1)
		{
			StartAng = atan2(-dy, dx);
		}
		else
		{
			EndAng = atan2(-dy, dx);
		}
	}


	qreal ss = sin(StartAng);
	qreal cs = cos(StartAng);
	qreal se = sin(EndAng);
	qreal ce = cos(EndAng);
	ControlList[0]->SetPoint(Center);
	ControlList[1]->SetPoint(Center + QPointF(Radius * cs, -Radius * ss));
	ControlList[2]->SetPoint(Center + QPointF(Radius * ce, -Radius * se));


	float sub = StartAng - EndAng;
	float Lenght;
	if (sub > 0)
	{
		Lenght = 6.2831852 - sub;
	}
	else
	{
		Lenght = EndAng - StartAng;
	}
	Lenght = Lenght / 3.1415928 * 180;

	ItemPath.clear();
	ItemPath.moveTo(Center);
	ItemPath.arcTo(QRectF(Center.x() - Radius, Center.y() - Radius, Radius * 2, Radius * 2), StartAng / 3.1415928 * 180, Lenght);
	ItemPath.closeSubpath();
	return true;
}

//**************************************多边形*********************************************************
PolygonItem::PolygonItem(QVector<QPointF> pList) :BaseItem(Polygon)
{
	Create(DefPolygon(pList));
}
PolygonItem::PolygonItem(DefPolygon defPolygon) :BaseItem(Polygon)
{
	Create(defPolygon);
}
void PolygonItem::AddPoint(QPointF p, int index)
{
	polygon << p;
	ControlList << new ControlItem(this, QPointF(), polygon.size() + 1);
}
void PolygonItem::SetObject(DefPolygon defPolygon)
{

}
DefPolygon PolygonItem::GetObject()
{
	return DefPolygon();
}
void PolygonItem::Create(DefPolygon defPolygon)
{
	ControlList << new ControlItem(this, QPointF(), 0);
	for (int i = 0; i < defPolygon.PList.size(); i++)
	{
		polygon << defPolygon.PList[i];
		ControlList << new ControlItem(this, QPointF(), i + 1);
	}
	UpDate();
}
bool PolygonItem::UpDate(int index)
{
	if (index > 0)
	{
		polygon[index - 1] = ControlList[index]->GetPoint();
	}
	if (index < 0)
	{
		for (int i = 0; i < polygon.size(); i++)
		{
			ControlList[i + 1]->SetPoint(polygon[i]);
		}
	}
	QPointF Sum(0, 0);
	for (auto item : polygon)
	{
		Sum += item;
	}
	if (polygon.size() > 0)
	{
		Sum = Sum / polygon.size();
	}
	ControlList[0]->SetPoint(Sum);
	ItemPath.clear();
	//    ItemPath.moveTo(polygon[0]);
	//    for(int i=1;i<polygon.size()-2;i+=2)  
	//    {
	//        ItemPath.cubicTo(polygon[i], polygon[i+1], polygon[i+2]);
	//    }
	ItemPath.addPolygon(polygon);
	ItemPath.closeSubpath();
	return true;
}

//**************************************圆端矩形*********************************************************
RoundRectangle1Item::RoundRectangle1Item(qreal x, qreal y, qreal width, qreal height, qreal r) :BaseItem(Round_End_Rectangle)
{
	Create(DefRoundRect(x, y, width, height, r));
}
RoundRectangle1Item::RoundRectangle1Item(DefRoundRect roundRect) :BaseItem(Round_End_Rectangle)
{
	Create(roundRect);
}
void RoundRectangle1Item::SetObject(DefRoundRect roundRect)
{
	Center = this->mapFromScene(roundRect.X, roundRect.Y);
	Radius = roundRect.R;
	Width = roundRect.Width;
	Height = roundRect.Height;
	UpDate();
}
DefRoundRect RoundRectangle1Item::GetObject()
{
	return DefRoundRect(this->mapToScene(Center), Width, Height, Radius);
}
void RoundRectangle1Item::Create(DefRoundRect roundRect)
{
	Width = roundRect.Width;
	Height = roundRect.Height;
	Radius = roundRect.R;
	ControlList << new ControlItem(this, QPointF(), 0);
	ControlList << new ControlItem(this, QPointF(), 1);
	ControlList << new ControlItem(this, QPointF(), 2);
	UpDate();
}
bool RoundRectangle1Item::UpDate(int index)
{

	if (index > 0)
	{
		QPointF Pf = ControlList[1]->GetPoint();
		QPointF Pfc = ControlList[2]->GetPoint();

		if (index == 1)
		{
			//设置1号控制点
			qreal tmpx = Pf.x() <= Radius ? Radius : Pf.x();
			qreal tmpy = Pf.y() <= Radius ? Radius : Pf.y();
			ControlList[1]->SetPoint(QPointF(tmpx, tmpy));

			//更新尺寸
			Width = qAbs(tmpx - Center.x()) * 2;
			Height = qAbs(tmpy - Center.y()) * 2;

			//控制点2位置被动更新 需要用旧的位置 懒的修改了 有时间再改
			qreal retX = 0;
			qreal retY = 0;
			if (tmpx == Pfc.x())
			{
				retX = tmpx;
				retY = -tmpy + Radius;
			}
			else {
				retX = tmpx - Radius;
				retY = -tmpy;
			}
			ControlList[2]->SetPoint(QPointF(retX, retY));
		}
		if (index == 2)
		{
			//获取控制点2坐标
			Pfc = ControlList[2]->GetBefPoint();
			qreal dx = ControlList[2]->dX();
			qreal dy = ControlList[2]->dY();
			qreal retX = 0;
			qreal retY = 0;
			if (Pf.x() == Pfc.x() && (-1) * Pf.y() != Pfc.y())
			{
				retX = Pfc.x();
				retY = Pfc.y() + dy;
			}
			else if (Pf.x() != Pfc.x() && (-1) * Pf.y() == Pfc.y())
			{
				retX = Pfc.x() + dx;
				retY = Pfc.y();
			}
			else if (Pf.x() == Pfc.x() && (-1) * Pf.y() == Pfc.y())
			{
				if (abs(dx) >= abs(dy))
				{
					retX = Pfc.x() + dx;
					retY = Pfc.y();
				}
				else {
					retX = Pfc.x();
					retY = Pfc.y() + dy;
				}
			}
			if (retX > Pf.x())
			{
				retX = Pf.x();
			}
			else if (retX < 0)
			{
				retX = 0;
			}
			if (retY < (-1) * Pf.y())
			{
				retY = (-1) * Pf.y();
			}
			else if (retY > 0)
			{
				retY = 0;
			}


			//设定控制点2
			ControlList[2]->SetPoint(QPointF(retX, retY));
			{
				//更新半径
				qreal dx = 0;
				qreal dy = 0;
				qreal absX = abs(Pf.x());
				qreal absY = abs(Pf.y());

				if (Pfc.x() >= 0 && Pfc.y() < 0)
				{
					dx = absX - Pfc.x();
					dy = absY + Pfc.y();
				}
				else if (Pfc.x() < 0 && Pfc.y() < 0)
				{
					dx = absX + Pfc.x();
					dy = absY + Pfc.y();
				}
				else if (Pfc.x() < 0 && Pfc.y() >= 0)
				{
					dx = absX + Pfc.x();
					dy = absY - Pfc.y();
				}
				else if (Pfc.x() >= 0 && Pfc.y() >= 0)
				{
					dx = absX - Pfc.x();
					dy = absY - Pfc.y();
				}
				Radius = dx >= dy ? dx : dy;
				Radius = Radius < 0 ? 0 : Radius;
			}
		}

	}
	else
	{
		ControlList[0]->SetPoint(Center);
		ControlList[1]->SetPoint(Center + QPointF(Width / 2, Height / 2));
		ControlList[2]->SetPoint(Center + QPointF(Width / 2, -Height / 2));
	}


	QRectF ret(Center - QPointF(Width / 2, Height / 2), Center + QPointF(Width / 2, Height / 2));
	ItemPath.clear();
	ItemPath.addRoundedRect(ret, Radius, Radius);
	ItemPath.closeSubpath();
	return true;
}
//**************************************直线*********************************************************
LineItem::LineItem(qreal x1, qreal y1, qreal x2, qreal y2) :BaseItem(LineObj)
{
	Create(DefLine(QPointF(x1, y1), QPointF(x2, y2)));
}
LineItem::LineItem(DefLine defLine) :BaseItem(LineObj)
{
	Create(defLine);
}
void LineItem::SetObject(DefLine defLine)
{
	polygon[0] = this->mapFromScene(defLine.X1, defLine.Y1);
	polygon[1] = this->mapFromScene(defLine.X2, defLine.Y2);
	UpDate();
}
DefLine LineItem::GetObject()
{
	return DefLine(this->mapToScene(polygon[0]), this->mapToScene(polygon[1]));
}
bool LineItem::UpDate(int index)
{

	if (index > 0)
	{
		if (index == 1)
		{
			polygon[0] = ControlList[1]->GetPoint();

		}
		else if (index == 2)
		{
			polygon[1] = ControlList[2]->GetPoint();
		}
	}
	ControlList[0]->SetPoint((polygon[0] + polygon[1]) / 2); \
		ControlList[1]->SetPoint(polygon[0]);
	ControlList[2]->SetPoint(polygon[1]);
	ItemPath.clear();
	PathVir.clear();

	ItemPath.addPolygon(polygon);
	PathVir = Stroker.createStroke(ItemPath);
	return true;
}
void LineItem::Create(DefLine defLine)
{
	polygon << QPointF(defLine.X1, defLine.Y1) << QPointF(defLine.X2, defLine.Y2);
	ControlList << new ControlItem(this, QPointF(), 0);
	ControlList << new ControlItem(this, QPointF(), 1);
	ControlList << new ControlItem(this, QPointF(), 2);
	UpDate();
}

//**************************************卡尺*********************************************************
Caliper::Caliper(qreal x1, qreal y1, qreal x2, qreal y2, qreal height) :BaseItem(LineObj)
{
	Create(DefCaliper(x1, y1, x2, y2, height));
}
Caliper::Caliper(DefCaliper defCaliper) :BaseItem(LineObj)
{
	Create(defCaliper);
}
void Caliper::SetObject(DefCaliper defCaliper)
{
	polygon[0] = this->mapFromScene(defCaliper.X1, defCaliper.Y1);
	polygon[1] = this->mapFromScene(defCaliper.X2, defCaliper.Y2);
	Center = (polygon[0] + polygon[1]) / 2;
	Height = defCaliper.Width;
	UpDate();
}
DefCaliper Caliper::GetObject()
{
	return DefCaliper(this->mapToScene(polygon[0]), this->mapToScene(polygon[1]), Height);
}
bool Caliper::UpDate(int index)
{

	if (index > 0)
	{
		if (index == 1)
		{
			polygon[0] = ControlList[1]->GetPoint();
		}
		else if (index == 2)
		{
			polygon[1] = ControlList[2]->GetPoint();
		}
		else if (index == 3)
		{
			QPointF Pf = ControlList[3]->GetPoint();
			qreal dx = Pf.x() - Center.x();
			qreal dy = Pf.y() - Center.y();
			Height = sqrt(dx * dx + dy * dy) * 2;
		}
		Center = (polygon[0] + polygon[1]) / 2;
	}

	ControlList[0]->SetPoint(Center);
	ControlList[1]->SetPoint(polygon[0]);
	ControlList[2]->SetPoint(polygon[1]);

	auto dt = polygon[0] - polygon[1];
	qreal dx = dt.x();
	qreal dy = dt.y();
	angle = atan2(dy, -dx);

	qreal s = sin(angle);
	qreal c = cos(angle);
	ControlList[3]->SetPoint(Center + QPointF(s * Height / 2, c * Height / 2));
	Lenth = sqrt(dx * dx + dy * dy);


	ItemPath.clear();
	PathVir.clear();


	QTransform trans;
	trans.translate(Center.x(), Center.y());
	trans.rotate(angle * 180 / PI);
	//旋转到水平、画椭圆只能水平状态 angle
	PathVir.addRect(-Lenth / 2, -abs(Height / 2), Lenth, abs(Height));
	//旋转到正常角度
	trans.rotate(-2 * angle * 180 / PI);
	//将画好的椭圆旋转到正常位置
	PathVir = trans.map(PathVir);


	static qreal l = 25.0;//箭头长度
	qreal atn1 = atan2(dy, dx);
	qreal atn2 = atan2(dx, dy);
	QPointF Arrow1(polygon[0].x() - l * cos(atn1 - 0.5), polygon[0].y() - l * sin(atn1 - 0.5));
	QPointF Arrow2(polygon[0].x() - l * sin(atn2 - 0.5), polygon[0].y() - l * cos(atn2 - 0.5));
	QPolygonF Arr;
	Arr << Arrow1 << polygon[0] << Arrow2;

	ItemPath.addPolygon(polygon);
	ItemPath.addPolygon(Arr);

	return true;
}
void Caliper::Create(DefCaliper defCaliper)
{
	polygon << QPointF(defCaliper.X1, defCaliper.Y1) << QPointF(defCaliper.X2, defCaliper.Y2);
	Center = (polygon[0] + polygon[1]) / 2;
	Height = defCaliper.Width;
	ControlList << new ControlItem(this, QPointF(), 0);
	ControlList << new ControlItem(this, QPointF(), 1);
	ControlList << new ControlItem(this, QPointF(), 2);
	ControlList << new ControlItem(this, QPointF(), 3);
	UpDate();
}

//**************************************锚点*********************************************************
PointItem::PointItem(qreal x, qreal y) :BaseItem(PointObj)
{
	Create(DefPoint(x, y));
}
PointItem::PointItem(DefPoint defPoint) :BaseItem(PointObj)
{
	Create(defPoint);
}
void PointItem::SetObject(DefPoint defPoint)
{
	Center = this->mapFromScene(defPoint.X1, defPoint.Y1);
	UpDate();
}
DefPoint PointItem::GetObject()
{
	return DefPoint(this->mapToScene(Center));
}
bool PointItem::UpDate(int index)
{
	if (index == 1)
	{
		Center = ControlList[1]->GetPoint();
	}
	ControlList[0]->SetPoint(Center);
	ControlList[1]->SetPoint(Center);
	return true;
}
void PointItem::Create(DefPoint defPoint)
{
	Center = QPointF(defPoint.X1, defPoint.Y1);
	ControlList << new ControlItem(this, QPointF(), 0, 0);
	ControlList << new ControlItem(this, QPointF(), 1);
	UpDate();
}
