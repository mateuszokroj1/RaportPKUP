#include "WindowController.hpp"

#include <include/IProcess.hpp>
#include <include/IRepositoryAccessor.hpp>

#include <QtCore/QStandardPaths>
#include <QtCore/QString>
#include <QtCore/QUuid>
#include <QtGui/QDesktopServices>
#include <QtQml/QQmlContext>
#include <QtQml/qqmllist.h>

using namespace Qt::Literals::StringLiterals;

namespace RaportPKUP::UI
{
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
		stream << commit->repositoryName();
		stream << " & ";
		stream << commit->time().toString("dd-MM-yyyy");
		stream << " & ";
		stream << commit->id();
		stream << " & \\RaggedRight{";
		stream << commit->message();
		stream << "} & ";
		stream << commit->duration;

		if (commits.last() != commit)
			stream << "\\\\\n \\hline\n";

		stream << '\n';

		++counter;
		total_duration += commit->duration;
	}

	return total_duration;
}

void WindowController::saveRaportToFile(QString filename_url) // TODO escape LaTEX special chars
{
	QString filename;
	if (filename_url.startsWith("file:///"))
		filename = filename_url.mid(8);

	const auto tex_file =
		filename.endsWith(".pdf", Qt::CaseInsensitive) ? filename.chopped(4) + ".tex" : filename + ".tex";

	{
		QFile file(tex_file);
		if (!file.open(QIODeviceBase::NewOnly | QIODeviceBase::ReadWrite | QIODeviceBase::Text))
			return;

		if (!file.isWritable())
		{
			file.close();
			return;
		}

		QLocale defaultLocale;

		{
			QTextStream stream(&file);

			stream << "\\documentclass[9pt]{extreport}\n"
					  "\\usepackage[a4paper, total={7in, 9in}]{geometry}\n"
					  "\\usepackage{ragged2e}\n\\usepackage[T1]{fontenc}\n"
					  "\\usepackage{longtable}\n"
					  "\\usepackage{array}\n"
					  "\\title{Raport dotyczący Podwyższonych Kosztów Uzyskania Przychodów - }\n"
					  "\\begin{document}\n";
			stream << "\\begin{flushright} " << city() << ", " << raportDate().toString("dd-MM-yyyy")
				   << "\\end{flushright}" << '\n';

			stream << "\\paragraph{" << '\n';

			stream << "\\begin{center} Raport od " << defaultLocale.toString(fromDay()) << " do "
				   << defaultLocale.toString(toDay()) << "\\end{center}" << '\n';
			stream << "}" << '\n';

			stream << "\\begin{flushleft} Lista przekazanych utworów objętych majątkowym prawem autorskim, "
					  "wytworzonych i "
					  "przekazanych pracodawcy przez pracownika: "
				   << authorName() << ".\\end{flushleft}" << '\n';

			stream << "\\begin{center}" << '\n';
			stream << "\\begin{longtable}{|p{1cm}|p{3cm}|p{3cm}|p{1.5cm}|p{3.5cm}|p{2cm}|}" << '\n';

			stream << "\\hline \\multicolumn{1}{|c|}{\\textbf{Lp.}} & \\multicolumn{1}{c|}{\\textbf{Nazwa "
					  "repozytorium}} "
					  "& "
					  "\\multicolumn{1}{c|}{\\textbf{Data wykonania}} & \\multicolumn{1}{|c|}{\\textbf{ID}} & "
					  "\\multicolumn{1}{c|}{\\textbf{Tytuł}} & \\multicolumn{1}{c|}{\\textbf{Liczba godzin}} \\\\ "
					  "\\hline"
				   << '\n';
			stream << "\\endfirsthead" << '\n';

			stream << "\\multicolumn{6}{c}{} \\\\" << '\n';
			stream << "\\hline \\multicolumn{1}{|c|}{\\textbf{Lp.}} & \\multicolumn{1}{c|}{\\textbf{Nazwa "
					  "repozytorium}} "
					  "& "
					  "\\multicolumn{1}{c|}{\\textbf{Data wykonania}} & \\multicolumn{1}{|c|}{\\textbf{ID}} & "
					  "\\multicolumn{1}{c|}{\\textbf{Tytuł}} & \\multicolumn{1}{c|}{\\textbf{Liczba godzin}} \\\\ "
					  "\\hline"
				   << '\n';
			stream << "\\endhead" << '\n';

			stream << "\\hline" << '\n';
			stream << "\\endfoot" << '\n';

			const auto duration = saveCommitsToStream(stream, _commits);

			stream << "\\end{longtable}" << '\n';
			stream << "\\end{center}" << '\n';

			stream << "\\begin{flushleft}";
			stream << "Łączny czas pracy poświęcony na wytworzenie utworów objętych prawem autorskim w podanym wyżej "
					  "okresie: "
				   << duration << " godzin.";
			stream << "\\end{flushleft}" << '\n';

			stream << "\\paragraph{}" << '\n';
			stream << "\\begin{center}" << '\n';
			stream << "\\begin{tabular}{p{55mm} p{55mm}}" << '\n';

			stream << "\\begin{flushleft}Podpis pracodawcy\\end{flushleft} & \\begin{flushright} Podpis "
					  "pracownika\\end{flushright}"
				   << '\n';

			stream << "\\end{tabular}" << '\n';
			stream << "\\end{center}" << '\n';

			stream << "\\end{document}";

			stream.flush();
		}

		file.flush();
		file.close();
	}

	QDesktopServices::openUrl(QUrl::fromLocalFile(filename));
}

QString WindowController::previewDocument() const
{
	return "<!DOCTYPE html><html><body><h1>Demo</h1></body></html>";
}
} // namespace RaportPKUP::UI
