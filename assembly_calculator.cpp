#include "assembly_calculator.h"
#include "./ui_assembly_calculator.h"


Assembly_Calculator::Assembly_Calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Assembly_Calculator)
{
    ui->setupUi(this);

    this->valorAtual = 0.0;

    ui->expressao->setText(QString::number(this->valorAtual));

    ui->expressao->installEventFilter(this); // Necessário para o cursor.

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

    for (QPushButton * botao : botoesNumero)
    {
        connect(botao, &QPushButton::released, this, &Assembly_Calculator::numeroPressionado);
    }
}

Assembly_Calculator::~Assembly_Calculator()
{
    delete ui;
}

void Assembly_Calculator::setNovoTexto(const QString &texto)
{
    QString novoTexto = texto;

    // Previne zeros iniciais (ex: "0123" -> "123")
    if ((texto.startsWith("0")) &&
        (texto.length() > 1))
    {
        novoTexto.remove(0, 1); // Remove o primeiro 0.
    }

    ui->expressao->setText(novoTexto);
}

void Assembly_Calculator::numeroPressionado()
{
    QPushButton * botao = (QPushButton *)sender();

    QString digito = botao->text(); // Lê o número do botão.

    QString novoTexto = ui->expressao->text() + digito;

    this->setNovoTexto(novoTexto);

    ui->expressao->setFocus();
}

void Assembly_Calculator::on_apagar_clicked()
{
    ui->expressao->clear();

    this->valorAtual = 0.0;

    ui->expressao->setText("0");
}

/*
 * Posiciona o cursor no final do display ao
 * selecionar o display pela primeira vez.
*/
bool Assembly_Calculator::eventFilter(QObject * obj, QEvent * event)
{
    if (obj == ui->expressao && event->type() == QEvent::MouseButtonPress) {
        /*
         * Garante que o cursor estará no final
         *  pós manipulação padrão do mouse.
        */
        QTimer::singleShot(0, [this]() {
            ui->expressao->setCursorPosition(ui->expressao->text().length());
        });
    }

    return QMainWindow::eventFilter(obj, event);
}

void Assembly_Calculator::on_expressao_textEdited(const QString &texto)
{
    QRegularExpression padroesInvalidos(
        "(?:[+\\-*/^]{2,})"           // Operadores repetidos: ++, --, etc.
        "|(?:\\d+\\.\\d+\\.)"         // Vários pontos decimais no mesmo número
        "|(?:[+\\-*/^]\\.)"           // Operador seguido de ponto: +.
        "|(?:\\.[+\\-*/^])"           // Ponto seguido de operador: .+
        "|(?:\\(\\.)"                 // (. inválido
        "|(?:\\.\\))"                 // .) inválido
        "|(?:\\d+\\.\\()"             // Número decimal seguido de parêntese: 0.(
        "|(?:\\(\\))"                 // Parênteses vazios: ()
        "|(?:\\.\\()"                 // Ponto seguido de abre-parêntese: .(
    );


    if(texto.isEmpty())
    {
        this->on_apagar_clicked();

        return;
    }

    QString novoTexto = texto;

    // Previne operadores e "." consecutivos.
    if(texto.contains(padroesInvalidos))
    {
        // Reverte para o último texto valido.
        ui->expressao->blockSignals(true);

        novoTexto.chop(1); // Remove o operador consecutivo.

        ui->expressao->setText(novoTexto);

        ui->expressao->blockSignals(false);

        return;
    }

    this->setNovoTexto(novoTexto);
}


void Assembly_Calculator::on_igual_clicked()
{
    QString posfixa = this->polishNotator.toPosfixa(ui->expressao->text());

    double resultado = this->polishNotator.calcularPosfixa(posfixa);

    this->valorAtual = resultado;

    ui->expressao->setText(QString::number(this->valorAtual));
}


void Assembly_Calculator::on_expressao_returnPressed()
{
    this->on_igual_clicked();
}
