#include "ControlItem.h"
#include "BaseItem.h"
#include <QDebug>




//构造函数
ControlItem::ControlItem(QGraphicsItemGroup* parent, QPointF p, int type, int style) : QAbstractGraphicsShapeItem(parent)
, point(p), pointType(type), Style(style)
{
	RecSize = ((BaseItem*)parent)->property("ContrSize").toReal();
	PointSize = RecSize * 3;
	this->setPos(point);
	if (pointType == 0)
	{
		this->setCursor(Qt::ArrowCursor);
	}
	else
	{
		this->setCursor(Qt::PointingHandCursor);
		this->setVisible(false);
	}
	this->setBrush(QBrush(QColor(0, 100, 250, 70)));//控制点颜色
	setCacheMode(DeviceCoordinateCache);//设置缓冲 需要配合 update 使用

	//非常关键 确保点击子元素不会丢失焦点
	this->setFlags(QGraphicsItem::ItemIsSelectable |
		QGraphicsItem::ItemIsMovable |
		ItemStopsFocusHandling);
	myPen.setWidthF(2);//缩放后线宽 0 为任何情况下1像素
	myPen.setColor(QColor(0, 100, 250));



	if (style == 1)
	{
		ItemPath.addRect(QRectF(-RecSize, -RecSize, RecSize * 2, RecSize * 2));
	}
	else
	{
		ItemPath.addRect(QRectF(-PointSize, -PointSize, PointSize * 2, PointSize * 2));
	}

}

QPointF ControlItem::GetPoint()
{
	return point;
}
void ControlItem::SetPoint(QPointF p)
{
	point = p;
	this->setPos(p);
}


//拖拽 鼠标感应区域
QRectF ControlItem::boundingRect() const
{
	return ItemPath.boundingRect(); //拖拽 鼠标感应区域
}
//位置重绘
void ControlItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	//位置重绘
	Q_UNUSED(option);
	Q_UNUSED(widget);

	if (Style)
	{
		painter->setPen(myPen);
		painter->setBrush(this->brush());
		this->setPos(point);
		painter->drawEllipse(ItemPath.boundingRect());
	}
	else
	{
		painter->setPen(myPen);
		painter->drawLine(QPointF(0, -PointSize), QPointF(0, PointSize));
		painter->drawLine(QPointF(PointSize, 0), QPointF(-PointSize, 0));
		this->setPos(point);

	}
}
//鼠标事件处理
void ControlItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->buttons() == Qt::LeftButton)
	{

		dx = event->scenePos().x() - event->lastScenePos().x();
		dy = event->scenePos().y() - event->lastScenePos().y();

		BaseItem* item = static_cast<BaseItem*>(this->parentItem());

		if (pointType == 0)
		{
			item->moveBy(dx, dy);
		}
		else
		{
			//记录上一次座标结果
			BefPoint = point;
			point = this->mapToParent(event->pos());
			//更新结果
			bool flg = item->UpDate(pointType);
			if (flg)
			{
				//结果正常、更新位置
				this->setPos(point);
			}
			else
			{
				//结果异常、退回上一次的位置
				point = BefPoint;
				this->setPos(point);
			}
		}

	}
}

void ControlItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{

	if (pointType == 0)
	{
		//点击中心点时、激活编辑
		BaseItem* item = static_cast<BaseItem*>(this->parentItem());
		item->setFocus();
	}
	QGraphicsItem::mousePressEvent(event);
}

