#pragma once

#include "Common.h"

#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class HttpLoader: public QObject
{
	Q_OBJECT

public:
	Page LoadHttpPage(const Url& url);

private:
	Url m_url;
	Page m_page;
};
