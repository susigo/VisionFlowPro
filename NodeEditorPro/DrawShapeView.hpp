#pragma once
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QColor>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>

class DrawShapeView :public QGraphicsView
{
public:

	DrawShapeView(QWidget* parent = Q_NULLPTR);

	~DrawShapeView();
private:
	QGraphicsScene* m_scene;
	QColor m_bg_color = QColor(34, 34, 34, 255);
	QColor m_grid_color = QColor(10, 10, 10, 255);
	QColor m_hint_bg_color = QColor(0, 255, 0, 100);
	QPen m_grid_pen = QPen(m_grid_color, 3);
	QString m_hint_str;
	QPoint m_cur_pos;
private:
	void drawHintInfo(QPainter* painter);
protected:
	void mousePressEvent(QMouseEvent* event) override;

	void mouseReleaseEvent(QMouseEvent* event) override;

	void mouseMoveEvent(QMouseEvent* event) override;

	void paintEvent(QPaintEvent* event) override;

	void drawBackground(QPainter* painter, const QRectF& rect) override;

	void drawItems(QPainter* painter, int numItems, QGraphicsItem* items[], const QStyleOptionGraphicsItem options[]) override;
};

