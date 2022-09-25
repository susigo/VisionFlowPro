#include "ShapeDrawView.hpp"
#include <QFileDialog>

ShapeDrawView::ShapeDrawView(QWidget* parent)
{
	if (parent != Q_NULLPTR)
	{
		this->setParent(parent);
	}
	penColor = QColor(0, 255, 0, 200);
	brushColor = QColor(0, 255, 0, 100);
	brushDivColor = QColor(255, 0, 0, 100);
	bgColor = QColor(34, 34, 34);
	pen = QPen(penColor, 2);
	brush = QBrush(brushColor);
	this->setMouseTracking(true);

	MenuInit();

	//LoadImage("C:/Users/Public/Documents/MVTec/HALCON-20.11-Steady/examples/images/die/die_01.png");
}

void ShapeDrawView::FitShowImage(QPixmap const& pix)
{
	if (curImage != pix)
	{
		curImage = pix;
	}
	this->update();
}

void ShapeDrawView::LoadImage(QString const& fileName)
{
	if (!curImage.load(fileName))
	{
		return;
	}
	FitShowImage(curImage);
}

void ShapeDrawView::mousePressEvent(QMouseEvent* event)
{
	QWidget::mousePressEvent(event);
	m_pos = event->pos();
	if (event->button() == Qt::LeftButton && drawingFlag == true)
	{
		if (polygon.count() == 0)
		{
			polygon.append(m_pos);
		}
		polygon.append(m_pos);
	}
	else if (event->button() == Qt::RightButton)
	{
		if (!drawingFlag)
		{
			m_actMenu->exec(event->screenPos().toPoint());
			return;
		}
		if (polygon.count() > 0)
		{
			polygon.removeLast();
			polygon.append(polygon[0]);
		}
		QPainterPath tmpPath;
		tmpPath.addPolygon(polygon);

		path.clear();
		polygon.clear();
		comformPath.append(tmpPath);
		comformOp.append(shapeOperation);
		drawingFlag = false;
	}
	this->update();
}

void ShapeDrawView::mouseReleaseEvent(QMouseEvent* event)
{
	this->update();
	QWidget::mouseReleaseEvent(event);
}

void ShapeDrawView::mouseMoveEvent(QMouseEvent* event)
{
	QWidget::mouseMoveEvent(event);
	m_pos = event->pos();
	if (drawingFlag)
	{
		if (polygon.count() > 0)
		{
			polygon[polygon.count() - 1] = m_pos;
		}
	}
	this->update();
}

void ShapeDrawView::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::HighQualityAntialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(bgColor);
	painter.drawRect(this->rect());

	path.clear();
	pen.setWidth(2);
	painter.setPen(pen);
	painter.setBrush(brush);
	drawBackGroundImage(painter);
	drawCurrentShape(painter);
	if (drawingFlag)
	{
		drawHintLines(painter);
	}
	QWidget::paintEvent(event);
}

void ShapeDrawView::MenuInit()
{
	m_actMenu = new QMenu(this);
	QMenu* addMenu = new QMenu(u8"相加");
	QMenu* divMenu = new QMenu(u8"去除");
	QAction* act_openImage = new QAction(u8"打开");
	QAction* act_saveImage = new QAction(u8"保存");
	QAction* act_fitShow = new QAction(u8"还原");
	QAction* act_drawLine = new QAction(u8"绘制直线");
	QAction* act_drawRect1 = new QAction(u8"绘制正矩形");
	QAction* act_drawRect2 = new QAction(u8"绘制斜矩形");
	QAction* act_drawPolygon = new QAction(u8"绘制多边形");
	QAction* act_drawDivRect1 = new QAction(u8"绘制正矩形");
	QAction* act_drawDivRect2 = new QAction(u8"绘制斜矩形");
	QAction* act_drawDivPolygon = new QAction(u8"绘制多边形");
	m_actMenu->addAction(act_openImage);
	m_actMenu->addAction(act_saveImage);
	m_actMenu->addAction(act_fitShow);
	m_actMenu->addSeparator();
	m_actMenu->addAction(act_drawLine);
	m_actMenu->addMenu(addMenu);
	m_actMenu->addMenu(divMenu);
	addMenu->addAction(act_drawRect1);
	addMenu->addAction(act_drawRect2);
	addMenu->addAction(act_drawPolygon);

	divMenu->addAction(act_drawDivRect1);
	divMenu->addAction(act_drawDivRect2);
	divMenu->addAction(act_drawDivPolygon);

	connect(act_openImage, &QAction::triggered, [=]()
		{
			QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "C:/", tr("Image Files (*.png *.jpg *.bmp)"));
			if (fileName != "")
			{
				LoadImage(fileName);
			}
		});
	connect(act_saveImage, &QAction::triggered, [=]()
		{
			QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), "", tr("Curve TagName Files (*.jpeg)"));
			if (fileName != "")
			{
				QPixmap tmpPix;
				createRegionPixmap(comformPath, tmpPix);
				tmpPix.save(fileName);
			}
		});
	connect(act_drawLine, &QAction::triggered, [=]()
		{
			shapeOperation = oAdd;
			shapeType = tLine;
			drawingFlag = true;
		});
	connect(act_drawRect1, &QAction::triggered, [=]()
		{
			shapeOperation = oAdd;
			shapeType = tRectangle1;
			drawingFlag = true;
		});
	connect(act_drawRect2, &QAction::triggered, [=]()
		{
			shapeOperation = oAdd;
			shapeType = tRectangle2;
			drawingFlag = true;
		});
	connect(act_drawPolygon, &QAction::triggered, [=]()
		{
			shapeOperation = oAdd;
			shapeType = tPolygon;
			drawingFlag = true;
		});
	connect(act_drawDivRect1, &QAction::triggered, [=]()
		{
			shapeOperation = oDiv;
			shapeType = tRectangle1;
			drawingFlag = true;
		});
	connect(act_drawDivRect2, &QAction::triggered, [=]()
		{
			shapeOperation = oDiv;
			shapeType = tRectangle2;
			drawingFlag = true;
		});
	connect(act_drawDivPolygon, &QAction::triggered, [=]()
		{
			shapeOperation = oDiv;
			shapeType = tPolygon;
			drawingFlag = true;
		});
}

void ShapeDrawView::drawStart()
{
}

void ShapeDrawView::drawFinish()
{
}

void ShapeDrawView::drawBackGroundImage(QPainter& painter)
{
	if (!curImage.isNull())
	{
		painter.drawPixmap(this->rect(), curImage.scaled(this->width(), this->height(), Qt::KeepAspectRatio));
	}
	painter.setPen(Qt::NoPen);
	//已经绘制的图形
	for (int i = 0; i < comformPath.count(); i++)
	{
		if (comformOp[i] == oAdd)
		{
			painter.setBrush(brushColor);
		}
		else
		{
			painter.setBrush(brushDivColor);
		}
		painter.drawPath(comformPath[i]);
	}
}

void ShapeDrawView::drawCurrentShape(QPainter& painter)
{
	if (shapeOperation == oAdd)
	{
		pen.setColor(brushColor);
		painter.setBrush(brushColor);
	}
	else
	{
		pen.setColor(brushDivColor);
		painter.setBrush(brushDivColor);
	}
	painter.setPen(pen);
	//正在绘制的图形
	path.addPolygon(polygon);
	painter.drawPath(path);
}

void ShapeDrawView::drawHintLines(QPainter& painter)
{
	pen.setColor(Qt::red);
	pen.setWidth(1);
	painter.setPen(pen);
	painter.setBrush(Qt::NoBrush);
	painter.drawLine(m_pos.x(), 0, m_pos.x(), this->height());
	painter.drawLine(0, m_pos.y(), this->width(), m_pos.y());
}

void ShapeDrawView::createRegionPixmap(QVector<QPainterPath >const& _polygon, QPixmap& ori_pix)
{
	QPixmap pix = QPixmap(this->size());
	pix.fill(Qt::black);
	QPainter painter(&pix);
	painter.setPen(Qt::NoPen);
	for (int i = 0; i < _polygon.count(); i++)
	{
		if (comformOp[i] == oAdd)
		{
			painter.setBrush(Qt::white);
		}
		else
		{
			painter.setBrush(Qt::black);
		}
		painter.drawPath(_polygon[i]);
	}
	QSize ori_size = curImage.size();
	ori_pix = pix.scaled(ori_size);
}
