#pragma once

#include "Preset.hpp"

#include <QtCore/QObject>
#include <QtCore/QPointer>

#include <future>
#include <mutex>

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

	Q_INVOKABLE std::future<bool> loadFromFile();
	Q_INVOKABLE std::future<bool> saveToFile();

	QList<QPointer<Preset>> presets;

  signals:
	void loaded();

  private:
	std::mutex _mutex;
};
} // namespace RaportPKUP::UI
