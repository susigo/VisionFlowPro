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
#include "ShapeDrawView.hpp"

class RegionDrawer :public QWidget
{
public:
	RegionDrawer(QWidget* parent = Q_NULLPTR);
private:

	ShapeDrawView* m_view;
};

#endif // REGIONDRAWER_H
