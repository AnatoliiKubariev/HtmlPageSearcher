#pragma once

#include "Common.h"
#include "ui_HtmlPageSearcher.h"

#include <set>
#include <map>
#include <memory>
#include <thread>
#include <mutex>

#include <QMainWindow>


class HtmlPageSearcher: public QMainWindow
{
    Q_OBJECT

public:
    HtmlPageSearcher(QWidget* parent = 0);

private:
    void ShowResult(const std::string& text);

    Ui::HtmlPageSearcherClass m_ui;
};
