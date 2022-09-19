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
	HImageData() :m_himage(HImage()) {}
	HImageData(HImage const& h_image)
		:m_himage(h_image)
	{}
	NodeDataType type() const override
	{
		return { "HImage","P" };
	}
	HImage hImage() { return m_himage; }
private:
	HImage m_himage;
};
