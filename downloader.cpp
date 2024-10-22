#include "downloader.h"

Downloader::Downloader()
{
    QNetworkProxyFactory::setUseSystemConfiguration(true);
    connect(&manager, &QNetworkAccessManager::finished, this, &Downloader::downloadFinished);
}

Downloader::Downloader(const QString &_url,QObject *parent)
    : QObject(parent), downloadUrl(_url)
{
    QNetworkProxyFactory::setUseSystemConfiguration(true);
    connect(&manager, &QNetworkAccessManager::finished, this, &Downloader::downloadFinished);
    QUrl url = QUrl::fromEncoded(_url.toLocal8Bit());
    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);

#if QT_CONFIG(ssl)
    connect(reply, &QNetworkReply::sslErrors, this, &Downloader::sslErrors);
#endif

}
void Downloader::start(const QString &_url)
{
    QUrl url = QUrl::fromEncoded(_url.toLocal8Bit());
    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);

#if QT_CONFIG(ssl)
    connect(reply, &QNetworkReply::sslErrors, this, &Downloader::sslErrors);
#endif
}
void Downloader::setUrl(const QString &url)
{
    downloadUrl = url;
}

void Downloader::doDownload(const QUrl &url)
{
    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);

#if QT_CONFIG(ssl)
    connect(reply, &QNetworkReply::sslErrors, this, &Downloader::sslErrors);
#endif

    currentDownloads.append(reply);
}

void Downloader::execute()
{
    if (downloadUrl.isEmpty()) {
        qDebug() << "No URL set for downloading";
        return;
    }

    QUrl url = QUrl::fromEncoded(downloadUrl.toLocal8Bit());
    doDownload(url);
}

void Downloader::sslErrors(const QList<QSslError> &sslErrors)
{
#if QT_CONFIG(ssl)
    for (const QSslError &error : sslErrors)
        fprintf(stderr, "SSL error: %s\n", qPrintable(error.errorString()));
#else
    Q_UNUSED(sslErrors);
#endif
}

void Downloader::downloadFinished(QNetworkReply *reply)
{
    QUrl url = reply->url();
    if (reply->error()) {
        fprintf(stderr, "Download of %s failed: %s\n",
                url.toEncoded().constData(),
                qPrintable(reply->errorString()));
    } else {
        if (isHttpRedirect(reply)) {
            fputs("Request was redirected.\n", stderr);
        } else {
            QString filename = "/home/hoomanmoradpour/Documents/TestWeather1.json";
            loadedJson = QJsonDocument::fromJson(reply->readAll());
            qDebug() << loadedJson;
            QFile jsonFile(filename);
            if (jsonFile.open(QFile::WriteOnly)) {
                jsonFile.write(loadedJson.toJson());
                jsonFile.close();
            } else {
                qWarning() << "Could not open " << filename << " for writing.";
            }
        }
    }

    currentDownloads.removeAll(reply);
    reply->deleteLater();
    emit download_finished_sgnl();
}

bool Downloader::isHttpRedirect(QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    return statusCode == 301 || statusCode == 302 || statusCode == 303
           || statusCode == 305 || statusCode == 307 || statusCode == 308;
}


