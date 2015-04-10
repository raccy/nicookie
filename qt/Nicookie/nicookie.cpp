#include <QProcessEnvironment>
#include <QFile>
#include <QTemporaryFile>
#include <QSettings>
#include <QFileInfo>
#include <QDir>

#include <QDebug>

#include "nicookie.h"

const QString Nicookie::COOKIE_HOST = ".nicovideo.jp";
const QString Nicookie::COOKIE_NAME = "user_session";
const QString Nicookie::COOKIE_PATH = "/";

const QString Nicookie::INTERNET_EXPLORE = "Internet Explorer";
const QString Nicookie::SAFARI = "Safari";
const QString Nicookie::FIREFOX = "Mozilla Firefox";
const QString Nicookie::CHROME = "Google Chrome";
const QString Nicookie::OPERA = "Opera";

Nicookie::Nicookie(QObject *parent) : QObject(parent)
{
    this->error = QString();
    this->userSession = QString();
}

Nicookie::~Nicookie()
{

}

QString Nicookie::getUserSession(QString browser)
{
    this->error = QString();
    this->userSession = QString();
    if (browser == Nicookie::INTERNET_EXPLORE) {
#ifdef Q_OS_WIN
        findInternetExplorer();
#else
        this->error = "未対応のブラウザです。";
#endif // Q_OS_WIN
    } else if (browser == Nicookie::SAFARI) {
#ifdef Q_OS_OSX
        findSafari();
#else
        this->error = "未対応のブラウザです。";
#endif // Q_OS_OSX
    } else if (browser == Nicookie::FIREFOX) {
        findFirefox();
    } else if (browser == Nicookie::CHROME) {
        findChrome();
    } else if (browser == Nicookie::OPERA) {
        findOpera();
    }
    return this->userSession;
}

QStringList Nicookie::getBrowserList()
{
    QStringList list;
#ifdef Q_OS_WIN
    list << Nicookie::INTERNET_EXPLORER;
#endif // Q_OS_WIN
#ifdef Q_OS_OSX
    list << Nicookie::SAFARI;
#endif // Q_OS_OSX
    list << Nicookie::FIREFOX;
    list << Nicookie::CHROME;
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

#ifdef Q_OS_WIN
// # Windows #

// ## Internet Explorer ##
bool Nicookie::findInternetExplorer()
{
    this->error = "まだ、実装してないよ。";
    return false;
}
#endif // Q_OS_WIN

#ifdef Q_OS_OSX
// # Mac OSX #

// ## Safari ##
bool Nicookie::findSafari()
{
    this->error = "まだ、実装してないよ。";
    return false;
}
#endif // Q_OS_OSX

// # Windows and Mac OSX and Linux #

// ## Firefox ##
bool Nicookie::findFirefox()
{
    QString profile_ini;
#if defined(Q_OS_WIN)
    // TODO: ちゃんと書く
    profile_ini += QProcessEnvironment::systemEnvironment().value("HOME");
    profile_ini += "/Library/Application Support/Firefox/profiles.ini";
#elif defined(Q_OS_OSX)
    profile_ini += QProcessEnvironment::systemEnvironment().value("HOME");
    profile_ini += "/Library/Application Support/Firefox/profiles.ini";
#else
    // TODO: ちゃんと書く
    profile_ini += QProcessEnvironment::systemEnvironment().value("HOME");
    profile_ini += "/Library/Application Support/Firefox/profiles.ini";
#endif
    QStringList profile_list = firefoxGetProfileList(profile_ini);
    if (profile_list.isEmpty()) return false;
    for (auto &profile_path: profile_list) {
        if (firefoxFindValue(profile_path)) return true;
    }
    return false;
}

QStringList Nicookie::firefoxGetProfileList(const QString &profile_ini)
{
    QStringList list;
    QFile profile_file(profile_ini);
    if (!profile_file.exists()) {
        this->error = "プロファイル設定ファイルが見つかりません。";
        return list;
    }

    QTemporaryFile profile_temp;
    QString profile_temp_path = profile_temp.fileTemplate();
    profile_file.copy(profile_temp_path);
    QSettings profile(profile_temp_path, QSettings::IniFormat);
    for (QString &group: profile.childGroups()) {
        group.startsWith("Profile");
        profile.beginGroup(group);
        QString path;
        if (profile.value("IsRelative", 1).toInt() == 1) {
            path = QFileInfo(profile_file).dir().filePath(profile.value("Path").toString());
        } else {
            path = profile.value("Path").toString();
        }
        if (profile.value("Default", 0).toInt() == 1) {
            list.prepend(path);
        } else {
            list.append(path);
        }
        profile.endGroup();
    }

    if (list.isEmpty()) {
        this->error = "パスが一つも見つかりませんでした。";
    }
    return list;
}

bool Nicookie::firefoxFindValue(const QString &profile_poth)
{
    // TODO: 続き
    return false;
}





bool Nicookie::findChrome()
{
    this->error = "まだ、実装してないよ。";
    return false;
}

bool Nicookie::findOpera()
{
    this->error = "まだ、実装してないよ。";
    return false;
}

