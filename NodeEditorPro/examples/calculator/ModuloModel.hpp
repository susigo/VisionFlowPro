#pragma once

#include <QtCore/QObject>
#include <QtWidgets/QLineEdit>

#include "NodeDataModel.hpp"

#include <iostream>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class IntegerData;

class ModuloModel
	: public NodeDataModel
{
	Q_OBJECT

public:
	ModuloModel() = default;

	virtual
		~ModuloModel() = default;

public:

	QString
		caption() const override
	{
		return QStringLiteral("求模");
	}

	bool
		captionVisible() const override
	{
		return true;
	}

	bool
		portCaptionVisible(PortType, PortIndex) const override
	{
		return true;
	}

	QString
		portCaption(PortType portType, PortIndex portIndex) const override
	{
		switch (portType)
		{
		case PortType::In:
			if (portIndex == 0)
				return QStringLiteral("求模数");
			else if (portIndex == 1)
				return QStringLiteral("被模数");

			break;

		case PortType::Out:
			return QStringLiteral("结果");

		default:
			break;
		}
		return QString();
	}

	QString
		name() const override
	{
		return QStringLiteral("求模");
	}

public:

	QJsonObject
		save() const override;

public:

	unsigned int
		nPorts(PortType portType) const override;

	NodeDataType
		dataType(PortType portType, PortIndex portIndex) const override;

	std::shared_ptr<NodeData>
		outData(PortIndex port) override;

	void
		setInData(std::shared_ptr<NodeData>, int) override;

	QWidget*
		embeddedWidget() override { return nullptr; }

	NodeValidationState
		validationState() const override;

	QString
		validationMessage() const override;

private:

	std::weak_ptr<IntegerData> _number1;
	std::weak_ptr<IntegerData> _number2;

	std::shared_ptr<IntegerData> _result;

	NodeValidationState modelValidationState = NodeValidationState::Warning;
	QString modelValidationError = QStringLiteral("未连接或运行失败!");
};
