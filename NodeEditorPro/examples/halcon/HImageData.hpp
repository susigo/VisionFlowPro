#pragma once

#include <QtGui/QPixmap>

#include "NodeDataModel.hpp"
#include "halconcpp/HalconCpp.h"

using QtNodes::NodeData;
using QtNodes::NodeDataType;
using namespace HalconCpp;

class HImageData :public NodeData
{
public:
	HImageData()
	{
		m_himage = HImage();
	}
	HImageData(HImage& h_image)
	{
		if (h_image.IsInitialized())
		{
			m_himage = h_image;
		}
	}

	NodeDataType type() const override
	{
		return { "HImage","Image" };
	}
	HImage hImage() { return m_himage; }
	void setHImage(HImage const& _img)
	{
		if (!_img.IsInitialized())
		{
			return;
		}
		m_himage = _img;
	}
private:
	HImage m_himage;
};
