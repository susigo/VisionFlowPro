#ifndef QGView_H
#define QGView_H
/************************************************************************
  * @auther         :
  * @date           : 2021-4
  * @description    : 图像显示模块
  *滚轮控制缩放
  *双击还原最佳状态
  * 左键拖动
  * 可以添加ROI
  * ROI的线条不随缩放发生变化
************************************************************************/

#include <QGraphicsView>
#include "BaseItem.h"

class QLabel;
class QMouseEvent;
class QGScene;
class ImageItem;
class QGView : public QGraphicsView
{
    Q_OBJECT
public:
    QGView(QWidget *parent = nullptr);
    void ClearObj();
    QGScene* scene;
    QImage ImageCatch;
public slots:
    void DispImage(QImage& Image); //显示图片
    void AddItems(BaseItem *item); //添加ROI

    void DispItem(const QPainterPath& ItemPath,bool onImage=false); //绘制图形,在原图上绘制或蒙板上绘制
signals:
    void ScaleChange(qreal scale);
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void drawForeground(QPainter *painter, const QRectF &rect) override;
    virtual void drawBackground(QPainter *painter, const QRectF &rect) override;
    virtual void paintEvent(QPaintEvent *event) override;
private:
    void ZoomFrame(double value);
    void GetFit();
    double ZoomValue=1;
    double ZoomFit=1;
    double PixX=0;
    double PixY=0;
    QPixmap image;
    ImageItem* item;
    QWidget* StatueLight;
    QWidget* StatueInfo;
    QWidget* fWidget;
    QLabel* GrayValue;//显示灰度值
private:
    int MRSize=8;//拖拽区域大小
    QColor color1=QColor(255, 255, 255);        //颜色1
    QColor color2=QColor(0, 100, 100, 150);     //颜色2
    QPixmap bgPix=QPixmap(64, 64);              //背景图片

    QPixmap BackgroundImg;
    void drawBg();
signals:
    void Expand(QWidget* Prt);
};

#endif // QGView_H

