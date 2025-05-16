#include "assembly_calculator.h"
#include "./ui_assembly_calculator.h"

Assembly_Calculator::Assembly_Calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Assembly_Calculator)
{
    ui->setupUi(this);
}

Assembly_Calculator::~Assembly_Calculator()
{
    delete ui;
}
