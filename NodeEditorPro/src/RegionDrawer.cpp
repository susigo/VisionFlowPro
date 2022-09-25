#include "RegionDrawer.hpp"


//在类的cpp文件定义static变量
std::shared_ptr< RegionDrawer> RegionDrawer::instance = nullptr;
std::mutex RegionDrawer::s_mutex;

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

RegionDrawer::~RegionDrawer()
{

}

void RegionDrawer::ShowHImage(HalconCpp::HImage& h_image)
{
	HImageViewWidget::HImageToQPixmap(h_image, m_pixmap);
	m_view->FitShowImage(m_pixmap);
	this->show();
}

std::shared_ptr< RegionDrawer> RegionDrawer::GetInst()
{
	if (instance != nullptr)
	{
		return instance;
	}

	s_mutex.lock();
	if (instance != nullptr)
	{
		s_mutex.unlock();
		return instance;
	}
	instance = std::shared_ptr<RegionDrawer>(new RegionDrawer());
	s_mutex.unlock();
	return instance;
}
