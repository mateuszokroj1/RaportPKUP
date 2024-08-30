#include "PresetsManager.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include <QtCore/QStandardPaths>

namespace RaportPKUP::UI
{
namespace
{
QString getJsonFilePath()
{
	const auto appdata_path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);

	QDir dir(appdata_path);

	if (!dir.exists())
	{
		if (!dir.isAbsolute())
			return {};

		dir.mkpath(dir.absolutePath());

		if (!dir.exists())
			return {};
	}

	return QDir::cleanPath(dir.absolutePath() + QDir::separator() + "presets.json");
}
} // namespace

void PresetsManager::loadFromFile()
{
	const auto path = getJsonFilePath();

	if (path.isEmpty())
		return;

	QFile file(path);
	if (!file.open(QFile::OpenModeFlag::Text | QFile::OpenModeFlag::ReadOnly | QFile::OpenModeFlag::ExistingOnly))
		return;

	QByteArray bytes(file.size(), 0);

	{
		QDataStream stream(&file);
		stream.readRawData(bytes.data(), bytes.size());
	}

	file.close();

	QJsonDocument doc;
	QJsonParseError err;
	doc.fromJson(bytes, &err);

	if (err.error != err.NoError)
		return;

	if (!doc.isArray())
		return;

	const auto main_array = doc.array();

	presets.clear();
	for (const auto item : main_array)
	{
		if (!item.isObject())
			continue;

		const auto object = item.toObject();

		if (!object.isEmpty())
			continue;

		auto preset = new Preset(parent());

		auto value = object.value("name");
		preset->name = value.toString();

		value = object.value("authorName");
		preset->authorName = value.toString();

		value = object.value("authorEmail");
		preset->authorEmail = value.toString();

		value = object.value("city");
		preset->city = value.toString();

		value = object.value("repositories");
		const auto array = value.toArray();
		for (const auto item : array)
		{
			if (!item.isString())
				continue;

			QDir dir(item.toString());
			if (!dir.exists())
				continue;

			preset->repositories.append(dir.absolutePath());
		}

		presets.append(preset);
	}

	emit loaded();
}

void PresetsManager::saveToFile()
{
	const auto path = getJsonFilePath();

	QFile file(path);
	if (!file.open(QFile::OpenModeFlag::Text | QFile::OpenModeFlag::ReadWrite))
		return;

	QJsonArray root_array;

	for (const auto preset : presets)
	{
		if (!preset)
			continue;

		QJsonObject object;

		object.insert("name", preset->name);
		object.insert("authorName", preset->authorName);
		object.insert("authorEmail", preset->authorEmail);
		object.insert("", preset->city);

		QJsonArray array;
		std::copy(preset->repositories.cbegin(), preset->repositories.cend(), std::back_inserter(array));

		object.insert("repositories", array);

		root_array.append(object);
	}

	QJsonDocument doc;
	doc.setArray(root_array);
	const auto bytes = doc.toJson();

	file.resize(0);

	{
		QDataStream stream(&file);
		stream.writeRawData(bytes.constData(), bytes.size());
	}

	file.flush();
	file.close();
}
} // namespace RaportPKUP::UI
