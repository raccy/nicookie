#ifndef NICOOKIE_H
#define NICOOKIE_H

#include <QObject>
#include <QString>
#include <QStringList>

class Nicookie : public QObject
{
    Q_OBJECT
public:
    static const QString INTERNET_EXPLORE;
    static const QString SAFARI;
    static const QString FIREFOX;
    static const QString CHROME;
    static const QString OPERA;

private:
    QString error;

public:
    explicit Nicookie(QObject *parent = 0);
    ~Nicookie();
    QString getUserSession(QString browser);
    QStringList getBrowserList();
    QString errorString();
    bool hasError();

signals:

public slots:
};

#endif // NICOOKIE_H
