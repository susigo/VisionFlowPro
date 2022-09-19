#pragma once

#include <QtCore/QObject>
#include <QtCore/QUuid>
#include <QtCore/QVariant>

#include "PortType.hpp"
#include "NodeData.hpp"

#include "Serializable.hpp"
#include "ConnectionState.hpp"
#include "ConnectionGeometry.hpp"
#include "TypeConverter.hpp"
#include "QUuidStdHash.hpp"
#include "Export.hpp"
#include "memory.hpp"

class QPointF;

namespace QtNodes
{

	class Node;
	class NodeData;
	class ConnectionGraphicsObject;

	/**
	 * \brief 节点连接线
	 */
	class  Connection
		: public QObject
		, public Serializable
	{

		Q_OBJECT

	public:

		///	新的连接对象会连接到目标节点，连接的端口应当有 端口类型，端口序号
		///	连接对象的另一端应当是相反的端口。
		Connection(PortType portType,
			Node& node,
			PortIndex portIndex);

		Connection(Node& nodeIn,
			PortIndex portIndexIn,
			Node& nodeOut,
			PortIndex portIndexOut,
			TypeConverter converter =
			TypeConverter{});

		Connection(const Connection&) = delete;
		Connection operator=(const Connection&) = delete;

		~Connection();

	public:

		QJsonObject
			save() const override;

	public:

		QUuid
			id() const;

		/// Remembers the end being dragged.
		/// Invalidates Node address.
		/// Grabs mouse.
		void
			setRequiredPort(PortType portType);
		PortType
			requiredPort() const;

		void
			setGraphicsObject(std::unique_ptr<ConnectionGraphicsObject>&& graphics);

		/// Assigns a node to the required port.
		/// It is assumed that there is a required port, no extra checks
		void
			setNodeToPort(Node& node,
				PortType portType,
				PortIndex portIndex);

		void
			removeFromNodes() const;

	public:

		ConnectionGraphicsObject&
			getConnectionGraphicsObject() const;

		ConnectionState const&
			connectionState() const;
		ConnectionState&
			connectionState();

		ConnectionGeometry&
			connectionGeometry();

		ConnectionGeometry const&
			connectionGeometry() const;

		Node*
			getNode(PortType portType) const;

		Node*&
			getNode(PortType portType);

		PortIndex
			getPortIndex(PortType portType) const;

		void
			clearNode(PortType portType);

		NodeDataType
			dataType(PortType portType) const;

		void
			setTypeConverter(TypeConverter converter);

		bool
			complete() const;

	public: // data propagation

		void
			propagateData(std::shared_ptr<NodeData> nodeData) const;

		void
			propagateEmptyData() const;

	Q_SIGNALS:

		void
			connectionCompleted(Connection const&) const;

		void
			connectionMadeIncomplete(Connection const&) const;

	private:

		QUuid _uid;

	private:

		Node* _outNode = nullptr;
		Node* _inNode = nullptr;

		PortIndex _outPortIndex;
		PortIndex _inPortIndex;

	private:

		ConnectionState    _connectionState;
		ConnectionGeometry _connectionGeometry;

		std::unique_ptr<ConnectionGraphicsObject>_connectionGraphicsObject;

		TypeConverter _converter;

	Q_SIGNALS:

		void
			updated(Connection& conn) const;
	};
}
