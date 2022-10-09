#include "ShapeItemBase.h"

class ShapeItemLine
	:public ShapeItemBase
{
public:
	ShapeItemLine();

	~ShapeItemLine();
private:
	void calculateShape() override;

	void shapeInit() override;
};

