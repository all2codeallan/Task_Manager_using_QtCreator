// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <Windows.h>
#include "history.h"
#include "processwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void logUpdated(const QString &logEntry);

private slots:
    void on_addTaskButton_clicked();
    void on_listTasksButton_clicked();
    void on_deleteTaskButton_clicked();
    void on_executeTasksButton_clicked();
    void on_exitButton_clicked();

    void on_showProcessesButton_clicked();

private:
    Ui::MainWindow *ui;

    processwindow *processWindow;

    HistoryWindow historyWindow;
    QStringList historyLog;

    CRITICAL_SECTION criticalSection;
    QPushButton *exitButton;

    struct Task {
        char description[100];
        int priority;
        int completed;
    };
    static const int MAX_TASKS = 100;
    Task tasks[MAX_TASKS];
    int taskCount;

    void addTask(const QString &description, int priority);
    void listTasks();
    void deleteTask(int taskIndex);
    void executeTasks();
    DWORD WINAPI taskRunner(LPVOID arg);
    void addToHistory(const QString &action, const QString &taskName = "-", int priority = 0, bool completed = false);
};

#endif // MAINWINDOW_H
