#include "login.h"
#include "ui_login.h"
#include <QMessageBox>
#include <QPixmap>

Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    QPixmap pix(":/resources/img/login.png");
    int w = ui->label_3->width();
    int h = ui->label_3->height();
    ui->label_3->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
    this->setWindowTitle("Login Page");
    applyStylesheets();

}

Login::~Login()
{
    delete ui;
}
void Login::applyStylesheets()
{
    // Stylesheet for the login button
    ui->loginButton->setStyleSheet("QPushButton { background-color: #FFFFFF; color: #007ACC; border: 1px solid #007ACC; border-radius: 5px; }"
                                   "QPushButton:hover { background-color: #E6E6E6; }"
                                   "QPushButton:pressed { background-color: #CCCCCC; }");

    // Stylesheet for the group box
    ui->groupBox->setStyleSheet("QGroupBox { background-color: #FFFFFF; border: 5px solid #005CA3; border-radius: 5px; }");

    // Stylesheet for the labels
    ui->label->setStyleSheet("QLabel { color: #007ACC; padding: 2px; }");
    ui->label_2->setStyleSheet("QLabel { color: #007ACC; padding: 2px; }");

    // Stylesheet for the line edits
    ui->lineEdit->setStyleSheet("QLineEdit { background-color: #FFFFFF; border: 1px solid #005CA3; border-radius: 3px; }");
    ui->lineEdit_2->setStyleSheet("QLineEdit { background-color: #FFFFFF; border: 1px solid #005CA3; border-radius: 3px; }");
}


void Login::on_loginButton_clicked()
{
    QString username = ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();

    if (username == "user" && password == "pwd") {
        // Emit signal for successful login
        emit loginSuccessful();
        // Close the login window

        accept();
    } else {
        // Show error message for invalid login credentials
        QMessageBox::warning(this, tr("Invalid Login"), tr("Invalid username or password. Please try again."));
        // Clear password field
        ui->lineEdit_2->clear();
        // Set focus to the password field
        ui->lineEdit_2->setFocus();
    }
}
