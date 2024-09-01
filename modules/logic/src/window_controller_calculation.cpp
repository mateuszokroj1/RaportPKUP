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
	auto _ = std::async(
		[this]()
		{
			emit lockScreen();

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

				std::transform(visitor.commits.cbegin(), visitor.commits.cend(), std::back_inserter(_commits_temp),
							   [](Commit commit) { return std::make_unique<Commit>(commit); });
			}

			Filters::removeEmptyPointers(_commits_temp);
			Filters::unique<std::unique_ptr<Commit>, Commit::Id>(_commits_temp, [](const std::unique_ptr<Commit>& item)
																 { return item->id; });
			Filters::sortDescending<std::unique_ptr<Commit>, DateTime>(
				_commits_temp, [](const std::unique_ptr<Commit>& item) { return item->datetime; });

			emit searchingDone();
		});
}

void WindowController::whenCommitsLoaded()
{
	const auto temp_list = _commits;
	_commits.clear();
	emit commitsChanged();
	qDeleteAll(temp_list);

	std::ranges::transform(_commits_temp, std::back_inserter(_commits),
						   [this](std::unique_ptr<Commit>& commit) { return new CommitItem(std::move(commit), this); });

	_commits_temp.clear();

	emit commitsChanged();
	emit unlockScreen();
	emit showFilteringView();
}
} // namespace RaportPKUP::UI
