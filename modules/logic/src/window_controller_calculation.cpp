#include <QtCore/QStandardPaths>
#include <QtCore/QString>
#include <QtCore/QUuid>
#include <QtGui/QDesktopServices>
#include <QtQml/QQmlContext>
#include <QtQml/qqmllist.h>

#include <execution>

#include <include/Filters.hpp>
#include <include/IProcess.hpp>
#include <include/IRepositoryAccessor.hpp>

#include "InputDataState.hpp"
#include "WindowController.hpp"

using namespace Qt::Literals::StringLiterals;

namespace RaportPKUP::UI
{
void WindowController::searchForCommits()
{
	if (_last_thread.joinable())
	{
		qDebug() << "Another thread is working...";
		return;
	}

	emit lockScreen();
	_thread_finished = false;

	_last_thread = std::thread(
		[this]()
		{
			_commits_temp.clear();

			const DateTime from = fromDay().toStdSysDays();
			const DateTime to = toDay().toStdSysDays() + 23h + 59min + 59s;

			Author author;
			author.email = authorEmail().toStdWString();
			author.name = authorName().toStdWString();

			auto mutex = std::make_unique<std::mutex>();

			std::for_each(std::execution::par, _repositories.cbegin(), _repositories.cend(),
						  [this, &from, &to, &author, &mutex](const RepositoryListItem* repo)
						  {
							  repo->getRepository()->getCommitsFromTimeRange(from, to, author);

							  for (size_t i = 0; i < list.)
						  });

			Filters::removeEmptyPointers(_commits_temp);
			Filters::unique<std::unique_ptr<Commit>, Commit::Id>(_commits_temp, [](const std::unique_ptr<Commit>& item)
																 { return item->id; });
			Filters::sortDescending<std::unique_ptr<Commit>, DateTime>(
				_commits_temp, [](const std::unique_ptr<Commit>& item) { return item->datetime; });

			_thread_finished = true;
		});

	whenCommitsLoaded();
}

void WindowController::whenCommitsLoaded()
{
	while (!_thread_finished && !_is_about_to_quit)
		QCoreApplication::processEvents();

	if (_last_thread.joinable())
		_last_thread.join();

	if (_is_about_to_quit)
		return;

	const auto temp_list = _commits;
	_commits.clear();
	emit commitsChanged();
	qDeleteAll(temp_list);

	for (auto& commit : _commits_temp)
	{
		_commits.push_back(new CommitItem(std::move(commit), this));

		if (_is_about_to_quit)
			return;

		QCoreApplication::processEvents();
	}

	_commits_temp.clear();

	emit commitsChanged();
	emit unlockScreen();
	emit showFilteringView();
}
} // namespace RaportPKUP::UI
