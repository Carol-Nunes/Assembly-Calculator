#ifndef POLISH_NOTATION_H
#define POLISH_NOTATION_H

#include <QString>
#include <QStack>
#include <QMap>
#include <QtMath>

class Polish_Notation
{
public:
    Polish_Notation(bool emRadianos);

    void setEmRadianos(bool emRadianos);

    QString toPosfixa(const QString &infixa);

    double calcularPosfixa(const QString &posfixa);

private:
    QMap<QChar, int> precedencia;

    bool emRadianos;

    bool ehOperador(QChar c);

    bool ehDigitoOuDecimal(QChar c);

    bool ehFuncao(QString token);

    double aplicarOperacao(double a, double b, QString op);
};

#endif // POLISH_NOTATION_H
