#include "RegionDrawer.hpp"

RegionDrawer::RegionDrawer(QWidget* parent)

{
	m_view = new ShapeDrawView();
	btn_comform = new QPushButton(u8"完成");
	btn_cancel = new QPushButton(u8"取消");
	m_v_host = new QVBoxLayout(this);
	m_h_host = new QHBoxLayout();

	m_v_host->addWidget(m_view);
	m_v_host->addLayout(m_h_host);
	m_h_host->addWidget(btn_comform);
	m_h_host->addWidget(btn_cancel);
	this->resize(1280, 1024);
}
