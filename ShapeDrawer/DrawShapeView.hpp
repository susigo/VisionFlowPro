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
#include <QScrollBar>
#include <QPainterPath>
#include <QPolygonF>
#include "DrawViewParams.h"
#include "ShapeItemLine.h"
#include "ShapeItemRect1.h"
#include "ShapeItemRect2.h"
#include "ShapeItemPolygon.h"
#include "halconcpp/HalconCpp.h"

static std::mutex draw_view_lock;
class DrawShapeView :public QGraphicsView
{
	Q_OBJECT
public:

	explicit DrawShapeView(QWidget* parent = Q_NULLPTR);

	~DrawShapeView() {};

	static DrawShapeView* instance;

	static DrawShapeView* getInst();
	//DrawShapeView(const DrawShapeView&);
	DrawShapeView& operator=(const DrawShapeView&);
private:
	class Deletor
	{
	public:
		~Deletor()
		{
			if (DrawShapeView::instance != nullptr)
			{
				delete DrawShapeView::instance;
			}
		}
	};
	static Deletor deletor;
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
	ShapeDataStruct shape_data;
	QList<ShapeItemBase*> shape_items;
	ShapeItemBase* cur_shape_item = nullptr;

	QString cur_image_name;
	QPixmap m_cur_pixmap;

	QPointF m_lastMousePos;  // 鼠标最后按下的位置
	QPointF m_centerPos;  // 
	qreal m_scale = 1.0;  // 缩放值

	QTransform m_transform;

	QMenu* m_menu;
	QGraphicsPixmapItem* m_pixmap_item;
	QGraphicsPathItem* m_draw_path_item;
	QPolygonF* m_draw_poly;
	QPainterPath tmpPath;

	QGraphicsLineItem* v_hint_line;
	QGraphicsLineItem* h_hint_line;

public:
	void FitShowImage(const QPixmap& pixmap);
	void FitShowImage(const QPixmap& pixmap, ShapeDataStruct& shape_data);
	static HalconCpp::HRegion GetHRegionFromData(const ShapeDataStruct& shape_data);
private:
	void MenuInit();
	void ParamInit();
public slots:
	void onOpenImage();
	void onFitImageShow();
	void onDrawLineShape();
	void onDrawRectangle1(ShapeMode mode);
	void onDrawRectangle2(ShapeMode mode);
	void onDrawPolygon(ShapeMode mode);
	void onDrawFreeDraw(ShapeMode mode);
	void onDrawComform();
	void onDrawCancel();
signals:
	void DrawFinished();
	void RegionComform(ShapeDataStruct shape_data);
private:
	void drawFinished();
	void drawHintInfo(QPainter* painter);
	void drawCurrentShape(QPainter* painter);
protected:
	void mousePressEvent(QMouseEvent* event) override;

	void mouseReleaseEvent(QMouseEvent* event) override;

	void mouseMoveEvent(QMouseEvent* event) override;

	void wheelEvent(QWheelEvent* event) override;

	void paintEvent(QPaintEvent* event) override;

	void drawBackground(QPainter* painter, const QRectF& rect) override;

};

