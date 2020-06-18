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
    string notFound;//строка из слов, которые не нашли
    int mode;//режим работы программы

    //******************внутренний интерфейс
    vector<string> tokenizerDictStr(string);
    vector<string> tokenizer(string);
    vector<int> wordIndexing(vector<string>);
    bool writeNotFound(int);
    bool writeOut(int);
    //******************внешний интерфейс
    bool readDictSynonyms();
    bool setMode();
    bool readFile(string); //эта штука читает файл и делит его содержимое на отдельные элементы
    bool processing(int);
    bool rewriteDict();

};
