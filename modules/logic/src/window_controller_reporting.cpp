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
QString escapeLatexChars(const QString& input)
{
	auto ret = input;
	ret.replace("\\", R"(\textbackslash{})");
	ret.replace("#", R"(\#)");
	ret.replace("$", R"(\$)");
	ret.replace("%", R"(\%)");
	ret.replace("&", R"(\&)");
	ret.replace("^", R"(\textasciicircum{})");
	ret.replace("_", R"(\_)");
	ret.replace("{", R"(\{)");
	ret.replace("}", R"(\})");
	ret.replace("~", R"(\textasciitilde{})");

	return ret;
}

uint saveCommitsToLaTexStream(QTextStream& stream, const QList<CommitItem*>& commits)
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
		stream << escapeLatexChars(commit->message());
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

void WindowController::saveRaportToFile(QString filename_url)
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
					  "\\usepackage{ragged2e}\n"
					  "\\usepackage[T1]{fontenc}\n"
					  "\\usepackage{mathptmx}\n"
					  "\\usepackage{longtable}\n"
					  "\\usepackage{array}\n"
					  "\\title{Raport dotyczący Podwyższonych Kosztów Uzyskania Przychodów - "
				   << raportDate().toString("dd-MM-yyyy")
				   << "}\n"
					  "\\begin{document}\n";
			stream << "\\begin{flushright}" << escapeLatexChars(city()) << ", " << raportDate().toString("dd-MM-yyyy")
				   << "\\end{flushright}" << '\n';

			stream << "\\paragraph{" << '\n';

			stream << "\\begin{center} Raport od " << defaultLocale.toString(fromDay(), "d MMMM yyyy") << " do "
				   << defaultLocale.toString(toDay(), "d MMMM yyyy") << "\\end{center}" << '\n';
			stream << "}" << '\n';

			stream << "\\begin{flushleft} Lista przekazanych utworów objętych majątkowym prawem autorskim, "
					  "wytworzonych i "
					  "przekazanych pracodawcy przez pracownika: "
				   << escapeLatexChars(authorName()) << ".\\end{flushleft}" << '\n';

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

			const auto duration = saveCommitsToLaTexStream(stream, _commits);

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

	QFileInfo info(tex_file);
	std::wstring cmd =
		L"E:/miktex-portable/texmfs/install/miktex/bin/x64/texify.exe -p " + info.fileName().toStdWString();
	const auto texify_process = _process_factory->createNew(cmd, info.absolutePath().toStdWString());

	texify_process->start();
	while (!texify_process->isError() && !texify_process->isFinished())
		QCoreApplication::processEvents();

	qDebug() << "texify: ErrorCode=" << texify_process->exitCode() << "\ntexify: " << texify_process->readError()
			 << "\n"
			 << "texify: " << texify_process->readOutput();

	QDesktopServices::openUrl(QUrl::fromLocalFile(filename));
}

uint saveCommitsToHtml(QString& text, const QList<CommitItem*>& commits)
{
	size_t counter = 1;
	uint total_duration = 0;

	for (const auto commit : commits)
	{
		if (!commit)
			continue;

		text.append("<tr><td style=\"border: 1px solid black;\">")
			.append(QString::number(counter))
			.append("</td><td style=\"border: 1px solid black;\">")
			.append(commit->repositoryName())
			.append("</td><td style=\"border: 1px solid black;\">")
			.append(commit->time().toString("dd-MM-yyyy"))
			.append("</td><td style=\"border: 1px solid black;\">")
			.append(commit->id())
			.append("</td><td style=\"border: 1px solid black;\">")
			.append(commit->message().toHtmlEscaped())
			.append("</td><td style=\"border: 1px solid black;\">")
			.append(commit->duration)
			.append("</td></tr>\n");

		++counter;
		total_duration += commit->duration;
	}

	return total_duration;
}

QString WindowController::previewDocument() const
{
	QString text;
	QLocale defaultLocale;

	text.append("<!DOCTYPE html>\n<html>\n<body style=\"font-family: serif; font-size: 9pt; margin: 2cm; padding: "
				"0;\">\n<p style=\"text-align: right; margin: 0.5cm 0;\">")
		.append(city().toHtmlEscaped())
		.append(", ")
		.append(raportDate().toString("dd-MM-yyyy"))
		.append("</p>\n<p style=\"text-align: center; font-weight: bold; margin: 0.5cm 0;\">Raport od ")
		.append(defaultLocale.toString(fromDay(), "d MMMM yyyy"))
		.append(" do ")
		.append(defaultLocale.toString(toDay(), "d MMMM yyyy"))
		.append("</p>\n<p style=\"margin: 0.5cm 0;\">Lista przekazanych utworów objętych majątkowym prawem autorskim, "
				"wytworzonych i przekazanych pracodawcy przez pracownika: ")
		.append(authorName().toHtmlEscaped())
		.append(
			".</p><table style=\"margin: 1cm auto; border-collapse: collapse; width: auto\">\n<tr><th style=\"border: "
			"1px solid black; width: 1cm;\">Lp.</th><th style=\"border: 1px solid black; width: 3cm;\">Nazwa "
			"repozytorium</th><th style=\"border: 1px solid black; width: 3cm;\">Data wykonania</th><th "
			"style=\"border: 1px solid black; width: 1.5cm;\">ID</th><th style=\"border: 1px solid black; width: "
			"3.5cm;\">Tytuł</th><th style=\"border: 1px solid black; width: 2cm;\">Liczba godzin</th></tr>\n");

	const auto duration = saveCommitsToHtml(text, _commits);

	text.append("</table>\n<p style=\"margin: 0.5cm 0;\">Łączny czas pracy poświęcony na wytworzenie utworów objętych "
				"prawem autorskim w podanym wyżej okresie: ")
		.append(QString::number(duration))
		.append(" godzin.</p>\n<br><br><br>\n<div style=\"display: flex; margin-bottom: 3cm; justify-content: "
				"space-evenly;\"><span>Podpis pracodawcy</span><span>Podpis pracownika</span></div>\n</body>\n</html>");

	return text;
}
} // namespace RaportPKUP::UI
