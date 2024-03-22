// processwindow.h
#ifndef PROCESSWINDOW_H
#define PROCESSWINDOW_H

#include <QDialog>
#include <QListWidget>

namespace Ui {
class processwindow;
}

class processwindow : public QDialog
{
    Q_OBJECT

public:
    explicit processwindow(QWidget *parent = nullptr);
    ~processwindow();

    void populateProcessList();

protected:
    void showEvent(QShowEvent *event) override;  // Add this line


private:
    Ui::processwindow *ui;

private slots:
    void on_refreshButton_clicked();
};

#endif // PROCESSWINDOW_H
