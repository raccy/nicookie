#ifndef NICOOKIEMAINWINDOW_H
#define NICOOKIEMAINWINDOW_H

#include <QMainWindow>
#include "nicookie.h"
namespace Ui {
class NicookieMainWindow;
}

class NicookieMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit NicookieMainWindow(QWidget *parent = 0);
	~NicookieMainWindow();

private slots:
	void on_pushButton_clicked();

private:
	Ui::NicookieMainWindow *ui;
	Nicookie *nicookie;
};

#endif // NICOOKIEMAINWINDOW_H
