#include "QGScene.h"
#include <QGraphicsSceneMouseEvent>
#include <QLineEdit>
#include <QGraphicsProxyWidget>
#include "BaseItem.h"


QGScene::QGScene(QObject* parent) : QGraphicsScene(parent)
{

}


void QGScene::UpDateZoom(qreal val)
{
	for (auto Item : this->items())
	{
		((BaseItem*)Item)->setProperty("ZoomVal", val);
	}
}

