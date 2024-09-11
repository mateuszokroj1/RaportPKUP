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

void WindowController::saveRaportToFile(QString filename_url)
{
	QString filename;
	if (filename_url.startsWith("file:///"))
		filename = filename_url.mid(8);

	const auto tex_file = filename + ".tex";

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

QString WindowController::previewDocument() const
{
	return "<!DOCTYPE html><html><body><h1>Demo</h1></body></html>";
}
} // namespace RaportPKUP::UI
