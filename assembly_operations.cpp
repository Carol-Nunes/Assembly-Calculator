#include "assembly_operations.h"


float adicao(float a, float b)
{

    float soma = 0;

    __asm{
            finit //Comando que incializa a FPU (ele esvazia a pilha)
            fld a //Carrega o valor de a para o topo da pilha
            fld b //Carrega o valor de b para o topo da pilha
            /*
             * Realiza a soma entre a e b, e armazena o resultado em st(1)
             * e dá um pop, fazendo com que st(1) se torne o novo topo da pilha.
            */
            faddp st(1), st
            /*
             * Armazena o valor que está no topo da pilha dentro da variável
             * 'soma'.
            */
            fstp  soma
    }

    return soma;

}


float subtracao(float a, float b)
{

    float resultado = 0;

    __asm{

            finit
            fld a
            fld b
            /*
             * Realiza a diferença entre a e b, e armazena o resultado em st(1)
             * e dá um pop, fazendo com que st(1) se torne o novo topo da pilha.
            */
            fsubp st(1), st
            /*
             * Armazena o valor que está no topo da pilha dentro da variável
             * 'resultado'
            */
            fstp resultado
    }

    return resultado;

}


float multiplicacao(float a, float b)
{

    float produto = 0;

    __asm{

            finit
            fld a
            fld b
            /*
             * Realiza o produto entre a e b, e armazena o resultado em st(1)
             * e dá um pop, fazendo com que st(1) se torne o novo topo da pilha.
            */
            fmulp st(1), st
            /*
             * Armazena o valor que está no topo da pilha dentro da variável
             * 'produto'
            */
            fstp produto

    }

    return produto;

}


float divisao(float a, float b)
{

    float resultado = 0;

    __asm{

            finit
            fld a
            fld b
            /*
             * Realiza a divisão  a/b, e armazena o resultado em st(1)
             * e dá um pop, fazendo com que st(1) se torne o novo topo da pilha.
            */
            fdivp st(1), st
            /*
             * Armazena o valor que está no topo da pilha dentro da variável
             * 'resultado' e dá um pop.
            */
            fstp resultado

    }

    return resultado;

}

float transformaGrauEmRadiano(float a)
{
    float constante = 180;
    float resultado;

    __asm{
            fld a
            fldpi //Carrega o valor de pi para a pilha.
            /*
             * Realiza a multiplicação  a(grau) * 180, e armazena o resultado em st(1)
             * e dá um pop, fazendo com que st(1) se torne o novo topo da pilha.
            */
            fmulp st(1), st
            fld constante //Carrega 180 para a pilha
            /*
             * Realiza a divisão  a * pi/180, e armazena o resultado em st(1)
             * e dá um pop, fazendo com que st(1) se torne o novo topo da pilha.
            */
            fdivp st(1), st
            /*
             * Armazena o valor que está no topo da pilha dentro da variável
             * 'resultado' e dá um pop.
            */
            fstp resultado
    }

    return resultado;
}


float calcularSeno(float a, int isRadiano)
{
    float resultado;

    if(!isRadiano)
    {

        a = transformaGrauEmRadiano(a);

    }

    __asm {

            finit
            fld a
            /*
             * Calcula o valor do seno do elemento que está no topo da pilha.
            */
            fsin
            /*
             * Armazena o valor que está no topo da pilha dentro da variável
             * 'resultado' e dá um pop.
            */
            fstp resultado
    }

    return resultado;

}


float calcularCosseno(float a, int isRadiano)
{

    float resultado;

    if(!isRadiano)
    {
        a = transformaGrauEmRadiano(a);
    }


    __asm {

            finit
            fld a
            /*
             * Calcula o valor do cosseno do elemento que está no topo da pilha.
            */
            fcos
            /*
             * Armazena o valor que está no topo da pilha dentro da variável
             * 'resultado' e dá um pop.
            */
            fstp resultado

    }

    return resultado;

}


float calcularTangente(float a, int isRadiano)
{

    float resultado;

    if(!isRadiano)
    {
        a = transformaGrauEmRadiano(a);
    }


    __asm {

            finit
            fld a
            /*
             * Calcula o valor do seno e do cosseno do elemento que está no topo da pilha.
             */
            fsincos
            /*
             * Realiza a divisão st(1)/st. Lembre-se tangente é sen(x)/cos(x).
            */
            fdivp st(1), st
            /*
             * Armazena o valor que está no topo da pilha dentro da variável
             * 'resultado' e dá um pop.
            */
            fstp resultado

    }

    return resultado;

}



float calcularRaizQuadrada(float a)
{

    float resultado;

    __asm {

            finit
            fld a
            /*
             * Calcula o valor da raiz quadrada do elemento que está no topo da pilha.
            */
            fsqrt
            /*
             * Armazena o valor que está no topo da pilha dentro da variável
             * 'resultado' e dá um pop.
            */
            fstp resultado

    }

    return resultado;

}


float calcularElementoElevadoAoQuadrado(float a)
{

    float resultado;

    __asm {

            finit
            fld a
            fld st // Duplica o topo.
            /*
             * Calcula o valor do elemento que está no topo da pilha ao quadrado
             * e dá um pop no topo.
            */
            fmulp st(1), st
            /*
             * Armazena o valor que está no topo da pilha dentro da variável
             * 'resultado' e dá um pop.
            */
            fstp resultado

    }

    return resultado;

}


float calcularXElevadoAY(float x, float y)
{

    float resultado;


    __asm {

            finit
            fld y
            fld1 // Carrega o valor 1 no topo da pilha.
            fld x
            fyl2x //faz st = st(1) * log(st)2
            fmul // Calcula o produto entre st, st(1)
            fld st//Duplica o topo
            /*
             * Realiza o trucamento em st, ou seja,
             * deixa só a parte inteira do valor armazenado no topo.
            */
            frndint
            /*
             * Subtração entre a parte inteira e o número fracionário.
             * Assim, st(1) armazena a parte fracionária do número.
            */
            fsub st(1), st
            /*
             * Alterna os valores.	Ou seja, st vira st(1) e st(1) vira st.
            */
            fxch
            f2xm1 // st = 2st – 1
            fld1
            fadd
            fscale //st = st * 2(int(st(1))
            /*
             * Armazena o valor que está no topo da pilha dentro da variável
             * 'resultado' e dá um pop.
            */
            fstp resultado

    }

    return resultado;


}


float calcularRaizNdeX(float x, float n)
{
    float expoente;

    __asm {

            finit
            fld1
            fld n
            fdivp st(1), st
            fstp expoente
    }

    return calcularXElevadoAY(x, expoente);

}


float calcularLog(float n, float x)
{

    float resultado;

    __asm {

            finit
            fld1
            fld n
            fyl2x //st = 1.log2(n)
            fld1
            fdiv st, st(1) // st = 1 / log2(n)
            fld x
            fyl2x //st = (1 / log2(n)) * log2(x) = st = log2(x) / log2(n)
            fstp resultado
    }

    return resultado;

}


float calcularFatorial(int n)
{

    float resultado;

    __asm {


    }

    return resultado;

}


float calcularArcoTangente(float a, int isRadiano)
{

    if(!isRadiano)
    {
        a = transformaGrauEmRadiano(a);
    }

    float resultado;

    __asm {

            finit
            fld1
            fld a
            fpatan //Calcula o valor do arctg(st(1) / st) = arctg(a / 1) = arctg (a)
            fstp resultado
    }

    return resultado;

}


/*
 * Para fazer a função seguinte foi levada em consideração a seguinte
 * relação trigonométrica: arcsin(x) = arctan(x / sqrt(1 - x^2))
*/
float calcularArcoSeno(float a, int isRadiano)
{

    if(!isRadiano)
    {
        a = transformaGrauEmRadiano(a);
    }

    float resultado;

    __asm {

            finit

            fld a
            fld st(0) //Duplica o topo. Assim st = a, st(1) = a
            fmul // st = a * a

            fld1 // st = 1 , st(1) = a^2
            fsub // st = 1 - a^2
            fsqrt // st = sqrt(1 - a^2)

            fld a // st = a, st(1) = sqrt(1 - a^2)
            fdiv // st = a / sqrt(1 - a^2)

            fld1
            /*
             * Calcula o valor do arctg(st(1) / st) =
             * = arctg((a / sqrt(1 - a^2)) / 1) = arcsen (a)
            */
            fpatan

            fstp resultado
    }

    return resultado;
}

/*
 * Para a construção dessa função foi usada a seguinte relação
 * trigonométrica: arccos(x) = π/2 - arcsen(x)
*/
float calcularArcoCosseno(float a, int isRadiano)
{

    if(!isRadiano)
    {
        a = transformaGrauEmRadiano(a);
    }

    float resultado;
    float b = calcularArcoSeno(a, isRadiano);

    __asm {

            finit

            fldpi

            fld1
            fld st(0) // Duplica o topo. Assim st = 1, st(1) = 1, st(2) = pi
            fadd // st = 2, st(1) = pi

            fdiv // st = st(1) / st = pi / 2

            fld b // st = arcsen(a), st(1) = pi / 2

            fsub // st = st(1) - st = (pi / 2) - arcsen(a)

            fstp resultado
    }

    return resultado;

}


float calcularFatorial(int n)
{
    float resultado = 1.0f;

    __asm {

        mov ecx, n // Move n para ecx
        test ecx, ecx //Verifica se n = 0
        jz fim // Se n = 0, é dado um salto para "fim"

        fld1 // Carrega 1 no topo da pilha

    loop_fatorial:

        fild dword ptr [n]   // Converte n para float e carrega na pilha
        fmulp st(1), st(0)    // Multiplica pelo acumulador e dá um pop
        dec dword ptr [n]    // Decrementa n
		    jnz loop_fatorial // Se não deu zero, o loop continua

        /*
        O topo é armazenado dentro de "resultado" e é dado um pop.
        */
        fstp resultado

    fim:

    }

    return resultado;
}


