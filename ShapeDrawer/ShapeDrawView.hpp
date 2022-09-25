#pragma once
#include <QPainterPath>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QMenu>

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

class ShapeDrawView :public QWidget
{
public:
	ShapeDrawView(QWidget* parent = Q_NULLPTR);
	void FitShowImage(QPixmap const& pix);
	void LoadImage(QString const& fileName);
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
	/**
	 * \brief 根据在
	 * \param _polygon
	 * \param pix
	 */
	void createRegionPixmap(QVector< QPolygonF > const& _polygon, QPixmap& pix);
private:
	QMenu* m_actMenu;
	QPen pen;
	QBrush brush;
	QColor penColor;
	QColor brushColor;
	QColor bgColor;
	QPainterPath path;
	ShapeType shapeType;
	ShapeOperation shapeOperation;
	QVector< QPainterPath> comformPath;
	QVector<ShapeOperation> comformOp;
	QPolygonF polygon;
	QPointF m_pos;
	QVector<QPointF> points;
	bool drawingFlag = false;
	/**
	 * \brief 显示所有确认的图片与区域
	 */
	QPixmap curImage;
	/**
	 * \brief 保存所有确认的区域
	 */
	QVector<QPixmap> regionLayer;

};