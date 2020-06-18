#include "Header.h"

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "rus");
    if (argc == 1)
    {
        cout << "Отсутствуют текстовые файлы для обработки" << endl;
        return -2;
    }

    Example ex;
    if (!ex.readDictSynonyms())
    {
        return -2;
    }
    if (!ex.setMode())
    {
        cout << "Введено некорректное значение в поле выбора режима работы" << endl;
        return -3;
    }

    for (int i = 1; i < argc; i++)//поочередно обрабатываем каждый файл
    {
        if (!ex.readFile(argv[i]))
        {
            cout << "Ошибка при открытии файла №" << i << endl;
            return -4;
        }
        if (!ex.processing(i))
        {
            cout << "Ошибка при обработке файла №" << i << endl;
            return -5;
        }
    }

    if (!ex.rewriteDict())
    {
        return -6;
    }

    return 0;
}


