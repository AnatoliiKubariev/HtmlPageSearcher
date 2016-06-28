#include "HttpLoaderQueue.h"

HttpLoaderQueue::~HttpLoaderQueue()
{
    {
        std::unique_lock<std::mutex> lock(m_mutex_urls);

        while(!m_url_to_load.empty())
        {
            m_url_to_load.pop();
        }
        m_stop = true;
    }

    m_condition_push.notify_all();
    for(auto& thread : m_threads)
    {
        thread.join();
    }
}

HttpLoaderQueue::HttpLoaderQueue(const Url& star_url, const size_t threads_number)
    : m_url_to_load({star_url})
    , m_stop(false)
{
    for(auto i = 0; i < threads_number; ++i)
    {
        m_threads.push_back(std::thread(&HttpLoaderQueue::RunThreads, this));
    }
}


void HttpLoaderQueue::Push(const Url& url)
{
    {
        std::unique_lock<std::mutex> lock(m_mutex_urls);
        //m_condition_push.wait(lock, [&]() { return ; });

        m_url_to_load.push(url);
    }

    m_condition_push.notify_one();
}

std::pair<Url, Page> HttpLoaderQueue::Pop()
{
    std::unique_lock<std::mutex> lock(m_mutex_pages);
    m_condition_pop.wait(lock, [&]() { return !m_web_pages.empty(); });

    std::pair<Url, Page> to_pop = m_web_pages.front();
    m_web_pages.pop();

    return to_pop;
}

void HttpLoaderQueue::Stop()
{
    m_stop = true;
    m_condition_push.notify_all();
}

void HttpLoaderQueue::RunThreads()
{
    Url current_url;
    while(true)
    {
        {
            std::unique_lock<std::mutex> lock(m_mutex_urls);

            m_condition_push.wait(lock, [&]() { return !m_url_to_load.empty() || m_stop; });

            if(m_url_to_load.empty() && m_stop)
            {
                return;
            }
            current_url = m_url_to_load.front();
            m_url_to_load.pop();
        }
        LoadHttpPage(current_url);
    }
}


void HttpLoaderQueue::LoadHttpPage(const Url& url)
{
    QNetworkRequest request(QUrl(QString::fromStdString(url)));
    QNetworkAccessManager request_manager;
    QNetworkReply* reply = request_manager.get(request);

    Page page;
    QEventLoop loop;
    connect(reply, &QNetworkReply::readyRead, [&]()
    {
        page = reply->readAll().toStdString();
    });
    connect(reply, &QNetworkReply::finished, [&]()
    {
        reply->deleteLater();
        loop.exit();
    });
    loop.exec();

    std::unique_lock<std::mutex> lock(m_mutex_pages);
    m_web_pages.emplace(url, page);
    m_condition_pop.notify_one();
}