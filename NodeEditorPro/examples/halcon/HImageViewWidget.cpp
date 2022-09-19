#include "HImageViewWidget.hpp"

HImageViewWidget::HImageViewWidget(QWidget* parent)

{
	if (parent != Q_NULLPTR)
	{
		this->setParent(parent);
	}

	cur_pixmap = new QPixmap();
}

void HImageViewWidget::showImage(HImage const& _himg)
{
	Hlong width;
	Hlong height;
	double zoom_ratio = 1.0;
	_himg.GetImageSize(&width, &height);
	if (width > this->width())
	{
		zoom_ratio = 1.0 * this->width() / width;
	}
	cur_image = _himg.ZoomImageSize(width * zoom_ratio, height * zoom_ratio, "bilinear");
	HImageToQPixmap(cur_image, *cur_pixmap);
	this->update();
}

void HImageViewWidget::HImageToQPixmap(HImage const& _img, QPixmap& tar_pixmap)
{

	Hlong w, h; HString hType;
	Hlong width;
	Hlong height;
	QImage tar_img;

	_img.GetImageSize(&width, &height);
	HTuple type = _img.GetImageType();
	//获取HImage的通道数
	HTuple hChannels = _img.CountChannels();
	if (strcmp(type[0].S(), "byte")) // 如果不是 byte 类型，则失败
	{
		return;
	}
	QImage::Format format;
	switch (hChannels[0].I())
	{
	case 1:
		format = QImage::Format_Grayscale8;
		break;
	case 3:
		format = QImage::Format_RGB32;
		break;
	default:
		return;
	}
	if (tar_img.width() != width || tar_img.height() != height || tar_img.format() != format)
	{
		tar_img = QImage(static_cast<int>(width),
			static_cast<int>(height),
			format);
	}
	if (hChannels == 1)
	{
		//获取HImage的数据指针
		uchar* pBuf = (uchar*)_img.GetImagePointer1(&hType, &w, &h);

		//创建QImage图片
		tar_img = QImage(w, h, QImage::Format_Indexed8);

		//memcpy
		for (int i = 0; i < h; i++, pBuf += w)
		{
			uchar* pDest = tar_img.scanLine(i);
			memcpy(pDest, pBuf, w);
		}

		//tar_pixmap = QPixmap::fromImage(tar_img);
	}
	else if (hChannels == 3)
	{
		uchar* R, * G, * B;
		_img.GetImagePointer3(reinterpret_cast<void**>(&R),
			reinterpret_cast<void**>(&G),
			reinterpret_cast<void**>(&B), &hType, &width, &height);

		for (int row = 0; row < height; row++)
		{
			QRgb* line = reinterpret_cast<QRgb*>(tar_img.scanLine(row));
			for (int col = 0; col < width; col++)
			{
				line[col] = qRgb(*R++, *G++, *B++);
			}
		}
	}
	tar_pixmap = QPixmap::fromImage(tar_img);
}

void HImageViewWidget::paintEvent(QPaintEvent* event)
{
	//QPainter painter(this);
	if (!cur_pixmap->isNull())
	{
		this->setPixmap(cur_pixmap->scaled(this->width(), this->height(), Qt::KeepAspectRatio));
	}
	QLabel::paintEvent(event);
}

