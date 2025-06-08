#include "polish_notation.h"
#include "assembly_operations.h"
#include <cmath>

Polish_Notation::Polish_Notation() {
    this->precedencia['^'] = 4;
    this->precedencia['!'] = 4;
    this->precedencia['*'] = 3;
    this->precedencia['/'] = 3;
    this->precedencia['+'] = 2;
    this->precedencia['-'] = 2;
    this->precedencia['('] = 1;
}

QString Polish_Notation::toPosfixa(const QString &infixa) {
    QStack<QString> pilha; // Pilha da expressão aritmética.
    QString posfixa; // A expressão posfixa resultante.
    QString pilhaNumero; // Uma pilha só com os números.

    bool ultimoFoiOperador = true; // Indica se o último caracter foi operador ou '('.

    for (int i = 0; i < infixa.length(); ++i)
    {
        QChar c = infixa[i];

        /*
         * Identifica o início de funções como
         * sin, cos, tan, etc.
        */
        if (c.isLetter())
        {
            QString funcao;

            while (i < infixa.length() && infixa[i].isLetter())
            {
                funcao += infixa[i];
                ++i;
            }

            i--; // Compensar o loop externo.

            pilha.push(funcao);
            ultimoFoiOperador = false;
            continue;
        }

        /*
         * Se chegou num operador,
         * ou outro caractere que não é número nem parte de número
        */
        if (c == '-' && (ultimoFoiOperador || infixa[i - 1] == '('))
        {
            // Trata número como negativo.
            pilhaNumero += c;
            continue;
        }

        // Se chegou em operador, primeiro empilha número acumulado.
        if (ehOperador(c))
        {
            if (!pilhaNumero.isEmpty())
            {
                posfixa += pilhaNumero + ' ';
                pilhaNumero.clear();
            }

            // Adiciona o operador na pilha de operadores.
            while (!pilha.isEmpty()
                   && this->precedencia[pilha.top()[0]] >= this->precedencia[c])
            {
                posfixa += pilha.pop() + ' ';
            }

            pilha.push(c);
            ultimoFoiOperador = true;
            continue;
        }

        // Acumula número ou parte dele
        if (ehDigitoOuDecimal(c))
        {
            pilhaNumero += c;

            ultimoFoiOperador = false;

            continue;
        }

        if (c == '(')
        {
            pilha.push(c);
            ultimoFoiOperador = true;
        }
        else if (c == ')')
        {
            // Primeiro, adiciona o número que veio antes do ')'
            if (!pilhaNumero.isEmpty())
            {
                posfixa += pilhaNumero + ' ';
                pilhaNumero.clear();
            }

            // Adiciona a expressão entre parentêses.
            while(!pilha.isEmpty() && pilha.top() != '(')
            {
                posfixa += pilha.pop() + ' ';
            }

            if (!pilha.isEmpty()) pilha.pop(); // Remove '('.

            // Função antes de parentêses.
            if (!pilha.isEmpty() && pilha.top().length() > 1)
            {
                posfixa += pilha.pop() + ' ';
            }

            ultimoFoiOperador = false;
        }
        else if(ehOperador(c))
        {

            while (!pilha.isEmpty() &&
                   this->precedencia[pilha.top()[0]] >= this->precedencia[c])
            {
                posfixa += pilha.pop() + ' ';
            }

            pilha.push(c);
            ultimoFoiOperador = true;
        }
        else
        {
            ultimoFoiOperador = false;
        }

    }

    if (!pilhaNumero.isEmpty())
    {
        posfixa += pilhaNumero + ' ';
    }

    while (!pilha.isEmpty())
    {
        posfixa += pilha.pop() + ' ';
    }

    return posfixa.trimmed();
}

double Polish_Notation::calcularPosfixa(const QString &posfixa)
{
    QStack<double> pilha;
    QStringList tokens = posfixa.split(' ', Qt::SkipEmptyParts);

    if(posfixa.isEmpty()) return 0.0;

    for (const QString &token : tokens)
    {
        if (ehFuncao(token))
        {
            double a = pilha.pop();

            pilha.push(this->aplicarOperacao(a, 0, token));
        }
        else if (ehOperador(token[0]) && token.length() == 1)
        {
            if (pilha.size() < 2) return 0.0;

            double b = pilha.pop();
            double a = pilha.pop();

            pilha.push(this->aplicarOperacao(a, b, token));
        }
        else
        {
            bool valorConvertido; // Indica se o valor foi convertido para double.

            double valor = token.toDouble(&valorConvertido);

            if (valorConvertido)
            {
                pilha.push(valor);
            }
            else
            {
                // Erro na conversão da expressão.
                return 0.0;
            }
        }
    }

    return !pilha.isEmpty() ? pilha.pop() : 0.0;
}

bool Polish_Notation::ehOperador(QChar c)
{
    return this->precedencia.contains(c);
}

bool Polish_Notation::ehDigitoOuDecimal(QChar c)
{
    return c.isDigit() || c == '.';
}

bool Polish_Notation::ehFuncao(QString token)
{
    if (token == "sin" ||
        token == "cos" ||
        token == "tan" ||
        token == "log" ||
        token == "!"   ||
        token == "sqrt")
    {
        return true;
    }

    return false;
}

/**
 * Aplica as operações em assembly
 * de acordo com o operador.
 */
double Polish_Notation::aplicarOperacao(double a, double b, QString op)
{
    if (op == "+")
    {
        return adicao(a, b);
    }
    else if (op == "-")
    {
        return subtracao(a, b);
    }
    else if (op == "*")
    {
        return multiplicacao(a, b);
    }
    else if (op == "/")
    {
        return divisao(a, b);
    }
    else if (op == "^")
    {
        return calcularXElevadoAY(a, b);
    }
    else if (op == "sin")
    {
        return calcularSeno(a, true);
    }
    else if (op == "cos")
    {
        return calcularCosseno(a, true);
    }
    else if (op == "tan")
    {
        return calcularTangente(a, true);
    }
    else if (op == "log")
    {
        return calcularLog(10, a);
    }
    else if (op == "!")
    {
        return calcularFatorial(a);
    }
    else if (op == "sqrt")
    {
        return calcularRaizQuadrada(a);
    }
    else
    {
        return op.toDouble();
    }
}
