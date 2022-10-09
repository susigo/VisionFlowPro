#include "ShapeItemBase.h"


ShapeItemBase::ShapeItemBase(EShapeType type) :types(type)
{

	setHandlesChildEvents(false);//设置后才能将事件传递到子元素
	if (type != EShapeType::sNone)       //模式选择 自定义模式用于显示亚像素轮廓和Region  不设定任何属性
	{
		this->setFlags(QGraphicsItem::ItemIsSelectable |
			QGraphicsItem::ItemIsMovable |
			QGraphicsItem::ItemIsFocusable);
	}
	this->setCursor(Qt::ArrowCursor);
	activeHandle = nullptr;
	bounding_rect = QRectF(0, 0, 100, 100);
	ItemPath.addRect(bounding_rect);
	ItemShape.addRect(bounding_rect);
}

ShapeItemBase::~ShapeItemBase()
{

}

void ShapeItemBase::focusInEvent(QFocusEvent* event)
{
	Q_UNUSED(event);
	this->setZValue(99);
	shape_active = true;
}

void ShapeItemBase::focusOutEvent(QFocusEvent* event)
{
	Q_UNUSED(event);
	this->setZValue(1);
	shape_active = false;
}

QRectF ShapeItemBase::boundingRect() const
{
	return ItemPath.boundingRect().united(ItemShape.boundingRect());
}

QPainterPath ShapeItemBase::shape() const
{
	return ItemShape.united(ItemPath);
}

void ShapeItemBase::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	QGraphicsItemGroup::paint(painter, option, widget);
	painter->setPen(Qt::NoPen);
	if (shape_active)
	{
		painter->setBrush(activeBackgroundColor);
		//painter->drawRect(this->boundingRect());
	}
	else
	{
		if (m_shape_mode == ShapeMode::mAdd)
		{
			painter->setBrush(addBackgroundColor);
		}
		else
		{
			painter->setBrush(divBackgroundColor);
		}
	}
	if (!ItemPath.isEmpty())
	{
		painter->setPen(ItemColor);
		//painter->setBrush(Qt::NoBrush);
		painter->drawPath(ItemPath);
	}
}

