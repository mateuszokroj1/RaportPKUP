#pragma once

#include "Preset.hpp"

#include <QtCore/QObject>
#include <QtCore/QPointer>

namespace RaportPKUP::UI
{
class PresetsManager final : public QObject
{
	Q_OBJECT

  public:
	PresetsManager(QObject* parent) : QObject(parent)
	{
	}
	PresetsManager(const PresetsManager&) = delete;
	PresetsManager(PresetsManager&&) = delete;

	Q_INVOKABLE void loadFromFile();
	Q_INVOKABLE void saveToFile();

	QList<QPointer<Preset>> presets;

  signals:
	void loaded();
};
} // namespace RaportPKUP::UI
