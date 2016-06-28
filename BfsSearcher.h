#pragma once

#include"Common.h"

#include <functional>
#include <vector>
#include <map>
#include <string>

#include <QObject>

class BfsSearcher: public QObject
{
    Q_OBJECT

public:
    BfsSearcher(Url& start_url,
                const size_t m_url_max_number, const size_t threads_number,
                std::function<void(const Url&, Page&)>);

    ~BfsSearcher()
    {

    }

    void BFS();
    void Stop();

signals:
    void ProgresIncliment(int value);

private:
    std::vector<Url> FindUrls(Page& page);

    Url m_start_url;
    size_t m_url_max_number;
    size_t m_threads_number;
    std::function<void(const Url&, Page&)> m_page_handler;
    bool m_stop;

    const std::string m_valid_url_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&\'()*+,;=";
    const std::vector<std::string> m_allowed_extension = {"", ""};

};