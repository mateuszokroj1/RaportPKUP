#include <QtCore/QStandardPaths>
#include <QtCore/QString>
#include <QtCore/QUuid>
#include <QtGui/QDesktopServices>
#include <QtQml/QQmlContext>
#include <QtQml/qqmllist.h>

// #include <tbb/tbb.h>

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

void uniqueIdAndSortDescendingByDate(std::vector<std::unique_ptr<Commit>>& commits)
{
	bool can_break = true;

	do
	{
		can_break = true;

		for (int position = 0; position < commits.size() - 1; ++position)
		{
			if (!commits[position])
			{
				commits.erase(commits.begin() + position);
				can_break = false;
				break;
			}

			for (int offset = 1; position + offset < commits.size(); ++offset)
			{
				if (std::strcmp(commits[position]->id, commits[position + offset]->id) == 0)
				{
					commits.erase(commits.begin() + position + offset);
					can_break = false;
					break;
				}

				if (commits[position]->datetime < commits[position + offset]->datetime)
				{
					commits[position].swap(commits[position + offset]);
					can_break = false;
					break;
				}
			}

			if (!can_break)
				break;
		}
	} while (!can_break);
}

void WindowController::searchForCommits()
{
	std::vector<std::unique_ptr<Commit>> result;
	const std::chrono::system_clock::time_point from = fromDay().toStdSysDays();
	const std::chrono::system_clock::time_point to = toDay().toStdSysDays();
	Author author;
	author.email = authorEmail().toStdWString();
	author.name = authorName().toStdWString();

	std::unique_ptr<std::mutex> mutex = std::make_unique<std::mutex>();

	for (auto repo : _repositories)
	// tbb::parallel_for_each(_repositories.begin(), _repositories.end(),
	//					   [&](RepositoryListItem* item)
	{
		RepositoryVisitorImpl visitor(from, to, author);
		repo->accept(visitor);

		// std::unique_lock locker(*mutex);
		std::transform(visitor.commits.cbegin(), visitor.commits.cend(), std::back_inserter(result),
					   [](Commit commit) { return std::make_unique<Commit>(commit); });
		//					   });
	}

	uniqueIdAndSortDescendingByDate(result);

	const auto temp_list = _commits;
	_commits.clear();
	emit commitsChanged();
	qDeleteAll(temp_list);

	std::transform(result.begin(), result.end(), std::back_inserter(_commits),
				   [this](std::unique_ptr<Commit>& commit) { return new CommitItem(std::move(commit), this); });

	emit commitsChanged();
}
} // namespace RaportPKUP::UI
