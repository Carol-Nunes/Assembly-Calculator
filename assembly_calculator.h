#ifndef ASSEMBLY_CALCULATOR_H
#define ASSEMBLY_CALCULATOR_H

#include "polish_notation.h"

#include <QMainWindow>
#include <QRegularExpressionValidator>
#include <QValidator>
#include <QTimer>

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

protected:
    bool eventFilter(QObject * watched, QEvent * event) override;

private slots:
    void setNovoTexto(const QString &texto);

    void numeroPressionado();

    void on_apagar_clicked();

    void on_expressao_textEdited(const QString &texto);

    void on_igual_clicked();

    void on_expressao_returnPressed();

private:
    Ui::Assembly_Calculator *ui;

    double valorAtual;

    Polish_Notation polishNotator;

};
#endif // ASSEMBLY_CALCULATOR_H
