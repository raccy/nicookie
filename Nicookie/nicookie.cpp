#include <QProcessEnvironment>
#include <QFile>
#include <QTemporaryFile>
#include <QSettings>
#include <QFileInfo>
#include <QDir>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

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
    profile_ini += QProcessEnvironment::systemEnvironment().value("APPDATA");
    profile_ini += "\\Mozilla\\Firefox\\profiles.ini";
#elif defined(Q_OS_OSX)
    profile_ini += QProcessEnvironment::systemEnvironment().value("HOME");
    profile_ini += "/Library/Application Support/Firefox/profiles.ini";
#else
    profile_ini += QProcessEnvironment::systemEnvironment().value("HOME");
    profile_ini += "/.mozilla/firefox/profiles.ini";
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
    if (profile_file.copy(profile_temp_path)) {
        this->error = "プロファイル設定ファイルの読み込みに失敗しました。";
        return list;
    }
    QSettings profile_settings(profile_temp_path, QSettings::IniFormat);
    if (profile_settings.status() != QSettings::NoError) {
        this->error = "プロファイル設定ファイルが正しくありません。";
        return list;
    }
    for (auto &group: profile_settings.childGroups()) {
        if (group.startsWith("Profile")) {
            profile_settings.beginGroup(group);
            QString path;
            if (profile_settings.value("IsRelative", 1).toInt() == 1) {
                path = QFileInfo(profile_file).dir().filePath(
                            profile_settings.value("Path").toString());
            } else {
                path = profile_settings.value("Path").toString();
            }
            if (profile_settings.value("Default", 0).toInt() == 1) {
                list.prepend(path);
            } else {
                list.append(path);
            }
            profile_settings.endGroup();
        }
    }

    if (list.isEmpty()) {
        this->error = "パスが一つも見つかりませんでした。";
    }
    return list;
}

bool Nicookie::firefoxFindValue(const QString &profile_poth)
{
    QString query = "SELECT value FROM moz_cookies WHERE "
                    "host = :host AND "
                    "name = :name AND "
                    "path = :path";
    QMap<QString, QVariant> placeholders;
    placeholders[":host"] = Nicookie::COOKIE_HOST;
    placeholders[":name"] = Nicookie::COOKIE_NAME;
    placeholders[":path"] = Nicookie::COOKIE_PATH;
    QMap<QString, QVariant> values;
    values["value"] = QVariant();

    QString cookies_path = QDir(profile_poth).filePath("cookies.sql");
    if (querySqlite3(cookies_path, query, placeholders, values)) {
        this->userSession = values["value"].toString();
        return true;
    } else {
        return false;
    }
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

bool Nicookie::querySqlite3(const QString &sqlite3_file,
                                   const QString &query,
                                   const QMap<QString, QVariant> &placeholders,
                                   QMap<QString, QVariant> &values)
{
    qDebug() << sqlite3_file;
    bool result = false;
    do {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",
                                                    "nicookie_sqlite3");
        if (!db.isValid()) {
            this->error = "SQLite3が使用できません。"
                          "ライブラリが不足していませんか？";
            break;
        }
        db.setDatabaseName(sqlite3_file);
        if (!db.open()) {
            this->error = "SQLiete3のファイルを開けませんでした。";
            db.close();
            break;
        }

        qDebug() << db.tables().size();
        for (auto &table: db.tables()) {
            qDebug() << table;
        }


        QSqlQuery sql_query(db);

        qDebug() << query;
        if (!sql_query.prepare(query)) {
            qDebug() << db.lastError().text();
            qDebug() << sql_query.lastError().text();
            this->error = "SQL文が不正です。";
            db.close();
            break;
        }
        for (auto &name: placeholders.keys()) {
            sql_query.bindValue(name, placeholders[name]);
        }
        if (!sql_query.exec()) {
            this->error = "SQLの実行に失敗しました。";
            sql_query.finish();
            db.close();
            break;
        }
        if (!sql_query.first()) {
            this->error = "検索したレコードが見つかりませんでした。";
            sql_query.finish();
            db.close();
            break;
        }
        for (auto &name: values.keys()) {
            values[name] = sql_query.value(name);
        }
        sql_query.finish();
        db.close();
        result = true;
    } while(false);
    QSqlDatabase::removeDatabase("nicookie_sqlite3");
    return result;
}

