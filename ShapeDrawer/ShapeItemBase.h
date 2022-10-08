#pragma once
#include <qobject.h>
#include<QGraphicsItemGroup>
#include "DrawViewParams.h"
#include "ShapeControlItem.h"

class ShapeItemBase :
	public QObject, QGraphicsItemGroup
{
	Q_OBJECT
public:
	void SetIndex(int num);
	void SetZoomVal(qreal ZoomVal) { scaler = ZoomVal; };
	qreal GetContrSize() const { return ContrSize; }
	virtual bool Update(int index) = NULL;
	void DrawIndex(const QPointF& P);
protected:
	ShapeItemBase(EShapeType type);
	virtual ~ShapeItemBase() = default;
	//virtual void focusInEvent(QFocusEvent* event) override;
	//virtual void focusOutEvent(QFocusEvent* event) override;
	//virtual QRectF boundingRect() const override;
	//virtual QPainterPath shape() const override;
	//void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
protected:
	QList<ShapeControlItem* > ControlList;
	qreal scaler;                                   //缩放系数
	const qreal ContrSize = 8;                        //控制点尺寸
	EShapeType types;                                 //枚举类型
	QColor ItemColor = QColor(0, 100, 200);           //线条颜色
	QColor Background = QColor(0, 160, 230, 50);       //填充背景颜色

	QString ItemDiscrib = QString::fromLocal8Bit("描述");

	QPolygonF polygon;                              //轮廓点集
	QPainterPathStroker Stroker;                    //轮廓处理工具

	QPointF Center;                                 //中心点
	QPainterPath ItemPath;                          //有边框区域
	QPainterPath PathVir;                           //无边框区域
};

