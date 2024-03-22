#ifndef HISTORYWINDOW_H
#define HISTORYWINDOW_H

#include <QDialog>
#include <QTableWidget>
#include <QVBoxLayout>

class HistoryWindow : public QDialog
{
    Q_OBJECT

public:
    explicit HistoryWindow(QWidget *parent = nullptr);
    ~HistoryWindow();

public slots:
    void updateHistory(const QString &logEntry);

private:
    QVBoxLayout *layout;
    QTableWidget *historyTable;
};

#endif // HISTORYWINDOW_H
