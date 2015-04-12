/*
 *
 * - safari http://www.securitylearn.net/2012/10/27/cookies-binarycookies-reader/
 * - chrome http://n8henrie.com/2014/05/decrypt-chrome-cookies-with-python/
 * - chrome(win) https://gist.github.com/DakuTree/98c8362fb424351b803e
 * - win http://raidersec.blogspot.jp/2013/06/how-browsers-store-your-passwords-and.html
 *
 *
 *
 */



#ifndef NICOOKIE_H
#define NICOOKIE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QVariant>
#include <QByteArray>
#include <QIODevice>

class Nicookie : public QObject
{
    Q_OBJECT
public:
    static const QString COOKIE_URL;
    static const QString COOKIE_HOST;
    static const QString COOKIE_NAME;
    static const QString COOKIE_PATH;

    static const QString INTERNET_EXPLORER;
    static const QString SAFARI;
    static const QString FIREFOX;
    static const QString CHROME;
    static const QString OPERA;

private:
    QString error;
    QString userSession;

public:
    explicit Nicookie(QObject *parent = 0);
    ~Nicookie();
    QString getUserSession(QString browser);
    QStringList getBrowserList();
    QString errorString();
    bool hasError();

private:
#ifdef Q_OS_WIN
    bool findInternetExplorer();
#endif // Q_OS_WIN
#ifdef Q_OS_OSX
    bool findSafari();
    bool safariFindFile(QIODevice &device);
    bool safariFindPage(QIODevice &device);
    bool safariFindCookie(QIODevice &device);
#endif // Q_OS_OSX
    bool findFirefox();
    QStringList firefoxGetProfileList(const QString &profile_ini);
    bool firefoxFindValue(const QString &cookies_path);

    bool findChrome();
    bool chromeFindValue(const QString &cookies_path);
    QString chromeDecrypt(const QByteArray &encrypt_data);

    bool findOpera();

    bool querySqlite3(const QString &sqlite3_file,
                                 const QString &query,
                                 const QMap<QString, QVariant> &placeholders,
                                 QMap<QString, QVariant> &values);
    quint32 readUint32BE(QIODevice &device);
    quint32 readUint32LE(QIODevice &device);
    bool checkSameStr(QIODevice &device, const QString &str);
    QString readStr(QIODevice &device);
signals:

public slots:
};

#endif // NICOOKIE_H
