#pragma once

#include <set>
#include <string>

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "ui_HtmlPageSearcher.h"

class HtmlPageSearcher: public QMainWindow
{
	Q_OBJECT

public:
	HtmlPageSearcher(QWidget* parent = 0);

	void ReadHtml(const QUrl& url);
	void FindUrl();

private:
	void ReadData();
	void Finished();

	Ui::HtmlPageSearcherClass ui;

	std::string m_request_string;

	QNetworkAccessManager m_request_manager;
	QNetworkReply* m_reply;
	QEventLoop m_loop;

	size_t m_url_number = 20;
	std::set<std::string> m_urls;

	const std::string m_valid_url_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&\'()*+,;=";
};
