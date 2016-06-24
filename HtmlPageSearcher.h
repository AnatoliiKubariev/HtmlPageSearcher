#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_HtmlPageSearcher.h"

class HtmlPageSearcher : public QMainWindow
{
	Q_OBJECT

public:
	HtmlPageSearcher(QWidget *parent = 0);
	~HtmlPageSearcher();

private:
	Ui::HtmlPageSearcherClass ui;
};
