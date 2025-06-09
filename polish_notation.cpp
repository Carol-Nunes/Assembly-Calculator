#include "polish_notation.h"
#include "assembly_operations.h"


Polish_Notation::Polish_Notation(bool emRadianos)
{
    this->emRadianos = emRadianos;

    this->precedencia['^'] = 4;
    this->precedencia['r'] = 4; // 'r' de root.
    this->precedencia['!'] = 4;
    this->precedencia['*'] = 3;
    this->precedencia['/'] = 3;
    this->precedencia['+'] = 2;
    this->precedencia['-'] = 2;
    this->precedencia['('] = 1;
}

void Polish_Notation::setEmRadianos(bool emRadianos)
{
    this->emRadianos = emRadianos;
}

/**
 * Converte uma expressão matemática infixa para a notação posfixa (Notação Polonesa Reversa).
 *
 * Este método lê a expressão da esquerda para a direita e a converte,
 * lidando com a precedência de operadores, parênteses, funções e operadores customizados.
 *
 */
QString Polish_Notation::toPosfixa(const QString &infixa) {
    /*
     * A pilha de operadores e funções. Temporariamente armazena operadores
     * e funções até que seu lugar na saída seja determinado pela precedência.
    */
    QStack<QString> pilha;

    /*
     * A string de saída final em notação posfixa.
     * Os operandos (números) são adicionados diretamente aqui.
    */
    QString posfixa;

    /*
     * Um buffer temporário para construir números de múltiplos dígitos ou decimais.
     * Ex: para "123.45", ele acumulará '1', '2', '3', '.', '4', '5'.
    */
    QString pilhaNumero;

    /*
     * Flag para diferenciar o operador de subtração (-) do sinal de negativo.
     * Se o último token foi um operador ou '(', um '-' subsequente é um sinal.
    */
    bool ultimoFoiOperador = true;

    // Itera por cada caractere da expressão infixa.
    for (int i = 0; i < infixa.length(); ++i)
    {
        QChar c = infixa[i];

        // Se o caractere é um dígito ou um ponto decimal, ele faz parte de um número.
        if (c.isDigit() || (c == '.'))
        {
            pilhaNumero += c;           // Acumula o caractere no buffer de número.
            ultimoFoiOperador = false;  // Atualiza a flag, pois um número foi encontrado.
        }
        // Tratamento especial para o sinal de negativo (operador unário).
        else if (c == '-' && ultimoFoiOperador)
        {
            pilhaNumero += c;           // Adiciona o '-' ao buffer de número.
            ultimoFoiOperador = false;  // O próximo token não pode ser outro sinal de negativo.
        }
        else if (c.isLetter())
        {
            /*
             *  Antes de processar uma função/operador, o número acumulado (se houver)
             *  deve ser enviado para a saída. Ex: em "5*sin(90)", o "5" vai para a saída antes de "sin".
            */
            if (!pilhaNumero.isEmpty())
            {
                posfixa += pilhaNumero + ' ';
                pilhaNumero.clear();
            }

            // Constrói o token completo (ex: "sin", "cos", "root").
            QString token;

            while (i < infixa.length() && infixa[i].isLetter())
            {
                token += infixa[i];
                i++;
            }
            i--; // Corrige o contador do loop principal, compensando o incremento extra.

            // Trata tokens específicos.
            if (token == "PI") // Constantes vão direto para a saída.
            {
                posfixa += token + ' ';
                ultimoFoiOperador = false;
            }
            else if (token == "root") // 'root' é um operador binário customizado.
            {
                /*
                 * Lógica de precedência: desempilha operadores da pilha que têm
                 *  precedência maior ou igual à de 'root'.
                */
                while (!pilha.isEmpty() &&
                       this->precedencia.value(pilha.top().at(0), 0) >= this->precedencia['r'])
                {
                    posfixa += pilha.pop() + ' ';
                }

                pilha.push(token); // Empilha o operador 'root'.

                ultimoFoiOperador = true;
            }
            else // Se não for PI nem root, é uma função (ex: sin, cos, log).
            {
                pilha.push(token);      // Funções são empilhadas para aguardar seus argumentos.
                ultimoFoiOperador = true; // Uma função se comporta como um '(', esperando um operando.
            }
        }
        else if (c == '(') // Abre parênteses sempre é empilhado.
        {
            pilha.push(QString(c));
            ultimoFoiOperador = true;
        }
        else if (c == ')') // Fecha parênteses descarrega a pilha.
        {
            // Envia qualquer número que veio imediatamente antes do ')' para a saída.
            if (!pilhaNumero.isEmpty())
            {
                posfixa += pilhaNumero + ' ';
                pilhaNumero.clear();
            }
            // Desempilha todos os operadores até encontrar o '(' correspondente.
            while (!pilha.isEmpty() && pilha.top() != "(")
            {
                posfixa += pilha.pop() + ' ';
            }
            if (!pilha.isEmpty()) pilha.pop(); // Remove o '(' da pilha.

            /*
             * Se, após remover o '(', o topo da pilha for uma função,
             * significa que os argumentos da função acabaram. A função deve ir para a saída.
            */
            if (!pilha.isEmpty() && ehFuncao(pilha.top()))
            {
                posfixa += pilha.pop() + ' ';
            }
            ultimoFoiOperador = false;
        }
        else if (ehOperador(c))
        {
            // Envia o número à esquerda do operador para a saída.
            if (!pilhaNumero.isEmpty())
            {
                posfixa += pilhaNumero + ' ';
                pilhaNumero.clear();
            }

            /*
             * Lógica de precedência: desempilha operadores com precedência maior ou igual.
             * O '.value(key, 0)' trata de forma segura casos onde o topo da pilha não é um operador (ex: uma função).
            */
            while (!pilha.isEmpty() &&
                   this->precedencia.value(pilha.top().at(0), 0) >= this->precedencia[c])
            {
                posfixa += pilha.pop() + ' ';
            }
            pilha.push(QString(c)); // Empilha o operador atual.
            ultimoFoiOperador = true;
        }
    }

    // Após o loop, envia qualquer número restante no buffer para a saída.
    if (!pilhaNumero.isEmpty())
    {
        posfixa += pilhaNumero + ' ';
    }

    // Descarrega todos os operadores restantes na pilha para a saída.
    while (!pilha.isEmpty())
    {
        posfixa += pilha.pop() + ' ';
    }

    // Retorna a string posfixa final, removendo espaços extras no início/fim.
    return posfixa.trimmed();
}

double Polish_Notation::calcularPosfixa(const QString &posfixa)
{
    QStack<double> pilha;
    QStringList tokens = posfixa.split(' ', Qt::SkipEmptyParts);

    if(posfixa.isEmpty()) return 0.0;

    for (const QString &token : tokens)
    {
        if (token == "PI")
        {
            pilha.push(M_PI);
        }
        else if (ehFuncao(token))
        {
            if (pilha.size() < 1) return 0.0;

            double a = pilha.pop();

            pilha.push(this->aplicarOperacao(a, 0, token));
        }
        else if (token == "root")
        {
            if (pilha.size() < 2) return 0.0;

            double a = pilha.pop();
            double b = pilha.pop();

            pilha.push(this->aplicarOperacao(a, b, token));
        }
        else if (token == "!")
        {
            if (pilha.size() < 1) return 0.0;

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
    return token == "sin"  || token == "cos" || token == "tan" ||
           token == "log"  || token == "sqrt" ||
           token == "arcsin" || token == "arccos" || token == "arctan";
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
        return calcularSeno(a, this->emRadianos);
    }
    else if (op == "cos")
    {
        return calcularCosseno(a, this->emRadianos);
    }
    else if (op == "tan")
    {
        return calcularTangente(a, this->emRadianos);
    }
    else if (op == "arcsin")
    {
        return calcularArcoSeno(a, this->emRadianos);
    }
    else if (op == "arccos")
    {
        return calcularArcoCosseno(a, this->emRadianos);
    }
    else if (op == "arctan")
    {
        return calcularArcoTangente(a, this->emRadianos);
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
    else if (op == "root")
    {
        return calcularRaizNdeX(a, b);
    }
    else
    {
        return op.toDouble();
    }
}
