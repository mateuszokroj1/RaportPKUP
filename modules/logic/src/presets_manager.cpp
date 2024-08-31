#include "PresetsManager.hpp"

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

std::future<bool> PresetsManager::loadFromFile()
{
	return std::async(
		[this]()
		{
			std::lock_guard lock(_mutex);

			const auto path = getJsonFilePath();

			if (path.isEmpty())
				return false;

			QFile file(path);
			if (!file.open(QFile::OpenModeFlag::Text | QFile::OpenModeFlag::ReadOnly |
						   QFile::OpenModeFlag::ExistingOnly))
				return false;

			const auto bytes = file.readAll();

			file.close();

			QJsonParseError err;
			auto doc = QJsonDocument::fromJson(bytes, &err);

			if (err.error != err.NoError)
			{
				qDebug() << err.errorString();
				return false;
			}

			if (!doc.isArray())
				return false;

			const auto main_array = doc.array();

			presets.clear();
			for (const auto item : main_array)
			{
				if (!item.isObject())
					continue;

				const auto object = item.toObject();

				if (object.isEmpty())
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

			return true;
		});
}

std::future<bool> PresetsManager::saveToFile()
{
	return std::async(
		[this]()
		{
			std::lock_guard lock(_mutex);

			const auto path = getJsonFilePath();

			QFile file(path);
			if (!file.open(QFile::OpenModeFlag::Text | QFile::OpenModeFlag::ReadWrite))
				return false;

			QJsonArray root_array;

			for (const auto& preset : presets)
			{
				if (!preset)
					continue;

				QJsonObject object;

				object.insert("name", preset->name);
				object.insert("authorName", preset->authorName);
				object.insert("authorEmail", preset->authorEmail);
				object.insert("city", preset->city);

				QJsonArray array;
				std::copy(preset->repositories.cbegin(), preset->repositories.cend(), std::back_inserter(array));
				object.insert("repositories", array);

				root_array.append(object);
			}

			QJsonDocument doc;
			doc.setArray(root_array);
			const auto bytes = doc.toJson();

			file.resize(0);
			file.write(bytes);
			file.flush();
			file.close();
			return true;
		});
}
} // namespace RaportPKUP::UI
