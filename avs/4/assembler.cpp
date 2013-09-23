#include "assembler.h"

Assembler::Assembler(const QString &filename) {
    LOG << "start";
    //    register init
    reg_[ "AX" ] = reg_[ "AL" ] = 0x0;
    reg_[ "CX" ] = reg_[ "CL" ] = 0x1;
    reg_[ "DX" ] = reg_[ "DL" ] = 0x2;
    reg_[ "BX" ] = reg_[ "BL" ] = 0x3;
    reg_[ "SP" ] = reg_[ "AH" ] = 0x4;
    reg_[ "BP" ] = reg_[ "CH" ] = 0x5;
    reg_[ "SI" ] = reg_[ "DH" ] = 0x6;
    reg_[ "DI" ] = reg_[ "BH" ] = 0x7;
    sr_[ "ES" ] = 0x0;
    sr_[ "CS" ] = 0x1;
    sr_[ "SS" ] = 0x2;
    sr_[ "DS" ] = 0x3;
    //    word bit init
    w_[ "AL" ] = w_[ "AH" ] = false;
    w_[ "CL" ] = w_[ "CH" ] = false;
    w_[ "DL" ] = w_[ "DH" ] = false;
    w_[ "BL" ] = w_[ "BH" ] = false;
    w_[ "AX" ] = true;
    w_[ "CX" ] = true;
    w_[ "DX" ] = true;
    w_[ "BX" ] = true;
    //    init
    //    Push Operand onto Stack
    com1_["PUSH"]["R"] = 0x50;
    com1_["PUSH"]["SR"] = 0x06;
//    com1_["PUSH"]["B"] = 0x6a00;
//    com1_["PUSH"]["W"] = 0x680000;

    com1_["POP"]["R"] = 0x58;
    com1_["POP"]["SR"] = 0x07;


    com2_["AND"]["R"]["R"] = 0x20C0;
// оставлено до лучших времен
//    com2_["AND"]["R"]["B"] = 0x80;
//    com2_["AND"]["R"]["W"] = 0x82;
//    com2_["AND"]["A"]["D"] = 0x2400;

    com2_["ADD"]["R"]["R"] = 0x00C0;
// оставлено до лучших времен
//    com2_["ADD"]["R"]["B"] = 0x80;
//    com2_["ADD"]["R"]["W"] = 0x82;
//    com2_["SHR"]["A"]["D"] = 0x0500;

    com2_["SHR"]["R"]["R"] = 0xD2E8;
    com2_["SHR"]["R"]["1"] = 0xD0E8;

    QFile ifs(filename);
    ifs.open(QIODevice::ReadOnly);

    asmCommands_.clear();
    while (!ifs.atEnd()) {
        QString line = ifs.readLine();
        QStringList tmp = line.split(QRegExp(",| "), QString::SkipEmptyParts);
        asmCommands_.push_back(tmp);
    }

    ifs.close();
}

void Assembler::makeHEX() {

    hexCommands_.clear();
    for (com::Iterator command = asmCommands_.begin(); command != asmCommands_.end(); ++command) {
        quint16 tmp_arg = 0;
        quint64 tmp_com = 0;
        QString op, arg1, arg2;
        QString type1, type2;
        int length = 0;
        bool int8 = false;
        bool wordBit = false;
        QStringList currentCommand;

        if (command->size() == 2) {
            op = (*command)[0].trimmed().toUpper();
            arg1 = (*command)[1].trimmed().toUpper();

            if (com1_.find(op) != com1_.end()) { // command exists
                if (reg_.find(arg1) != reg_.end()) { // register exists
                    type1 = "R";
                    tmp_com = com1_[op][type1] | reg_[arg1];
                    tmp_arg = 0;
                    length = 2;
                } else if (sr_.find(arg1) != sr_.end()) { // register exists
                    type1 = "SR";
                    tmp_com = com1_[op][type1] | (sr_[arg1] << 3);
                    tmp_arg = 0;
                    length = 2;
                }

                hexCommands_.append(QString("%1").arg(QString::number(tmp_com, 16), length, QChar('0')).toUpper());

            } else {
                LOG << "unknown command";
                return;
            }
        }

        if (command->size() == 3) {
            LOG << "2 arg";
            QString op = (*command)[0].trimmed().toUpper();
            QString arg1 = (*command)[1].trimmed().toUpper();
            QString arg2 = (*command)[2].trimmed().toUpper();


            if (com2_.find(op) != com2_.end()) { // command exists
                if (reg_.find(arg1) != reg_.end()) { // register exists
                    type1 = "R";
                    if (reg_.find(arg2) != reg_.end()) { // second arg - register
                        type2 = "R";
                        length = 4;
                        if (w_[arg1] == w_[arg2]) {
                            wordBit = w_[arg1];
                            tmp_com = com2_[op][type1][type2] | (wordBit << 8);
                            tmp_com |= (reg_[arg2] << 3);
                            tmp_com |= (reg_[arg1]);
                        } else if (op == "SHR") {
                            type2 = "R";
                            wordBit = w_[arg1];
                            tmp_com = com2_[op][type1][type2] | (wordBit << 8);
                            tmp_com |= reg_[arg1];
                        } else {
                            LOG << "invalid combination of opcode and operands";
                            return;
                        }
                    } else if (op == "SHR")  {
                        type2 = "1";
                        wordBit = w_[arg1];
                        tmp_com = com2_[op][type1][type2] | (wordBit << 8);
                        tmp_com |= reg_[arg1];                    
                    } else {
                        //                    if (op == "SHR") {
                        //                        type2 = "B";

                        //                    }
                        LOG << "invalid combination of opcode and operands";
                        return;
                    }

                    hexCommands_.append(QString("%1").arg(QString::number(tmp_com, 16), length, QChar('0')).toUpper());
                }
            } else {
                LOG << "unknown command";
                return;
            }
        }
    }
}

    void Assembler::writeHEX(const QString & filename) {
        QFile ofs(filename);
        ofs.open(QIODevice::WriteOnly);
         QTextStream out(&ofs);
        for (QStringList::Iterator line = hexCommands_.begin(); line != hexCommands_.end(); ++line)
            out << *line << "\n";

        ofs.close();
    }

    Assembler::~Assembler() {

    }

