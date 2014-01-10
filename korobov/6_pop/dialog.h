#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QModelIndex>
#include "message.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    
private:
    Ui::Dialog *ui;
    QList< QPair<quint64, quint64> > list_;
private slots:
    void showMessage(QModelIndex index);
};

#endif // DIALOG_H
