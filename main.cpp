#include "assembly_calculator.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Assembly_Calculator w;
    w.show();
    return a.exec();
}
