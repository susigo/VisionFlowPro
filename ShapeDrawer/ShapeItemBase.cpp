#include "ShapeItemBase.h"

void ShapeItemBase::SetIndex(int num)
{
	if (ControlList.length() > 0)
	{
		//ControlList[0]->index = num;
	}
}

ShapeItemBase::ShapeItemBase(EShapeType type) :types(type)
{
	Stroker.setWidth(6);
	setHandlesChildEvents(false);//设置后才能将事件传递到子元素
	if (type != EShapeType::sNone)       //模式选择 自定义模式用于显示亚像素轮廓和Region  不设定任何属性
	{
		this->setFlags(QGraphicsItem::ItemIsSelectable |
			QGraphicsItem::ItemIsMovable |
			QGraphicsItem::ItemIsFocusable);
	}
	this->setCursor(Qt::ArrowCursor);
}
