#ifndef ASSEMBLY_CALCULATOR_H
#define ASSEMBLY_CALCULATOR_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class Assembly_Calculator;
}
QT_END_NAMESPACE

class Assembly_Calculator : public QMainWindow
{
    Q_OBJECT

public:
    Assembly_Calculator(QWidget *parent = nullptr);
    ~Assembly_Calculator();

private:
    Ui::Assembly_Calculator *ui;
};
#endif // ASSEMBLY_CALCULATOR_H
