#include "QGView.h"
#include <QScrollBar>
#include <QDebug>
#include "ControlItem.h"
#include "BaseItem.h"
#include <QFileDialog>
#include <QAction>
#include <QLabel>
#include "QGScene.h"
#include <QHBoxLayout>


QGView::QGView(QWidget* parent) : QGraphicsView(parent)
{

	this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);//解决拖动是背景图片残影
	setDragMode(QGraphicsView::ScrollHandDrag);
	drawBg();

	//反锯齿
	setRenderHints(QPainter::Antialiasing);
	// 隐藏水平/竖直滚动条
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setBackgroundBrush(Qt::gray);

	/*以鼠标中心进行缩放*/
	setMouseTracking(true);
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);//设置视口变换的锚点，这个属性控制当视图做变换时应该如何摆放场景的位置
	setResizeAnchor(QGraphicsView::AnchorUnderMouse);

	// 设置场景范围
	setSceneRect(INT_MIN / 2, INT_MIN / 2, INT_MAX, INT_MAX);
	//setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));

	scene = new QGScene(this);
	this->setScene(scene);



	addAction(new QAction(QString::fromLocal8Bit("保存原图"), this));
	addAction(new QAction(QString::fromLocal8Bit("保存截图"), this));
	setContextMenuPolicy(Qt::ActionsContextMenu);

	GrayValue = new QLabel(this);//显示灰度值
	GrayValue->setStyleSheet("color:rgb(50,255,0); background-color:rgb(80,80,80,180); font-size: 23px;font-weight: 400px;");
	GrayValue->setFixedWidth(160);
	GrayValue->setFixedHeight(60);
}
void QGView::DispImage(QImage& Image)
{
	ImageCatch = Image.copy();
	image = QPixmap::fromImage(Image);
	GetFit();
}
void QGView::AddItems(BaseItem* item)
{
	this->scene->addItem(item);
	scene->UpDateZoom(1.0 / ZoomValue);
}

void QGView::DispItem(const QPainterPath& ItemPath, bool onImage)
{
	if (onImage)
	{
		// QPainter painter(&pixmap); 
	}
	else
	{

	}
}


void QGView::ClearObj()
{
	foreach(auto item, scene->items())
	{
		scene->removeItem(item);
	}
}
void QGView::ZoomFrame(double value)
{
	scene->UpDateZoom(1.0 / value);
	this->setTransform(QTransform(value, 0, 0, value, 0, 0));
}
void QGView::GetFit()
{
	if (this->width() < 1 || image.width() < 1)
	{
		return;
	}
	//图片自适应方法
	qreal winWidth = this->width();
	qreal winHeight = this->height();
	qreal ScaleWidth = (image.width() + 1) / winWidth;
	qreal ScaleHeight = (image.height() + 1) / winHeight;
	qreal row1, column1;
	if (ScaleWidth >= ScaleHeight)
	{
		row1 = -(1) * ((winHeight * ScaleWidth) - image.height()) / 2;
		column1 = 0;
		ZoomValue = 1 / ScaleWidth;
	}
	else
	{
		row1 = 0;
		column1 = -(1.0) * ((winWidth * ScaleHeight) - image.width()) / 2;
		ZoomValue = 1 / ScaleHeight;
	}
	PixX = column1 * ZoomValue;
	PixY = row1 * ZoomValue;
	ZoomFrame(ZoomValue);
	QScrollBar* pHbar = this->horizontalScrollBar();
	pHbar->setSliderPosition(PixX);
	QScrollBar* pVbar = this->verticalScrollBar();
	pVbar->setSliderPosition(PixY);
}
void QGView::drawBg()
{
	bgPix.fill(color1);
	QPainter painter(&bgPix);
	painter.fillRect(0, 0, 32, 32, color2);
	painter.fillRect(32, 32, 32, 32, color2);
	painter.end();
}
void QGView::mousePressEvent(QMouseEvent* event)
{
	//qDebug()<<"click";
	QGraphicsView::mousePressEvent(event);
}
void QGView::resizeEvent(QResizeEvent* event)
{
	GetFit();
	QGraphicsView::resizeEvent(event);
}
void QGView::mouseReleaseEvent(QMouseEvent* event)
{
	QGraphicsView::mouseReleaseEvent(event);
}
void QGView::mouseDoubleClickEvent(QMouseEvent* event)
{
	GetFit();
	QGraphicsView::mouseDoubleClickEvent(event);
}
void QGView::mouseMoveEvent(QMouseEvent* event)
{
	auto item = this->mapToScene(event->pos());
	int X = item.x();
	int Y = item.y();

	if (X > -1 && X<ImageCatch.width() && Y>-1 && Y < ImageCatch.height())
	{
		int R, G, B;
		ImageCatch.pixelColor(X, Y).getRgb(&R, &G, &B);

		QString InfoVal = QString("[%1 %2 %3]\n{X%4 Y%5}").arg(QString::number(R), 3, ' ')
			.arg(QString::number(G), 3, ' ')
			.arg(QString::number(B), 3, ' ')
			.arg(QString::number((int)X), 4, ' ')
			.arg(QString::number((int)Y), 4, ' ');
		GrayValue->setText(InfoVal);
	}

	QGraphicsView::mouseMoveEvent(event);
}
void QGView::wheelEvent(QWheelEvent* event)
{
	ZoomValue = event->angleDelta().y() > 0 ? ZoomValue * 1.1 : ZoomValue * 0.9;
	ZoomValue = qMax(ZoomValue, 0.1);
	ZoomValue = qMin(ZoomValue, 40.0);
	ZoomFrame(ZoomValue);
}
void QGView::drawForeground(QPainter* painter, const QRectF& rect)
{
	Q_UNUSED(rect);
	Q_UNUSED(painter);
	painter->drawPixmap(QPoint(), QPixmap(100, 100));//绘制背景曾
}
void QGView::drawBackground(QPainter* painter, const QRectF& rect)
{
	Q_UNUSED(rect);
	painter->drawPixmap(QPoint(), image);//绘制背景曾
}
void QGView::paintEvent(QPaintEvent* event)
{
	QPainter paint(this->viewport());
	paint.drawTiledPixmap(QRect(QPoint(0, 0), QPoint(this->width(), this->height())), bgPix);//绘制背景曾
	QGraphicsView::paintEvent(event);
}




