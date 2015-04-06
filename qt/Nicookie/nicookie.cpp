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
    this->error = QString();
#ifdef _WIN32
    if (browser == Nicookie::INTERNET_EXPLORE) {
        this->error = "まだ、実装してないよ。";
    } else if (browser == Nicookie::FIREFOX) {
        this->error = "まだ、実装してないよ。";
    } else if (browser == Nicookie::CHROME) {
        this->error = "まだ、実装してないよ。";
    } else if (browser == Nicookie::OPERA) {
        this->error = "まだ、実装してないよ。";
    }
#endif // _WIN32
#ifdef __APPLE__
    if (browser == Nicookie::SAFARI) {
        this->error = "まだ、実装してないよ。";
//        return osxSafari();
    } else if (browser == Nicookie::FIREFOX) {
        this->error = "まだ、実装してないよ。";
//        return osxFirefox();
    } else if (browser == Nicookie::CHROME) {
        this->error = "まだ、実装してないよ。";
//        return osxChrome();
    } else if (browser == Nicookie::OPERA) {
        this->error = "まだ、実装してないよ。";
    }
#endif // __APPLE__
#ifdef __linux__
    if (browser == Nicookie::FIREFOX) {
        this->error = "まだ、実装してないよ。";
    } else if (browser == Nicookie::CHROME) {
        this->error = "まだ、実装してないよ。";
    } else if (browser == Nicookie::OPERA) {
        this->error = "まだ、実装してないよ。";
    }
#endif // __linux__
    return "";
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

QString Nicookie::errorString()
{
    return this->error;
}

bool Nicookie::hasError()
{
    return !this->error.isEmpty();
}

