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
    connect(m_ui.m_button_start, &QPushButton::released, this, &HtmlPageSearcher::Started);
    connect(m_ui.m_button_stop, &QPushButton::released, this, &HtmlPageSearcher::Stoped);
    connect(m_ui.m_button_pause, &QPushButton::released, this, &HtmlPageSearcher::Paused);

}

void HtmlPageSearcher::Started()
{
    std::string start_url = m_ui.m_start_url_text_edit->toPlainText().toStdString();
    std::string search_text = m_ui.m_search_text_edit->toPlainText().toStdString();
    const size_t url_max_number = m_ui.m_url_max_number_spin_box->value();
    const size_t threads_number = m_ui.m_threads_number_spin_box->value();

    m_ui.m_text_edit->clear();
    m_ui.m_progress_bar->setValue(0);
    m_ui.m_progress_bar->setMaximum(url_max_number);

    m_ui.m_start_url_text_edit->setDisabled(true);
    m_ui.m_search_text_edit->setDisabled(true);
    m_ui.m_url_max_number_spin_box->setDisabled(true);
    m_ui.m_threads_number_spin_box->setDisabled(true);

    
    auto& text_edit = m_ui.m_text_edit;
    auto page_handler = [text_edit](const Url& text, const Page& page)
    {
        text_edit->insertPlainText(QString::fromStdString(text + "\n"));
        text_edit->moveCursor(QTextCursor::NextRow);
    };

    QThread* search_thread = new QThread;
    BfsSearcher* searcher = new BfsSearcher(start_url, search_text, url_max_number, threads_number, page_handler);

    searcher->moveToThread(search_thread);
    connect(search_thread, &QThread::started, searcher, &BfsSearcher::BFS);
    connect(search_thread, &QThread::finished, search_thread, &QThread::deleteLater);
    connect(searcher, &BfsSearcher::ProgresIncliment, m_ui.m_progress_bar, &QProgressBar::setValue);
    search_thread->start();
}

void HtmlPageSearcher::Stoped()
{
    m_ui.m_start_url_text_edit->setEnabled(true);
    m_ui.m_search_text_edit->setEnabled(true);
    m_ui.m_url_max_number_spin_box->setEnabled(true);
    m_ui.m_threads_number_spin_box->setEnabled(true);

}

void HtmlPageSearcher::Paused()
{

}