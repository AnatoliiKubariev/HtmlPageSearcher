#include "HtmlPageSearcher.h"

#include <queue>

HtmlPageSearcher::HtmlPageSearcher(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}


void HtmlPageSearcher::BFS(const Url& start_url)
{
	std::queue<Url> fringe({start_url});
	while(m_visited.size() <= m_url_number)
	{
		Url url = fringe.front();
		Page page = m_loader.LoadHttpPage(url);
		m_web_peges[url] = page;
		fringe.pop();
		m_visited.insert(url);
		std::vector<Url> temp_urls = FindUrl(page);
		for(auto& u : temp_urls)
		{
			fringe.push(u);
		}
	}

	for(auto v: m_visited)
	{
		ui.textEdit->moveCursor(QTextCursor::NextRow);
		ui.textEdit->insertPlainText(QString::fromStdString(v));
		ui.textEdit->moveCursor(QTextCursor::NextRow);
		ui.textEdit->insertPlainText("\n");
	}
}

std::vector<Url> HtmlPageSearcher::FindUrl(std::string& request_string)
{
	std::transform(request_string.begin(), request_string.end(), request_string.begin(), tolower);
	std::vector<std::string> urls;
	size_t url_begin = 0;
	while(request_string.find("http://", url_begin) != std::string::npos)
	{
		url_begin = request_string.find("http://", url_begin);
		size_t url_end = request_string.find_first_not_of(m_valid_url_chars, url_begin);

		std::string url(request_string.begin() + url_begin, request_string.begin() + url_end);
		urls.push_back(url);
		url_begin = url_end;
	}
	return urls;
}