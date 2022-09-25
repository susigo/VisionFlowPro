#pragma once

#include <QWidget>
#include <QLabel>
#include <QGraphicsView>
#include <QPixmap>
#include <QPainter>
#include "halconcpp/HalconCpp.h"
using namespace HalconCpp;

class HImageViewWidget
	:public QLabel
{
public:
	HImageViewWidget(QWidget* parent = Q_NULLPTR);
	virtual ~HImageViewWidget() {}
	void showImage(HImage const& _himg);
public:
	static void HImageToQPixmap(HImage const& _img, QPixmap& tar_img);
	static bool QImage2HImage(QImage& from, HalconCpp::HImage& to);
	static void QPixmapToHRegion(QPixmap const& _pix, HRegion& tar_img);
protected:
	void paintEvent(QPaintEvent* event) override;
private:
	HImage cur_image;
	QPixmap* cur_pixmap;
	// 实例化画家对象，this指定的是绘图设备
	QPainter painter;
};

