#pragma once

#include <QMetaEnum>
#include <QObject>
#include <QtQml/QQmlEngine>
#include <QtQml/qqmlregistration.h>

namespace RaportPKUP::UI
{
class InputDataState
{
	Q_GADGET
	QML_ELEMENT
	QML_UNCREATABLE("Enum type")

  public:
	enum States
	{
		Indeterminate = 0,
		Valid = 1,
		Invalid = 2
	};

	Q_ENUM(States)
};
} // namespace RaportPKUP::UI

Q_DECLARE_METATYPE(RaportPKUP::UI::InputDataState::States)
