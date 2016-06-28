#pragma once

#include "Common.h"

#include <queue>
#include <utility>
#include <thread>
#include <mutex>

#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class HttpLoaderQueue: public QObject
{
    Q_OBJECT

public:
    ~HttpLoaderQueue();
    HttpLoaderQueue(const Url& star_url, const size_t threads_number);

    void Push(const Url& url);
    std::pair<Url, Page> Pop();

    void Stop();

private:
    void RunThreads();
    void LoadHttpPage(const Url& url);

    std::queue<Url> m_url_to_load;
    std::queue<std::pair<Url, Page>> m_web_pages;

    bool m_stop;
    std::vector<std::thread> m_threads;
    std::mutex m_mutex_urls;
    std::mutex m_mutex_pages;
    std::condition_variable m_condition_push;
    std::condition_variable m_condition_pop;
};