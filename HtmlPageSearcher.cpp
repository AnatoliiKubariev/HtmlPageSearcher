#include "HtmlPageSearcher.h"

#include <queue>
#include <thread>
#include <memory>

#include <QPushButton>
#include <QMessageBox>

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
    if(m_ui.m_start_url_text_edit->toPlainText().isEmpty())
    {
        QMessageBox message_box;
        message_box.setText("Error");
        message_box.setInformativeText("Start URL empty");
        message_box.exec();
        return;
    }
    if(m_ui.m_search_text_edit->toPlainText().isEmpty())
    {
        QMessageBox message_box;
        message_box.setText("Error!");
        message_box.setInformativeText("Start Search text empty");
        message_box.exec();
        return;
    }

    std::string start_url = m_ui.m_start_url_text_edit->toPlainText().toStdString();
    m_search_text = m_ui.m_search_text_edit->toPlainText().toStdString();
    const size_t url_max_number = m_ui.m_url_max_number_spin_box->value();
    const size_t threads_number = m_ui.m_threads_number_spin_box->value();

    m_ui.m_text_edit->clear();
    m_ui.m_progress_bar->setValue(0);
    m_ui.m_progress_bar->setMaximum(url_max_number);

    m_ui.m_start_url_text_edit->setDisabled(true);
    m_ui.m_search_text_edit->setDisabled(true);
    m_ui.m_url_max_number_spin_box->setDisabled(true);
    m_ui.m_threads_number_spin_box->setDisabled(true);
    m_ui.m_button_start->setDisabled(true);

    m_ui.m_button_stop->setEnabled(true);

    auto& text_edit = m_ui.m_text_edit;
    auto page_handler = [&](const Url& text, Page& page)
    {
        if(FindText(page, m_search_text))
        {
            text_edit->insertPlainText(QString::fromStdString(text + "\n"));
            text_edit->moveCursor(QTextCursor::NextRow);
        }
    };

    QThread* search_thread = new QThread(this);
    m_searcher.reset(new BfsSearcher(start_url, url_max_number, threads_number, page_handler));
    m_searcher->moveToThread(search_thread);
    connect(search_thread, &QThread::started, m_searcher.get(), &BfsSearcher::BFS);
    connect(search_thread, &QThread::finished, search_thread, &QThread::deleteLater);
    connect(m_searcher.get(), &BfsSearcher::ProgressIncrement, m_ui.m_progress_bar, &QProgressBar::setValue);
    search_thread->start();
}

void HtmlPageSearcher::Stoped()
{
    m_ui.m_start_url_text_edit->setEnabled(true);
    m_ui.m_search_text_edit->setEnabled(true);
    m_ui.m_url_max_number_spin_box->setEnabled(true);
    m_ui.m_threads_number_spin_box->setEnabled(true);
    m_ui.m_button_start->setEnabled(true);

    m_ui.m_button_stop->setDisabled(true);

    m_searcher->Stop();
}

void HtmlPageSearcher::Paused()
{
}

bool HtmlPageSearcher::FindText(Page& page, std::string& text_to_search)
{
    std::transform(page.begin(), page.end(), page.begin(), tolower);
    std::transform(text_to_search.begin(), text_to_search.end(), text_to_search.begin(), tolower);
    bool res = false;
    if(page.find(text_to_search) != std::string::npos)
    {
        res = true;
    }
    return res;
}