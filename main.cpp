#include "HtmlPageSearcher.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	HtmlPageSearcher w;
	w.show();

	w.ReadHtml(QUrl("http://pastebin.com/"));
	w.FindUrl();

	return a.exec();
}
