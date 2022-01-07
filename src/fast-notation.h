#ifndef FAST_NOTATION_H
#define FAST_NOTATION_H

#include <QDebug>
#include <QString>
#include <QVector>
#include <QStack>
#include <qmath.h>

#define VARIABLE_NAME_FOR_SCALING_FORMULA  "x"

typedef struct postfix_unit
{
    float payload = 0;
    bool op_flag = false;
}postfix_unit;

class fast_notation    //структура содержащая операнды и операции над ними для быстрого вычисления скейлинга при логгинге
{
public:
    QVector<postfix_unit> postfix_notation;
    void setVar(float x);

    QString getPostfixNotation(QString InfixNotation);
    // расчет по текстовой нотации, оставил что бы не забыть
    float Computing(QString input, float variable_value);

    void setFastNotation(QString InfixNotation);
    float fast_calc( float x);

    // расчет по быстрой нотации нов инт, уже не помню зачем
    int int_fast_calc( int x);

private:
    char variable_position = 0;              //позиция переменной в массиве

    bool IsDelimeter(QChar c);
    bool IsOperator(QChar c);
    bool IsVar(QChar c)       //Метод возвращает true, если проверяемый символ - переменная
    {
        QString a = "x";

        if (a.indexOf(c) != -1)
            return true;
        return false;
    }
    int GetPriority(QChar s)  //Метод возвращает приоритет оператора
    {
        //QString a = "()+-*/^";
        switch (s.toLatin1())
        {
        case '(': return 0;
        case ')': return 1;
        case '+': return 2;
        case '-': return 3;
        case '*': return 4;
        case '/': return 4;
        case '^': return 5;
        default: return 6;
        }
    }
    void set_op(QChar s)
    {
        postfix_unit temp_postfix_unit;
        temp_postfix_unit.payload = s.toLatin1();
        temp_postfix_unit.op_flag = true;
        postfix_notation.append(temp_postfix_unit);
    };
    void set_const(float var)
    {
        postfix_unit temp_postfix_unit;
        temp_postfix_unit.payload = var;
        temp_postfix_unit.op_flag = false;
        postfix_notation.append(temp_postfix_unit);
    }
    void set_var()
    {
        postfix_unit temp_postfix_unit;
        postfix_notation.append(temp_postfix_unit);
        variable_position = postfix_notation.size() - 1;
    }
};

#endif // FAST_NOTATION_H
