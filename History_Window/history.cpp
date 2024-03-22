#include "history.h"
#include <QDateTime>

HistoryWindow::HistoryWindow(QWidget *parent) :
    QDialog(parent)
{
    layout = new QVBoxLayout(this);
    historyTable = new QTableWidget(this);
    historyTable->setColumnCount(5); // Adding a column for date and time
    historyTable->setHorizontalHeaderLabels({"Date", "Time", "Action", "Task", "Priority", "Completed"});
    layout->addWidget(historyTable);
}

HistoryWindow::~HistoryWindow()
{
    delete layout;
    delete historyTable;
}

void HistoryWindow::updateHistory(const QString &logEntry)
{
    QStringList fields = logEntry.split(", ");
    int row = historyTable->rowCount();
    historyTable->insertRow(row);

    // Add date and time
    QDateTime currentDateTime = QDateTime::currentDateTime();
    historyTable->setItem(row, 0, new QTableWidgetItem(currentDateTime.date().toString()));
    historyTable->setItem(row, 1, new QTableWidgetItem(currentDateTime.time().toString()));

    for (int i = 0; i < fields.size(); ++i) {
        QTableWidgetItem *item = new QTableWidgetItem(fields[i]);
        historyTable->setItem(row, i + 2, item); // Start from column index 2 to skip date and time columns
    }
}

