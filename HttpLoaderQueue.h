#pragma once

#include "Common.h"
#include "WebPage.h"

#include <atomic>
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
    WebPage Pop();

    void Stop();
    bool IsEmpty();

private:
    void RunThreads();
    void LoadHttpPage(const Url& url);

    std::queue<Url> m_url_to_load;
    std::queue<WebPage> m_web_pages;
    std::atomic_int m_runnig_tasks_counter;

    bool m_stop;
    std::vector<std::thread> m_threads;
    std::mutex m_mutex_urls;
    std::mutex m_mutex_pages;
    std::condition_variable m_condition_push;
    std::condition_variable m_condition_pop;
};

struct Decrementor
{
    Decrementor(std::atomic_int& counter)
        :m_counter(counter)
    {
        ++m_counter;
    }
    ~Decrementor()
    {
        --m_counter;
    }

    std::atomic_int& m_counter;
};