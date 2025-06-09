#include "assembly_calculator.h"
#include "./ui_assembly_calculator.h"


Assembly_Calculator::Assembly_Calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Assembly_Calculator)
{
    ui->setupUi(this);

    this->modoInverso = false;

    this->valorAtual = 0.0;

    this->polishNotator = new Polish_Notation(false);

    ui->expressao->setText(QString::number(this->valorAtual));

    ui->expressao->installEventFilter(this); // Necessário para o cursor.

    /*
     *  Adicionando um validador.
     *  Ele permitirá que sejam usados apenas
     *  números e operadores.
     */
    QRegularExpression rx("^(arcsin|arccos|arctan|sin|cos|tan|log|PI|n!|sqrt(\\[[0-9.]*\\]\\{[0-9.]*\\})?|[0-9+\\-*/^.(){}!\\s])*$");

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
    delete this->polishNotator;
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
    QString expressao = ui->expressao->text();

    // Converte neg(x) para (0-X).

    expressao.replace(QRegularExpression("neg\\((.*?)\\)"), "(0-\\1)");

    // Converte sin^2(x) para (sin(x))^2, por exemplo.
    expressao.replace(QRegularExpression("(sin|cos|tan)\\^2\\((.*?)\\)"), "(\\1(\\2))^2");

    // Converte sqrt[y]{x} para uma nova sintaxe interna "y root x"
    expressao.replace(QRegularExpression("sqrt\\[(.*?)\\]\\{(.*?)\\}"), "(\\1)root(\\2)");

    QString posfixa = this->polishNotator->toPosfixa(expressao);

    double resultado = this->polishNotator->calcularPosfixa(posfixa);

    this->valorAtual = resultado;

    ui->expressao->setText(QString::number(this->valorAtual));
}


void Assembly_Calculator::on_expressao_returnPressed()
{
    this->on_igual_clicked();
}

void Assembly_Calculator::on_soma_clicked()
{
    ui->expressao->insert("+");

    ui->expressao->setFocus();
}


void Assembly_Calculator::on_subtracao_clicked()
{
    ui->expressao->insert("-");

    ui->expressao->setFocus();
}


void Assembly_Calculator::on_multiplicacao_clicked()
{
    ui->expressao->insert("*");

    ui->expressao->setFocus();
}


void Assembly_Calculator::on_divisao_clicked()
{
    ui->expressao->insert("/");

    ui->expressao->setFocus();
}


void Assembly_Calculator::on_parentese_1_clicked()
{
    ui->expressao->insert("(");

    ui->expressao->setFocus();
}


void Assembly_Calculator::on_parentese_2_clicked()
{
    ui->expressao->insert(")");

    ui->expressao->setFocus();
}


void Assembly_Calculator::on_virgula_clicked()
{
    ui->expressao->insert(".");

    ui->expressao->setFocus();
}


void Assembly_Calculator::on_seno_clicked()
{
    ui->expressao->insert(this->modoInverso ? "arcsin(" : "sin(");

    ui->expressao->setFocus();
}


void Assembly_Calculator::on_cosseno_clicked()
{
    ui->expressao->insert(this->modoInverso ? "arccos(" : "cos(");

    ui->expressao->setFocus();
}


void Assembly_Calculator::on_tangente_clicked()
{
    ui->expressao->insert(this->modoInverso ? "arctan(" : "tan(");

    ui->expressao->setFocus();
}

void Assembly_Calculator::on_graus_clicked()
{
    this->polishNotator->setEmRadianos(false);
}

void Assembly_Calculator::on_radianos_clicked()
{
    this->polishNotator->setEmRadianos(true);
}

void Assembly_Calculator::on_inversa_clicked()
{
    this->modoInverso = !this->modoInverso;

    if (this->modoInverso)
    {
        ui->seno->setText("arcsin");
        ui->cosseno->setText("arccos");
        ui->tangente->setText("arctan");
    }
    else
    {
        ui->seno->setText("sin");
        ui->cosseno->setText("cos");
        ui->tangente->setText("tan");
    }
}


void Assembly_Calculator::on_pi_clicked()
{
    ui->expressao->insert("PI");

    ui->expressao->setFocus();
}


void Assembly_Calculator::on_fatorial_clicked()
{
    ui->expressao->insert("!");

    ui->expressao->setFocus();
}


void Assembly_Calculator::on_exponenciacao_clicked()
{
    ui->expressao->insert("^");

    ui->expressao->setFocus();
}


void Assembly_Calculator::on_quadrado_clicked()
{
    ui->expressao->insert("^2");

    ui->expressao->setFocus();
}


void Assembly_Calculator::on_log_clicked()
{
    ui->expressao->insert("log");

    ui->expressao->setFocus();
}


void Assembly_Calculator::on_raiz_clicked()
{
    ui->expressao->insert("sqrt[]{}");

    ui->expressao->setFocus();
}


void Assembly_Calculator::on_sinal_clicked()
{
    QString textoAtual = ui->expressao->text();
    int cursorPosition = textoAtual.length(); // Trabalharemos no final da string

    // Encontra o início do último número ou token na string
    int i = cursorPosition - 1;
    while (i >= 0 && (textoAtual[i].isDigit() || textoAtual[i] == '.')) {
        i--;
    }
    // 'i' agora está no caractere ANTES do número.
    // O início do número é em 'i + 1'.
    int inicioNumero = i + 1;

    // Se não houver número no final, não faz nada
    if (inicioNumero >= cursorPosition) {
        ui->expressao->setFocus();
        return;
    }

    // Verifica se o número já está negado (ex: "neg(5)")
    if (inicioNumero > 4 && textoAtual.mid(inicioNumero - 4, 4) == "neg(")
    {
        // Remove "neg(" e o ")" final
        QString numero = textoAtual.mid(inicioNumero, cursorPosition - inicioNumero - 1);
        textoAtual.remove(inicioNumero - 4, cursorPosition - (inicioNumero - 4)); // Remove neg(...)
        textoAtual.insert(inicioNumero - 4, numero); // Insere o número de volta
    }
    else // Nega o número
    {
        QString numero = textoAtual.mid(inicioNumero, cursorPosition - inicioNumero);
        textoAtual.remove(inicioNumero, cursorPosition - inicioNumero); // Remove o número
        textoAtual.insert(inicioNumero, QString("neg(%1)").arg(numero)); // Envolve com neg()
    }

    ui->expressao->setText(textoAtual);
    ui->expressao->setFocus();
}

