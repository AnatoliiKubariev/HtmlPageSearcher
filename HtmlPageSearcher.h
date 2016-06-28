#pragma once

#include "BfsSearcher.h"
#include "Common.h"
#include "ui_HtmlPageSearcher.h"

#include <memory>
#include <string>

#include <QMainWindow>
#include <QThread>

class HtmlPageSearcher: public QMainWindow
{
    Q_OBJECT

public:
    HtmlPageSearcher(QWidget* parent = 0);

private:
    void Started();
    void Stoped();
    void Paused();
    bool FindText(Page& page, std::string& text_to_search);

    std::string m_search_text;
    std::unique_ptr<BfsSearcher> m_searcher;

    Ui::HtmlPageSearcherClass m_ui;
};
