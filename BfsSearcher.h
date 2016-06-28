#pragma once

#include"Common.h"
#include "HttpLoaderQueue.h"

#include <functional>
#include <vector>
#include <map>
#include <string>

#include <QObject>

class BfsSearcher: public QObject
{
    Q_OBJECT

public:
    using PageHandler = std::function<void(const Url&, Page&)>;

    BfsSearcher(Url& start_url,
                const size_t m_url_max_number, const size_t threads_number,
                PageHandler page_handler);

    void BFS();
    void Stop();

signals:
    void ProgressIncrement(int value);

private:
    std::vector<Url> FindUrls(Page& page);

    size_t m_url_max_number;
    PageHandler m_page_handler;
    HttpLoaderQueue m_fringe;
    bool m_stop;

    const std::string m_valid_url_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&\'()*+,;=";
    const std::vector<std::string> m_allowed_extension = {"", ""};

};