#ifndef ASSEMBLY_CALCULATOR_H
#define ASSEMBLY_CALCULATOR_H

#include <QMainWindow>
#include <QRegularExpressionValidator>
#include <QValidator>

#define BOTOES_NUMERO 10

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

private slots:
    void numeroPressionado();

    void on_apagar_clicked();

    void on_expressao_cursorPositionChanged(int arg1, int arg2);

    void on_expressao_textEdited(const QString &texto);

private:
    Ui::Assembly_Calculator *ui;

    double valorAtual;
};
#endif // ASSEMBLY_CALCULATOR_H
