#pragma once
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QColor>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QMenu>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>

#include "DrawViewParams.h"

class DrawShapeView :public QGraphicsView
{
	Q_OBJECT
public:

	explicit DrawShapeView(QWidget* parent = Q_NULLPTR);

	~DrawShapeView() = default;
private:
	QGraphicsScene* m_scene;
	QColor m_bg_color = QColor(34, 34, 34, 255);
	QColor m_grid_color = QColor(10, 10, 10, 255);
	QColor m_hint_bg_color = QColor(0, 255, 0, 100);
	QPen m_grid_pen = QPen(m_grid_color, 3);
	QString m_hint_str;
	QPoint m_cur_pos_view;
	QPointF m_cur_pos_scene;
	QPointF m_hint_tl;

	ViewMode view_mode = ViewMode::tNone;
	EShapeType draw_shape;
	ShapeMode shape_mode;

	QString cur_image_name;
	QPixmap m_cur_pixmap;

	qreal m_translateSpeed;  // 平移速度
	qreal m_zoomDelta;  // 缩放的增量
	QPoint m_lastMousePos;  // 鼠标最后按下的位置
	qreal m_scale;  // 缩放值
	//QMatrix m_matrix;
	QTransform m_transform;

	QMenu* m_menu;
	QGraphicsTextItem* m_hint_text_item;
	QGraphicsPixmapItem* m_pixmap_item;
public:
	void FitShowImage(const QPixmap& pixmap);
private:
	void MenuInit();
	void ParamInit();
public slots:
	void onOpenImage();
	void onFitImageShow();
	void onDrawLineShape();
	void onDrawRectangle1(ShapeMode& mode);
	void onDrawRectangle2(ShapeMode& mode);
	void onDrawPolygon(ShapeMode& mode);
	void onDrawFreeDraw(ShapeMode& mode);
	void onDrawComform();
	void onDrawCancel();
signals:
	void DrawFinished();
private:
	void drawHintInfo(QPainter* painter);
protected:
	void mousePressEvent(QMouseEvent* event) override;

	void mouseReleaseEvent(QMouseEvent* event) override;

	void mouseMoveEvent(QMouseEvent* event) override;

	void paintEvent(QPaintEvent* event) override;

	void drawBackground(QPainter* painter, const QRectF& rect) override;

};

