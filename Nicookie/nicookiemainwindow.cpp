#include <QMessageBox>

#include "nicookiemainwindow.h"
#include "ui_nicookiemainwindow.h"

NicookieMainWindow::NicookieMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NicookieMainWindow)
{
    ui->setupUi(this);
    nicookie = new Nicookie(this);
    for (auto browser: nicookie->getBrowserList()) {
        ui->comboBox->addItem(browser);
    }
}

NicookieMainWindow::~NicookieMainWindow()
{
    delete ui;
}

void NicookieMainWindow::on_pushButton_clicked()
{
    ui->lineEdit->setText(nicookie->getUserSession(
                              ui->comboBox->currentText()));
    if (nicookie->hasError()) {
        QString error = nicookie->errorString();
        QMessageBox::information(this, "Nicookie", error);
    }
}
