#include "coder.h"

Coder::Coder() {
    koef_ = new koef;
    koef_->kds = 0;
    koef_->apk = 0;
    koef_->haffman = 0;
    H_init();
}

Coder::~Coder() {
    delete koef_;
}

void Coder::setBlackLength(int black) {
    black_ = black;
}

void Coder::setWhiteLength(int white) {
    white_ = white;
}

QString Coder::code(QString value) {
    QStringList lines = value.split("\n");
    int n, ncode;

    /***********************************************************************************/
    /* KDS */
    n = 0;
    ncode = 0;
    QString tmp0 = QString::null;
    for (QStringList::Iterator series = lines.begin(); series != lines.end(); ++series) {
        QStringList s = series->split(" ");

        bool isWhite = true;

        for (QStringList::Iterator it = s.begin(); it != s.end(); ++it) {
            int s = it->toInt();
            int divisor = (isWhite) ? white_ : black_;
            int length = qPow(2, divisor) - 1;
            n += s;
            if (s == 0) {
                for (int i = 0; i < white_; ++i) {
                    tmp0.append("0");
                    ++ncode;
                }
            } else if (s < length) {
                tmp0.append(QString("%1").arg(QString::number(s, 2), divisor, QChar('0')));
                tmp0.append(" ");

                ncode += divisor;
            } else if (s % length == 0) {
                for (int i = 0; i < s / length; ++i)
                    tmp0.append(QString::number(length, 2));
                tmp0.append(QString("%1").arg(QString::number(0, 2), divisor, QChar('0')));
                tmp0.append(" ");

                ncode += (divisor * (s / length + 1));
            } else {
                for (int i = 0; i < s / length; ++i)
                    tmp0.append(QString::number(length, 2));
                tmp0.append(QString("%1").arg(QString::number(s % length, 2), divisor, QChar('0')));
                tmp0.append(" ");

                ncode += (divisor * (s / length + 1));
            }
            isWhite = !isWhite;
        }
    }

    koef_->kds = n * 1.0 / ncode;

    /***********************************************************************************/
    /* APK */
    n = 0;
    ncode = 0;
    QString tmp1 = QString::null;
    for (QStringList::Iterator series = lines.begin(); series != lines.end(); ++series) {
        QStringList s = series->split(" ");

        int L = 0;
        for (QStringList::Iterator it = s.begin(); it != s.end(); ++it)
            L += it->toInt();
        n+=L;

        int divisor = ceil(log2(L));

        int current = 0;
        for (QStringList::Iterator it = s.begin(); it != s.end(); ++it) {

            current += it->toInt();

            if (current <= L) {
                tmp1.append(QString("%1").arg(QString::number(current, 2), divisor, QChar('0')));
                tmp1.append(" ");

                ncode += divisor;
            }
        }
        tmp1.append("\n");
    }

    koef_->apk = n * 1.0 / ncode;
    /***********************************************************************************/
    /* Haffman */
    n = 0;
    ncode = 0;
    QString tmp2 = QString::null;

    for (QStringList::Iterator series = lines.begin(); series != lines.end(); ++series) {
        QStringList s = series->split(" ");
        bool isWhite = true;
        for (QStringList::Iterator it = s.begin(); it != s.end(); ++it) {
            int s = it->toInt();
            n += s;

            if (H.contains(s)) {
                tmp2.append(H[s][isWhite]);
                tmp2.append(" ");
                ncode += H[s][isWhite].size();
            } else {
                while (s != 0) {
                    QMap<int, QMap<bool, QString> >::Iterator m;
                    bool equal = false;
                    for (m = H.begin(); m != H.end(); ++m) {

                        if (m.key() > s) {
                            break;
                        }
                        if (m.key() == s) {
                            equal = true;
                            break;
                        }
                    }

                    if (!equal)
                        --m;

                    tmp2.append(H[m.key()][isWhite]);
                    ncode += H[m.key()][isWhite].size();
                    s -= m.key();
                }
                tmp2.append(" ");
            }

            isWhite = !isWhite;
        }

    }

    koef_->haffman = n * 1.0 / ncode;
    /***********************************************************************************/
    switch(show_) {
    case 0:
        return tmp0;
    case 1:
        return tmp1;
    case 2:
        return tmp2;
    default:
        return tmp0;
    }
}

QString Coder::getKDS() const {
    return QString::number(koef_->kds);
}

QString Coder::getAPK() const {
    return QString::number(koef_->apk);
}

QString Coder::getHaffman() const {
    return QString::number(koef_->haffman);
}

void Coder::setShow(int show) {
    show_ = show;
}

void Coder::H_init() {
    H[0][true] = "00110101";
    H[0][false] = "0000110111";
    H[1][true] = "000111";
    H[1][false] = "010";
    H[2][true] = "0111";
    H[2][false] = "11";
    H[3][true] = "1000";
    H[3][false] = "10";
    H[4][true] = "1011";
    H[4][false] = "011";
    H[5][true] = "1100";
    H[5][false] = "0011";
    H[6][true] = "1110";
    H[6][false] = "0010";
    H[7][true] = "1111";
    H[7][false] = "00011";
    H[8][true] = "10011";
    H[8][false] = "000101";
    H[9][true] = "10100";
    H[9][false] = "000100";
    H[10][true] = "00111";
    H[10][false] = "0000100";
    H[11][true] = "01000";
    H[11][false] = "0000101";
    H[12][true] = "001000";
    H[12][false] = "0000111";
    H[13][true] = "000011";
    H[13][false] = "00000100";
    H[14][true] = "110100";
    H[14][false] = "00000111";
    H[15][true] = "110101";
    H[15][false] = "000011000";
    H[16][true] = "101010";
    H[16][false] = "0000010111";
    H[17][true] = "101011";
    H[17][false] = "0000011000";
    H[18][true] = "0100111";
    H[18][false] = "0000001000";
    H[19][true] = "0001100";
    H[19][false] = "00001100111";
    H[20][true] = "0001000";
    H[20][false] = "00001101000";
    H[21][true] = "0010111";
    H[21][false] = "00001101100";
    H[22][true] = "0000011";
    H[22][false] = "00000110111";
    H[23][true] = "0000100";
    H[23][false] = "00000101000";
    H[24][true] = "0101000";
    H[24][false] = "00000010111";
    H[25][true] = "0101011";
    H[25][false] = "00000011000";
    H[26][true] = "0010011";
    H[26][false] = "000011001010";
    H[27][true] = "0100100";
    H[27][false] = "000011001011";
    H[28][true] = "0011000";
    H[28][false] = "000011001100";
    H[29][true] = "00000010";
    H[29][false] = "000011001101";
    H[30][true] = "00000011";
    H[30][false] = "000001101000";
    H[31][true] = "00011010";
    H[31][false] = "000001101001";
    H[32][true] = "00011011";
    H[32][false] = "000001101010";
    H[33][true] = "00010010";
    H[33][false] = "000001101011";
    H[34][true] = "00010011";
    H[34][false] = "000011010010";
    H[35][true] = "00010100";
    H[35][false] = "000011010011";
    H[36][true] = "00010101";
    H[36][false] = "000011010100";
    H[37][true] = "00010110";
    H[37][false] = "000011010101";
    H[38][true] = "00010111";
    H[38][false] = "000011010110";
    H[39][true] = "00101000";
    H[39][false] = "000011010111";
    H[40][true] = "00101001";
    H[40][false] = "000001101100";
    H[41][true] = "00101010";
    H[41][false] = "000001101101";
    H[42][true] = "00101011";
    H[42][false] = "000011011010";
    H[43][true] = "00101100";
    H[43][false] = "000011011011";
    H[44][true] = "00101101";
    H[44][false] = "000001010100";
    H[45][true] = "00000100";
    H[45][false] = "000001010101";
    H[46][true] = "00000101";
    H[46][false] = "000001010110";
    H[47][true] = "00001010";
    H[47][false] = "000001010111";
    H[48][true] = "00001011";
    H[48][false] = "000001100100";
    H[49][true] = "01010010";
    H[49][false] = "000001100101";
    H[50][true] = "01010011";
    H[50][false] = "000001010010";
    H[51][true] = "01010100";
    H[51][false] = "000001010011";
    H[52][true] = "01010101";
    H[52][false] = "000000100100";
    H[53][true] = "00100100";
    H[53][false] = "000000110111";
    H[54][true] = "00100101";
    H[54][false] = "000000111000";
    H[55][true] = "01011000";
    H[55][false] = "000000100111";
    H[56][true] = "01011001";
    H[56][false] = "000000101000";
    H[57][true] = "01011010";
    H[57][false] = "000001011000";
    H[58][true] = "01011011";
    H[58][false] = "000001011001";
    H[59][true] = "01001010";
    H[59][false] = "000000101011";
    H[60][true] = "01001011";
    H[60][false] = "000000101100";
    H[61][true] = "00110010";
    H[61][false] = "000001011010";
    H[62][true] = "00110011";
    H[62][false] = "000001100110";
    H[63][true] = "00110100";
    H[63][false] = "000001100111";

    H[64][true] = "11011";
    H[64][false] = "0000001111";
    H[128][true] = "10010";
    H[128][false] = "000011001000";
    H[192][true] = "010111";
    H[192][false] = "000011001001";
    H[256][true] = "0110111";
    H[256][false] = "000001011011";
    H[320][true] = "00110110";
    H[320][false] = "000000110011";
    H[384][true] = "00110111";
    H[384][false] = "000000110100";
    H[448][true] = "01100100";
    H[448][false] = "000000110101";
    H[512][true] = "01100101";
    H[512][false] = "0000001101100";
    H[576][true] = "01101000";
    H[576][false] = "0000001101101";
    H[640][true] = "01100111";
    H[640][false] = "0000001001010";
    H[704][true] = "011001100";
    H[704][false] = "0000001001011";
    H[768][true] = "011001101";
    H[768][false] = "0000001001100";
    H[832][true] = "011010010";
    H[832][false] = "0000001001101";
    H[896][true] = "011010011";
    H[896][false] = "0000001110010";
    H[960][true] = "011010100";
    H[960][false] = "0000001110011";
    H[1024][true] = "011010101";
    H[1024][false] = "0000001110100";
    H[1088][true] = "011010110";
    H[1088][false] = "0000001110101";
    H[1152][true] = "011010111";
    H[1152][false] = "0000001110110";
    H[1216][true] = "011011000";
    H[1216][false] = "0000001110111";
    H[1280][true] = "011011001";
    H[1280][false] = "0000001010010";
    H[1344][true] = "011011010";
    H[1344][false] = "0000001010011";
    H[1408][true] = "011011011";
    H[1408][false] = "0000001010100";
    H[1472][true] = "010011000";
    H[1472][false] = "0000001010101";
    H[1536][true] = "010011001";
    H[1536][false] = "0000001011010";
    H[1600][true] = "010011010";
    H[1600][false] = "0000001011011";
    H[1664][true] = "011000";
    H[1664][false] = "0000001100100";
    H[1728][true] = "010011011";
    H[1728][false] = "0000001100101";

}
