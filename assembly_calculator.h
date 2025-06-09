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

    void on_soma_clicked();

    void on_subtracao_clicked();

    void on_multiplicacao_clicked();

    void on_divisao_clicked();

    void on_parentese_1_clicked();

    void on_parentese_2_clicked();

    void on_virgula_clicked();

    void on_seno_clicked();

    void on_cosseno_clicked();

    void on_tangente_clicked();

    void on_graus_clicked();

    void on_radianos_clicked();


    void on_inversa_clicked();

    void on_pi_clicked();

    void on_fatorial_clicked();

    void on_exponenciacao_clicked();

    void on_quadrado_clicked();

    void on_log_clicked();

    void on_raiz_clicked();

    void on_sinal_clicked();

private:
    Ui::Assembly_Calculator *ui;

    bool modoInverso;

    double valorAtual;

    Polish_Notation * polishNotator;

};
#endif // ASSEMBLY_CALCULATOR_H
