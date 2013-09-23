#include "code.h"


Code::Code(const QString seq) :
    sequence_(seq)
{
}

void Code::setSeq(const QString seq) {
    LOG << "setSeq()!";
    sequence_ = seq;
}

QList<Code::level> Code::getLevels(Code::type type) const {
    QList<Code::level> tmp;
    switch(type) {
        case Code::NRZ: {
            for(int i = 0; i < sequence_.length(); ++i) {
                tmp.append((sequence_.at(i) == '1')? Code::H : Code::L);
                tmp.append(tmp.last());
            }
        break;
        }
    case Code::NRZI: {
        Code::level last = Code::L;
        for (int i = 0; i < sequence_.length(); ++i) {
            if (sequence_.at(i) == '1') {
                if (last == Code::L)
                    last = Code::H;
                else
                    last = Code::L;
            }
            tmp.append(last);
            tmp.append(tmp.last());
        }
        break;
    }
    case Code::Manchester: {
        Code::level last = Code::L;
        for (int i = 0; i < sequence_.length(); ++i) {
            if (sequence_.at(i) == '0') {
                if (last == Code::L)
                    last = Code::H;
                tmp.append(last);
                last = Code::L;
            } else {
                if (last == Code::H)
                    last = Code::L;
                tmp.append(last);
                last = Code::H;
            }
            tmp.append(last);
        }
        break;
    }

    case Code::AMI: {
        Code::level last = Code::L;
        for (int i = 0; i < sequence_.length(); ++i) {
            if (sequence_.at(i) == '0') {
                tmp.append(Code::M);
            } else {
                if (last == Code::L)
                    last = Code::H;
                else
                    last = Code::L;
                tmp.append(last);
            }
            tmp.append(tmp.last());
        }
        break;
    }

    case Code::HDB3: {
        Code::level last = Code::L;  // last inserted 1
        Code::level lastV = Code::M; // last inserted V
        int cnt0 = 0;
//        http://www.ci.ru/inform16_99/p_05_bil.htm
        for (int i = 0; i < sequence_.length(); ++i) {
            if (sequence_.at(i) == '0') {
                ++cnt0;
                if (cnt0 == 4) {
                    cnt0 = 0;
                    if (last == lastV) {
                        if (last == Code::L)
                            lastV = Code::H;
                        else
                            lastV = Code::L;
                        tmp.replace(tmp.size() - 6, lastV);
                        tmp.replace(tmp.size() - 5, lastV);
                    } else {
                        lastV = last;
                    }
                    last = lastV;
                    tmp.append(last);

                } else {
                    tmp.append(Code::M);
                }
            } else {
                cnt0 = 0;
                if (last == Code::L)
                    last = Code::H;
                else
                    last = Code::L;
                tmp.append(last);
            }
            tmp.append(tmp.last());
        }
        break;
    }
    case Code::_2B1Q: {
        for (int i = 0; i < sequence_.length() - 1; i+=2) {
            if (sequence_.at(i) == '0') {
                if (sequence_.at(i + 1) == '0')
                    tmp.append(Code::L);    // 00 -2.5
                else
                    tmp.append(Code::LM);   // 01 -0.8
            } else {
                if (sequence_.at(i + 1) == '0')
                    tmp.append(Code::H);    // 10 2.5
                else
                    tmp.append(Code::HM);   // 11 0.8
            }
            for (int j = 0; j < 3; ++j)
                tmp.append(tmp.last());
        }
        break;
        }
    }
    return tmp;
}
// private slots
 void Code::textChanged(QString seq) {
    setSeq(seq);
    emit sequenceUpdated();
 }
