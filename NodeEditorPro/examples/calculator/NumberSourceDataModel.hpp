#pragma once

#include <QtCore/QObject>

#include "NodeDataModel.hpp"

#include <iostream>

class DecimalData;

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class QLineEdit;

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class NumberSourceDataModel
	: public NodeDataModel
{
	Q_OBJECT

public:
	NumberSourceDataModel();

	virtual
		~NumberSourceDataModel() {}

public:

	QString
		caption() const override
	{
		return QStringLiteral("数字输入");
	}

	bool
		captionVisible() const override
	{
		return true;
	}

	QString
		name() const override
	{
		return QStringLiteral("数字输入");
	}

public:

	QJsonObject
		save() const override;

	void
		restore(QJsonObject const& p) override;

public:

	unsigned int
		nPorts(PortType portType) const override;

	NodeDataType
		dataType(PortType portType, PortIndex portIndex) const override;

	std::shared_ptr<NodeData>
		outData(PortIndex port) override;

	void
		setInData(std::shared_ptr<NodeData>, int) override
	{ }

	QWidget*
		embeddedWidget() override;

private Q_SLOTS:

	void
		onTextEdited(QString const& string);

private:

	std::shared_ptr<DecimalData> _number;

	QLineEdit* _lineEdit;
};
