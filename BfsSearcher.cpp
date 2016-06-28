#include "BfsSearcher.h"

#include "HttpLoaderQueue.h"

#include <set>

BfsSearcher::BfsSearcher(const Url& start_url, std::string text_to_search,
                         const size_t url_max_number, const size_t threads_number)
    : m_start_url(start_url)
    , m_text_to_search(text_to_search)
    , m_url_max_number(url_max_number)
    , m_threads_number(threads_number)
{

}

void BfsSearcher::BFS()
{
    HttpLoaderQueue fringe(m_start_url, m_threads_number);
    std::set<Url> visited;
    while(m_web_pages.size() < m_url_max_number)
    {
        std::pair<Url, Page> web_page = fringe.Pop();
        emit Found(web_page.first);
        //if(FindText(web_page.second, text_to_search))
        //{

        //}

        std::vector<Url> urls_to_load = FindUrls(web_page.second);
        for(auto& u : urls_to_load)
        {
            if(!visited.count(u))
            {
                fringe.Push(u);
            }
        }

        visited.insert(web_page.first);
        m_web_pages.insert(web_page);
    }
}

bool BfsSearcher::FindText(Page& page, std::string& text_to_search)
{
    std::transform(page.begin(), page.end(), page.begin(), tolower);
    std::transform(text_to_search.begin(), text_to_search.end(), text_to_search.begin(), tolower);
    bool res = false;
    if(page.find(text_to_search) != std::string::npos)
    {
        res = true;
    }
    return res;
}

std::vector<Url> BfsSearcher::FindUrls(Page& page)
{
    std::transform(page.begin(), page.end(), page.begin(), tolower);
    std::vector<std::string> urls;
    size_t url_begin = 0;
    while(page.find("http://", url_begin) != std::string::npos)
    {
        url_begin = page.find("http://", url_begin);
        size_t url_end = page.find_first_not_of(m_valid_url_chars, url_begin);

        std::string url(page.begin() + url_begin, page.begin() + url_end);
        urls.push_back(url);
        url_begin = url_end;
    }
    return urls;
}