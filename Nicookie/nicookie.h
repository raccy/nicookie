#ifndef NICOOKIE_H
#define NICOOKIE_H

#include <QObject>
#include <QString>
#include <QStringList>

class Nicookie : public QObject
{
	Q_OBJECT
public:
	static const QString COOKIE_HOST;
	static const QString COOKIE_NAME;
	static const QString COOKIE_PATH;

	static const QString INTERNET_EXPLORE;
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
#endif // Q_OS_OSX
	bool findFirefox();
	QStringList firefoxGetProfileList(const QString &profile_ini);
	bool firefoxFindValue(const QString &cookies_path);

	bool findChrome();
	bool findOpera();

signals:

public slots:
};

#endif // NICOOKIE_H
