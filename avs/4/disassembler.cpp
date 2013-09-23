#include "disassembler.h"

Disassembler::Disassembler(const QString &filename) {
    LOG << "start";
    //    register init
    reg_[ 0x0 ][false] = "AL";
    reg_[ 0x1 ][false] = "CL";
    reg_[ 0x2 ][false] = "DL";
    reg_[ 0x3 ][false] = "BL";
    reg_[ 0x4 ][false] = "AH";
    reg_[ 0x5 ][false] = "CH";
    reg_[ 0x6 ][false] = "DH";
    reg_[ 0x7 ][false] = "BH";

    reg_[ 0x0 ][true] = "AX";
    reg_[ 0x1 ][true] = "CX";
    reg_[ 0x2 ][true] = "DX";
    reg_[ 0x3 ][true] = "BX";
    reg_[ 0x4 ][true] = "SP";
    reg_[ 0x5 ][true] = "BP";
    reg_[ 0x6 ][true] = "SI";
    reg_[ 0x7 ][true] = "DI";

    sr_[ 0x0 ] = "ES";
    sr_[ 0x1 ] = "CS";
    sr_[ 0x2 ] = "SS";
    sr_[ 0x3 ] = "DS";
    //    init
    //    Push Operand onto Stack
    com1_[ 0x50 ] = "PUSH";
    com1_[ 0x06 ] = "PUSH";
    com1_[ 0x58 ] = "POP";
    com1_[ 0x07 ] = "POP";
    com2_[ 0x20 ] = "AND";
    com2_[ 0x01 ] = "ADD";
//    com2_[ 0xD2 ] = "SHR";
//    com2_[ 0xD0 ] = "SHR";


    QFile ifs(filename);
    ifs.open(QIODevice::ReadOnly);

    hexCommands_.clear();
    while (!ifs.atEnd()) {
        QString line = ifs.readLine();
        line.trimmed();
        line.remove(line.size() - 1, 1);
        bool *ok;
        hexCommands_.push_back(quint64(line.toInt(ok, 16)));
    }

    ifs.close();
}

void Disassembler::makeStrings() {
    for (QList< quint64 >::Iterator line = hexCommands_.begin(); line != hexCommands_.end(); ++line) {
        quint64 mask = 0;

        quint64 command = *line;

        while (command > 0xff)
            command >>= 8;

        if (command & 0xe0) // reg
            mask = command & 0xf8;
        else if (command & 0x07) // seg reg
            mask = command & 0x07;

        QMap<quint64, QString>::Iterator it1 = com1_.find(quint64(command & mask));

        if (it1 != com1_.end()) {
            QString asmCommand(*it1);

            switch (command & mask) {
                case 0x50:
                case 0x58: {
                    asmCommand.append(" " + reg_[(*line) & 0x07][1]);
                    break;
                }

                case 0x06:
                case 0x07: {
                    asmCommand.append(" " + sr_[((*line) & 0x18) >> 3]);
                    break;}
            };

            asmCommands_.append(asmCommand);
        } else {

            LOG << "____" << *line;
            QMap<quint64, QString>::const_iterator mask2 = com2_.constBegin();
            while (mask2 != com2_.constEnd()) {

                quint64 tmp = *line;

                if (command == mask2.key()) {
                    LOG << mask2.key();
                    QString asmCommand(com2_[mask2.key()]);

                    switch (mask2.key()) {
                    case 0x20: {
                            LOG << "===" << *line;
                            asmCommand.append(" " + reg_[(*line) & 0x07][(*line) & 0x0100]);
                            asmCommand.append(", " + reg_[(*line) & 0x07][(*line) & 0x0100]);
                            break;
                        }
                        case 0x01:
//                        LOG << "===" << (((*line) & 0x0380) >> 5);
                        LOG << "===" << (*line >> 8 & 0xfe);
                            asmCommand.append(" " + reg_[(*line >> 8) & 0xfe][(*line) & 0x0100]);
//                            asmCommand.append(", " + reg_[((*line) & 0x0380) >> 8][(*line) & 0x0100]);
                            break;

    //                        case 0xD0:
    //                            asmCommand.append(" " + reg_[(*line) & 0x07][(*line) & 0x0100] + ", 1");
    //                            break;
    //                        case 0xD2:
    //                            asmCommand.append(" " + reg_[(*line) & 0x07][(*line) & 0x0100] + ", CL");
    //                            break;
                    }

                    asmCommands_.append(asmCommand);
                    mask2 = com2_.constEnd();
                } else {
                   ++mask2;
                }
            }
        }
    }
}

void Disassembler::writeStrings(const QString &filename) {
    QFile ofs(filename);
    ofs.open(QIODevice::WriteOnly);
    QTextStream out(&ofs);
    for (QStringList::Iterator line = asmCommands_.begin(); line != asmCommands_.end(); ++line) {
        out << *line << "\n";
        LOG << *line;
    }
        ofs.close();
}

Disassembler::~Disassembler() {
}

int Disassembler::size(quint64 value) const {
    int counter = 0;
    while (value != 0) {
        value >>= 4;
        ++counter;
    }
    return counter;
}

