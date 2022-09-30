#pragma once
#include <mutex>
#include <QPainterPath>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QMenu>
#include "halconcpp/HalconCpp.h"
#include "RegionPixmapData.hpp"


const int oAdd = 0;
const int oDiv = 1;

enum ShapeType
{
	tNone,
	tLine,
	tRectangle1,
	tRectangle2,
	tPolygon
};

extern std::mutex s_mutex;

class ShapeDrawView :public QWidget
{
	Q_OBJECT
public:
	ShapeDrawView(QWidget* parent = Q_NULLPTR);
	void FitShowImage(QPixmap const& pix, RegionPixmapData reg_data);
	void LoadFileImage(QString const& fileName);
	void createRegionPixmap(QVector< QPainterPath > const& _polygon, QPixmap& pix);
	void getPixmap(QPixmap& pix);
	QPixmap getPixmap();
	RegionPixmapData getRegionData();
	void getRegionData(RegionPixmapData* data) const;
	bool getDrawFlag() { return drawComform; }
	void GetHRegionFromData(HalconCpp::HRegion* reg, RegionPixmapData& data);
	HalconCpp::HRegion GetHRegionFromData(RegionPixmapData const& data);
	static ShapeDrawView* getInst();
protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;

private:
	void MenuInit();
	void drawBackGroundImage(QPainter& painter);
	void drawCurrentShape(QPainter& painter);
	void drawHintLines(QPainter& painter);
	void calculateShapePolygon();
	void DrawFinished();
signals:
	void RegionFinished(RegionPixmapData region_data);
private:
	static ShapeDrawView* instance;

	QMenu* m_actMenu;
	QPen pen;
	QBrush brush;
	QColor penColor;
	QColor brushColor;
	QColor brushDivColor;
	QColor bgColor;
	QPainterPath path;

	ShapeType shapeType;
	int shapeOperation;
	double w_ratio = 1.0;
	double h_ratio = 1.0;
	RegionPixmapData region_data;

	QPolygonF polygon;
	QPointF m_pos;
	QVector<QPointF> points;
	bool drawingFlag = false;
	bool drawComform = false;
	bool shapeCreate = true;
	/**
	 * \brief 显示所有确认的图片与区域
	 */
	QPixmap curImage;
	/**
	 * \brief 保存所有确认的区域
	 */
	QVector<QPixmap> regionLayer;

};

//extern ShapeDrawView* shapeDrawer;


