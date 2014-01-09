#ifndef MESSAGEWINDOW_H
#define MESSAGEWINDOW_H

#include <QDialog>
#include "message.h"
#include "writemessage.h"
#include "ui_writemessage.h"
#include "readmessage.h"
#include "ui_readmessage.h"

namespace Ui {
    class WriteMessage;
    class ReadMessage;
}

class MessageWindow : public QDialog, public Message {

    Q_OBJECT

public:
    enum modes {
        read, write
    };
    explicit MessageWindow(modes mode, QWidget *parent = 0);
    ~MessageWindow();
private:
    Ui::WriteMessage *uiWM_;
    Ui::ReadMessage *uiRM_;
    modes mode_;
};

#endif // MESSAGEWINDOW_H
