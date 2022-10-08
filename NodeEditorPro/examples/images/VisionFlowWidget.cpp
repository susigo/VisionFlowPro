#include "VisionFlowWidget.hpp"

VisionFlowWidget::VisionFlowWidget(QWidget* parent)
{
	main_layout = new QVBoxLayout();
	header_layout = new QHBoxLayout();
	btn_load_scheme = new QPushButton("Load");
	btn_save_scheme = new QPushButton("Save");
	btn_clear_scene = new QPushButton("Clear");
	btn_test = new QPushButton("test");
	header_layout->setAlignment(Qt::AlignLeft);
	btn_load_scheme->setFixedWidth(120);
	btn_save_scheme->setFixedWidth(120);
	btn_clear_scene->setFixedWidth(120);
	this->setLayout(main_layout);
	main_layout->addLayout(header_layout);
	header_layout->addWidget(btn_load_scheme);
	header_layout->addWidget(btn_save_scheme);
	header_layout->addWidget(btn_clear_scene);
	header_layout->addWidget(btn_test);
	header_layout->setContentsMargins(0, 0, 0, 0);
	main_layout->setContentsMargins(1, 1, 1, 1);
	main_layout->setSpacing(1);
	m_scene = new FlowScene(registerDataModels());
	m_view = new FlowView(m_scene);
	main_layout->addWidget(m_view);
	this->setWindowTitle(QStringLiteral("节点编辑器"));
	this->resize(1280, 1024);
	setConnection();
}

VisionFlowWidget::~VisionFlowWidget()
{

}

void VisionFlowWidget::setConnection()
{
	connect(btn_load_scheme, &QPushButton::clicked, [=]()
		{
			m_scene->load();
		});
	connect(btn_save_scheme, &QPushButton::clicked, [=]()
		{
			m_scene->save();
		});
	connect(btn_clear_scene, &QPushButton::clicked, [=]()
		{
			m_scene->clearScene();
		});
	connect(btn_test, &QPushButton::clicked, [=]()
		{
			DrawShapeView::getInst()->show();
		});
}
