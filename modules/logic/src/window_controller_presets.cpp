#include "WindowController.hpp"

#include <include/IProcess.hpp>
#include <include/IRepositoryAccessor.hpp>

#include <QtCore/QStandardPaths>
#include <QtCore/QString>
#include <QtCore/QUuid>
#include <QtGui/QDesktopServices>
#include <QtQml/QQmlContext>
#include <QtQml/qqmllist.h>
#include <QtWidgets/QMessageBox>

using namespace Qt::Literals::StringLiterals;

namespace RaportPKUP::UI
{
void WindowController::savePreset(const QString& name)
{
	Preset* preset = nullptr;

	for (auto item : _presets)
	{
		if (item->name == name)
			return;
	}

	if (!preset)
		preset = new Preset(this);

	preset->name = name.trimmed();
	preset->authorName = _authorName.value();
	preset->authorEmail = _authorEmail.value();
	preset->city = _city.value();
	preset->repositories.clear();

	std::transform(_repositories.begin(), _repositories.end(), std::back_inserter(preset->repositories),
				   [](RepositoryListItem* item) { return item->path(); });

	_presets.push_back(preset);

	emit presetsChanged();
}

void WindowController::deletePreset(int index)
{
	if (index < 0)
		return;

	_presets.removeAt(index);

	emit presetsChanged();
}

void WindowController::recallPreset(int index)
{
	if (index < 0)
		return;

	const auto preset = _presets.at(index);

	setAuthorName(preset->authorName);
	setAuthorEmail(preset->authorEmail);

	if (!preset->city.isEmpty())
		setCity(preset->city);

	auto list = _repositories;
	qDeleteAll(list);
	_repositories.clear();

	const auto& accessor = _repository_detector->getAccessor();

	for (const auto& repoPath : preset->repositories)
	{
		const auto wstr = repoPath.toStdWString();

		if (auto repo = accessor.openRepository(std::filesystem::path(std::move(wstr))).get())
			_repositories.push_back(new RepositoryListItem(std::move(repo), this));
	}

	emit repositoriesChanged();
}

void WindowController::loadPresets()
{
	auto list = _presets;
	_presets.clear();
	qDeleteAll(list);

	std::copy(_presets_manager.presets.cbegin(), _presets_manager.presets.cend(), std::back_inserter(_presets));
	emit presetsChanged();
}

void WindowController::syncPresetsFile()
{
	if (_presets.size() == _presets_manager.presets.size())
	{
		bool broken = false;

		for (size_t i = 0; i < _presets.size(); ++i)
		{
			if (_presets[i] != _presets_manager.presets[i])
			{
				broken = true;
				break;
			}
		}

		if (!broken)
			return;
	}

	_presets_manager.presets.clear();
	std::copy(_presets.cbegin(), _presets.cend(), std::back_inserter(_presets_manager.presets));
	_presets_manager.saveToFile();
}
} // namespace RaportPKUP::UI
