#ifndef MATHPARSER2_H
#define MATHPARSER2_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <QList>
#include <QStack>
#include <QChar>
#include <qmath.h>
#include <QMap>
#include <QDebug>
#include <QtEndian>

#define VARIABLE_NAME_FOR_SCALING_FORMULA  "x"

struct postfix_unit
{
    float payload;
    bool op_flag;
};

struct fast_calc_struct                          //структура содержащая операнды и операции над ними для быстрого вычисления скейлинга при логгинге
{
    QVector<postfix_unit> postfix_notation{{0, false}, {0, false}, {0, true}};
    char variable_position = 0;              //позиция переменной в массиве
};



//class mathParser2
//{
//public:
//    mathParser2()
//    {

//    }

static  fast_calc_struct temp_fs;
static  postfix_unit temp_postfix_unit;

static  fast_calc_struct int_temp_fs;
static  postfix_unit int_temp_postfix_unit;


static char counter = 0;
inline static void set_op(QChar s)
{
    QString swst = "+-*/^";
    temp_postfix_unit.payload = swst.indexOf(s);
    temp_postfix_unit.op_flag = true;
    temp_fs.postfix_notation.append(temp_postfix_unit);
    counter++;
};
inline static void set_const(float var)
{
    temp_postfix_unit.payload = var;
    temp_postfix_unit.op_flag = false;
    temp_fs.postfix_notation.append(temp_postfix_unit);
    counter++;
}
inline static void set_var()
{
    temp_postfix_unit.payload = 0;
    temp_postfix_unit.op_flag = false;
    temp_fs.postfix_notation.append(temp_postfix_unit);
    temp_fs.variable_position = counter;
}

static void init()
{
    temp_fs.postfix_notation.clear();
    counter = 0;
}
static bool IsDelimeter(QChar c)//Метод возвращает true, если проверяемый символ - разделитель ("пробел" или "равно")
{QString a = " =";
    if ((a.indexOf(c) != -1))
        return true;
    return false;
}
static bool IsOperator(QChar c)//Метод возвращает true, если проверяемый символ - оператор
{
    QString a = "+-/*^()";

    if (a.indexOf(c) != -1)
        return true;
    return false;
}
static bool IsVar(QChar c)//Метод возвращает true, если проверяемый символ - переменная
{
    QString a = "x";

    if (a.indexOf(c) != -1)
        return true;
    return false;
}
static int GetPriority(QChar s)//Метод возвращает приоритет оператора
{
    QString a = "()+-*/^";
    switch (a.indexOf(s))
    {
    case 0: return 0;
    case 1: return 1;
    case 2: return 2;
    case 3: return 3;
    case 4: return 4;
    case 5: return 4;
    case 6: return 5;
    default: return 6;
    }
}
static QString get_notation_convert(QString input)
{
    QChar s;
    QString swst = "+-*/^";

    QString output{}; // = string.Empty; //Строка для хранения выражения

    QStack<QChar> operStack; //Стек для хранения операторов
    if (input.isEmpty())
    {
        input = "x * -1";
        // qDebug() << "Пустая строка!!! Данные помножены на минус";
    }
    for (int i = 0; i < input.length(); i++) //Для каждого символа в входной строке
    {
        //Разделители пропускаем
        if (IsDelimeter(input[i]))
            continue; //Переходим к следующему символу

        //Если символ - цифра, то считываем все число
        if (input[i].isDigit()) //Если цифра
        {

            //Читаем до разделителя или оператора, что бы получить число
            while (!IsDelimeter(input[i]) && !IsOperator(input[i]))
            {
                output += input[i]; //Добавляем каждую цифру числа к нашей строке
                i++; //Переходим к следующему символу

                if (i == input.length()) break; //Если символ - последний, то выходим из цикла
            }

            output +=  " "; //Дописываем после числа пробел в строку с выражением
            i--; //Возвращаемся на один символ назад, к символу перед разделителем
        }

        //Если символ - переменная, то считываем переменную
        if ( input[i].isLetter() ) //Если буква
        {
            //Читаем до разделителя или оператора, что бы получить имя
            while (!IsDelimeter(input[i]) && !IsOperator(input[i]))
            {
                output += input[i]; //Добавляем каждую букву числа к нашей строке
                i++; //Переходим к следующему символу

                if (i == input.length()) break; //Если символ - последний, то выходим из цикла
            }

            output += " "; //Дописываем после переменной пробел в строку с выражением

            i--; //Возвращаемся на один символ назад, к символу перед разделителем
        }

        //Если символ - оператор
        if (IsOperator(input[i])) //Если оператор
        {
            if (input[i] == '(' ) //Если символ - открывающая скобка
                operStack.push(input[i]); //Записываем её в стек
            else if (input[i] == ')') //Если символ - закрывающая скобка
            {
                //Выписываем все операторы до открывающей скобки в строку
                s = operStack.pop();
                while (s != '(')
                {
                    output += s + ' ';
                    s = operStack.pop();
                }
            }
            else //Если любой другой оператор
            {
                if (operStack.count() > 0) //Если в стеке есть элементы
                    if (GetPriority(input[i]) <= GetPriority(operStack.top())) //И если приоритет нашего оператора меньше или равен приоритету оператора на вершине стека
                        output = output + operStack.pop() + " "; //То добавляем последний оператор из стека в строку с выражением
                operStack.push(input[i]); //Если стек пуст, или же приоритет оператора выше - добавляем операторов на вершину стека
            }
        }
    }

    //Когда прошли по всем символам, выкидываем из стека все оставшиеся там операторы в строку
    while (operStack.count() > 0)
        output = output + operStack.pop() + " ";
    return output; //Возвращаем выражение в постфиксной записи
}

static fast_calc_struct set_notation(QString input)//тут в отличии от get notation заполняем структуру с числами что бы не парсить каждый раз строку( 5 раз выигрыш)
{
    init();
    QChar s;
    QString swst = "+-*/^";

    QStack<QChar> operStack; //Стек для хранения операторов
    if (input.isEmpty())
    {
        input = "x * -1";
        // qDebug() << "Пустая строка!!! Данные помножены на минус";
    }
    for (int i = 0; i < input.length(); i++) //Для каждого символа в входной строке
    {
        //Разделители пропускаем
        if (IsDelimeter(input[i]))
            continue; //Переходим к следующему символу

        //Если символ - цифра, то считываем все число
        if (input[i].isDigit()) //Если цифра
        {
            QString interm{}; //промежуточные числа
            //Читаем до разделителя или оператора, что бы получить число
            while (!IsDelimeter(input[i]) && !IsOperator(input[i]))
            {
                interm += input[i]; //Добавляем каждую цифру числа к нашей строке
                i++; //Переходим к следующему символу

                if (i == input.length()) break; //Если символ - последний, то выходим из цикла
            }

            set_const(interm.toFloat());
            i--; //Возвращаемся на один символ назад, к символу перед разделителем
        }

        //Если символ - переменная, то считываем переменную
        if ( input[i].isLetter() ) //Если буква
        {
            //Читаем до разделителя или оператора, что бы получить имя
            while (!IsDelimeter(input[i]) && !IsOperator(input[i]))
            {
                i++; //Переходим к следующему символу
                if (i == input.length()) break; //Если символ - последний, то выходим из цикла
            }
            set_var();                //добавляю  вместо переменной что бы в векторе появился элемент
            i--; //Возвращаемся на один символ назад, к символу перед разделителем
        }
        //Если символ - оператор
        if (IsOperator(input[i])) //Если оператор
        {
            if (input[i] == '(' ) //Если символ - открывающая скобка
                operStack.push(input[i]); //Записываем её в стек
            else if (input[i] == ')') //Если символ - закрывающая скобка
            {
                //Выписываем все операторы до открывающей скобки в строку
                s = operStack.pop();
                while (s != '(')
                {
                    set_op(s);
                    s = operStack.pop();
                }
            }
            else //Если любой другой оператор
            {
                if (operStack.count() > 0) //Если в стеке есть элементы
                    if (GetPriority(input[i]) <= GetPriority(operStack.top())) //И если приоритет нашего оператора меньше или равен приоритету оператора на вершине стека
                    {
                        s = operStack.pop();
                        set_op(s);
                    }
                operStack.push(input[i]); //Если стек пуст, или же приоритет оператора выше - добавляем операторов на вершину стека
            }
        }
    }
    //Когда прошли по всем символам, выкидываем из стека все оставшиеся там операторы в строку
    while (operStack.count() > 0)
    {
        s = operStack.pop();
        set_op(s);
    }
    return temp_fs;
}

static float Computing(QString input, float variable_value)
{

    float result = 0; //Результат
    QStack<float> temp; //Временный стек для решения

    for (int i = 0; i < input.length(); i++) //Для каждого символа в строке
    {
        //Если символ - цифра, то читаем все число и записываем на вершину стека
        if (input[i].isDigit())
        {
            QString a{};  // = string.Empty;

            while (!IsDelimeter(input[i]) && !IsOperator(input[i])) //Пока не разделитель
            {
                a += input[i]; //Добавляем
                i++;
                if (i == input.length()) break;
            }
            temp.push(a.toFloat()); //Записываем в стек

            i--;
        }
        else

            //Если символ - буква, то читаем все слово сравниваем с допустимым именем и записываем на вершину стека
            if (input[i].isLetter())
            {
                QString a = "";
                while (!IsDelimeter(input[i]) && !IsOperator(input[i])) //Пока не разделитель
                {
                    a += input[i]; //Добавляем
                    i++;
                    if (i == input.length()) break;
                }

                if (a == VARIABLE_NAME_FOR_SCALING_FORMULA)   //проверяем есть ли в списке переменных наше слово
                {
                    temp.push(variable_value); //Записываем в стек значение переменной

                }
                else {
                    qDebug() << a << "  Ахтунг!! Нет такой переменной в списке!!";
                    temp.push(0); //пишем ноль  что бы не закрашить прогу - похуй на результат

                    i--;
                }
            }
            else
                if (IsOperator(input[i])) //Если символ - оператор
                {
                    //Берем два последних значения из стека
                    float a = temp.pop();
                    float b = temp.pop();

                    QString swst = "+-*/^";
                    switch (swst.indexOf(input[i])) //И производим над ними действие, согласно оператору
                    {

                    case 0:  result = b + a;  break;
                    case 1:  result = b - a;  break;
                    case 2:  result = b * a;  break;
                    case 3: {
                        if (a == 0)
                        {
                            a = 0.001;
                            //               qDebug() << "Попытка деления на ноль!!!";
                        }
                        result = b / a;
                    } break;
                    case 4:  result = qPow(b, a);
                        break;
                    }
                    temp.push(result); //Результат вычисления записываем обратно в стек
                }
    }
    return temp.top(); //Забираем результат всех вычислений из стека и возвращаем его
}
static float fast_calc(fast_calc_struct temp_fs, float x)
{
    temp_postfix_unit.payload = x;
    temp_postfix_unit.op_flag = false;
    temp_fs.postfix_notation.replace(temp_fs.variable_position, temp_postfix_unit);

    float result = 0; //Результат
    QStack<float> temp; //Временный стек для решения
    for (int i = 0; i < temp_fs.postfix_notation.count(); i++) //Для каждого символа в строке
    {
        if ( !temp_fs.postfix_notation.at(i).op_flag )  //Если символ - цифра, то читаем все число и записываем на вершину стека
            temp.push(temp_fs.postfix_notation.at(i).payload); //Записываем в стек
        else
            if (temp_fs.postfix_notation.at(i).op_flag && temp.count() > 1 ) //Если символ - оператор
            {
                //Берем два последних значения из стека
                float a = temp.pop();
                float b = temp.pop();
                switch ( qRound( temp_fs.postfix_notation.at(i).payload ) ) //И производим над ними действие, согласно оператору
                {
                case 0:  result = b + a;  break;
                case 1:  result = b - a;  break;
                case 2:  result = b * a;  break;
                case 3: {
                    if (a == 0)
                    {
                        a = 0.001;//               qDebug() << "Попытка деления на ноль!!!";
                    }
                    result = b / a;
                } break;
                case 4:  result = qPow(b, a);
                    break;
                }
                temp.push(result); //Результат вычисления записываем обратно в стек
            }
    }
    return temp.top(); //Забираем результат всех вычислений из стека и возвращаем его
}
static int int_fast_calc(fast_calc_struct int_temp_fs, int x)
{
    int_temp_postfix_unit.payload = x;
    int_temp_postfix_unit.op_flag = false;
    int_temp_fs.postfix_notation.replace(int_temp_fs.variable_position, int_temp_postfix_unit);

    int result = 0; //Результат
    QStack<int> temp; //Временный стек для решения
    for (int i = 0; i < int_temp_fs.postfix_notation.count(); i++) //Для каждого символа в строке
    {
        if ( !int_temp_fs.postfix_notation.at(i).op_flag )  //Если символ - цифра, то читаем все число и записываем на вершину стека
            temp.push(int_temp_fs.postfix_notation.at(i).payload); //Записываем в стек
        else
            if (int_temp_fs.postfix_notation.at(i).op_flag) //Если символ - оператор
            {
                //Берем два последних значения из стека
                int a = temp.pop();
                int b = temp.pop();
                switch ( qRound( int_temp_fs.postfix_notation.at(i).payload ) ) //И производим над ними действие, согласно оператору
                {
                case 0:  result = b + a;  break;
                case 1:  result = b - a;  break;
                case 2:  result = b * a;  break;
                case 3: {
                    if (a == 0)
                    {
                        a = 0.001;//               qDebug() << "Попытка деления на ноль!!!";
                    }
                    result = b / a;
                } break;
                case 4:  result = qPow(b, a);
                    break;
                }
                temp.push(result); //Результат вычисления записываем обратно в стек
            }
    }
    return temp.top(); //Забираем результат всех вычислений из стека и возвращаем его
}

static qint64 typed(QString storagetype, uchar *in_buf, uint c, bool big)//кастуем данные к определенному типу
{
    if ( storagetype == "int8")
    {

        return ((qint8)(in_buf)[c]);

        //    return (qint8)(*((qint8*) (f + c)));
    }
    if ( storagetype == "int16")
    {
        if (big)
            return qFromBigEndian<qint16>(in_buf + 2 * c);
        else
            return qFromLittleEndian<qint16>(in_buf + 2 * c);
    }
    if ( storagetype == "int32")
    {
        if (big)
            return qFromBigEndian<qint32>(in_buf + 4 * c);
        else
            return qFromLittleEndian<qint32>(in_buf + 4 * c);
    }
    if ( storagetype == "uint8")
    {
        return ((quint8)(in_buf)[c]);
        //  return (quint8)(*((quint8*) (f + c)));
    }
    if ( storagetype == "uint16")
    {
        if (big)
            return qFromBigEndian<quint16>(in_buf + 2 * c);
        else
            return qFromLittleEndian<quint16>(in_buf + 2 * c);
    }
    if ( storagetype == "uint32")
    {
        if (big)
            return qFromBigEndian<quint32>(in_buf + 4 * c);
        else
            return qFromLittleEndian<quint32>(in_buf + 4 * c);
    }
    else
        return ((quint8)(in_buf)[c]);
};
//};

#endif // MATHPARSER2_H
