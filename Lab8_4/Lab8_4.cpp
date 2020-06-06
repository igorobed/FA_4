#pragma warning(disable : 4996)
#include <iostream>
#include <string>
#include <vector>
#include <json/json.h>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <time.h>
#include <locale.h>

using namespace std;

class Person
{
public:
    string name;//имя
    long id;//номер
    vector<long> friends_id;//номера друзей
    bool infected;//заражен или нет
    bool healed;//выздоровел или нет
    bool checked;//обработан
};

class Example
{
    unordered_map<long, Person> massive;//сюда загрузим json файл
    float pGetSick, pGetWell;//вероятности заболеть и выздороветь
    long firstInfected_id;//ключ первого инфицированного
public:
    long numbInfected;//число зараженных в данный момент
    bool setProbs();
    bool readJSON();
    void setFirstInfected();
    void waveInfections(Person&);
    void startWaveInfections();
    void waveHealing();
    //каждый метод ниже возвращает число человек, удовлетворяющих заданным требованиям и открывает файл output.txt на перезапись
    //чтобы записать туда имена и ключи найденных людей
    int printNotInfected();
    int printHealed();
    int printHealedSNH();
    int printNotInfectedSI();
};

int main()
{
    setlocale(LC_ALL, "rus");
    srand(time(NULL));
    Example e;
    if (!e.setProbs())//устанавливаем значения вероятностей заболеть и излечиться
    {
        cout << "Введено некорректное значение вероятности. Должно принадлежать [0..1]" << endl;
        return -2;
    }
    if (!e.readJSON())
    {
        cout << "Ошибка при считывании файла" << endl;
        return -2;
    }
    cout << "Число зараженных: " << e.numbInfected << endl;
    e.setFirstInfected();//заразить первого случайного человека, чтобы нвчать распространение болезни
    cout << "Число зараженных: " << e.numbInfected << endl;
    //запускаем волну заражений
    e.startWaveInfections();
    cout << "Число зараженных: " << e.numbInfected << endl;
    //запускаем волну выздоровлений
    e.waveHealing();
    cout << "Число зараженных: " << e.numbInfected << endl;

    int num;

    while (true)
    {
        cout << "1. Все не заразившиеся люди\n"
            "2. Все исцелившиеся\n"
            "3. Исцелившиеся люди, чье окружение не исцелилось\n"
            "4. Не заразившиеся люди, окружение которых заражено\n"
            "5. Выход\n";
        cout << "Введите комманду:\n";
        cin >> num;
        switch (num)
        {
        case 1:
            cout << "Не заразившиеся: " << e.printNotInfected() << endl;
            break;
        case 2:
            cout << "Исцелившиеся: " << e.printHealed() << endl;
            break;
        case 3:
            cout << "Исцелившиеся с больным окружением: " << e.printHealedSNH() << endl;
            break;
        case 4:
            cout << "Не заразившиеся с больным окружением: " << e.printNotInfectedSI() << endl;
            break;
        case 5:
            return 0;
            break;
        default:
            cout << "Ошибка при вводе номера комманды\n";
            break;
        }
    }
    return 0;
}

bool Example::setProbs()
{
    cout << "вероятность заболеть = "; //вероятность заболеть
    cin >> pGetSick;
    if ((pGetSick <= 0) || (pGetSick > 1))
        return false;
    cout << "вероятность излечиться = ";//вероятность излечиться
    cin >> pGetWell;
    if ((pGetWell < 0) || (pGetWell > 1))
        return false;
    this->numbInfected = 0;
    return true;
}

bool Example::readJSON()
{

    Json::Value humans;
    ifstream jsonFile("D:\\Учёба\\2 курс\\4 семестр\\фа\\лаба8.4(делаю)\\черновики\\Lab8_4Final\\Debug\\graph.json", ifstream::binary);
    if (!jsonFile.is_open())
    {
        return false;
    }
    Json::Reader reader;
    cout << "Идет чтение данных из файла" << endl;
    bool parsingSuccessful = reader.parse(jsonFile, humans, false);//что эта штука делает????
    jsonFile.close();
    if (!parsingSuccessful)
    {
        std::cout << reader.getFormatedErrorMessages() << endl;
        return false;
    }

    Json::Value::Members members = humans.getMemberNames();

    for (int i = 0; i < members.size(); i++)
    {
        Person h;
        h.name = humans[members[i]]["first_name"].asString() + " " + humans[members[i]]["last_name"].asString();
        h.id = stoi(members[i]);
        h.infected = false;
        h.healed = false;
        h.checked = false;
        //считываем друзей
        Json::Value friends_id = humans[members[i]]["friends_id"];
        for (int j = 0; j < friends_id.size(); j++)
        {
            h.friends_id.push_back(stol(friends_id[j].asString()));
        }
        massive.insert(pair<long, Person>(h.id, h));
    }
    cout << "Чтение данных из файла завершенно" << endl;
    return true;
}

void Example::setFirstInfected()
{
    int num = rand() % 1000;
    unordered_map<long, Person>::iterator it = this->massive.begin();
    while (num != 0)
    {
        it++;
        num--;
    }
    it->second.infected = true;//вот у нас и первый зараженный
    this->numbInfected += 1;
    this->firstInfected_id = it->first;//сохраняем ключ первого зараженного чела
}

void Example::waveInfections(Person& h)
{
    h.checked = true;//данный чел обработан
    for (long id : h.friends_id)//идем по идентификаторам друзей
    {
        int prob = rand() % 100 + 1;
        if (prob <= (this->pGetSick * 100))
        {
            unordered_map<long, Person>::iterator it = this->massive.find(id);
            if (it != this->massive.end() && !it->second.checked)//если элемент не последний и мы его еще не обрабатывали
            {
                it->second.infected = true;
                this->numbInfected += 1;
                waveInfections(it->second);
            } 
        }
    }
}

void Example::startWaveInfections()
{
    //работает до тех пор, пока все checked не станут равными true
    waveInfections((this->massive.find(this->firstInfected_id))->second);
}

void Example::waveHealing()
{
    int prob;
    for (unordered_map<long, Person>::iterator it = this->massive.begin(); it != this->massive.end(); it++)
    {
        if (it->second.infected == true)
        {
            prob = rand() % 100 + 1;
            if (prob <= (this->pGetWell * 100))
            {
                it->second.healed = true;
                it->second.infected = false;
                this->numbInfected -= 1;
            }
        }
    }
}

int Example::printNotInfected()
{
    int result = 0;
    ofstream out("output.txt");
    if (!out.is_open())
    {
        cout << "Ошибка при открытии выходного файла" << endl;
        return -1;
    }
    for (unordered_map<long, Person>::iterator it = this->massive.begin(); it != this->massive.end(); ++it)
    {
        if (it->second.infected == false && it->second.healed == false)
        {
            out << it->first << "   " << it->second.name << endl;//записываем в файл
            result += 1;
        }
    }
    out.close();
    return result;
}

int Example::printHealed()
{
    int result = 0;
    ofstream out("output.txt");
    if (!out.is_open())
    {
        cout << "Ошибка при открытии выходного файла" << endl;
        return -1;
    }
    for (unordered_map<long, Person>::iterator it = this->massive.begin(); it != this->massive.end(); ++it)
    {
        if (it->second.infected == false && it->second.healed == true)
        {
            out << it->first << "   " << it->second.name << endl;//записываем в файл
            result += 1;
        }
    }
    out.close();
    return result;
}

int Example::printHealedSNH()
{
    int result = 0;
    ofstream out("output.txt");
    if (!out.is_open())
    {
        cout << "Ошибка при открытии выходного файла" << endl;
        return -1;
    }
    bool all;
    for (unordered_map<long, Person>::iterator it = this->massive.begin(); it != this->massive.end(); ++it)
    {

        if (it->second.infected == false && it->second.healed == true)
        {
            all = true;
            for (auto i = it->second.friends_id.begin(); i != it->second.friends_id.end(); ++i)
            {
                if ((this->massive.find(*i))->second.healed == true)//если мы встречаем исцелившегося приятеля
                {
                    all = false;
                    break;
                }
            }
            if (all == true)
            {
                out << it->first << "   " << it->second.name << endl;//записываем в файл
                result += 1;
            }
        }
    }
    out.close();
    return result;
}

int Example::printNotInfectedSI()
{
    int result = 0;
    ofstream out("output.txt");
    if (!out.is_open())
    {
        cout << "Ошибка при открытии выходного файла" << endl;
        return -1;
    }
    bool all;
    for (unordered_map<long, Person>::iterator it = this->massive.begin(); it != this->massive.end(); ++it)
    {
        if (it->second.infected == false && it->second.healed == false)
        {
            all = true;
            for (auto i = it->second.friends_id.begin(); i != it->second.friends_id.end(); ++i)
            {
                if ((this->massive.find(*i))->second.infected == false)//если мы встретили не зараженного друга(не все зараженные)
                {
                    all = false;
                }
            }
            if (all == true)
            {
                out << it->first << "   " << it->second.name << endl;//записываем в файл
                result += 1;
            }
        }
    }
    out.close();
    return result;
}
