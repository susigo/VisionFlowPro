#ifndef REGIONDRAWER_H
#define REGIONDRAWER_H

#include <mutex>
#include <QLabel>
#include <QFileDialog>
#include <QMenu>
#include <QPainter>
#include <QPainterPath>
#include <QWheelEvent>
#include <QDebug>
#include <QTime>
#include <QVBoxLayout>
#include <QPushButton>
#include "halconcpp/HalconCpp.h"
#include "ShapeDrawView.hpp"
#include "halcon/HImageViewWidget.hpp"

class RegionDrawer :public QWidget
{
public:
	RegionDrawer(QWidget* parent = Q_NULLPTR);
	~RegionDrawer();
	void ShowHImage(HalconCpp::HImage& h_image);
	QPixmap GetPixmap();
private:
	QPushButton* btn_comform;
	QPushButton* btn_cancel;
	QVBoxLayout* m_v_host;
	QHBoxLayout* m_h_host;
	ShapeDrawView* m_view;
	QPixmap m_pixmap;
public:
	bool viewResult = false;
	static std::shared_ptr< RegionDrawer> instance;
	static std::mutex s_mutex;
public:
	static std::shared_ptr< RegionDrawer> GetInst();
};



#endif // REGIONDRAWER_H
