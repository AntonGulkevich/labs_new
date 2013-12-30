#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_encodeRB_toggled();
    void on_decodeRB_toggled();

    void on_inputTE_textChanged();

private:
    Ui::Dialog *ui_;
    enum t_mode {encode, decode};
    t_mode mode_;
    QString alphabet_;
};

#endif // DIALOG_H
