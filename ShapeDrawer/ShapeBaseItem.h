#ifndef BASEITEM_H
#define BASEITEM_H
/************************************************************************
  * @auther         :
  * @date           : 2021-4
  * @description    : ROI模块
  *基类模块和各种子类
  *
************************************************************************/
#include "DefineDataType.h"
#include <QObject>
#include "ControlItem.h"
#include <QList>

//
//**************************************基类***************************************************
class BaseItem : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
    Q_PROPERTY(qreal ZoomVal WRITE SetZoomVal)
    Q_PROPERTY(qreal ContrSize READ GetContrSize)
public:
    enum ItemType
    {
        AutoItem =0 ,           // 自定义类型
        Circle,         // 圆
        Ellipse,            // 椭圆
        Concentric_Circle,  // 同心圆
        Pie,                // 饼
        Chord,              // 和弦
        Rectangle,          // 矩形
        RectangleR,          // 矩形
        Square,             // 正方形
        Polygon,            // 多边形
        Round_End_Rectangle,// 圆端矩形
        Rounded_Rectangle,  // 圆角矩形
        LineObj,   // 圆角矩形
        PointObj
    };
    void SetIndex(int num);
    void SetZoomVal(qreal ZoomVal){scaler=ZoomVal;};
    qreal GetContrSize() const{return ContrSize;}
    virtual bool UpDate(int index)=NULL;
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget) override;
    void DrawIndex(const QPointF& P);
protected:
    BaseItem(ItemType type);
    virtual ~BaseItem();
    virtual void focusInEvent(QFocusEvent *event) override;
    virtual void focusOutEvent(QFocusEvent *event) override;
    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
protected:
    QList<ControlItem* > ControlList;
    qreal scaler;                                   //缩放系数
    const qreal ContrSize=8;                        //控制点尺寸
    ItemType types;                                 //枚举类型
    QColor ItemColor=QColor(0, 100, 200);           //线条颜色
    QColor Background=QColor(0, 160, 230,50);       //填充背景颜色

    QString ItemDiscrib=QString::fromLocal8Bit("描述");

    QPolygonF polygon;                              //轮廓点集
    QPainterPathStroker Stroker;                    //轮廓处理工具

    QPointF Center;                                 //中心点
    QPainterPath ItemPath;                          //有边框区域
    QPainterPath PathVir;                           //无边框区域

};
//**************************************矩形*******************************************************
class RectangleItem: public BaseItem
{
    Q_OBJECT
public:
    RectangleItem(qreal x, qreal y, qreal width, qreal height);
    RectangleItem(DefRect rec);

    void SetObject(DefRect rec);
    DefRect GetObject();
protected:
    void Create(DefRect rec);
    bool UpDate(int index=-1) override;
};
//**************************************矩形 旋转*******************************************************
class RectangleRItem: public BaseItem
{
    Q_OBJECT
public:
    RectangleRItem(qreal x, qreal y, qreal Lenth1, qreal Lenth2,qreal Pi);
    RectangleRItem(DefRotateRect rec);

    void SetObject(DefRotateRect rec);
    DefRotateRect GetObject();
protected:
    void Create(DefRotateRect rec);
    bool UpDate(int index=-1) override;
protected:
    qreal angle=0;
};
//**************************************椭圆*******************************************************
class EllipseItem: public BaseItem
{
    Q_OBJECT
public:
    EllipseItem(qreal x, qreal y, qreal Lenth1, qreal Lenth2,qreal Pi);
    EllipseItem(DefRotateEllipse rec);

    void SetObject(DefRotateEllipse rec);
    DefRotateEllipse GetObject();
protected:
    void Create(DefRotateEllipse rec);
    bool UpDate(int index=-1) override;
protected:
    qreal angle=0;
    qreal angleDEG=0;
};
//**************************************圆*******************************************************
class CircleItem:public BaseItem
{
    Q_OBJECT
public:
    CircleItem(qreal x, qreal y, qreal R);
    CircleItem(DefCircle circle);

    void SetObject(DefCircle circle);
    DefCircle GetObject();
protected:
    void Create(DefCircle circle);
    bool UpDate(int index=-1) override;
    qreal Radius;
};
//**************************************同心圆*********************************************************
class ConcentricCircleItem:public BaseItem
{
    Q_OBJECT
public:
    ConcentricCircleItem(qreal x, qreal y, qreal RadiusMin, qreal RadiusMax);
    ConcentricCircleItem(DefConcentricCircle concentricCircle);

    void SetObject(DefConcentricCircle concentricCircle);
    DefConcentricCircle GetObject();
protected:
    void Create(DefConcentricCircle concentricCircle);
    bool UpDate(int index=-1) override;
    qreal RadiusMin;
    qreal RadiusMax;
};
//**************************************饼图*********************************************************
class PieItem:public BaseItem
{
    Q_OBJECT
public:
    PieItem(qreal x, qreal y,qreal radius, qreal startAng, qreal endAng);
    PieItem(DefPie defPie);

    void SetObject(DefPie defPie);
    DefPie GetObject();
protected:
    void Create(DefPie defPie);
    bool UpDate(int index=-1) override;
    qreal Radius;
    qreal StartAng;
    qreal EndAng;
};
//**************************************多边形*********************************************************
class PolygonItem:public BaseItem
{
    Q_OBJECT
public:
    PolygonItem(QVector<QPointF> pList);
    PolygonItem(DefPolygon defPolygon);

    void AddPoint(QPointF p,int index=0);
    void SetObject(DefPolygon defPolygon);
    DefPolygon GetObject();
protected:
    void Create(DefPolygon defPolygon);
    bool UpDate(int index=-1) override;
};
//**************************************圆端矩形*********************************************************
class RoundRectangle1Item:public BaseItem
{
    Q_OBJECT//
public:
    RoundRectangle1Item(qreal x, qreal y, qreal width, qreal height,qreal r);
    RoundRectangle1Item(DefRoundRect roundRect);

    void SetObject(DefRoundRect roundRect);
    DefRoundRect GetObject();
protected:
    void Create(DefRoundRect roundRect);
    bool UpDate(int index=-1) override;
    qreal Width;
    qreal Height;
    qreal Radius;
};
//**************************************直线*********************************************************
class LineItem:public BaseItem
{
    Q_OBJECT
public:
    LineItem(qreal x1, qreal y1,qreal x2, qreal y2);
    LineItem(DefLine defLine);

    void SetObject(DefLine defLine);
    DefLine GetObject();
protected:
    bool UpDate(int index=-1) override;
    void Create(DefLine defLine);

};
//**************************************卡尺*********************************************************
class Caliper:public BaseItem
{
    Q_OBJECT
public:
    Caliper(qreal x1, qreal y1,qreal x2, qreal y2,qreal height);
    Caliper(DefCaliper defCaliper);

    void SetObject(DefCaliper defCaliper);
    DefCaliper GetObject();

protected:
    bool UpDate(int index=-1) override;
    void Create(DefCaliper defCaliper);

    QPointF P1Glob;
    QPointF P2Glob;
    QPointF P1;
    QPointF P2;
    qreal Height;
    qreal angle;
    qreal Lenth;
};
//**************************************点*********************************************************
class PointItem:public BaseItem
{
    Q_OBJECT
public:
    PointItem(qreal x, qreal y);
    PointItem(DefPoint defPoint);

    void SetObject(DefPoint defPoint);
    DefPoint GetObject();
protected:
    bool UpDate(int index=-1) override;
    void Create(DefPoint defPoint);
};



#endif // BASEITEM_H

