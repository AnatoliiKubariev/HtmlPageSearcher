#include "HtmlPageSearcher.h"

#include "BfsSearcher.h"

#include <queue>
#include <thread>
#include <memory>

#include <QPushButton>
#include <QThread>

HtmlPageSearcher::HtmlPageSearcher(QWidget* parent)
    : QMainWindow(parent)
{
    m_ui.setupUi(this);

    QThread* search_thread = new QThread;
    BfsSearcher* searcher = new BfsSearcher("http://finger-style.ru/", "guitar", 20, 2);
    searcher->moveToThread(search_thread);

    connect(search_thread, &QThread::started, searcher, &BfsSearcher::BFS);
    connect(searcher, &BfsSearcher::Found, this, &HtmlPageSearcher::ShowResult, Qt::DirectConnection);
    connect(search_thread, &QThread::finished, search_thread, &QThread::deleteLater);
    search_thread->start();
}

void HtmlPageSearcher::ShowResult(const std::string& text)
{
    m_ui.m_text_edit->moveCursor(QTextCursor::NextRow);
    m_ui.m_text_edit->insertPlainText(QString::fromStdString(text));
    m_ui.m_text_edit->moveCursor(QTextCursor::NextRow);
    m_ui.m_text_edit->insertPlainText("\n");
}