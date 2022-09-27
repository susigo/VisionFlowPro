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

enum ShapeOperation
{
	oAdd,
	oDiv
};

enum ShapeType
{
	tNone,
	tLine,
	tRectangle1,
	tRectangle2,
	tPolygon
};

class RegionPixmapData
{
public:
	int width = 512;
	int height = 512;
	QList<QPolygonF> comformPolygon;
	QList<QPainterPath> comformPath;
	QList<ShapeOperation> comformOp;
public:
	RegionPixmapData()
	{
		width = 512;
		height = 512;

		comformPolygon = QList<QPolygonF>();
		comformPath = QList<QPainterPath>();
		comformOp = QList<ShapeOperation>();
		comformOp.clear();
		comformPath.clear();
		comformPolygon.clear();
	}
	~RegionPixmapData()
	{

	}
};

static std::mutex s_mutex;
class ShapeDrawView :public QWidget
{
	Q_OBJECT
public:
	ShapeDrawView(QWidget* parent = Q_NULLPTR);
	void FitShowImage(QPixmap const& pix, RegionPixmapData reg_data);
	void LoadImage(QString const& fileName);
	/**
	 * \brief 根据在
	 * \param _polygon
	 * \param pix
	 */
	void createRegionPixmap(QVector< QPainterPath > const& _polygon, QPixmap& pix);
	void getPixmap(QPixmap& pix);
	QPixmap getPixmap();
	RegionPixmapData getRegionData();
	void getRegionData(RegionPixmapData* data) const;
	bool getDrawFlag() { return drawComform; }
	static ShapeDrawView* GetInst()
	{
		if (instance == nullptr)
		{
			instance = new ShapeDrawView();
		}
		//s_mutex.lock();
		if (instance != nullptr)
		{
			//s_mutex.unlock();
			return instance;
		}
		instance = new ShapeDrawView();
		//s_mutex.unlock();
		return instance;
	}
	static void GetHRegionFromData(HalconCpp::HRegion* reg, RegionPixmapData& data);
protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
private:
	void MenuInit();
	void drawStart();
	void drawFinish();
	void drawBackGroundImage(QPainter& painter);
	void drawCurrentShape(QPainter& painter);
	void drawHintLines(QPainter& painter);

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
	ShapeOperation shapeOperation;

	RegionPixmapData region_data;

	QPolygonF polygon;
	QPointF m_pos;
	QVector<QPointF> points;
	bool drawingFlag = false;
	bool drawComform = false;
	/**
	 * \brief 显示所有确认的图片与区域
	 */
	QPixmap curImage;
	/**
	 * \brief 保存所有确认的区域
	 */
	QVector<QPixmap> regionLayer;

};




