#include "WindowController.hpp"

#include <include/IProcess.hpp>
#include <include/IRepositoryAccessor.hpp>

#include <QtCore/QStandardPaths>
#include <QtCore/QString>
#include <QtCore/QUuid>
#include <QtGui/QDesktopServices>
#include <QtQml/QQmlContext>
#include <QtQml/qqmllist.h>

#include <execution>

using namespace Qt::Literals::StringLiterals;

namespace RaportPKUP::UI
{
void WindowController::searchForCommits()
{
	emit lockScreen();
	_thread_finished = false;

	std::thread thread(
		[this]()
		{
			_commits_temp.clear();

			const DateTime from = fromDay().toStdSysDays();
			const DateTime to = toDay().toStdSysDays() + 23h + 59min + 59s;

			Author author;
			author.email = authorEmail().toStdWString();
			author.name = authorName().toStdWString();

			SynchronizationContainerWrapper<Commit, Commit::Key> container(_commits_temp);

			std::for_each(std::execution::par, _repositories.cbegin(), _repositories.cend(),
						  [this, &from, &to, &author, &container](const RepositoryListItem* repo)
						  {
							  if (_calculation_cancelled.stop_requested())
								  return;

							  repo->getRepository()->getCommitsFromTimeRange(from, to, author, container);
						  });

			_thread_finished = true;
		});

	while (!_thread_finished && !_calculation_cancelled.stop_requested())
		QCoreApplication::processEvents();

	if (thread.joinable())
		thread.join();

	if (_calculation_cancelled.stop_requested())
		return;

	const auto temp_list = _commits;
	_commits.clear();
	emit commitsChanged();
	qDeleteAll(temp_list);

	auto it = _commits_temp.cend();
	do
	{
		--it;
		_commits.push_back(new CommitItem(it->second, this));

		if (_calculation_cancelled.stop_requested())
			return;
	} while (it != _commits_temp.cbegin());

	emit commitsChanged();
	emit unlockScreen();
	emit showFilteringView();
}
} // namespace RaportPKUP::UI
