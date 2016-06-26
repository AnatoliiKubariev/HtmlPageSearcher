#pragma once

#include "Common.h"
#include "HttpLoader.h"
#include "ui_HtmlPageSearcher.h"

#include <set>
#include <map>

#include <QMainWindow>


class HtmlPageSearcher: public QMainWindow
{
	Q_OBJECT

public:
	HtmlPageSearcher(QWidget* parent = 0);

	void BFS(const Url& start_url);

	std::vector<Url> FindUrl(std::string& request_string);

private:
	Ui::HtmlPageSearcherClass ui;
	HttpLoader m_loader;

	std::map<Url, Page> m_web_peges;
	std::set<Url> m_visited;
	size_t m_url_number = 20;

	const std::string m_valid_url_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&\'()*+,;=";
};
