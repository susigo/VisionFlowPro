
#include "DrawShapeView.hpp"


DrawShapeView::DrawShapeView(QWidget* parent) :
	m_scene(new QGraphicsScene())
{
	if (parent != Q_NULLPTR)
	{
		this->setParent(parent);
	}
	m_scene->setSceneRect(-3000, -3000, 6000, 6000);
	this->setScene(m_scene);
	this->resize(1204, 720);
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setRenderHint(QPainter::Antialiasing);
	this->setMouseTracking(true);
	/*以鼠标中心进行缩放*/
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);//设置视口变换的锚点，这个属性控制当视图做变换时应该如何摆放场景的位置
	setResizeAnchor(QGraphicsView::AnchorUnderMouse);

}

DrawShapeView::~DrawShapeView()
{

}

void DrawShapeView::drawHintInfo(QPainter* painter)
{
	painter->setBrush(m_hint_bg_color);
	painter->setPen(Qt::NoPen);
	painter->drawRoundedRect(10, 10, 200, 25, 3, 3);
	m_hint_str.clear();
	m_hint_str = QString("mouse:%1,%2").arg(m_cur_pos.x()).arg(m_cur_pos.y());
}

void DrawShapeView::mousePressEvent(QMouseEvent* event)
{
	QGraphicsView::mousePressEvent(event);
}

void DrawShapeView::mouseReleaseEvent(QMouseEvent* event)
{
	QGraphicsView::mouseReleaseEvent(event);
}

void DrawShapeView::mouseMoveEvent(QMouseEvent* event)
{
	QGraphicsView::mouseMoveEvent(event);
	m_cur_pos = event->pos();

	this->viewport()->repaint();
}

void DrawShapeView::paintEvent(QPaintEvent* event)
{
	QPainter painter(this->viewport());
	drawHintInfo(&painter);
	QGraphicsView::paintEvent(event);
}

void DrawShapeView::drawBackground(QPainter* painter, const QRectF& rect)
{
	QGraphicsView::drawBackground(painter, rect);
	painter->setBrush(m_bg_color);
	painter->setPen(Qt::NoPen);
	painter->drawRect(rect);
	painter->setBrush(Qt::NoBrush);
	painter->setPen(m_grid_pen);
	painter->drawLine(-3000, 0, 3000, 0);
	painter->drawLine(0, -3000, 0, 3000);
	painter->setPen(Qt::white);
	painter->drawText(10, 25, m_hint_str);

}

void DrawShapeView::drawItems(QPainter* painter, int numItems, QGraphicsItem* items[],
	const QStyleOptionGraphicsItem options[])
{
	QGraphicsView::drawItems(painter, numItems, items, options);
}
