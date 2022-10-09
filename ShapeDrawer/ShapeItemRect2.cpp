#include "ShapeItemRect2.h"

ShapeItemRect2::ShapeItemRect2(ShapeMode shape_mode, QPointF pos) :
	ShapeItemBase(EShapeType::sRectangle2Add)
{
	m_shape_mode = shape_mode;
	shapeInit();
}

ShapeItemRect2::~ShapeItemRect2()
{
}

void ShapeItemRect2::calculateShape()
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
			rotateShape(activeHandle->getDy());
		}
		else {

			sizeChanged();
		}
	}
}

void ShapeItemRect2::rotateShape(qreal delta)
{
	m_transform = this->transform();
	m_transform.rotate(1);
	qreal delta_angle = 1.0;
	if (delta > 0)
	{
		delta_angle = 1.0;
	}
	else
	{
		delta_angle = -1.0;
	}
	m_transform = m_transform.rotate(delta_angle);
	this->setTransform(m_transform);
}

void ShapeItemRect2::shapeInit()
{
	ControlList.append(new ShapeControlItem(this, ControlItemType::cCenter, QPointF(50, 50)));
	ControlList.append(new ShapeControlItem(this, ControlItemType::cEdgeNode, QPointF(10, 10)));
	ControlList.append(new ShapeControlItem(this, ControlItemType::cEdgeNode, QPointF(90, 10)));
	ControlList.append(new ShapeControlItem(this, ControlItemType::cEdgeNode, QPointF(90, 90)));
	ControlList.append(new ShapeControlItem(this, ControlItemType::cEdgeNode, QPointF(10, 90)));
	ControlList.append(new ShapeControlItem(this, ControlItemType::cRotate, QPointF(50, 10)));
	itemPolygon.append(QPointF(ControlList[1]->x(), ControlList[1]->y()));
	itemPolygon.append(QPointF(ControlList[2]->x(), ControlList[2]->y()));
	itemPolygon.append(QPointF(ControlList[3]->x(), ControlList[3]->y()));
	itemPolygon.append(QPointF(ControlList[4]->x(), ControlList[4]->y()));
	ControlList[2]->setVisible(false);
	ControlList[4]->setVisible(false);
	for (auto elem : ControlList)
	{
		connect(elem, SIGNAL(PositionChanged()), this, SLOT(calculateShape()));
	}
	calculateShape();
}

void ShapeItemRect2::sizeChanged()
{
	activeHandle = dynamic_cast<ShapeControlItem*>(this->sender());

	ControlList[0]->setX(0.5 * (ControlList[1]->x() + ControlList[3]->x()));
	ControlList[0]->setY(0.5 * (ControlList[1]->y() + ControlList[3]->y()));
	ControlList[5]->setX(ControlList[0]->x());
	ControlList[5]->setY(ControlList[1]->y());

	ControlList[2]->setX(ControlList[3]->x());
	ControlList[2]->setY(ControlList[1]->y());

	ControlList[4]->setX(ControlList[1]->x());
	ControlList[4]->setY(ControlList[3]->y());

	itemPolygon[0] = ControlList[1]->getCenterPointF();
	itemPolygon[1] = ControlList[2]->getCenterPointF();
	itemPolygon[2] = ControlList[3]->getCenterPointF();
	itemPolygon[3] = ControlList[4]->getCenterPointF();

	ItemPath.clear();
	ItemPath.addPolygon(itemPolygon);
	ItemPath.closeSubpath();
	ItemShape.clear();
	ItemShape.addRect(ItemPath.boundingRect());
}

