#include <QtCore/QStandardPaths>
#include <QtCore/QString>
#include <QtCore/QUuid>
#include <QtGui/QDesktopServices>
#include <QtQml/qqmllist.h>

// #include <tbb/tbb.h>

#include <include/IProcess.hpp>
#include <include/IRepositoryAccessor.hpp>

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

WindowController::WindowController(std::weak_ptr<Application> app) : _application(app)
{
	if (auto app_ptr = _application.lock())
	{
		auto weak = app_ptr->get<IProcessFactory>();
		if (!(_process_factory = weak.lock()))
			return;

		_repository_detector = app_ptr->get<IRepositoryDetector>().lock();

		if (auto qml_engine = app_ptr->getQmlEngine())
			creatingSteps(qml_engine);
	}

	const auto today = QDate::currentDate();
	_fromDay.setValue(QDate(today.year(), today.month(), 1));
	_toDay.setValue(QDate(today.year(), today.month(), today.daysInMonth()));

	connect(this, &WindowController::commitsChanged, this, &WindowController::isFilteringEnabledChanged);
}

WindowController::~WindowController() noexcept
{
	qDeleteAll(_items_list);
	qDeleteAll(_repositories);
	qDeleteAll(_commits);
}

void WindowController::creatingSteps(QQmlApplicationEngine* qml)
{
	auto item = new MainViewItem(this);
	{
		item->setName(u"Wprowadzanie danych"_qs);
		item->enable(true);

		const QUrl url(u"qrc:/qt/qml/content/DataInputStepView.qml"_qs);
		QQmlComponent component(qml, url, item);
		auto view = qobject_cast<QQuickItem*>(component.create());
		auto errors = component.errors();

		if (view && errors.empty())
		{
			item->setContent(view);

			_items_list.push_back(item);
		}
	}

	{
		item = new MainViewItem(this);
		item->setName(u"Uzupełnianie godzin"_qs);
		item->enable(true);

		const QUrl url(u"qrc:/qt/qml/content/DataFilteringStepView.qml"_qs);
		QQmlComponent component(qml, url, item);
		auto view = qobject_cast<QQuickItem*>(component.create());
		auto errors = component.errors();

		if (view && errors.empty())
		{
			item->setContent(view);

			_items_list.push_back(item);
		}
	}

	{
		item = new MainViewItem(this);
		item->setName(u"Generowanie raportu"_qs);
		item->enable(true);

		const QUrl url(u"qrc:/qt/qml/content/ReportingStepView.qml"_qs);
		QQmlComponent component(qml, url, item);
		auto view = qobject_cast<QQuickItem*>(component.create());
		auto errors = component.errors();

		if (view && errors.empty())
		{
			item->setContent(view);

			_items_list.push_back(item);
		}
	}

	_items = QQmlListProperty<MainViewItem>(this, &_items_list);
	emit itemsChanged();
}

QQmlListProperty<RepositoryListItem> WindowController::repositories()
{
	return QQmlListProperty<RepositoryListItem>(this, &_repositories);
}

QQmlListProperty<CommitItem> WindowController::commits()
{
	return QQmlListProperty<CommitItem>(this, &_commits);
}

QString WindowController::authorName() const
{
	return _authorName.value();
}

QString WindowController::authorEmail() const
{
	return _authorEmail.value();
}

QDate WindowController::fromDay() const
{
	return _fromDay.value();
}

QDate WindowController::toDay() const
{
	return _toDay.value();
}

QString WindowController::repositoryPath() const
{
	return _repositoryPath.value();
}

bool WindowController::canFetchBefore() const
{
	return _canFetchBefore.value();
}

bool WindowController::isFilteringEnabled() const
{
	return _commits.count();
}

void WindowController::setAuthorName(QString value)
{
	_authorName.setValue(std::move(value));
}

void WindowController::setAuthorEmail(QString value)
{
	_authorEmail.setValue(std::move(value));
}

void WindowController::setFromDay(QDate value)
{
	if (value > _toDay.value())
		value = _toDay.value();

	_fromDay.setValue(std::move(value));
}

void WindowController::setToDay(QDate value)
{
	if (value < _fromDay.value())
		value = _fromDay.value();

	_toDay.setValue(std::move(value));
}

void WindowController::setRepositoryPath(QString value)
{
	QDir dir(value);
	if (!dir.exists())
	{
		_repositoryPath.setValue("");
		return;
	}

	_repositoryPath.setValue(value);
}

void WindowController::setCanFetchBefore(bool value)
{
	_canFetchBefore.setValue(value);
}

QBindable<QString> WindowController::bindableAuthorName() const
{
	return &_authorName;
}

QBindable<QString> WindowController::bindableAuthorEmail() const
{
	return &_authorEmail;
}

QBindable<QDate> WindowController::bindableFromDay() const
{
	return &_fromDay;
}

QBindable<QDate> WindowController::bindableToDay() const
{
	return &_toDay;
}

QBindable<QString> WindowController::bindableRepositoryPath() const
{
	return &_repositoryPath;
}

QBindable<bool> WindowController::bindableCanFetchBefore() const
{
	return &_canFetchBefore;
}

void WindowController::browseForRepository()
{ /*
	QFileDialog dialog;
	dialog.setAcceptMode(QFileDialog::AcceptOpen);
	dialog.setFileMode(QFileDialog::Directory);
	dialog.setViewMode(QFileDialog::ViewMode::List);
	dialog.setOptions({QFileDialog::Option::ShowDirsOnly});
	dialog.setModal(true);

	if (dialog.exec() != QDialog::DialogCode::Accepted)
		return;

	const auto& accessor = _repository_detector->getAccessor();

	const auto dir = dialog.directory();
	if (auto corrected_dir = _repository_detector->detect(dir.filesystemPath()).get())
	{
		setRepositoryPath(QString::fromStdWString(corrected_dir->generic_wstring()));
	}*/
}

void WindowController::addRepository()
{
	const auto& accessor = _repository_detector->getAccessor();
	const auto wstr = repositoryPath().toStdWString();

	// TODO find duplicate
	std::optional<Author> author;
	if (auto repo = accessor.openRepository(std::filesystem::path(std::move(wstr))).get())
	{
		author = repo->getDefaultAuthor();

		_repositories.push_back(new RepositoryListItem(std::move(repo), this));
		emit repositoriesChanged();
	}

	if (_repositories.count() == 1 && author)
	{
		setAuthorEmail(QString::fromStdWString(author->email));
		setAuthorName(QString::fromStdWString(author->name));
	}
}

void WindowController::removeRepository(RepositoryListItem* ptr)
{
	const auto index = _repositories.indexOf(ptr);
	if (index < 0 || index >= _repositories.count())
		return;

	_repositories.removeAt(index);
	ptr->deleteLater();

	emit repositoriesChanged();
}

void WindowController::clearRepositories()
{
	qDeleteAll(_repositories);
	_repositories.clear();

	emit repositoriesChanged();
}

void WindowController::searchForCommits()
{
	std::list<Commit> result;
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
		// TODO implement fetch before

		RepositoryVisitorImpl visitor(from, to, author);
		repo->accept(visitor);

		std::unique_lock locker(*mutex);
		std::copy(visitor.commits.cbegin(), visitor.commits.cend(), std::back_inserter(result));
		//					   });
	}

	// std::unique(result.begin(), result.end(), [](const Commit& a, const Commit& b) { return a.id == b.id; });

	// std::sort(result.begin(), result.end(), [](const Commit& a, const Commit& b) { return a.datetime >= b.datetime;
	// });

	qDeleteAll(_commits);
	_commits.clear();

	std::transform(result.cbegin(), result.cend(), std::back_inserter(_commits),
				   [this](const Commit& commit) { return new CommitItem(commit, this); });

	emit commitsChanged();
}

void WindowController::removeCommit(CommitItem* ptr)
{
	const auto index = _commits.indexOf(ptr);
	if (index < 0 || index >= _commits.count())
		return;

	_commits.removeAt(index);
	ptr->deleteLater();

	emit repositoriesChanged();
}

uint saveCommitsToStream(QTextStream& stream, const QList<CommitItem*>& commits)
{
	size_t counter = 1;
	uint total_duration = 0;

	for (const auto commit : commits)
	{
		if (!commit)
			continue;

		stream << counter;
		stream << " & ";
		stream << commit->id();
		stream << " & ";
		stream << commit->time().toString();
		stream << " & ";
		stream << commit->message();
		stream << " & ";
		stream << commit->duration;

		if (commits.last() != commit)
			stream << "\\\\\n \\hline \\\\";

		stream << '\n';

		++counter;
		total_duration += commit->duration;
	}

	return total_duration;
}

void WindowController::saveRaportToFile()
{
	//	QFileDialog dialog;
	//	dialog.setAcceptMode(QFileDialog::AcceptSave);
	//	dialog.setFileMode(QFileDialog::FileMode::AnyFile);

	//	dialog.setNameFilter("Adobe PDF (*.pdf)");

	//	dialog.setModal(true);

	//	if (dialog.exec() != QDialog::DialogCode::Accepted)
	//		return;

	const auto root_temp_dir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
	const auto id = QUuid::createUuid().toString(QUuid::StringFormat::Id128);
	const auto filename = root_temp_dir + "/" + id + ".tmp.tex";

	{
		QFile file(filename);
		if (!file.open(QIODeviceBase::NewOnly | QIODeviceBase::ReadWrite | QIODeviceBase::Text))
			return;

		if (!file.isWritable())
		{
			file.close();
			return;
		}

		{
			QTextStream stream(&file);

			stream << "\\documentclass[10pt, a4paper, oneside]{report}" << '\n';
			stream << "\\usepackage{ragged2e}" << '\n';
			stream << "\\usepackage[T1]{fontenc}" << '\n';
			stream << "\\usepackage{longtable}" << '\n';
			stream << "\\usepackage{array}" << '\n';

			stream << "\\title{Raport}";

			stream << "\\begin{document}";
			stream << "\\begin{flushright} Słupsk, " << QDate::currentDate().toString("dd-MM-yyyy")
				   << "\\end{flushright}";

			stream << "\\paragraph{";

			stream << "\\begin{center} Raport od " << fromDay().toString() << " do " << toDay().toString()
				   << "\\end{center}";
			stream << "}";

			stream
				<< "\\begin{flushleft} Lista przekazanych utworów objętych majątkowym prawem autorskim, wytworzonych i "
				   "przekazanych pracodawcy przez pracownika: "
				<< authorName() << ".\\end{flushleft}";

			stream << "\\begin{center}";
			stream << "\\begin{longtable}{| l | l | l | l | l |}";

			stream << "\\hline \\multicolumn{1}{| c |}{\\textbf{Lp.}} & \\multicolumn{1}{c |}{\\textbf{ID}} & "
					  "\\multicolumn{1}{c |}{\\textbf{Data publikacji}} & \\multicolumn{1}{c |}{\\textbf{Tytuł}} & "
					  "\\multicolumn{1}{c "
					  "|}{\\textbf{Liczba godzin}}\\\\ \\hline";
			stream << "\\endfirsthead";

			stream << "\\multicolumn{5}{c}%";
			stream << "{} \\\\";
			stream << "\\hline \\multicolumn{1}{| c |}{\\textbf{Lp.}} & \\multicolumn{1}{c |}{\\textbf{ID}} & "
					  "\\multicolumn{1}{c |}{\\textbf{Data publikacji}} & \\multicolumn{1}{c |}{\\textbf{Tytuł}} & "
					  "\\multicolumn{1}{c "
					  "|}{\\textbf{Liczba godzin}}\\\\ \\hline";
			stream << "\\endhead";

			stream << "\\hline";
			stream << "\\endfoot";

			const auto duration = saveCommitsToStream(stream, _commits);

			stream << "\\end{longtable}";
			stream << "\\end{center}";

			stream << "\\begin{flushleft}";
			stream << "Łączny czas pracy poświęcony na wytworzenie utworów objętych prawem autorskim w podanym wyżej "
					  "okresie: "
				   << duration << " godzin.";
			stream << "\\end{flushleft}";

			stream << "\\paragraph{}";
			stream << "\\begin{center}";
			stream << "\\begin{tabular}{p{55mm} p{55mm}}";

			stream << "\\begin{flushleft}Podpis pracodawcy\\end{flushleft} & \\begin{flushright} Podpis "
					  "pracownika\\end{flushright}";

			stream << "\\end{tabular}";
			stream << "\\end{center}";

			stream << "\\end{document}";

			stream.flush();
		}

		file.flush();
		file.close();
	}

	QDesktopServices::openUrl(QUrl::fromLocalFile(filename));
}
} // namespace RaportPKUP::UI
