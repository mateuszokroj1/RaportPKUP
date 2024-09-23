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
	ret.replace("{", R"(\{)");
	ret.replace("}", R"(\})");
	ret.replace("#", R"(\#)");
	ret.replace("$", R"(\$)");
	ret.replace("%", R"(\%)");
	ret.replace("&", R"(\&)");
	ret.replace("^", R"(\textasciicircum{})");
	ret.replace("_", R"(\_)");
	ret.replace("~", R"(\textasciitilde{})");
	ret.replace(R"(\textbackslash\{\})", R"(\textbackslash{})");

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
		L"miktex/miktex-portable/texmfs/install/miktex/bin/x64/texify.exe -p " + info.fileName().toStdWString();
	const auto texify_process = _process_factory->createNew(cmd, info.absolutePath().toStdWString());

	texify_process->start();

	emit lockScreen2();
	while (!texify_process->isError() && !texify_process->isFinished())
		QCoreApplication::processEvents();

	qDebug() << "texify: ErrorCode=" << texify_process->exitCode() << "\ntexify: " << texify_process->readError()
			 << "\n"
			 << "texify: " << texify_process->readOutput();

	emit unlockScreen();

	if (texify_process->isError())
		emit showWarning("Wystąpił błąd podczas generowania pliku PDF w aplikacji zewnetrznej.");
	else
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

		text.append("<tr><td>")
			.append(QString::number(counter))
			.append("</td><td>")
			.append(commit->repositoryName())
			.append("</td><td>")
			.append(commit->time().toString("dd-MM-yyyy"))
			.append("</td><td>")
			.append(commit->id())
			.append("</td><td>")
			.append(commit->message().toHtmlEscaped())
			.append("</td><td>")
			.append(QString::number(commit->duration))
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

	text.append("<style>#root { font-family: serif; font-size: 10pt; margin: 2cm; padding: 0;}.right { text-align: "
				"right; }p, h1 { margin: 0.5cm 0; }p { text-align: left; }h1 { text-align: center; font-weight: bold; "
				"font-size: inherit; }table { margin: 1cm auto; border-collapse: collapse; font: inherit; }td, th { "
				"border: 1px solid black; }#captions { border: "
				"none; margin: 1cm auto; }#captions td { width: 40cm; border: none; text-align: center; "
				"}</style>\n<div id=\"root\">\n<br><br><p class=\"right\" margin=30>")
		.append(city().toHtmlEscaped())
		.append(", ")
		.append(raportDate().toString("dd-MM-yyyy"))
		.append("</p><br>\n<h1 margin=30>Raport od ")
		.append(defaultLocale.toString(fromDay(), "d MMMM yyyy"))
		.append(" do ")
		.append(defaultLocale.toString(toDay(), "d MMMM yyyy"))
		.append("</h1><br>\n<p margin=30>Lista przekazanych utworów objętych majątkowym prawem autorskim, wytworzonych "
				"i przekazanych pracodawcy przez pracownika: ")
		.append(authorName().toHtmlEscaped())
		.append(".</p><br>\n<table margin=30 align=center border=1 cellpadding=15>\n<tr><th id=\"header1\" "
				"width=60>Lp.</th><th id=\"header2\" width=200>Nazwa repozytorium</th><th id=\"header3\" "
				"width=200>Data wykonania</th><th id=\"header4\" width=100>ID</th><th id=\"header5\" "
				"width=300>Tytuł</th><th id=\"header6\" width=120>Liczba godzin</th></tr>\n");

	const auto duration = saveCommitsToHtml(text, _commits);

	text.append("</table><br>\n<p>Łączny czas pracy poświęcony na wytworzenie utworów objętych prawem autorskim w "
				"podanym wyżej okresie: ")
		.append(QString::number(duration))
		.append(
			" godzin.</p>\n<br><br><br>\n<table align=center id=\"captions\">\n<tr><td width='500' align=center>Podpis "
			"pracodawcy</td><td width='500' align=center>Podpis pracownika</td></tr>\n</table>\n<br><br></div>");

	return text;
}
} // namespace RaportPKUP::UI
