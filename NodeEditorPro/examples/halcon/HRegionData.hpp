#pragma once

#include <QtGui/QPixmap>

#include "NodeDataModel.hpp"
#include "halconcpp/HalconCpp.h"

using QtNodes::NodeData;
using QtNodes::NodeDataType;
using namespace HalconCpp;

class HRegionData :public NodeData
{
public:
	HRegionData()
	{
		m_hRegion = HRegion();
	}
	HRegionData(HRegion& _hregion)
	{
		if (_hregion.IsInitialized())
		{
			m_hRegion = _hregion;
		}
	}
	virtual ~HRegionData()
	{

	}
	NodeDataType type() const override
	{
		return { "HRegion","Region" };
	}
	HRegion* hRegion() { return &m_hRegion; }
	void setHRegion(HRegion const& _hregion)
	{
		if (!_hregion.IsInitialized())
		{
			return;
		}
		m_hRegion = _hregion;
	}
	QSize getSize()
	{
		return m_size;
	}
	void setSize(QSize _size)
	{
		m_size = _size;
	}
private:
	HRegion m_hRegion;
	QSize m_size;
};
