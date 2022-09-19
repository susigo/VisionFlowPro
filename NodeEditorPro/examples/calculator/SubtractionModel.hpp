#pragma once

#include <QtCore/QObject>
#include <QtWidgets/QLabel>

#include "NodeDataModel.hpp"

#include "MathOperationDataModel.hpp"

#include "DecimalData.hpp"

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class SubtractionModel : public MathOperationDataModel
{
public:

	virtual
		~SubtractionModel() {}

public:

	QString
		caption() const override
	{
		return QStringLiteral("减法节点");
	}

	virtual bool
		portCaptionVisible(PortType portType, PortIndex portIndex) const override
	{
		Q_UNUSED(portType); Q_UNUSED(portIndex);
		return true;
	}

	virtual QString
		portCaption(PortType portType, PortIndex portIndex) const override
	{
		switch (portType)
		{
		case PortType::In:
			if (portIndex == 0)
				return QStringLiteral("减数");
			else if (portIndex == 1)
				return QStringLiteral("被减数");

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
		return QStringLiteral("减法");
	}

private:

	void
		compute() override
	{
		PortIndex const outPortIndex = 0;

		auto n1 = _number1.lock();
		auto n2 = _number2.lock();

		if (n1 && n2)
		{
			modelValidationState = NodeValidationState::Valid;
			modelValidationError = QString();
			_result = std::make_shared<DecimalData>(n1->number() -
				n2->number());
		}
		else
		{
			modelValidationState = NodeValidationState::Warning;
			modelValidationError = QStringLiteral("缺失节点或运行失败!");
			_result.reset();
		}

		Q_EMIT dataUpdated(outPortIndex);
	}
};
