#include "RegionDrawer.hpp"

RegionDrawer::RegionDrawer(QWidget* parent) :
	m_view(new ShapeDrawView())

{
	this->resize(1280, 1024);
	m_view->resize(1280, 1024);
	m_view->show();
}
