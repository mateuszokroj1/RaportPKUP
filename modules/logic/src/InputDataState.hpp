#pragma once

#include <QMetaEnum>
#include <QObject>
#include <QtQml/qqmlregistration.h>

namespace RaportPKUP::UI
{
class InputDataState
{
	Q_GADGET
	QML_VALUE_TYPE(inputDataState)

  public:
	enum States : char
	{
		Indeterminate = 0,
		Valid = 1,
		Invalid = 2
	};

	Q_ENUM(States)
};

namespace Qt
{
Q_NAMESPACE
QML_NAMED_ELEMENT(InputDataState)
} // namespace Qt
} // namespace RaportPKUP::UI
