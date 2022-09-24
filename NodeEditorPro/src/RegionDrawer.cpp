#include "RegionDrawer.hpp"

RegionDrawer::RegionDrawer(QWidget* parent) :QLabel(parent)
{
	this->resize(1280, 1024);
}

void RegionDrawer::setLocalFilename(QString name)
{
	LocalFileName = name;
}

void RegionDrawer::setPaintingType(int num)
{
	paintingType = num;
	return;
}

void RegionDrawer::setBrightness(float num)
{
	brightness = num;
	isBriAndConChanged = true;
	update();
}

void RegionDrawer::setContrast(float num)
{
	contrast = num;
	isBriAndConChanged = true;
	update();
}

void RegionDrawer::changePaintingState()
{
	isPainting == false ? isPainting = true : isPainting = false;
}

void RegionDrawer::ChangeBriAndConChangedState()
{
	isBriAndConChanged == false ? isBriAndConChanged = true : isBriAndConChanged = false;
}


bool RegionDrawer::_rectsIsEmpty()
{
	if (_rects.size() == 0)
	{
		return true;
	}
	else {
		return false;
	}
}

bool RegionDrawer::_ellipsesIsEmpty()
{
	if (_ellipses.size() == 0)
	{
		return true;
	}
	else {
		return false;
	}
}

QImage RegionDrawer::Mat2QImage(const Mat& mat)
{
	//8位，通道数为1
	if (mat.type() == CV_8UC1)
	{
		QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
		image.setColorCount(256);
		for (int i = 0; i < 256; i++)
		{
			image.setColor(i, qRgb(i, i, i));
		}

		uchar* pSrc = mat.data;
		for (int row = 0; row < mat.rows; row++)
		{
			uchar* pDest = image.scanLine(row);
			memcpy(pDest, pSrc, mat.cols);
			pSrc += mat.step;
		}
		return image;
	}
	//8位，通道数为3
	else if (mat.type() == CV_8UC3)
	{
		const uchar* pSrc = (const uchar*)mat.data;
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
		return image.rgbSwapped();
	}
	//8位，通道数为4
	else if (mat.type() == CV_8UC4)
	{
		qDebug() << "CV_8UC4";

		const uchar* pSrc = (const uchar*)mat.data;
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
		return image.copy();
	}
	//32位，通道数为3
	else if (mat.type() == CV_32FC3)
	{
		cv::Mat temp;
		mat.convertTo(temp, CV_8UC3);

		const uchar* pSrc = (const uchar*)temp.data;
		QImage image(pSrc, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
		return image.rgbSwapped();
	}
	else
	{
		qDebug() << "ERROR: Mat could not be converted to QImage.";
		return QImage();
	}
}

Mat RegionDrawer::QImageToMat(QImage image)
{
	cv::Mat mat;
	switch (image.format())
	{
	case QImage::Format_ARGB32:
	case QImage::Format_RGB32:
	case QImage::Format_ARGB32_Premultiplied:
		mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
		break;
	case QImage::Format_RGB888:
		mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
		cv::cvtColor(mat, mat, cv::COLOR_RGB2GRAY);
		break;
	case QImage::Format_Indexed8:
		mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
		break;
	}
	return mat;
}

QImage RegionDrawer::ImageAdjustment(QImage image)
{
	//    Mat image_Mat = QImageToMat(image);
	image_Mat = cv::imread(LocalFileName.toLocal8Bit().toStdString());
	image_Mat.convertTo(image_Mat, -1, static_cast<double>(contrast), static_cast<double>(brightness));
	return Mat2QImage(image_Mat);
}

void RegionDrawer::contextMenuEvent(QContextMenuEvent* event)
{
	QPoint pos = event->pos();
	pos = this->mapToGlobal(pos);
	QMenu* menu = new QMenu(this);

	QAction* loadImage = new QAction(this);
	loadImage->setText("选择图片");
	connect(loadImage, &QAction::triggered, this, &RegionDrawer::OnSelectImage);
	menu->addAction(loadImage);
	menu->addSeparator();

	QAction* zoomInAction = new QAction(this);
	zoomInAction->setText("放大");
	connect(zoomInAction, &QAction::triggered, this, &RegionDrawer::OnZoomInImage);
	menu->addAction(zoomInAction);

	QAction* zoomOutAction = new QAction(this);
	zoomOutAction->setText("缩小");
	connect(zoomOutAction, &QAction::triggered, this, &RegionDrawer::OnZoomOutImage);
	menu->addAction(zoomOutAction);

	QAction* presetAction = new QAction(this);
	presetAction->setText("还原");
	connect(presetAction, &QAction::triggered, this, &RegionDrawer::OnPresetImage);
	menu->addAction(presetAction);
	menu->addSeparator();

	QAction* pantingBack = new QAction(this);
	pantingBack->setText("撤回");
	connect(pantingBack, &QAction::triggered, this, &RegionDrawer::OnPaintingback);
	menu->addAction(pantingBack);

	menu->exec(pos);
}

void RegionDrawer::paintEvent(QPaintEvent* event)
{
	QTime qTime;
	qTime.start();

	QPainter painter(this);
	painter.setPen(Qt::NoPen);
	painter.setBrush(bgColor);
	painter.drawRect(this->rect());

	painter.setPen(QPen(Qt::red, 3)); //设置画笔粗细为3，颜色为红色

	if (!LocalFileName.isNull())
		//        return QWidget::paintEvent(event);
	{

		// 根据窗口计算应该显示的图片的大小
		int width = qMin(Image.width(), this->width());
		int height = width * 1.0 / (Image.width() * 1.0 / Image.height());
		height = qMin(height, this->height());
		width = height * 1.0 * (Image.width() * 1.0 / Image.height());

		// 平移
		painter.translate(this->width() / 2 + XPtInterval, this->height() / 2 + YPtInterval);

		// 缩放
		painter.scale(ZoomValue, ZoomValue);//垂直水平两个方向

		//改变亮度和对比度
		if (isBriAndConChanged)
		{
			Image = ImageAdjustment(Image);
			isBriAndConChanged = false;
		}


		// 绘制图像
		QRect picRect(-width / 2, -height / 2, width, height);
		painter.drawImage(picRect, Image);
	}

	//绘画已有图形

	QPainter painter2(this);
	painter2.setPen(QPen(Qt::red, 3)); //设置画笔粗细为3，颜色为红色

	painter2.translate(this->width() / 2 + XPtInterval, this->height() / 2 + YPtInterval);
	painter2.scale(ZoomValue, ZoomValue);//垂直水平两个方向

	int i1 = 0, i2 = 0;//各种图形的索引
	for (int c = 0; c < shapeType.size(); c++)
	{
		if (shapeType.at(c) == RECTTYPE)
		{
			painter2.drawRect(_rects.at(i1++));
		}
		else if (shapeType.at(c) == ELLIPSETYPE)
		{
			painter2.drawEllipse(_ellipses.at(i2++));
		}
	}
	//根据是否需要的绘画状态进行绘图
	if (isPainting && (Pressed == true))
	{
		if (shapeType.last() == RECTTYPE)
		{
			painter2.drawRect(_rects.last());
		}
		if (shapeType.last() == ELLIPSETYPE)
		{
			painter2.drawEllipse(_ellipses.last());
		}
	}

	qDebug() << "update()刷新时间：" << qTime.elapsed() << "ms";
}

void RegionDrawer::wheelEvent(QWheelEvent* event)
{
	int value = event->delta();
	if (value > 0)  //放大
		OnZoomInImage();
	else            //缩小
		OnZoomOutImage();

	update();
}

void RegionDrawer::mousePressEvent(QMouseEvent* event)
{
	//开始绘图
	if (isPainting)
	{
		Pressed = true;
		this->setCursor(Qt::CrossCursor);
		if (event->button() == Qt::LeftButton)
		{
			if (paintingType == RECTTYPE)
			{
				QRect rect;//鼠标按下，矩形开始
				_rects.append(rect);
				shapeType.append(RECTTYPE);
				QRect& lastRect = _rects.last();//拿到新矩形
				lastRect.setTopLeft((event->pos() - QPoint(this->width() / 2, this->height() / 2) - QPoint(XPtInterval, YPtInterval)) / ZoomValue);//记录鼠标的坐标(新矩形的左上角坐标)
			}
			else if (paintingType == ELLIPSETYPE)
			{
				QRect ellipse;//鼠标按下，圆形开始
				_ellipses.append(ellipse);
				shapeType.append(ELLIPSETYPE);
				QRect& lastEllipse = _ellipses.last();
				lastEllipse.setTopLeft((event->pos() - QPoint(this->width() / 2, this->height() / 2) - QPoint(XPtInterval, YPtInterval)) / ZoomValue);
			}
		}
		return;
	}
	//记录初始坐标
	OldPos = event->pos();
	Pressed = true;
}

void RegionDrawer::mouseMoveEvent(QMouseEvent* event)
{
	if (!Pressed)
		return QWidget::mouseMoveEvent(event);

	if (isPainting && Pressed && (event->buttons() == Qt::LeftButton))
	{
		if (paintingType == RECTTYPE)
		{
			QRect& lastRect = _rects.last();//拿到新矩形

			lastRect.setBottomRight((event->pos() - QPoint(this->width() / 2, this->height() / 2) - QPoint(XPtInterval, YPtInterval)) / ZoomValue);//更新矩形的右下角坐标

		}
		else if (paintingType == ELLIPSETYPE)
		{
			QRect& lastEllipse = _ellipses.last();
			lastEllipse.setBottomRight((event->pos() - QPoint(this->width() / 2, this->height() / 2) - QPoint(XPtInterval, YPtInterval)) / ZoomValue);
		}
		update();
		return;
	}

	if (LocalFileName.isNull())
		return;
	this->setCursor(Qt::SizeAllCursor);
	QPoint pos = event->pos();
	int xPtInterval = pos.x() - OldPos.x();
	int yPtInterval = pos.y() - OldPos.y();

	XPtInterval += xPtInterval;
	YPtInterval += yPtInterval;

	OldPos = pos;
	update();
}

void RegionDrawer::mouseReleaseEvent(QMouseEvent* event)
{
	if (isPainting && Pressed)
	{
		if (paintingType == RECTTYPE)
		{
			QRect& lastRect = _rects.last();//拿到新矩形
			lastRect.setBottomRight((event->pos() - QPoint(this->width() / 2, this->height() / 2) - QPoint(XPtInterval, YPtInterval)) / ZoomValue);//更新矩形的右下角坐标
		}
		else if (paintingType == ELLIPSETYPE)
		{
			QRect& lastEllipse = _ellipses.last();
			lastEllipse.setBottomRight((event->pos() - QPoint(this->width() / 2, this->height() / 2) - QPoint(XPtInterval, YPtInterval)) / ZoomValue);
		}
		changePaintingState();
		Pressed = false;
		this->setCursor(Qt::ArrowCursor);
		return;
	}
	Pressed = false;
	setCursor(Qt::ArrowCursor);
}

void RegionDrawer::OnSelectImage()
{
	LocalFileName = QFileDialog::getOpenFileName(this, "Open Image", "./", tr("Images (*.png *.xpm *.jpg)"));

	QFile file(LocalFileName);
	if (!file.exists())
		return;
	ZoomValue = 1.0;
	XPtInterval = 0;
	YPtInterval = 0;
	Image.load(LocalFileName);
	//    image_Mat = cv::imread(LocalFileName.toLocal8Bit().toStdString());
	//    Image = Image.convertToFormat(QImage::Format_RGB888);
	update();
}

void RegionDrawer::OnZoomInImage()
{
	ZoomValue += 0.15;
	update();
}

void RegionDrawer::OnZoomOutImage()
{
	ZoomValue -= 0.15;
	if (ZoomValue <= 0)
	{
		ZoomValue = 0.15;
		return;
	}

	update();
}

void RegionDrawer::OnPresetImage()
{
	ZoomValue = 1.0;
	XPtInterval = 0;
	YPtInterval = 0;
	//    brightness = 0;
	//    contrast = 1;
	//    ChangeBriAndConChangedState();
	update();
}

void RegionDrawer::OnPaintingback()
{
	if (_rectsIsEmpty() && _ellipsesIsEmpty())
		return;
	if (shapeType.last() == RECTTYPE)
	{
		_rects.pop_back();
		shapeType.pop_back();
		update();
	}
	else if (shapeType.last() == ELLIPSETYPE)
	{
		_ellipses.pop_back();
		shapeType.pop_back();
		update();
	}
}
