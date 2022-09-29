#ifndef CONTROLITEM_H
#define CONTROLITEM_H
/************************************************************************
  * @auther         :
  * @date           : 2021-4
  * @description    : 控制点
  * ROI模块中美的控制点
  * 中心点ju'xin矩形 点击后可移动
  * 控制点圆形、点击后可以拖动
************************************************************************/

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


class ControlItem : public QObject, public QAbstractGraphicsShapeItem
{
    Q_OBJECT
public:
    explicit ControlItem(QGraphicsItemGroup* parent, QPointF p, int type, int style=1);
    QPointF GetPoint();
    QPointF GetBefPoint(){return BefPoint;}
    void SetPoint(QPointF p);
    qreal dX(){return dx;};
    qreal dY(){return dy;};

    int index=0;
    int Style=1;
protected:
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;


private:
    QPainterPath ItemPath;
    QPen myPen = this->pen();
    QPointF point;
    int pointType;

    qreal dx;
    qreal dy;
    QPointF BefPoint;

private:
    qreal RecSize;
    qreal PointSize;
};

#endif // CONTROLITEM_H

