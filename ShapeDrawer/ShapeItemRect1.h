#include "ShapeItemBase.h"

class ShapeItemRect1
	:public ShapeItemBase
{
public:
	ShapeItemRect1(ShapeMode shape_mode);

	~ShapeItemRect1();
private:
	void calculateShape() override;

	void shapeInit() override;

	void sizeChanged();
};

