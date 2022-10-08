#include "ShapeControlItem.h"
#include "ShapeItemBase.h"

//构造函数
ShapeControlItem::ShapeControlItem(QGraphicsItemGroup* parent, QPointF p, EShapeType type, int style)
	: QAbstractGraphicsShapeItem(parent)
{
	setPos(p);
	setAcceptHoverEvents(true);
	if (type != EShapeType::sNone)       //模式选择 自定义模式用于显示亚像素轮廓和Region  不设定任何属性
	{
		this->setFlags(QGraphicsItem::ItemIsSelectable |
			QGraphicsItem::ItemIsMovable |
			QGraphicsItem::ItemIsFocusable);
	}
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
}

void ShapeControlItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{

	QGraphicsItem::mousePressEvent(event);
}

void ShapeControlItem::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
	Q_UNUSED(event);
	emit PositionChanged(this->pos());
}
