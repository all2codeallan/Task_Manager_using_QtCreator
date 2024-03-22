#include "mainwindow.h"
#include "login.h"
#include "processwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Create instances of the main window and the login window
    MainWindow w;
    Login loginWindow;
    processwindow processWindow;
    loginWindow.setStyleSheet("background: url(:/resources/img/lbg2.jpg) center center no-repeat;");



    // Create and show the splash screen
    QSplashScreen *splash = new QSplashScreen;
    QPixmap splashPixmap(":/resources/img/logo.png");
    splash->setPixmap(splashPixmap);
    splash->show();

    // Resize the splash screen to match the size of the image
    splash->setFixedSize(splashPixmap.size());

    // Close the splash screen after 3 seconds
    QTimer::singleShot(3000, splash, &QSplashScreen::close);

    // Show the login window after a delay
    QTimer::singleShot(3100, nullptr, [&]() {
        splash->deleteLater(); // Delete the splash screen to avoid memory leak
        loginWindow.show(); // Show the login window
    });

    // Connect the login window's loginSuccessful signal to a lambda function
    // that closes the login window and shows the main window
    QObject::connect(&loginWindow, &Login::loginSuccessful, [&]() {
        loginWindow.close(); // Close the login window
        w.show(); // Show the main window
    });

    // Execute the application event loop
    return a.exec();
}
