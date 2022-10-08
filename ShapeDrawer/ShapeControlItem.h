#pragma once
#include <QObject>
#include <QAbstractGraphicsShapeItem>
#include <QPointF>
#include <QPen>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QCursor>
#include <QKeyEvent>
#include <QList>
#include <QDebug>
#include "DrawViewParams.h"

class ShapeControlItem :
	public QObject
	, public QAbstractGraphicsShapeItem
{
	Q_OBJECT
public:
	explicit ShapeControlItem(QGraphicsItemGroup* parent,
		QPointF p,
		EShapeType type, int style = 1);
private:
	QRectF bounding_rect;
	int handle_size = 5;
	int handle_active_size = 8;
	bool active_states = false;
	QColor handle_color = QColor(255, 255, 0, 100);
	QColor handle_active_color = QColor(255, 0, 0, 100);
signals:
	void PositionChanged(QPointF pos);
protected:
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
	virtual QRectF boundingRect() const override;
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
	virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	virtual void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override;
};

