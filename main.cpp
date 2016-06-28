#include "HtmlPageSearcher.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HtmlPageSearcher w;
    w.show();

    return a.exec();
}
