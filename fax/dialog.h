#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDebug>
#include <coder.h>

#define LOG qDebug() << "Dialog::"
namespace Ui {
    class Dialog;
}

class Dialog : public QDialog {
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_codeButton_clicked();
    void on_encodeButton_clicked();

private:
    Ui::Dialog *ui_;
    Coder *coder_;
};

#endif // DIALOG_H
