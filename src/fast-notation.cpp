#include "fast-notation.h"

void fast_notation::setVar(float x)
{
    postfix_notation[variable_position].payload = x;
    postfix_notation[variable_position].op_flag = false;
}

float fast_notation::fast_calc(float x)
{
    if(postfix_notation.size() < 2)
        return x;
    setVar(x);
    //postfix_unit temp_postfix_unit;
    //temp_postfix_unit.payload = x;
    //temp_postfix_unit.op_flag = false;
    //qDebug() << "temp_fs " << (uchar)temp_fs.postfix_notation.size()//.postfix_notation[0]
    //            ;
    //temp_fs.postfix_notation.replace(temp_fs.variable_position, temp_postfix_unit);

    float result = 0; //Результат
    QStack<float> temp; //Временный стек для решения
    for (int i = 0; i < postfix_notation.count(); i++) //Для каждого юнита в векторе
    {
        if ( !postfix_notation.at(i).op_flag )  //Если юнит - цифра, то читаем его и записываем на вершину стека
            temp.push(postfix_notation.at(i).payload); //Записываем в стек
        else
            if (postfix_notation.at(i).op_flag && temp.count() > 1 ) //Если символ - оператор
            {
                //Берем два последних значения из стека
                float a = temp.pop();
                float b = temp.pop();
                switch ( qRound( postfix_notation.at(i).payload ) ) //И производим над ними действие, согласно оператору
                {
                case '+':  result = b + a;  break;
                case '-':  result = b - a;  break;
                case '*':  result = b * a;  break;
                case '/': {
                    if (a == 0)
                    {
                        a = 0.001;//               qDebug() << "Попытка деления на ноль!!!";
                    }
                    result = b / a;
                } break;
                case '^':  result = qPow(b, a);
                    break;
                }
                temp.push(result); //Результат вычисления записываем обратно в стек
            }
    }
    return temp.top(); //Забираем результат всех вычислений из стека и возвращаем его
}

int fast_notation::int_fast_calc(int x)
{
    setVar(x);


    int result = 0; //Результат
    QStack<int> temp; //Временный стек для решения
    for (int i = 0; i < postfix_notation.count(); i++) //Для каждого символа в строке
    {
        if ( !postfix_notation.at(i).op_flag )  //Если символ - цифра, то читаем все число и записываем на вершину стека
            temp.push(postfix_notation.at(i).payload); //Записываем в стек
        else
            if (postfix_notation.at(i).op_flag) //Если символ - оператор
            {
                //Берем два последних значения из стека
                int a = temp.pop();
                int b = temp.pop();
                switch ( qRound( postfix_notation.at(i).payload ) ) //И производим над ними действие, согласно оператору
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

QString fast_notation::getPostfixNotation(QString InfixNotation)
{
    QChar s;
    QString swst = "+-*/^";
    QString output{}; // = string.Empty; //Строка для хранения выражения
    QStack<QChar> operStack; //Стек для хранения операторов
    if (InfixNotation.isEmpty())
        InfixNotation = "x * -1";  // qDebug() << "Пустая строка!!! Данные помножены на минус";
    for (int i = 0; i < InfixNotation.length(); i++) //Для каждого символа в входной строке
    {
        //Разделители пропускаем
        if (IsDelimeter(InfixNotation[i]))
            continue; //Переходим к следующему символу
        //Если символ - цифра, то считываем все число
        if (InfixNotation[i].isDigit()) //Если цифра
        {
            //Читаем до разделителя или оператора, что бы получить число
            while (!IsDelimeter(InfixNotation[i]) && !IsOperator(InfixNotation[i]))
            {
                output += InfixNotation[i]; //Добавляем каждую цифру числа к нашей строке
                i++; //Переходим к следующему символу
                if (i == InfixNotation.length()) break; //Если символ - последний, то выходим из цикла
            }
            output +=  " "; //Дописываем после числа пробел в строку с выражением
            i--; //Возвращаемся на один символ назад, к символу перед разделителем
        }
        //Если символ - переменная, то считываем переменную
        if ( InfixNotation[i].isLetter() ) //Если буква
        {
            //Читаем до разделителя или оператора, что бы получить имя
            while (!IsDelimeter(InfixNotation[i]) && !IsOperator(InfixNotation[i]))
            {
                output += InfixNotation[i]; //Добавляем каждую букву числа к нашей строке
                i++; //Переходим к следующему символу
                if (i == InfixNotation.length()) break; //Если символ - последний, то выходим из цикла
            }
            output += " "; //Дописываем после переменной пробел в строку с выражением
            i--; //Возвращаемся на один символ назад, к символу перед разделителем
        }
        //Если символ - оператор
        if (IsOperator(InfixNotation[i])) //Если оператор
        {
            if (InfixNotation[i] == '(' ) //Если символ - открывающая скобка
                operStack.push(InfixNotation[i]); //Записываем её в стек
            else if (InfixNotation[i] == ')') //Если символ - закрывающая скобка
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
                    if (GetPriority(InfixNotation[i]) <= GetPriority(operStack.top())) //И если приоритет нашего оператора меньше или равен приоритету оператора на вершине стека
                        output = output + operStack.pop() + " "; //То добавляем последний оператор из стека в строку с выражением
                operStack.push(InfixNotation[i]); //Если стек пуст, или же приоритет оператора выше - добавляем операторов на вершину стека
            }
        }
    }
    //Когда прошли по всем символам, выкидываем из стека все оставшиеся там операторы в строку
    while (operStack.count() > 0)
        output = output + operStack.pop() + " ";
    return output; //Возвращаем выражение в постфиксной записи
}

float fast_notation::Computing(QString input, float variable_value)
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

void fast_notation::setFastNotation(QString InfixNotation)//тут в отличии от get notation заполняем структуру с числами что бы не парсить каждый раз строку( 5 раз выигрыш)
{
    QChar s;
    QStack<QChar> operStack; //Стек для хранения операторов
    if (InfixNotation.isEmpty())
        InfixNotation = "x * -1";// qDebug() << "Пустая строка!!! Данные помножены на минус";
    for (int i = 0; i < InfixNotation.length(); i++) //Для каждого символа в входной строке
    {
        //Разделители пропускаем
        if (IsDelimeter(InfixNotation[i]))
            continue; //Переходим к следующему символу
        if (InfixNotation[i].isDigit())  //Если символ - цифра, то считываем все число
        {
            QString interm{}; //промежуточные числа
            //Читаем до разделителя или оператора, что бы получить число
            while (!IsDelimeter(InfixNotation[i]) && !IsOperator(InfixNotation[i]))
            {
                interm += InfixNotation[i]; //Добавляем каждую цифру числа к нашей строке
                i++; //Переходим к следующему символу
                if (i == InfixNotation.length()) break; //Если символ - последний, то выходим из цикла
            }
            set_const(interm.toFloat());
            i--; //Возвращаемся на один символ назад, к символу перед разделителем
        }
        //Если символ - переменная, то считываем переменную
        if ( InfixNotation[i].isLetter() ) //Если буква
        {
            //Читаем до разделителя или оператора, что бы получить имя
            while (!IsDelimeter(InfixNotation[i]) && !IsOperator(InfixNotation[i]))
            {
                i++; //Переходим к следующему символу
                if (i == InfixNotation.length()) break; //Если символ - последний, то выходим из цикла
            }
            set_var();                //добавляю  вместо переменной что бы в векторе появился элемент
            i--; //Возвращаемся на один символ назад, к символу перед разделителем
        }
        //Если символ - оператор
        if (IsOperator(InfixNotation[i])) //Если оператор
        {
            if (InfixNotation[i] == '(' ) //Если символ - открывающая скобка
                operStack.push(InfixNotation[i]); //Записываем её в стек
            else if (InfixNotation[i] == ')') //Если символ - закрывающая скобка
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
                    if (GetPriority(InfixNotation[i]) <= GetPriority(operStack.top())) //И если приоритет нашего оператора меньше или равен приоритету оператора на вершине стека
                    {
                        s = operStack.pop();
                        set_op(s);
                    }
                operStack.push(InfixNotation[i]); //Если стек пуст, или же приоритет оператора выше - добавляем операторов на вершину стека
            }
        }
    }
    //Когда прошли по всем символам, выкидываем из стека все оставшиеся там операторы в строку
    while (operStack.count() > 0)
    {
        s = operStack.pop();
        set_op(s);
    }
    //return temp_fs;
}

bool fast_notation::IsDelimeter(QChar c) //Метод возвращает true, если проверяемый символ - разделитель ("пробел" или "равно")
{
    QString a = " =";
    if ((a.indexOf(c) != -1))
        return true;
    return false;
}

bool fast_notation::IsOperator(QChar c)  //Метод возвращает true, если проверяемый символ - оператор
{
    QString a = "+-/*^()";
    if (a.indexOf(c) != -1)
        return true;
    return false;
}
