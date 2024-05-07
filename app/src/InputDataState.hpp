#pragma once

#include <QMetaEnum>

namespace RaportPKUP
{
class InputDataState final : public QObject
{
  public:
	InputDataState(QObject *parent = nullptr) : QObject(parent)
	{
	}

	enum States : char
	{
		Indeterminate = 0,
		Valid = 1,
		Invalid = 2
	};

	Q_ENUM(States)
};
} // namespace RaportPKUP
