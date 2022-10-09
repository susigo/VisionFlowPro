#include "ShapeItemBase.h"

class ShapeItemRect2
	:public ShapeItemBase
{
public:
	ShapeItemRect2(ShapeMode shape_mode, QPointF pos = QPointF(0, 0));

	~ShapeItemRect2();
private:
	QTransform m_transform;
private:
	void calculateShape() override;

	void rotateShape(qreal delta) override;

	void shapeInit() override;

	void sizeChanged();
};

