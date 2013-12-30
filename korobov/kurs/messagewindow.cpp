#include "messagewindow.h"

MessageWindow::MessageWindow(modes mode, QWidget *parent) :
    QDialog(parent), mode_(mode)
{
    if (mode_ == write) {
        uiWM_ = new Ui::WriteMessage;
        uiRM_ = 0;
        uiWM_->setupUi(this);

    }
    if (mode_ == read) {
        uiWM_ = 0;
        uiRM_ - new Ui::ReadMessage;
        uiRM_->setupUi(this);
    }
}

MessageWindow::~MessageWindow()
{
    if (mode_ == write) {
        delete uiWM_;
    }
    if (mode_ == read) {
        delete uiRM_;
    }
}
