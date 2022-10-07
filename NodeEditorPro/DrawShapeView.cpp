
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

	m_transform.reset();

	m_transform.scale(m_scale, m_scale);
	this->setTransform(m_transform);
	m_centerPos = QPointF(m_cur_pixmap.width() * 0.5, m_cur_pixmap.height() * 0.5);
	centerOn(m_centerPos);
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
	m_menu->addAction(actDrawLine);
	m_menu->addMenu(mAddShape);
	m_menu->addMenu(mDivShape);
	m_menu->addAction(actComform);
	m_menu->addAction(actCancel);

	connect(actOpenImage, SIGNAL(triggered()), this, SLOT(onOpenImage()));
	connect(actFitImage, SIGNAL(triggered()), this, SLOT(onFitImageShow()));
	connect(actDrawLine, SIGNAL(triggered()), this, SLOT(onDrawLineShape()));

	connect(actDrawRectangle1Add, &QAction::triggered, this, [=]() {
		this->onDrawRectangle1(ShapeMode::mAdd); });
	connect(actDrawRectangle2Add, &QAction::triggered, this, [=]() {
		this->onDrawRectangle2(ShapeMode::mAdd); });
	connect(actDrawPolygonAdd, &QAction::triggered, this, [=]() {
		this->onDrawPolygon(ShapeMode::mAdd); });
	connect(actDrawFreeDrawAdd, &QAction::triggered, this, [=]() {
		this->onDrawFreeDraw(ShapeMode::mAdd); });

	connect(actDrawRectangle1Div, &QAction::triggered, this, [=]() {
		this->onDrawRectangle1(ShapeMode::mDiv); });
	connect(actDrawRectangle2Div, &QAction::triggered, this, [=]() {
		this->onDrawRectangle2(ShapeMode::mDiv); });
	connect(actDrawPolygonDiv, &QAction::triggered, this, [=]() {
		this->onDrawPolygon(ShapeMode::mDiv); });
	connect(actDrawFreeDrawDiv, &QAction::triggered, this, [=]() {
		this->onDrawFreeDraw(ShapeMode::mDiv); });

	connect(actComform, SIGNAL(triggered()), this, SLOT(onDrawComform()));
	connect(actCancel, SIGNAL(triggered()), this, SLOT(onDrawCancel()));

}

void DrawShapeView::ParamInit()
{
	m_pixmap_item = new QGraphicsPixmapItem();
	m_draw_path_item = new QGraphicsPathItem();
	m_draw_poly = new QPolygonF();
	v_hint_line = new QGraphicsLineItem();
	h_hint_line = new QGraphicsLineItem();
	m_pixmap_item->setZValue(0);
	m_scene->addItem(m_pixmap_item);
	m_scene->addItem(m_draw_path_item);

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
	draw_shape = EShapeType::sLine;
	view_mode = ViewMode::tDrawing;
	m_scene->addItem(v_hint_line);
	m_scene->addItem(h_hint_line);
}

void DrawShapeView::onDrawRectangle1(ShapeMode mode)
{
	if (mode == ShapeMode::mAdd)
	{
		draw_shape = EShapeType::sRectangle1Add;
	}
	else
	{
		draw_shape = EShapeType::sRectangle1Div;
	}
	view_mode = ViewMode::tDrawing;
	m_scene->addItem(v_hint_line);
	m_scene->addItem(h_hint_line);
}

void DrawShapeView::onDrawRectangle2(ShapeMode mode)
{
	if (mode == ShapeMode::mAdd)
	{
		draw_shape = EShapeType::sRectangle2Add;
	}
	else
	{
		draw_shape = EShapeType::sRectangle2Div;
	}
	view_mode = ViewMode::tDrawing;
	m_scene->addItem(v_hint_line);
	m_scene->addItem(h_hint_line);
}

void DrawShapeView::onDrawPolygon(ShapeMode mode)
{
	if (mode == ShapeMode::mAdd)
	{
		draw_shape = EShapeType::sPolygonAdd;
	}
	else
	{
		draw_shape = EShapeType::sPolygonDiv;
	}
	view_mode = ViewMode::tDrawing;
	m_scene->addItem(v_hint_line);
	m_scene->addItem(h_hint_line);
}

void DrawShapeView::onDrawFreeDraw(ShapeMode mode)
{
}

void DrawShapeView::onDrawComform()
{
}

void DrawShapeView::onDrawCancel()
{
}

void DrawShapeView::drawFinished()
{
	m_scene->removeItem(v_hint_line);
	m_scene->removeItem(h_hint_line);
	if (m_draw_poly->count() > 1)
	{
		m_draw_poly->last() = m_draw_poly->first();
		shape_data.shapePolygon.append(*m_draw_poly);
		shape_data.shapeType.append(draw_shape);
		tmpPath.clear();
		tmpPath.addPolygon(*m_draw_poly);
		m_draw_path_item->setPath(tmpPath);
		m_draw_poly->clear();
	}
	view_mode = ViewMode::tNone;
	shape_mode = ShapeMode::mNone;
	draw_shape = EShapeType::sNone;
}

void DrawShapeView::drawHintInfo(QPainter* painter)
{
	m_hint_str.clear();
	m_hint_str = QString("mouse:%1,%2").arg(m_cur_pos_view.x()).arg(m_cur_pos_view.y());
	m_hint_str.append(QString(" - mouse scene:%1,%2").arg(m_cur_pos_scene.x()).arg(m_cur_pos_scene.y()));
	painter->setPen(Qt::white);
	painter->setBrush(Qt::NoBrush);
	painter->drawText(10, 25, m_hint_str);
}

void DrawShapeView::drawCurrentShape(QPainter* painter)
{
	painter->setPen(Qt::NoPen);
	painter->setBrush(m_hint_bg_color);
	for (auto tmp_polygon : shape_data.shapePolygon) {
		QPainterPath tmp_view_path;
		QPolygonF tmp_polygon_view;
		for (auto _point : tmp_polygon)
		{
			tmp_polygon_view.append(mapFromScene(_point));
		}
		if (!tmp_polygon_view.isEmpty())
		{
			tmp_view_path.addPolygon(tmp_polygon_view);
			painter->drawPath(tmp_view_path);
		}
	}
}

void DrawShapeView::mousePressEvent(QMouseEvent* event)
{
	QGraphicsView::mousePressEvent(event);
	m_lastMousePos = mapToScene(event->pos());
	if (event->button() == Qt::RightButton)
	{
		if (view_mode != ViewMode::tDrawing)
		{
			m_menu->exec(mapToGlobal(event->pos()));
		}
		else
		{
			drawFinished();
		}
	}
	else if (event->button() == Qt::LeftButton)
	{
		if (view_mode == ViewMode::tDrawing)
		{
			if (m_draw_poly->count() == 0)
			{
				m_draw_poly->append(m_lastMousePos);
			}
			m_draw_poly->append(m_lastMousePos);
		}
		switch (draw_shape)
		{
		case EShapeType::sLine:
		case EShapeType::sRectangle1Add:
		case EShapeType::sRectangle1Div:
			if (m_draw_poly->count() == 3)
			{
				drawFinished();
			}
			break;
		case EShapeType::sRectangle2Add:
		case EShapeType::sRectangle2Div:
			if (m_draw_poly->count() == 4)
			{
				drawFinished();
			}
			break;
		default:
			break;
		}
	}
	else if (event->button() == Qt::MiddleButton)
	{

		view_mode = ViewMode::tTranslate;

	}
}

void DrawShapeView::mouseReleaseEvent(QMouseEvent* event)
{
	QGraphicsView::mouseReleaseEvent(event);
	if (event->button() == Qt::RightButton)
	{
	}
	else if (event->button() == Qt::LeftButton)
	{
	}
	else if (event->button() == Qt::MiddleButton)
	{
		if (view_mode != ViewMode::tDrawing)
		{
			view_mode = ViewMode::tNone;
		}
	}
}

void DrawShapeView::mouseMoveEvent(QMouseEvent* event)
{
	QGraphicsView::mouseMoveEvent(event);
	m_cur_pos_view = event->pos();
	m_cur_pos_scene = mapToScene(m_cur_pos_view);

	if (view_mode == ViewMode::tTranslate)
	{
		m_centerPos.setX(m_centerPos.x() - m_cur_pos_scene.x() + m_lastMousePos.x());
		m_centerPos.setY(m_centerPos.y() - m_cur_pos_scene.y() + m_lastMousePos.y());
		centerOn(m_centerPos);
	}
	if (view_mode == ViewMode::tDrawing)
	{
		v_hint_line->setLine(
			m_cur_pos_scene.x(),
			m_cur_pos_scene.y() - 1000,
			m_cur_pos_scene.x(),
			m_cur_pos_scene.y() + 1000);
		h_hint_line->setLine(
			m_cur_pos_scene.x() - 1000,
			m_cur_pos_scene.y(),
			m_cur_pos_scene.x() + 1000,
			m_cur_pos_scene.y());
		if (m_draw_poly->count() > 1)
		{
			m_draw_poly->last() = m_cur_pos_scene;
			tmpPath.clear();
			tmpPath.addPolygon(*m_draw_poly);
			m_draw_path_item->setPath(tmpPath);
		}
	}
	this->viewport()->repaint();
}

void DrawShapeView::wheelEvent(QWheelEvent* event)
{
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	qreal scaleFactor = 1.0;
	if (event->delta() > 0)
	{
		scaleFactor = 1.2;
	}
	else
	{
		scaleFactor = 1 / 1.2;
	}
	qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
	if (factor < 0.01 || factor > 2000)
		return;
	scale(scaleFactor, scaleFactor);
}

void DrawShapeView::paintEvent(QPaintEvent* event)
{
	QGraphicsView::paintEvent(event);
	QPainter painter(this->viewport());
	drawCurrentShape(&painter);
	drawHintInfo(&painter);
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
}

