#include "ShapeItemLine.h"

ShapeItemLine::ShapeItemLine(QPointF pos) :
	ShapeItemBase(EShapeType::sLine)
{
	this->setPos(pos);
	shapeInit();
}

ShapeItemLine::~ShapeItemLine()
{
}

void ShapeItemLine::calculateShape()
{
	if (this->sender() == nullptr)
	{
		ControlList[1]->setX(0.5 * (ControlList[0]->x() + ControlList[2]->x()));
		ControlList[1]->setY(0.5 * (ControlList[0]->y() + ControlList[2]->y()));
		ItemPath.clear();
		itemPolygon[0] = ControlList[0]->getCenterPointF();
		itemPolygon[1] = ControlList[2]->getCenterPointF();
		ItemPath.addPolygon(itemPolygon);
		ItemPath.closeSubpath();
		ItemShape.clear();
		ItemShape.addRect(ItemPath.boundingRect());
	}
	else
	{
		activeHandle = dynamic_cast<ShapeControlItem*>(this->sender());

		if (activeHandle->getType() == cCenter)
		{
			//this->moveBy(activeHandle->getDx(), activeHandle->getDy());
		}
		else if (activeHandle->getType() == cRotate)
		{

		}
		else {

			ControlList[1]->setX(0.5 * (ControlList[0]->x() + ControlList[2]->x()));
			ControlList[1]->setY(0.5 * (ControlList[0]->y() + ControlList[2]->y()));
			ItemPath.clear();
			itemPolygon[0] = ControlList[0]->getCenterPointF();
			itemPolygon[1] = ControlList[2]->getCenterPointF();
			ItemPath.addPolygon(itemPolygon);
			ItemPath.closeSubpath();
			ItemShape.clear();
			ItemShape.addRect(ItemPath.boundingRect());
		}
	}
}

void ShapeItemLine::shapeInit()
{
	ControlList.append(new ShapeControlItem(this, ControlItemType::cEdgeNode, QPointF(10, 10)));
	ControlList.append(new ShapeControlItem(this, ControlItemType::cCenter, QPointF(50, 50)));
	ControlList.append(new ShapeControlItem(this, ControlItemType::cEdgeNode, QPointF(90, 90)));
	itemPolygon.append(QPointF(ControlList[0]->x(), ControlList[0]->y()));
	itemPolygon.append(QPointF(ControlList[2]->x(), ControlList[2]->y()));
	for (auto elem : ControlList)
	{
		connect(elem, SIGNAL(PositionChanged()), this, SLOT(calculateShape()));
	}
	calculateShape();
}
