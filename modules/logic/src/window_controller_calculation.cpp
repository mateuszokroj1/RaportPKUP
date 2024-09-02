#include <QtCore/QStandardPaths>
#include <QtCore/QString>
#include <QtCore/QUuid>
#include <QtGui/QDesktopServices>
#include <QtQml/QQmlContext>
#include <QtQml/qqmllist.h>

#include <include/Filters.hpp>
#include <include/IProcess.hpp>
#include <include/IRepositoryAccessor.hpp>

#include "InputDataState.hpp"
#include "WindowController.hpp"

using namespace Qt::Literals::StringLiterals;

namespace RaportPKUP::UI
{
class RepositoryVisitorImpl : public IRepositoryVisitor
{
  public:
	RepositoryVisitorImpl(const std::chrono::system_clock::time_point& from,
						  const std::chrono::system_clock::time_point& to, const Author& author)
		: from(from), to(to), author(author)
	{
	}

	void visit(const IRepository& repository) override
	{
		commits = repository.getCommitsFromTimeRange(from, to, author).get();
	}

	const std::chrono::system_clock::time_point from;
	const std::chrono::system_clock::time_point to;
	const Author author;

	std::list<Commit> commits;
};

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
			const std::chrono::system_clock::time_point from = fromDay().toStdSysDays();
			const std::chrono::system_clock::time_point to = toDay().toStdSysDays();
			Author author;
			author.email = authorEmail().toStdWString();
			author.name = authorName().toStdWString();

			for (auto repo : _repositories)
			{
				RepositoryVisitorImpl visitor(from, to, author);
				repo->accept(visitor);

				for (const auto& commit : visitor.commits)
				{
					if (_is_about_to_quit)
						return;

					_commits_temp.push_back(std::make_unique<Commit>(commit));

					QCoreApplication::processEvents();
				}
			}

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
