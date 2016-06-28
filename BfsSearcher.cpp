#include "BfsSearcher.h"

#include <set>

BfsSearcher::BfsSearcher(Url& start_url,
                         const size_t url_max_number, const size_t threads_number,
                         std::function<void(const Url&, Page&)>(page_handler))
    : m_url_max_number(url_max_number)
    , m_page_handler(page_handler)
    , m_fringe(start_url, threads_number)
    , m_stop(false)
{

}

void BfsSearcher::BFS()
{
    std::set<Url> visited;
    while(visited.size() < m_url_max_number && !m_stop)
    {
        std::pair<Url, Page> web_page = m_fringe.Pop();
        visited.insert(web_page.first);
        m_page_handler(web_page.first, web_page.second);
        ProgressIncrement(visited.size());

        std::vector<Url> urls_to_load = FindUrls(web_page.second);
        for(auto& u : urls_to_load)
        {
            if(visited.count(u) == 0)
            {
                m_fringe.Push(u);
            }
        }
    }
}

void BfsSearcher::Stop()
{
    m_stop = true;
    m_fringe.Stop();
}

std::vector<Url> BfsSearcher::FindUrls(Page& page)
{
    std::vector<std::string> urls;
    size_t url_begin = page.find("http://", 0);
    while(url_begin != std::string::npos)
    {
        size_t url_end = page.find_first_not_of(m_valid_url_chars, url_begin);

        std::string url(page.begin() + url_begin, page.begin() + url_end);
        if(url.back() == '\\' || url.back() == '/')
        {
            url.pop_back();
        }
        urls.push_back(url);
        url_begin = page.find("http://", url_end);
    }
    return urls;
}