#include "ShapeItemBase.h"

class ShapeItemPolygon
	:public ShapeItemBase
{
public:
	ShapeItemPolygon(QPolygonF polygon, ShapeMode shape_mode, QPointF pos = QPointF(0, 0));

	~ShapeItemPolygon();
private:
	void calculateShape() override;
	void sizeChanged();
	void shapeInit() override;
};

