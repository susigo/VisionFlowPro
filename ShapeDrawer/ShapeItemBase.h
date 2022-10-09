#pragma once
#include <qobject.h>
#include<QGraphicsItemGroup>
#include "DrawViewParams.h"
#include "ShapeControlItem.h"

class ShapeItemBase :
	public QObject, public QGraphicsItemGroup
{
	Q_OBJECT
public:
	ShapeItemBase(EShapeType type);
	virtual ~ShapeItemBase();
	void SetZoomVal(qreal ZoomVal) { scaler = ZoomVal; }
	qreal GetContrSize() const { return ContrSize; }
	QPolygonF GetShapePoygonF()
	{
		//if (!itemPolygon.isClosed())
		//{
		//	itemPolygon.append(itemPolygon.first());
		//}
		for (int i = 0; i < itemPolygon.count(); i++)
		{
			itemPolygon[i] = mapToScene(itemPolygon[i]);
		}
		return itemPolygon;
	}
	ShapeMode GetShapeMode() { return m_shape_mode; }
private:
	virtual void shapeInit() = 0;
public	slots:
	virtual void calculateShape() = 0;
	virtual void rotateShape(qreal delta) {};
protected:
	void focusInEvent(QFocusEvent* event) override;

	void focusOutEvent(QFocusEvent* event) override;

	QRectF boundingRect() const override;

	QPainterPath shape() const override;

	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

protected:
	ShapeControlItem* activeHandle;
	QList<ShapeControlItem* > ControlList;
	QRectF bounding_rect;
	qreal scaler;                                   //缩放系数
	const qreal ContrSize = 8;                        //控制点尺寸
	EShapeType types;                                 //枚举类型
	QColor ItemColor = QColor(0, 100, 200);           //线条颜色
	QColor BackgroundColor = QColor(0, 160, 230, 100);       //填充背景颜色
	QColor addBackgroundColor = QColor(0, 255, 0, 100);       //填充背景颜色
	QColor divBackgroundColor = QColor(255, 0, 0, 50);       //填充背景颜色
	QColor activeBackgroundColor = QColor(255, 255, 0, 100);       //填充背景颜色
	bool shape_active = false;
	QString ItemDiscrib = QString::fromLocal8Bit("描述");

	ShapeMode m_shape_mode = ShapeMode::mNone;
	QPointF Center;                                 //中心点
	QPainterPath ItemShape;                          //有边框区域
	QPainterPath ItemPath;                          //有边框区域
	QPolygonF itemPolygon;
};

