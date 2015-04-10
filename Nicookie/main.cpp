#include "nicookiemainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NicookieMainWindow w;

    w.show();

    return a.exec();
}
