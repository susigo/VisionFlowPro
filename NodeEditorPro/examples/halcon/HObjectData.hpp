#pragma once

#include <QtGui/QPixmap>

#include "NodeDataModel.hpp"
#include "halconcpp/HalconCpp.h"

using QtNodes::NodeData;
using QtNodes::NodeDataType;
using namespace HalconCpp;

class HObjecData :public NodeData
{
public:
	HObjecData()
	{
		m_hObject = HObject();
	}
	HObjecData(HObject& _obj)
	{
		if (_obj.IsInitialized())
		{
			m_hObject = _obj;
		}
	}
	virtual ~HObjecData()
	{

	}
	NodeDataType type() const override
	{
		return { "HObject","data" };
	}

	HObject* hObject() { return &m_hObject; }

	void setObject(HObject const& _obj)
	{
		if (!_obj.IsInitialized())
		{
			return;
		}
		m_hObject = _obj;
	}
private:
	HObject m_hObject;
};
