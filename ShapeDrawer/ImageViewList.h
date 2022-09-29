#ifndef IMAGEVIEWLIST_H
#define IMAGEVIEWLIST_H
/************************************************************************
  * @auther         :
  * @date           : 2021-4
  * @description    : 布局后的显示模块
  *布局多个显示模块之间的关系
  *形成整体布局、并动态调整布局、显示信息
************************************************************************/

#include <QWidget>
#include <QGridLayout>
#include "QGraphicsViews.h"

class ImageViewList : public QWidget
{
	Q_OBJECT
public:
	explicit ImageViewList(QWidget* parent = nullptr);
	//设定需要的显示控件
	void SetViewCount(int num);
	QList<QGraphicsViews*> ViewList;
private:
	QGridLayout* DispLayout;
	bool Ex = false;//全屏显示标志位
private slots:
	void Expand(QWidget* ptr);

public slots:
	//相机标号 从1开始
	void DispInfo(int CamNum, int statue, int Result = 0, QString str = "", bool All = false);
	//相机标号 从1开始  
	void DispImage(int CamNum, QImage& img);

};

#endif // IMAGEVIEWLIST_H

