#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui_(new Ui::Dialog)
{
    ui_->setupUi(this);
    ui_->encodeRB->setChecked(true);
    alphabet_ = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
}

Dialog::~Dialog()
{
    delete ui_;
}

void Dialog::on_encodeRB_toggled() {
    mode_ = encode;
}

void Dialog::on_decodeRB_toggled() {
    mode_ = decode;
}

void Dialog::on_inputTE_textChanged() {
    QString input = ui_->inputTE->toPlainText();
    QString output = "";
    if (mode_ == encode) {
        int ch[3];
        int index[4];
        int i = 0;
        while (i < input.length()) {
            for (int j = 0; j < 3; ++j) {
                ch[j] = (i < input.size()) ? input.at(i++).unicode() : 0;
            }

            index[0] = ch[0] >> 2;
            index[1] = ((ch[0] & 3) << 4) | (ch[1] >> 4);
            index[2] = ((ch[1] & 15) << 2) | (ch[2] >> 6);
            index[3] = ch[2] & 63;

            if (ch[1] == 0) {
                index[2] = index[3] = 64;
            } else if (ch[2] == 0) {
                index[3] = 64;
            }

            for (int j = 0; j < 4; ++j) {
                output.append(alphabet_.at(index[j]));
            }
        }

        /* [check section] */
            QByteArray text(input.toStdString().c_str());
            qDebug() << text.toBase64();
        /* [/check section] */

    } // encode

    if (mode_ == decode) {
        int ch[4];
        int index[3];
        int i = 0;
        while (i < input.length()) {
            for (int j = 0; j < 4; ++j) {
                ch[j] = (i < input.size()) ? input.at(i++).unicode() : 0;
                ch[j] = alphabet_.indexOf(ch[j]);
            }

            index[0] = ((ch[0] & 63) << 2) | (ch[1] >> 4);
            index[1] = ((ch[1] & 15) << 4) | (ch[2] >> 2);
            index[2] = ((ch[2] & 3) << 6) | (ch[3]);

            output.append(QChar(index[0]));
            if (ch[2] != 64) {
                output.append(QChar(index[1]));
            }
            if (ch[3] != 64) {
                output.append(QChar(index[2]));
            }
        }

        /* [check section] */
            QByteArray text(input.toStdString().c_str());
            QByteArray precode(QByteArray::fromBase64(text));
            qDebug() << precode.data();
        /* [/check section] */

    } // decode

    ui_->outputTE->setPlainText(output);
}
