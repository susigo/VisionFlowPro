#pragma once

#include <QtCore/QString>

#include "Export.hpp"

namespace QtNodes
{

	struct NodeDataType
	{
		QString id;
		QString name;

		friend bool operator<(QtNodes::NodeDataType const& d1,
			QtNodes::NodeDataType const& d2)
		{
			return d1.id < d2.id;
		}

		friend bool operator==(const QtNodes::NodeDataType& d1,
			const QtNodes::NodeDataType& d2) noexcept
		{
			return d1.id == d2.id;
		}
	};

	/// Class represents data transferred between nodes.
	/// @param type is used for comparing the types
	/// The actual data is stored in subtypes
	class NodeData
	{
	public:

		virtual ~NodeData() = default;

		virtual bool sameType(NodeData const& nodeData) const
		{
			return (this->type().id == nodeData.type().id);
		}

		/**
		 * \brief 节点标签
		 * \return 标签名字
		 */
		virtual NodeDataType type() const = 0;
	};
}
