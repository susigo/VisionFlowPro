#include "ShapeControlItem.h"
#include "ShapeItemBase.h"

//构造函数
ShapeControlItem::ShapeControlItem(QGraphicsItemGroup* parent,
	ControlItemType type,
	QPointF p, int style)
	: QAbstractGraphicsShapeItem(parent)
{
	setPos(p);
	setAcceptHoverEvents(true);
	handle_type = type;
	this->setFlags(QGraphicsItem::ItemIsSelectable |
		QGraphicsItem::ItemIsMovable |
		QGraphicsItem::ItemIsFocusable);

	bounding_rect = QRectF(-handle_size * 0.5, -handle_size * 0.5,
		handle_size, handle_size);
}

void ShapeControlItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
	active_states = true;
	Q_UNUSED(event);
}

void ShapeControlItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
	active_states = false;
	Q_UNUSED(event);
}

//拖拽 鼠标感应区域
QRectF ShapeControlItem::boundingRect() const
{
	return bounding_rect; //拖拽 鼠标感应区域
}
//位置重绘
void ShapeControlItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->setPen(Qt::NoPen);
	if (active_states)
	{
		painter->setBrush(handle_active_color);
		bounding_rect = QRectF(-handle_active_size * 0.5, -handle_active_size * 0.5,
			handle_active_size, handle_active_size);
	}
	else
	{
		painter->setBrush(handle_color);
		bounding_rect = QRectF(-handle_size * 0.5, -handle_size * 0.5,
			handle_size, handle_size);
	}
	painter->drawRoundedRect(bounding_rect, 2, 2);
}
//鼠标事件处理
void ShapeControlItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->buttons() == Qt::LeftButton)
	{
		last_point = cur_point;
		cur_point = this->mapToParent(event->pos());
		dx = cur_point.x() - last_point.x();
		dy = cur_point.y() - last_point.y();
		if (this->handle_type != cCenter && this->handle_type != cRotate)
		{
			//结果正常、更新位置
			this->setPos(cur_point);
		}
	}
	emit PositionChanged();
}

void ShapeControlItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{

	QGraphicsItem::mousePressEvent(event);
}
