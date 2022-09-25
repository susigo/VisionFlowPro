#ifndef REGIONDRAWER_H
#define REGIONDRAWER_H

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

#include "ShapeDrawView.hpp"

class RegionDrawer :public QWidget
{
public:
	RegionDrawer(QWidget* parent = Q_NULLPTR);
private:
	QPushButton* btn_comform;
	QPushButton* btn_cancel;
	QVBoxLayout* m_v_host;
	QHBoxLayout* m_h_host;
	ShapeDrawView* m_view;
};



#endif // REGIONDRAWER_H
