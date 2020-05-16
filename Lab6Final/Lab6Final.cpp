#include <iostream>
#include <fstream>
#include <string>
#include <locale.h>
#include <vector>

using namespace std;

vector<string> tokenizer(string);
vector<int> wordIndexing(vector<string>);
int compare(string, string);
int numberSyllables(string);
string wordFromLine(string);
string findOtherWord(string);


int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "rus");
    string line;
    string inputString;

    if (argc != 3)
    {
        cout << "Введены не все аргументы" << endl;
        exit(-5);
    }

    //считываем весь входной файл(кодировка ANSI, Windows-1251) в одну строку
    //ifstream in("D:\\Учёба\\2 курс\\4 семестр\\фа\\лаба6\\Lab6Final\\input.txt");
    ifstream in(argv[1]);
    if (in.is_open())
    {
        inputString = "";
        while (getline(in, line))
        {
            inputString += line + "\n";
        }
    }
    else
    {
        cout << "Ошибка чтения файла\n";
        return -2;
    }
    in.close();

    //токенизируем исходный текст
    vector<string> splitInputString = tokenizer(inputString);

    //находим индексы слов
    vector<int> wordIndex = wordIndexing(splitInputString);

    for (int i = 0; i < wordIndex.size(); i++)
    {
        cout << "Исходное слово: " << splitInputString[wordIndex[i]] << endl;
        splitInputString[wordIndex[i]] = findOtherWord(splitInputString[wordIndex[i]]);
        cout << "Слово-замена: " << splitInputString[wordIndex[i]] << endl;
    }

    //сформируем единую строку и запишем ее в файл
    line = "";
    for (int i = 0; i < splitInputString.size(); i++)
    {
        line += splitInputString[i];
    }

    cout << line << endl;
    //ofstream out("D:\\Учёба\\2 курс\\4 семестр\\фа\\лаба6\\Lab6Final\\output.txt");
    ofstream out(argv[2]);
    if (out.is_open())
    {
        out << line;
    }
    else
    {
        cout << "Ошибка открытия файла вывода\n";
        return -3;
    }
    out.close();
    return 0;
}

vector<string> tokenizer(string input_s)
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

vector<int> wordIndexing(vector<string> mass_s)
{
    vector<int> index;
    for (int i = 0; i < mass_s.size(); i++)
    {
        if ((mass_s[i][0] != ' ') && (mass_s[i][0] != '\t') && (mass_s[i][0] != '\n') && (mass_s[i][0] != 0) 
            && mass_s[i][0] != '.' && mass_s[i][0] != ',' && mass_s[i][0] != '!' && mass_s[i][0] != '?')
            index.push_back(i);
    }
    return index;
}

int compare(string temp_s1, string temp_s2)
{
    int comp_value = 0;
    if ((temp_s1.size() == 0) && (temp_s2.size() == 0))
    {
        return -1;
    }
    else if ((temp_s1.size() == 0) || (temp_s2.size() == 0))
    {
        return 0;
    }
    else if (temp_s1.size() == temp_s2.size())
    {
        string::iterator it1 = temp_s1.end() - 1;
        string::iterator it2 = temp_s2.end() - 1;

        for (; it1 != temp_s1.begin(); --it1, --it2)
        {
            if (*it1 == *it2)
            {
                comp_value += 1;
                if (comp_value == (temp_s1.size() - 1))
                {
                    if (*(it1 - 1) == *(it2 - 1))
                        return -1;
                }
            }
            else
            {
                return comp_value;
            }
        }
    }
    else if (temp_s1.size() > temp_s2.size())
    {
        string::iterator it1 = temp_s1.end() - 1;
        string::iterator it2 = temp_s2.end() - 1;
        for (; it2 != temp_s2.begin(); --it2, --it1)
        {
            if (*it1 == *it2)
            {
                comp_value += 1;
                if (comp_value == (temp_s2.size() - 1))
                {
                    if (*(it1 - 1) == *(it2 - 1))
                        return -1;
                }
            }
            else
            {
                return comp_value;
            }
        }
    }
    else
    {
        string::iterator it1 = temp_s1.end() - 1;
        string::iterator it2 = temp_s2.end() - 1;
        for (; it1 != temp_s1.begin(); --it1, --it2)
        {
            if (*it1 == *it2)
            {
                comp_value += 1;
                if (comp_value == (temp_s1.size() - 1))
                {
                    if (*(it1 - 1) == *(it2 - 1))
                        return -1;
                }

            }
            else
            {
                return comp_value;
            }
        }
    }
    return ((comp_value == temp_s1.size()) && (comp_value == temp_s2.size())) ? -1 : comp_value;
}

int numberSyllables(string temp_s)
{
    int numb = 0;

    string vowel = "ёуеыаоэяиюЁУЕЫАОЭЯИЮ";
    for (size_t i = 0; i < temp_s.size(); i++)
    {
        for (size_t j = 0; j < vowel.size(); j++)
        {
            if (temp_s[i] == vowel[j])
            {
                numb++;
                break;
            }
        }
    }
    return numb;
}

string wordFromLine(string input_s)//для считывания слов из словаря
{
    string result = "";
    if (input_s == "")
        return result;
    for (int i = 0; i < input_s.size(); i++)
    {
        if (input_s[i] == 0 || input_s[i] == '\n')
            break;
        else if (input_s[i] == ' ' || input_s[i] == '\t')
            continue;
        else
            result += input_s[i];
    }
    return result;
}

string findOtherWord(string input_s)//может искать некорректно если подаю строки с буквами в верхнем регистре
{
    int nSyllablesInput = numberSyllables(input_s); 
    int max = 0;
    string line;
    string result = input_s;
    int num = result.size() / 2 + 1;
    if (result.size() <= 2)
        return result;
    ifstream inDict("zdb-win.txt");
    switch (result[result.size() - 1])
    {
    
    case 'б':
        for (int i = 0; i < 10895; i++)
            getline(inDict, line);
        break;
    case 'в':
        for (int i = 0; i < 11052; i++)
            getline(inDict, line);
        break;
    case 'г':
        for (int i = 0; i < 11408; i++)
            getline(inDict, line);
        break;
    case 'д':
        for (int i = 0; i < 11848; i++)
            getline(inDict, line);
        break;
    case 'е':
        for (int i = 0; i < 12610; i++)
            getline(inDict, line);
        break;
    case 'ё':
        for (int i = 0; i < 17237; i++)
            getline(inDict, line);
        break;
    case 'ж':
        for (int i = 0; i < 17311; i++)
            getline(inDict, line);
        break;
    case 'з':
        for (int i = 0; i < 17493; i++)
            getline(inDict, line);
        break;
    case 'и':
        for (int i = 0; i < 17956; i++)
            getline(inDict, line);
        break;
    case 'й':
        for (int i = 0; i < 18952; i++)
            getline(inDict, line);
        break;
    case 'к':
        for (int i = 0; i < 39808; i++)
            getline(inDict, line);
        break;
    case 'л':
        for (int i = 0; i < 45102; i++)
            getline(inDict, line);
        break;
    case 'м':
        for (int i = 0; i < 45725; i++)
            getline(inDict, line);
        break;
    case 'н':
        for (int i = 0; i < 46909; i++)
            getline(inDict, line);
        break;
    case 'о':
        for (int i = 0; i < 48675; i++)
            getline(inDict, line);
        break;
    case 'п':
        for (int i = 0; i < 50810; i++)
            getline(inDict, line);
        break;
    case 'р':
        for (int i = 0; i < 51090; i++)
            getline(inDict, line);
        break;
    case 'с':
        for (int i = 0; i < 53086; i++)
            getline(inDict, line);
        break;
    case 'т':
        for (int i = 0; i < 53879; i++)
            getline(inDict, line);
        break;
    case 'у':
        for (int i = 0; i < 56610; i++)
            getline(inDict, line);
        break;
    case 'ф':
        for (int i = 0; i < 56824; i++)
            getline(inDict, line);
        break;
    case 'х':
        for (int i = 0; i < 57002; i++)
            getline(inDict, line);
        break;
    case 'ц':
        for (int i = 0; i < 57190; i++)
            getline(inDict, line);
        break;
    case 'ч':
        for (int i = 0; i < 58042; i++)
            getline(inDict, line);
        break;
    case 'ш':
        for (int i = 0; i < 58174; i++)
            getline(inDict, line);
        break;
    case 'щ':
        for (int i = 0; i < 58313; i++)
            getline(inDict, line);
        break;
    case 'ы':
        for (int i = 0; i < 58329; i++)
            getline(inDict, line);
        break;
    case 'ь':
        for (int i = 0; i < 58645; i++)
            getline(inDict, line);
        break;
    case 'э':
        for (int i = 0; i < 82448; i++)
            getline(inDict, line);
        break;
    case 'ю':
        for (int i = 0; i < 82452; i++)
            getline(inDict, line);
        break;
    case 'я':
        for (int i = 0; i < 82518; i++)
            getline(inDict, line);
        break;
    default:
        break;
    }
    
    //ifstream inDict("D:\\Учёба\\2 курс\\4 семестр\\фа\\лаба6\\Lab6Final\\zdb-win.txt");
    
    if (!inDict.is_open())
    {
        cout << "Ошибка открытия словаря\n";
        return 0;
    }
    while (getline(inDict, line))
    {
        line = wordFromLine(line);
        if (line.find_first_of("-", 0) != string::npos)
            continue;
        if (nSyllablesInput == numberSyllables(line))
        {
            if (compare(input_s, line) > max)
            {
                max = compare(input_s, line);
                result = line;
                if (max >= num)
                    break;
            }
        }
    }

    inDict.close();
    return result;
}
