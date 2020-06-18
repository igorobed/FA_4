#pragma once
#include <iostream>
#include <locale.h>
#include <fstream>
#include <vector>
#include <map>
#include <string>

using namespace std;

class Example
{
public:
    map<string, vector<string>> dict_synonyms;
    vector<string> tokens_input_file;
    vector<int> indexes_words;
    string notFound;//������ �� ����, ������� �� �����
    int mode;//����� ������ ���������

    //******************���������� ���������
    vector<string> tokenizerDictStr(string);
    vector<string> tokenizer(string);
    vector<int> wordIndexing(vector<string>);
    bool writeNotFound(int);
    bool writeOut(int);
    //******************������� ���������
    bool readDictSynonyms();
    bool setMode();
    bool readFile(string); //��� ����� ������ ���� � ����� ��� ���������� �� ��������� ��������
    bool processing(int);
    bool rewriteDict();

};
