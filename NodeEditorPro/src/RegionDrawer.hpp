#ifndef REGIONDRAWER_H
#define REGIONDRAWER_H

#include <QLabel>
#include <QFileDialog>
#include <QMenu>
#include <QPainter>
#include <QWheelEvent>
#include <QDebug>
#include <QTime>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;

#pragma execution_character_set("utf-8")

const int RECTTYPE = 1;
const int ELLIPSETYPE = 2;

class RegionDrawer : public QLabel
{
public:
	RegionDrawer(QWidget* parent = nullptr);

public:
	void setLocalFilename(QString name);
	void setPaintingType(int num);
	void setBrightness(float num);
	void setContrast(float num);
	void changePaintingState();
	void ChangeBriAndConChangedState();
	bool _rectsIsEmpty();
	bool _ellipsesIsEmpty();
	QImage Mat2QImage(const Mat& mat);       //Mat2QImage格式转换
	Mat QImageToMat(QImage image);           //QImage2Mat格式转换
	QImage ImageAdjustment(QImage image);    //改变图片的亮度和对比度

protected:
	void contextMenuEvent(QContextMenuEvent* event) override;   //右键菜单
	void paintEvent(QPaintEvent* event) override;                        //QPaint画图
	void wheelEvent(QWheelEvent* event) override;               //鼠标滚轮滚动
	void mousePressEvent(QMouseEvent* event) override;          //鼠标摁下
	void mouseMoveEvent(QMouseEvent* event) override;           //鼠标松开
	void mouseReleaseEvent(QMouseEvent* event) override;        //鼠标发射事件

public slots:
	void OnSelectImage();       //选择打开图片
	void OnZoomInImage();       //图片放大
	void OnZoomOutImage();      //图片缩小
	void OnPresetImage();       //图片还原
	void OnPaintingback();       //画图回撤

private:
	QImage Image;                    //显示的图片
	Mat image_Mat;                   //改变的图片
	qreal ZoomValue = 1.0;           //鼠标缩放值
	int XPtInterval = 0;             //平移X轴的值
	int YPtInterval = 0;             //平移Y轴的值
	int paintingType = 0;            //绘图种类
	QPoint OldPos;                   //旧的鼠标位置
	bool Pressed = false;            //鼠标是否被摁压
	bool isPainting = false;         //是否在画图
	bool isBriAndConChanged = false; //亮度对比度是否改变
	QString LocalFileName;           //图片名
	QVector<QRect> _rects;           //矩形容器
	QVector<QRect> _ellipses;        //圆形容器
	QVector<int>shapeType;           //记录绘图类型
	float brightness = 0;            //亮度
	float contrast = 1;              //对比度
	QColor bgColor = QColor(34, 34, 34);
};

#endif // REGIONDRAWER_H
