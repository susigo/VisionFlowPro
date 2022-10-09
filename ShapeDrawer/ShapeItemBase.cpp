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
	shape_active = true;
	//for (int i = 1; i < ControlList.length(); i++)
	//{
	//	ControlList[i]->setVisible(true);
	//}
}

void ShapeItemBase::focusOutEvent(QFocusEvent* event)
{
	Q_UNUSED(event);
	shape_active = false;
	//for (int i = 1; i < ControlList.length(); i++)
	//{
	//	ControlList[i]->setVisible(false);
	//}
}

QRectF ShapeItemBase::boundingRect() const
{
	return ItemPath.boundingRect().united(ItemShape.boundingRect());
}

QPainterPath ShapeItemBase::shape() const
{
	return ItemShape;
}

void ShapeItemBase::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	QGraphicsItemGroup::paint(painter, option, widget);
	painter->setPen(Qt::NoPen);
	if (shape_active)
	{
		painter->setBrush(activeBackgroundColor);
	}
	else
	{
		painter->setBrush(BackgroundColor);
	}
	painter->drawRect(bounding_rect);
	if (!ItemPath.isEmpty())
	{
		painter->setPen(Qt::white);
		painter->setBrush(Qt::NoBrush);
		painter->drawPath(ItemPath);
	}

}

