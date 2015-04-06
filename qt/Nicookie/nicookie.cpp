#include "nicookie.h"

const QString Nicookie::INTERNET_EXPLORE = "Internet Explorer";
const QString Nicookie::SAFARI = "Safari";
const QString Nicookie::FIREFOX = "Mozilla Firefox";
const QString Nicookie::CHROME = "Google Chrome";
const QString Nicookie::OPERA = "Opera";

Nicookie::Nicookie(QObject *parent) : QObject(parent)
{

}

Nicookie::~Nicookie()
{

}

QString Nicookie::getUserSession(QString browser)
{
    return "dummy";
}

QStringList Nicookie::getBrowserList()
{
    QStringList list;
#ifdef _WIN32
    list << Nicookie::INTERNET_EXPLORER;
    list << Nicookie::FIREFOX;
    list << Nicookie::CHROME;
#endif // _WIN32
#ifdef __APPLE__
    list << Nicookie::SAFARI;
    list << Nicookie::FIREFOX;
    list << Nicookie::CHROME;
#endif // __APPLE__
#ifdef __linux__
    list << Nicookie::FIREFOX;
    list << Nicookie::CHROME;
#endif // __linux__
    return list;
}

