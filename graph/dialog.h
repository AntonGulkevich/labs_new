#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QGraphicsScene>
#include <QDebug>
#include "code.h"
#define LOG qDebug() << "Dialog::"

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
    Ui::Dialog *ui_;
    QGraphicsScene *scene_;
    Code *code_;
    const int periodOffset_ = 30;
    const int xmin_ = 0;
    const int xmax_ = 580;
    const int ymin_ = -100;
    const int ymax_ = 100;
    struct {
        QPen red;
        QPen black;
        QPen gray;
        QPen grid_b;
        QPen grid_g;
    } pen_;

    int getY(Code::level) const;
    void grid() const;
private slots:
    void updateScene();
    void drawGraph() const;
    void drawDigits(QString) const;
    void on_pushButton_clicked();
    void on_groupBox_clicked();
};

#endif // DIALOG_H
