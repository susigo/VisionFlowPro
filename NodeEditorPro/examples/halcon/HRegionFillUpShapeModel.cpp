#include "HRegionFillUpShapeModel.hpp"
#include <QtCore/QEvent>

#include "halconcpp/HalconCpp.h"

using namespace HalconCpp;

HRegionFillUpShapeModel::HRegionFillUpShapeModel()
{
	m_hRegion = std::make_shared<HRegionData>();
	m_result = std::make_shared<HRegionData>();

	m_widget = new QWidget();
	m_host = new QVBoxLayout();
	m_combo_feature = new QComboBox();

	m_minvalEdit = new QLineEdit();
	m_maxvalEdit = new QLineEdit();
	m_widget->setContentsMargins(0, 0, 0, 0);
	m_host->setSpacing(1);
	m_widget->setLayout(m_host);
	m_host->addWidget(m_combo_feature);
	m_host->addWidget(m_minvalEdit);
	m_host->addWidget(m_maxvalEdit);
	m_widget->setFixedHeight(90);
	m_widget->setFixedWidth(100);
	m_combo_feature->addItem("area");
	m_combo_feature->addItem("compactness");
	m_combo_feature->addItem("convexity");
	m_combo_feature->addItem("anisometry");
	m_minvalEdit->setText("0.0");
	m_maxvalEdit->setText("100.0");

	connect(m_minvalEdit, &QLineEdit::textChanged, [=]()
		{
			m_minval = m_minvalEdit->text().toDouble();
			RunTask();
		});
	connect(m_maxvalEdit, &QLineEdit::textChanged, [=]()
		{
			m_maxval = m_maxvalEdit->text().toDouble();
			RunTask();
		});
	connect(m_combo_feature, &QComboBox::currentTextChanged, [=]()
		{
			m_feature = m_combo_feature->currentText();
			RunTask();
		});
}

bool HRegionFillUpShapeModel::RunTask()
{
	Q_EMIT computingStarted();
	PortIndex const outPortIndex = 0;
	try
	{
		HalconCpp::FillUpShape(*m_hRegion->hRegion(), m_result->hRegion(),
			m_feature.toStdString().c_str(),
			m_minval,
			m_maxval);
		modelValidationState = NodeValidationState::Valid;
		modelValidationError = QString();
	}
	catch (...)
	{
		modelValidationState = NodeValidationState::Warning;
		modelValidationError = QStringLiteral("缺失或运行失败!");
	}

	Q_EMIT dataUpdated(outPortIndex);
	Q_EMIT computingFinished();
	return true;
}

unsigned int HRegionFillUpShapeModel::
nPorts(PortType portType) const
{
	unsigned int result = 1;

	switch (portType)
	{
	case PortType::In:
		result = 1;
		break;

	case PortType::Out:
		result = 1;

	default:
		break;
	}

	return result;
}

NodeValidationState HRegionFillUpShapeModel::validationState() const
{
	return modelValidationState;
}

QString HRegionFillUpShapeModel::validationMessage() const
{
	return modelValidationError;
}

QJsonObject HRegionFillUpShapeModel::save() const
{
	QJsonObject result = NodeDataModel::save();

	result.insert("m_minval", m_minval);
	result.insert("m_maxval", m_maxval);
	result.insert("m_feature", m_feature);
	return result;
}

void HRegionFillUpShapeModel::restore(QJsonObject const& json_values)
{
	NodeDataModel::restore(json_values);
	m_minval = json_values.value("m_minval").toDouble();
	m_maxval = json_values.value("m_maxval").toDouble();
	m_feature = json_values.value("m_feature").toString();

	m_combo_feature->setCurrentText(m_feature);
	m_minvalEdit->setText(QString::number(m_minval));
	m_maxvalEdit->setText(QString::number(m_maxval));
}

NodeDataType
HRegionFillUpShapeModel::dataType(PortType, PortIndex) const
{
	return HRegionData().type();
}

void HRegionFillUpShapeModel::
setInData(std::shared_ptr<NodeData> data, int portIndex)
{
	auto hImageData =
		std::dynamic_pointer_cast<HRegionData>(data);
	if (hImageData == nullptr)
	{
		return;
	}
	switch (portIndex)
	{
	case 0:
		m_hRegion->setHRegion(*hImageData->hRegion());
		m_result->setSize(hImageData->getSize());
		break;
	default:
		break;
	}
	RunTask();
}

std::shared_ptr<NodeData>
HRegionFillUpShapeModel::
outData(PortIndex)
{
	return std::static_pointer_cast<HRegionData>(m_result);
}
