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
        cout << "������ ��� �������� �����-�������" << endl;
        return false;
    }
    string line;
    vector<string> arr_str, arr_str_syn;
    while (getline(in, line))
    {
        arr_str = tokenizerDictStr(line);//���� ������������ ������ ��� ������� �������� � �����
        //� ���� �� ����� �� ������ �� arr_str ����� ���������� � ���
        line = arr_str[0];
        arr_str_syn.clear();
        for (int i = 1; i < arr_str.size(); ++i)
        {
            arr_str_syn.push_back(arr_str[i]);
        }
        //������ � ���� ���� line(����� ����� ������) � arr_str_syn(����� � ���� ����� ��� �������� � �������, ��� ���� � �������)
        //������� ��� ��� � ���
        this->dict_synonyms.insert(pair<string, vector<string>>(line, arr_str_syn));
    }
    in.close();
    return true;
}

bool Example::setMode()
{
    cout << "������� 1, ����� ������� �������������� �����" << endl;
    cout << "������� 2, ����� ������� ��������� �����" << endl;
    cout << "���� �����: ";
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
                || input_s[i] == ',' || input_s[i] == '!' || input_s[i] == '?')//���������� ���� �� ������������
            {
                result.push_back(temp);
                temp.clear();

                temp += input_s[i];
                result.push_back(temp);
                temp.clear();

                pred_c = input_s[i];
            }
            else//���������� �����
            {
                temp += input_s[i];
                result.push_back(temp);
                temp.clear();
                pred_c = input_s[i];
            }
        }
        else//��������� �����
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
        cout << "������ ��� �������� ����� � �������" << endl;
        return false;
    }
    //�������������� ��� �������� �������, � �������� ���� ��������
    tokens_input_file.clear();
    indexes_words.clear();

    string line;
    string inputString = "";
    //��������� ������-���������� �����
    while (getline(in, line))
    {
        inputString += line + "\n";
    }
    //���� ����������� �� ����� � �������-�����������
    this->tokens_input_file = tokenizer(inputString);
    //���� ������� ����
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
            cout << "������ ��� ������ � ���� � ������������ �������" << endl;
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
            cout << "������ ��� ������ � ���� � ������������ �������" << endl;
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
        cout << "������ ��� ������ � ���� ���������������� ������" << endl;
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
    //��������, ��� ����� ������� ����� ������ ������� ��������� readFile
    if ((this->tokens_input_file.size() == 0) || (this->indexes_words.size() == 0))
    {
        cout << "������. ���� ���� ������ ����-��������, ���� ������ �������� ����" << endl;
        return false;
    }

    //� ��� ������ ������� ��� �����, ������� �� ���� ������� � �������
    this->notFound = "";
    string temp_s;

    if (mode == 1)
    {
        for (int i = 0; i < this->indexes_words.size(); ++i)
        {
            temp_s = this->tokens_input_file[indexes_words[i]];
            //���� �� �� ����� ����(�.�. ��������������� ��������� �����)
            if (this->dict_synonyms.find(temp_s) == this->dict_synonyms.end())
            {
                //�� �������� ������ ���� ����� � ������� ��������� � ���� ��������� ����
                bool isFind = false;//����� ������������, ���� ������ ������ ��� �����
                for (auto& item : this->dict_synonyms)
                {
                    if (isFind)
                    {
                        break;
                    }
                    //������� �������� � ������� ������� item(����� ����������� ������� ����� � ������� std::find)
                    for (string& word : item.second)
                    {
                        //���� �� ����� ����� � ���������, �� ��� ���� ������� � ������� tokens_input_file
                        //�������� �� �����-������
                        if (word == temp_s)
                        {
                            this->tokens_input_file[indexes_words[i]] = item.first;
                            isFind = true;
                            break;
                        }
                    }
                }
                //���� �� ��� � �� ����� ���� ����� �����
                if (!isFind)
                {
                    this->notFound += temp_s + "\n";
                }
            }
            else//���� �� ����� ��������������� ����� ������, �� ������ ����������� �� ����
            {
                continue;
            }
        }

    }
    //���� �� � ��� �����-��������
    else if (mode == 2)
    {
        for (int i = 0; i < this->indexes_words.size(); ++i)
        {
            temp_s = this->tokens_input_file[indexes_words[i]];
            //���� �� �� ����� ����(�.�. ��������������� ��������� �����)
            if (this->dict_synonyms.find(temp_s) == this->dict_synonyms.end())
            {
                //�� �������� ������ ���� ����� � ������� ��������� � ���� ��������� ����
                bool isFind = false;//����� ������������, ���� ������ ������ ��� �����
                for (auto& item : this->dict_synonyms)
                {
                    if (isFind)
                    {
                        break;
                    }
                    //������� �������� � ������� ������� item
                    for (string& word : item.second)
                    {
                        //���� �� ����� ����� � ���������, �� ��� ���� ������� � ������� tokens_input_file
                        //�������� �� �����-������
                        if (word == temp_s)
                        {
                            this->tokens_input_file[indexes_words[i]] = item.first;
                            isFind = true;
                            break;
                        }
                    }
                }
                //���� �� ��� � �� ����� ���� ����� �����
                if (!isFind)
                {
                    cout << "����� " << temp_s << " �� ������� � �������" << endl;
                    cout << "1. �������� � ������ ��������� ���������� �����\n"
                        "2. ������� ����� ��������� �����\n"
                        "3. �������� ��������� �����\n";

                    cout << "���� �����: ";
                    int num;
                    cin >> num;
                    switch (num)
                    {
                    case 1:
                    {
                        string key;
                        cout << "������� ��������� �����: ";
                        cin >> key;
                        auto it = this->dict_synonyms.find(key);
                        if (it == this->dict_synonyms.end())
                        {
                            cout << "�� ������� ��������� �����";
                        }
                        else
                        {
                            it->second.push_back(temp_s);
                        }
                        break;
                    }
                    case 2:
                    {
                        //����������� ����� ������ ����� ���������
                        cout << "������: " << temp_s << endl << "������� �������: ";
                        string str;
                        cin >> str;
                        vector<string> temp_vector;
                        temp_vector.push_back(str);
                        this->dict_synonyms.insert(pair<string, vector<string>>(temp_s, temp_vector));
                        break;
                    }
                    case 3:
                    {
                        cout << "������� ������ �� ������: ";
                        string str;
                        cin >> str;
                        auto it = this->dict_synonyms.find(str);
                        if (it == this->dict_synonyms.end())
                        {
                            cout << "�� ������� ��������� �����";
                        }
                        else
                        {
                            pair<string, vector<string>> p(temp_s, it->second);
                            this->dict_synonyms.erase(str);
                            this->dict_synonyms.insert(p);
                            cout << "����������� ������ �� ����� : " << temp_s << endl;
                        }
                        break;
                    }
                    default:
                    {
                        cout << "������ ������ ��������" << endl;
                        //����� �� ����� ���������� � ����� ��������� � ���� � �����������
                        break;

                    }

                    }

                }
            }
            else//���� �� ����� ��������������� ����� ������, �� ������ ����������� �� ����
            {
                continue;
            }
        }
    }
    else
    {
        cout << "����� ������ �� �����" << endl;
        return false;
    }
    //������� ����� ������ � ���� outNotFind.txt � ����������� �� ������ ��������������� �����
    //���� �� ������ �� ����������� outNotFind.txt, ����� ������ �������� � �����
    if (!writeNotFound(n))
    {
        cout << "������ ��� ������ � ���� ����������� ����" << endl;
        return false;
    }
    //������� ���� � ��������������� �������
    if (!writeOut(n))
    {
        cout << "������ ��� ������ � ���� � ��������������� �������" << endl;
        return false;
    }
    return true;
}

bool Example::rewriteDict()
{
    ofstream rD("newdict.txt");
    if (!rD.is_open())
    {
        cout << "������ ��� ���������� �������" << endl;
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