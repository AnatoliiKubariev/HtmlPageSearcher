#include "HtmlPageSearcher.h"

HtmlPageSearcher::HtmlPageSearcher(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

void HtmlPageSearcher::ReadHtml(const QUrl& url)
{
	QNetworkRequest request(url);

	m_reply = m_request_manager.get(request);

	connect(m_reply, &QNetworkReply::readyRead, this, &HtmlPageSearcher::ReadData);
	connect(m_reply, &QNetworkReply::finished, this, &HtmlPageSearcher::Finished);
	m_loop.exec();
}

void HtmlPageSearcher::FindUrl()
{
	std::transform(m_request_string.begin(), m_request_string.end(), m_request_string.begin(), tolower);

	size_t url_begin = 0;
	while(m_request_string.find("http://", url_begin) != std::string::npos)
	{
		url_begin = m_request_string.find("http://", url_begin);
		size_t url_end = m_request_string.find_first_not_of(m_valid_url_chars, url_begin);

		std::string url(m_request_string.begin() + url_begin, m_request_string.begin() + url_end);
		m_urls.insert(url);
		url_begin = url_end;
	}
}

void HtmlPageSearcher::ReadData()
{
	m_request_string = m_reply->readAll();
}

void HtmlPageSearcher::Finished()
{
	ui.textEdit->setPlainText(QString::fromStdString(m_request_string));

	m_reply->deleteLater();
	m_loop.exit();
}