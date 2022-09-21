#pragma once

#include <QtCore/QString>

namespace QtNodes
{

	class Style
	{
	public:

		virtual
			~Style() = default;

	private:

		virtual void
			loadJsonFile(QString fileName) = 0;
		virtual void
			saveJsonFile(QString fileName) = 0;
	};

}
