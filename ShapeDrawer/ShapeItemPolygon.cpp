#include "ShapeItemPolygon.h"

ShapeItemPolygon::ShapeItemPolygon(QPolygonF polygon, ShapeMode shape_mode, QPointF pos) :
	ShapeItemBase(EShapeType::sLine)
{
	m_shape_mode = shape_mode;
	itemPolygon = polygon;
	this->setPos(pos);
	shapeInit();
}

ShapeItemPolygon::~ShapeItemPolygon()
{
}

void ShapeItemPolygon::calculateShape()
{
	if (this->sender() == nullptr)
	{
		sizeChanged();
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
			sizeChanged();
		}
	}
}

void ShapeItemPolygon::sizeChanged()
{
	ItemPath.clear();
	int i = 0;
	for (auto elem : ControlList)
	{
		itemPolygon[i] = elem->getCenterPointF();
		i++;
	}
	ItemPath.addPolygon(itemPolygon);
	ItemPath.closeSubpath();
	ItemShape.clear();
	ItemShape.addRect(ItemPath.boundingRect());
}

void ShapeItemPolygon::shapeInit()
{
	ControlList.append(new ShapeControlItem(this, ControlItemType::cCenter, QPointF(50, 50)));
	for (auto elem : itemPolygon)
	{
		ControlList.append(new ShapeControlItem(this, cEdgeNode, elem));
	}
	for (auto elem : ControlList)
	{
		connect(elem, SIGNAL(PositionChanged()), this, SLOT(calculateShape()));
	}
	calculateShape();
}
