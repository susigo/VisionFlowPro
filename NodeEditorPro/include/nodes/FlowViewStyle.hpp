#pragma once

#include <QtGui/QColor>

#include "Export.hpp"
#include "Style.hpp"

namespace QtNodes
{

	class FlowViewStyle : public Style
	{
	public:

		FlowViewStyle();

		FlowViewStyle(QString jsonText);

	public:

		static void setStyle(QString jsonText);

	private:


		void loadJsonFile(QString fileName) override;
		void saveJsonFile(QString fileName) override;

	public:

		QColor BackgroundColor;
		QColor FineGridColor;
		QColor CoarseGridColor;
	};
}
