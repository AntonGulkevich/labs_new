#include <QCoreApplication>
#include "assembler.h"
#include "disassembler.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Assembler as("com.txt");
    as.makeHEX();
    as.writeHEX("asm.txt");
    Disassembler das("asm.txt");
    das.makeStrings();
    das.writeStrings("out.txt");

    return a.exec();
}
