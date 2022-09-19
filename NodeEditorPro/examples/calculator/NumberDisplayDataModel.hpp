#pragma once

#include <QtCore/QObject>

#include "NodeDataModel.hpp"

#include <iostream>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class QLabel;

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class NumberDisplayDataModel : public NodeDataModel
{
	Q_OBJECT

public:
	NumberDisplayDataModel();

	virtual
		~NumberDisplayDataModel() {}

public:

	QString
		caption() const override
	{
		return QStringLiteral("数字显示");
	}

	bool
		captionVisible() const override
	{
		return true;
	}

	QString
		name() const override
	{
		return QStringLiteral("数字显示");
	}

public:

	unsigned int
		nPorts(PortType portType) const override;

	NodeDataType
		dataType(PortType portType,
			PortIndex portIndex) const override;

	std::shared_ptr<NodeData>
		outData(PortIndex port) override;

	void
		setInData(std::shared_ptr<NodeData> data, int) override;

	QWidget*
		embeddedWidget() override;

	NodeValidationState
		validationState() const override;

	QString
		validationMessage() const override;

private:

	NodeValidationState modelValidationState = NodeValidationState::Warning;
	QString modelValidationError = QStringLiteral("未连接或运算失败！");

	QLabel* _label;
};
