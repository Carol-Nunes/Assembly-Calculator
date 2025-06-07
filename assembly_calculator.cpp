#include "assembly_calculator.h"
#include "./ui_assembly_calculator.h"
#include "assembly_operations.h"


Assembly_Calculator::Assembly_Calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Assembly_Calculator)
{
    ui->setupUi(this);

    this->valorAtual = 0.0;

    ui->expressao->setText(QString::number(this->valorAtual));
    /*
     *  Adicionando um validador.
     *  Ele permitirá que sejam usados apenas
     *  números e operadores.
     */
    QRegularExpression rx("[0-9+\\-*/().^!\\s]+");

    QValidator * validador = new QRegularExpressionValidator(rx, this);

    ui->expressao->setValidator(validador);

    /*
     * Inicializando os botões de número.
    */
    QPushButton * botoesNumero[BOTOES_NUMERO] = {
        ui->num_0, ui->num_1, ui->num_2, ui->num_3, ui->num_4,
        ui->num_5, ui->num_6, ui->num_7, ui->num_8, ui->num_9
    };

    for (QPushButton* botao : botoesNumero) {
        connect(botao, &QPushButton::clicked, this, &Assembly_Calculator::numeroPressionado);
    }
}

Assembly_Calculator::~Assembly_Calculator()
{
    delete ui;
}

void Assembly_Calculator::numeroPressionado()
{
    QPushButton * botao = (QPushButton *)sender();

    QString digito = botao->text(); // Lê o número do botão.

    QString novoTexto = ui->expressao->text() + digito;

    double novoValor = novoTexto.toDouble();

    this->valorAtual = novoValor;

    ui->expressao->setText(QString::number(this->valorAtual));
}

void Assembly_Calculator::on_apagar_clicked()
{
    ui->expressao->clear();

    this->valorAtual = 0.0;

    ui->expressao->setText(QString::number(this->valorAtual));
}

/*
 * Posiciona o cursor sempre no final do display.
*/
void Assembly_Calculator::on_expressao_cursorPositionChanged(int arg1, int arg2)
{
    if (ui->expressao->hasFocus())
    {
        ui->expressao->setCursorPosition(ui->expressao->text().length());
    }
}

void Assembly_Calculator::on_expressao_textEdited(const QString &texto)
{
    if(texto.isEmpty())
    {
        this->valorAtual = 0.0;

        ui->expressao->setText(QString::number(this->valorAtual));
    }
    else
    {
        this->valorAtual = texto.toDouble();
        ui->expressao->setText(QString::number(this->valorAtual));
    }
}

