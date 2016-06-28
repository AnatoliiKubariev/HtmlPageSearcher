#pragma once

#include"Common.h"

#include <vector>
#include <map>
#include <string>

#include <QObject>

class BfsSearcher: public QObject
{
    Q_OBJECT

public:
    BfsSearcher(const Url& start_url, std::string text_to_search,
                const size_t m_url_max_number, const size_t threads_number);

    void BFS();

signals:
    void Found(const std::string& text);

private:
    bool FindText(Page& page, std::string& text_to_search);
    std::vector<Url> FindUrls(Page& page);

    Url m_start_url;
    std::string m_text_to_search;
    size_t m_url_max_number;
    size_t m_threads_number;
    std::map<Url, Page> m_web_pages;

    const std::string m_valid_url_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&\'()*+,;=";
    const std::vector<std::string> m_allowed_extension = {"", ""};

};