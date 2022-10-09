#include "ShapeItemBase.h"

class ShapeItemLine
	:public ShapeItemBase
{
public:
	ShapeItemLine(QPointF pos = QPointF(0, 0));

	~ShapeItemLine();
private:
	void calculateShape() override;

	void shapeInit() override;
};

