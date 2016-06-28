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
    void Started();
    void Stoped();
    void Paused();

    Ui::HtmlPageSearcherClass m_ui;
};
