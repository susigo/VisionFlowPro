
#include "DrawShapeView.hpp"

#include <QFileDialog>


DrawShapeView::DrawShapeView(QWidget* parent) :
	m_scene(new QGraphicsScene())
{
	if (parent != Q_NULLPTR)
	{
		this->setParent(parent);
	}
	m_scene->setSceneRect(-3000, -3000, 6000, 6000);
	this->setScene(m_scene);
	this->resize(1204, 720);
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setRenderHint(QPainter::Antialiasing);
	this->setMouseTracking(true);
	/*以鼠标中心进行缩放*/
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);//设置视口变换的锚点，这个属性控制当视图做变换时应该如何摆放场景的位置
	setResizeAnchor(QGraphicsView::AnchorUnderMouse);
	MenuInit();
	ParamInit();
}

void DrawShapeView::FitShowImage(const QPixmap& pixmap)
{
	if (pixmap != m_cur_pixmap)
	{
		m_cur_pixmap = pixmap;
	}
	m_pixmap_item->setPixmap(m_cur_pixmap);
	qreal w_ratio = 1.0 * this->width() / m_cur_pixmap.width();
	qreal h_ratio = 1.0 * this->height() / m_cur_pixmap.height();
	if (w_ratio > h_ratio)
	{
		m_scale = h_ratio;
	}
	else
	{
		m_scale = w_ratio;
	}
	m_transform = this->transform();

	m_transform.setMatrix(
		m_scale, m_transform.m12(), m_transform.m13(),
		m_transform.m21(), m_scale, m_transform.m23(),
		m_cur_pixmap.width() * 0.5, m_cur_pixmap.height() * 0.5, m_transform.m33()
	);
	this->setTransform(m_transform);
}

void DrawShapeView::MenuInit()
{
	m_menu = new QMenu(QStringLiteral("右键菜单"), this);
	QAction* actOpenImage = new QAction(QStringLiteral("选择图片"));
	QAction* actFitImage = new QAction(QStringLiteral("重置显示"));
	QAction* actDrawLine = new QAction(QStringLiteral("绘制直线"));
	QMenu* mAddShape = new QMenu(QStringLiteral("相加图形"));
	QAction* actDrawRectangle1Add = new QAction(QStringLiteral("正矩形"));
	QAction* actDrawRectangle2Add = new QAction(QStringLiteral("斜矩形"));
	QAction* actDrawPolygonAdd = new QAction(QStringLiteral("多边形"));
	QAction* actDrawFreeDrawAdd = new QAction(QStringLiteral("涂鸦"));
	mAddShape->addAction(actDrawRectangle1Add);
	mAddShape->addAction(actDrawRectangle2Add);
	mAddShape->addAction(actDrawPolygonAdd);
	mAddShape->addAction(actDrawFreeDrawAdd);

	QMenu* mDivShape = new QMenu(QStringLiteral("相减图形"));
	QAction* actDrawRectangle1Div = new QAction(QStringLiteral("正矩形"));
	QAction* actDrawRectangle2Div = new QAction(QStringLiteral("斜矩形"));
	QAction* actDrawPolygonDiv = new QAction(QStringLiteral("多边形"));
	QAction* actDrawFreeDrawDiv = new QAction(QStringLiteral("涂鸦"));
	mDivShape->addAction(actDrawRectangle1Div);
	mDivShape->addAction(actDrawRectangle2Div);
	mDivShape->addAction(actDrawPolygonDiv);
	mDivShape->addAction(actDrawFreeDrawDiv);

	QAction* actComform = new QAction(QStringLiteral("确认"));
	QAction* actCancel = new QAction(QStringLiteral("取消"));
	m_menu->addAction(actOpenImage);
	m_menu->addAction(actFitImage);
	m_menu->addMenu(mAddShape);
	m_menu->addMenu(mDivShape);
	m_menu->addAction(actComform);
	m_menu->addAction(actCancel);

	connect(actOpenImage, SIGNAL(triggered()), this, SLOT(onOpenImage()));
	connect(actFitImage, SIGNAL(triggered()), this, SLOT(onFitImageShow()));
	connect(actDrawRectangle1Add, SIGNAL(triggered()), this, SLOT(onOpenImage(ShapeMode::mAdd)));
	connect(actDrawRectangle2Add, SIGNAL(triggered()), this, SLOT(onOpenImage(ShapeMode::mAdd)));
	connect(actDrawPolygonAdd, SIGNAL(triggered()), this, SLOT(onOpenImage(ShapeMode::mAdd)));
	connect(actDrawFreeDrawAdd, SIGNAL(triggered()), this, SLOT(onOpenImage(ShapeMode::mAdd)));

	connect(actDrawRectangle1Div, SIGNAL(triggered()), this, SLOT(onOpenImage(ShapeMode::mDiv)));
	connect(actDrawRectangle2Div, SIGNAL(triggered()), this, SLOT(onOpenImage(ShapeMode::mDiv)));
	connect(actDrawPolygonDiv, SIGNAL(triggered()), this, SLOT(onOpenImage(ShapeMode::mDiv)));
	connect(actDrawFreeDrawDiv, SIGNAL(triggered()), this, SLOT(onOpenImage(ShapeMode::mDiv)));

	connect(actComform, SIGNAL(triggered()), this, SLOT(onDrawComform()));
	connect(actCancel, SIGNAL(triggered()), this, SLOT(onDrawCancel()));

}

void DrawShapeView::ParamInit()
{
	m_hint_text_item = new QGraphicsTextItem();
	m_pixmap_item = new QGraphicsPixmapItem();
	m_hint_text_item->setZValue(99);
	m_pixmap_item->setZValue(0);
	m_hint_text_item->setDefaultTextColor(m_hint_bg_color);
	m_scene->addItem(m_pixmap_item);
	m_scene->addItem(m_hint_text_item);
}

void DrawShapeView::onOpenImage()
{
	cur_image_name.clear();
	cur_image_name = QFileDialog::getOpenFileName(nullptr,
		"Select an image",
		"C:",
		"Images (*.png *.jpeg *.jpg *.tiff *.bmp)"
	);
	if (cur_image_name.length() == 0)
	{
		return;
	}
	if (m_cur_pixmap.load(cur_image_name))
	{

		FitShowImage(m_cur_pixmap);
	}
	this->repaint();
}

void DrawShapeView::onFitImageShow()
{
	FitShowImage(m_cur_pixmap);
}

void DrawShapeView::onDrawLineShape()
{
}

void DrawShapeView::onDrawRectangle1(ShapeMode& mode)
{
}

void DrawShapeView::onDrawRectangle2(ShapeMode& mode)
{
}

void DrawShapeView::onDrawPolygon(ShapeMode& mode)
{
}

void DrawShapeView::onDrawFreeDraw(ShapeMode& mode)
{
}

void DrawShapeView::onDrawComform()
{
}

void DrawShapeView::onDrawCancel()
{
}

void DrawShapeView::drawHintInfo(QPainter* painter)
{
	m_hint_str.clear();
	m_hint_str = QString("mouse:%1,%2").arg(m_cur_pos_view.x()).arg(m_cur_pos_view.y());
	m_hint_str.append(QString(" - mouse scene:%1,%2").arg(m_cur_pos_scene.x()).arg(m_cur_pos_scene.y()));

	m_hint_tl = mapToScene(0, 0);
	m_hint_text_item->setPos(m_hint_tl);
	m_hint_text_item->setPlainText(m_hint_str);
}

void DrawShapeView::mousePressEvent(QMouseEvent* event)
{
	QGraphicsView::mousePressEvent(event);
	if (event->button() == Qt::RightButton)
	{
		if (view_mode != ViewMode::tDrawing)
		{
			m_menu->exec(mapToGlobal(event->pos()));
		}
	}
}

void DrawShapeView::mouseReleaseEvent(QMouseEvent* event)
{
	QGraphicsView::mouseReleaseEvent(event);
}

void DrawShapeView::mouseMoveEvent(QMouseEvent* event)
{
	QGraphicsView::mouseMoveEvent(event);
	m_cur_pos_view = event->pos();
	m_cur_pos_scene = mapToScene(m_cur_pos_view);
	this->viewport()->repaint();
}

void DrawShapeView::paintEvent(QPaintEvent* event)
{
	QPainter painter(this->viewport());
	QGraphicsView::paintEvent(event);
}

void DrawShapeView::drawBackground(QPainter* painter, const QRectF& rect)
{
	QGraphicsView::drawBackground(painter, rect);
	painter->setBrush(m_bg_color);
	painter->setPen(Qt::NoPen);
	painter->drawRect(rect);
	painter->setBrush(Qt::NoBrush);
	painter->setPen(m_grid_pen);
	painter->drawLine(-3000, 0, 3000, 0);
	painter->drawLine(0, -3000, 0, 3000);

	drawHintInfo(painter);

}

