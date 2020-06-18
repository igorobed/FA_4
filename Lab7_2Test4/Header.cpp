#pragma once
#include "Header.h"

vector<string> Example::tokenizerDictStr(string str)
{
    vector<string> result;
    string buf;
    for (int i = 0; i < str.size(); ++i)
    {
        if (str[i] == '<' || str[i] == '{')
        {
            continue;
        }
        else if (str[i] == '>')
        {
            result.push_back(buf);
            buf.clear();
        }
        else if (str[i] == ' ')
        {
            if (buf.size() != 0)
            {
                result.push_back(buf);
                buf.clear();
            }
            else
            {
                continue;
            }
        }
        else if (str[i] == '}')
        {
            result.push_back(buf);
            buf.clear();
            break;
        }
        else
        {
            buf += str[i];
        }
    }
    return result;
}

bool Example::readDictSynonyms()
{
    ifstream in("newdict.txt");
    if (!in.is_open())
    {
        cout << "Ошибка при открытии файла-словаря" << endl;
        return false;
    }
    string line;
    vector<string> arr_str, arr_str_syn;
    while (getline(in, line))
    {
        arr_str = tokenizerDictStr(line);//туда запихивается строка без символа перехода в конце
        //в этом же цикле мы строки из arr_str будем запихивать в мап
        line = arr_str[0];
        arr_str_syn.clear();
        for (int i = 1; i < arr_str.size(); ++i)
        {
            arr_str_syn.push_back(arr_str[i]);
        }
        //теперь у меня есть line(здесь лежит эталон) и arr_str_syn(здесь у меня лежат все синонимы к эталону, что были в словаре)
        //засунем это все в мап
        this->dict_synonyms.insert(pair<string, vector<string>>(line, arr_str_syn));
    }
    in.close();
    return true;
}

bool Example::setMode()
{
    cout << "Введите 1, чтобы выбрать автоматический режим" << endl;
    cout << "Введите 2, чтобы выбрать обучающий режим" << endl;
    cout << "Поле ввода: ";
    cin >> this->mode;
    if (this->mode == 1 || this->mode == 2)
    {
        return true;
    }
    else
    {
        return false;
    }
}

vector<string> Example::tokenizer(string input_s)
{
    vector<string> result;
    string temp;
    char pred_c;
    if (input_s == "")
        return result;
    for (int i = 0; i < input_s.size(); i++)
    {
        if (input_s[i] == ' ' || input_s[i] == '\t' || input_s[i] == '\n' || input_s[i] == 0 || i == input_s.size()
            || input_s[i] == '.' || input_s[i] == ',' || input_s[i] == '!' || input_s[i] == '?')
        {
            if (pred_c != ' ' && pred_c != '\t' && pred_c != '\n' || input_s[i] == '.'
                || input_s[i] == ',' || input_s[i] == '!' || input_s[i] == '?')//записываем один из разделителей
            {
                result.push_back(temp);
                temp.clear();

                temp += input_s[i];
                result.push_back(temp);
                temp.clear();

                pred_c = input_s[i];
            }
            else//записываем слово
            {
                temp += input_s[i];
                result.push_back(temp);
                temp.clear();
                pred_c = input_s[i];
            }
        }
        else//формируем слово
        {
            pred_c = input_s[i];
            temp += input_s[i];
        }
    }
    return result;
}

vector<int> Example::wordIndexing(vector<string> mass_s)
{
    vector<int> index;
    for (int i = 0; i < mass_s.size(); i++)
    {
        if ((mass_s[i][0] != ' ') && (mass_s[i][0] != '\t') && (mass_s[i][0] != '\n') && (mass_s[i][0] != 0)
            && mass_s[i][0] != '.' && mass_s[i][0] != ',' && mass_s[i][0] != '!' && mass_s[i][0] != '?' && (mass_s[i].size() > 1)
            && (mass_s[i][0] > '9' || mass_s[i][0] < '0'))
            index.push_back(i);
    }
    return index;
}

bool Example::readFile(string nameFile)
{
    ifstream in(nameFile);
    if (!in.is_open())
    {
        cout << "Ошибка при открытии файла с текстом" << endl;
        return false;
    }
    //предварительно над очистить вектора, с которыми хочу работать
    tokens_input_file.clear();
    indexes_words.clear();

    string line;
    string inputString = "";
    //формируем строку-содержимое файла
    while (getline(in, line))
    {
        inputString += line + "\n";
    }
    //файл разбивается на слова и символы-разделители
    this->tokens_input_file = tokenizer(inputString);
    //ищем индексы слов
    this->indexes_words = wordIndexing(tokens_input_file);
    in.close();
    return true;
}

bool Example::writeNotFound(int n)
{
    if (n == 1)
    {
        ofstream nF("notFound.txt");
        if (nF.is_open())
        {
            nF << this->notFound;
        }
        else
        {
            cout << "Ошибка при записи в файл с ненайденными словами" << endl;
            return false;
        }
        nF.close();
    }
    else
    {
        ofstream nF;
        nF.open("notFound.txt", std::ios::app);
        if (nF.is_open())
        {
            nF << this->notFound;
        }
        else
        {
            cout << "Ошибка при записи в файл с ненайденными словами" << endl;
            return false;
        }
        nF.close();
    }
    return true;
}

bool Example::writeOut(int n)
{
    string name = "out_" + to_string(n) + ".txt";
    ofstream outF(name);
    if (!outF.is_open())
    {
        cout << "Ошибка при записи в файл преобразованного текста" << endl;
        return false;
    }

    for (string& item : this->tokens_input_file)
    {
        outF << item;
    }

    outF.close();
    return true;
}

bool Example::processing(int n)
{
    //проверка, что перед вызовом этого метода успешно отработал readFile
    if ((this->tokens_input_file.size() == 0) || (this->indexes_words.size() == 0))
    {
        cout << "Ошибка. Пуст либо вектор слов-символов, либо вектор индексов слов" << endl;
        return false;
    }

    //в эту строку запишем все слова, которые не были найдены в словаре
    this->notFound = "";
    string temp_s;

    if (mode == 1)
    {
        for (int i = 0; i < this->indexes_words.size(); ++i)
        {
            temp_s = this->tokens_input_file[indexes_words[i]];
            //если мы не нашли ключ(т.е. соответствующее эталонное слово)
            if (this->dict_synonyms.find(temp_s) == this->dict_synonyms.end())
            {
                //мы начинаем искать наше слово в списках синонимов у всех эталонных слов
                bool isFind = false;//чтобы остановиться, если найдем нужное нам слово
                for (auto& item : this->dict_synonyms)
                {
                    if (isFind)
                    {
                        break;
                    }
                    //смотрим синонимы у каждого эталона item(можно попробовать сделать поиск с помощью std::find)
                    for (string& word : item.second)
                    {
                        //если мы нашли слово в синонимах, то над этот элемент в векторе tokens_input_file
                        //изменить на слово-эталон
                        if (word == temp_s)
                        {
                            this->tokens_input_file[indexes_words[i]] = item.first;
                            isFind = true;
                            break;
                        }
                    }
                }
                //если мы так и не нашли наше слово нигде
                if (!isFind)
                {
                    this->notFound += temp_s + "\n";
                }
            }
            else//если мы нашли соответствующее слово эталон, то замену производить не надо
            {
                continue;
            }
        }

    }
    //если же у нас режим-обучение
    else if (mode == 2)
    {
        for (int i = 0; i < this->indexes_words.size(); ++i)
        {
            temp_s = this->tokens_input_file[indexes_words[i]];
            //если мы не нашли ключ(т.е. соответствующее эталонное слово)
            if (this->dict_synonyms.find(temp_s) == this->dict_synonyms.end())
            {
                //мы начинаем искать наше слово в списках синонимов у всех эталонных слов
                bool isFind = false;//чтобы остановиться, если найдем нужное нам слово
                for (auto& item : this->dict_synonyms)
                {
                    if (isFind)
                    {
                        break;
                    }
                    //смотрим синонимы у каждого эталона item
                    for (string& word : item.second)
                    {
                        //если мы нашли слово в синонимах, то над этот элемент в векторе tokens_input_file
                        //изменить на слово-эталон
                        if (word == temp_s)
                        {
                            this->tokens_input_file[indexes_words[i]] = item.first;
                            isFind = true;
                            break;
                        }
                    }
                }
                //если мы так и не нашли наше слово нигде
                if (!isFind)
                {
                    cout << "Слово " << temp_s << " не найдено в словаре" << endl;
                    cout << "1. Добавить к списку синонимов эталонного слова\n"
                        "2. Создать новое эталонное слово\n"
                        "3. Заменить эталонное слово\n";

                    cout << "Поле ввода: ";
                    int num;
                    cin >> num;
                    switch (num)
                    {
                    case 1:
                    {
                        string key;
                        cout << "Введите эталонное слово: ";
                        cin >> key;
                        auto it = this->dict_synonyms.find(key);
                        if (it == this->dict_synonyms.end())
                        {
                            cout << "Не найдено эталонное слово";
                        }
                        else
                        {
                            it->second.push_back(temp_s);
                        }
                        break;
                    }
                    case 2:
                    {
                        //ненайденное слово станет новым эталонным
                        cout << "Эталон: " << temp_s << endl << "Введите синоним: ";
                        string str;
                        cin >> str;
                        vector<string> temp_vector;
                        temp_vector.push_back(str);
                        this->dict_synonyms.insert(pair<string, vector<string>>(temp_s, temp_vector));
                        break;
                    }
                    case 3:
                    {
                        cout << "Введите эталон на замену: ";
                        string str;
                        cin >> str;
                        auto it = this->dict_synonyms.find(str);
                        if (it == this->dict_synonyms.end())
                        {
                            cout << "Не найдено эталонное слово";
                        }
                        else
                        {
                            pair<string, vector<string>> p(temp_s, it->second);
                            this->dict_synonyms.erase(str);
                            this->dict_synonyms.insert(p);
                            cout << "Произведена замена на слово : " << temp_s << endl;
                        }
                        break;
                    }
                    default:
                    {
                        cout << "Ошибка выбора действия" << endl;
                        //слово не будет обработано и прост добавится в файл к ненайденным
                        break;

                    }

                    }

                }
            }
            else//если мы нашли соответствующее слово эталон, то замену производить не надо
            {
                continue;
            }
        }
    }
    else
    {
        cout << "Режим работы не задан" << endl;
        return false;
    }
    //сделать здесь запись в файл outNotFind.txt в зависимости от номера обрабатываемого файла
    //если он первый то пересоздаем outNotFind.txt, иначе делаем дозапись в конец
    if (!writeNotFound(n))
    {
        cout << "Ошибка при записи в файл ненайденных слов" << endl;
        return false;
    }
    //создает файл с преобразованным текстом
    if (!writeOut(n))
    {
        cout << "Ошибка при записи в файл с преобразованным текстом" << endl;
        return false;
    }
    return true;
}

bool Example::rewriteDict()
{
    ofstream rD("newdict.txt");
    if (!rD.is_open())
    {
        cout << "Ошибка при перезаписи словаря" << endl;
        return false;
    }
    string temp_s;
    for (auto& item : this->dict_synonyms)
    {
        temp_s = "<" + item.first + "> {";
        for (int i = 0; i < item.second.size(); ++i)
        {
            if (i != (item.second.size() - 1))
            {
                temp_s += item.second[i] + " ";
            }
            else
            {
                temp_s += item.second[i];
            }
        }
        temp_s += "}\n";
        rD << temp_s;
        cout << temp_s;
    }

    return true;
}