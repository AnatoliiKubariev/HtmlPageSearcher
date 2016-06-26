#include "HttpLoader.h"

Page HttpLoader::LoadHttpPage(const Url& url)
{
	m_url = url;

	QNetworkRequest request(QUrl(QString::fromStdString(url)));
	QNetworkAccessManager request_manager;
	QNetworkReply* reply = request_manager.get(request);

	QEventLoop loop;
	connect(reply, &QNetworkReply::readyRead, [&]() 
	{ 
		m_page = reply->readAll().toStdString(); 
	});
	connect(reply, &QNetworkReply::finished, [&]()
	{
		reply->deleteLater();
		loop.exit();
	});
	loop.exec();

	return m_page;
}