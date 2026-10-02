#define NOMINMAX
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <limits>
#include <windows.h>
#include <io.h>
#include <fcntl.h>

using namespace std;

// ===== UTF-8 <-> UTF-16 =====
wstring utf8_to_wstring(const string &s)
{
    if (s.empty())
        return L"";
    int size = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
    wstring result(size - 1, 0);
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, &result[0], size);
    return result;
}

string wstring_to_utf8(const wstring &s)
{
    if (s.empty())
        return "";
    int size = WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, nullptr, 0, nullptr, nullptr);
    string result(size - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, &result[0], size, nullptr, nullptr);
    return result;
}

// ===== Структура =====
struct Uchenik
{
    wstring name;
    vector<int> grades;
};

vector<Uchenik> students;
string filename;

// ===== Проверки =====
bool isValidName(const wstring &s) { return !s.empty(); }
bool isValidGrade(int g) { return g >= 1 && g <= 5; }

// ===== Загрузка =====
void loadFromFile()
{
    ifstream file(filename);
    if (!file)
    {
        wcout << L"Файл не найден, будет создан новый.\n";
        return;
    }

    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string word;
        Uchenik s;
        bool nameDone = false;

        while (ss >> word)
        {
            if (!nameDone && !isdigit((unsigned char)word[0]))
            {
                if (!s.name.empty())
                    s.name += L" ";
                s.name += utf8_to_wstring(word);
            }
            else
            {
                nameDone = true;
                int g = stoi(word);
                if (isValidGrade(g))
                    s.grades.push_back(g);
            }
        }
        if (!s.name.empty())
            students.push_back(s);
    }
}

// ===== Сохранение =====
void saveToFile()
{
    ofstream file(filename);
    for (auto &s : students)
    {
        file << wstring_to_utf8(s.name);
        for (int g : s.grades)
            file << " " << g;
        file << "\n";
    }
    wcout << L"Сохранено.\n";
}
bool isValidFIO(const wstring &fio)
{
    int spaces = 0;
    for (wchar_t c : fio)
        if (c == L' ')
            spaces++;

    return spaces == 2;
}

// ===== Добавить ученика =====
void addStudent()
{
    {
        Uchenik s;
        wcout << L"Введите Ф.И.О. (Фамилия И. О.): ";
        getline(wcin, s.name);

        if (!isValidFIO(s.name))
        {
            wcout << L"Ошибка! Формат: Фамилия И. О.\n";
            return;
        }

        students.push_back(s);
        wcout << L"Ученик добавлен.\n";
    }
}

// ===== Добавить оценки =====
void addGrades()
{
    int idx;
    wcout << L"Индекс ученика: ";
    wcin >> idx;
    wcin.ignore(numeric_limits<streamsize>::max(), L'\n');

    if (idx < 0 || idx >= (int)students.size())
    {
        wcout << L"Неверный индекс.\n";
        return;
    }

    wcout << L"Введите оценки через пробел: ";
    wstring line;
    getline(wcin, line);
    wstringstream ss(line);

    int g;
    while (ss >> g)
    {
        if (isValidGrade(g))
            students[idx].grades.push_back(g);
        else
            wcout << L"Неверная оценка: " << g << L"\n";
    }
}

// ===== Удалить ученика =====
void deleteStudent()
{
    int idx;
    wcout << L"Индекс ученика: ";
    wcin >> idx;

    if (idx < 0 || idx >= (int)students.size())
    {
        wcout << L"Неверный индекс.\n";
        return;
    }
    students.erase(students.begin() + idx);
    wcout << L"Ученик удален.\n";
}

// ===== Удалить оценки =====
void deleteGrades()
{
    int idx;
    wcout << L"Индекс ученика: ";
    wcin >> idx;

    if (idx < 0 || idx >= (int)students.size())
    {
        wcout << L"Неверный индекс.\n";
        return;
    }
    students[idx].grades.clear();
    wcout << L"Оценки удалены.\n";
}

// ===== Изменить оценку =====
void changeGrade()
{
    int idx, gidx, newg;
    wcout << L"Индекс ученика: ";
    wcin >> idx;
    wcout << L"Индекс оценки: ";
    wcin >> gidx;
    wcout << L"Новая оценка: ";
    wcin >> newg;

    if (idx < 0 || idx >= (int)students.size() ||
        gidx < 0 || gidx >= (int)students[idx].grades.size() ||
        !isValidGrade(newg))
    {
        wcout << L"Ошибка ввода.\n";
        return;
    }
    students[idx].grades[gidx] = newg;
    wcout << L"Оценка изменена.\n";
}

// ===== Средний балл =====
void calculateAverage()
{
    for (auto &s : students)
    {
        if (s.grades.empty())
        {
            wcout << s.name << L": нет оценок\n";
            continue;
        }
        double sum = 0;
        for (int g : s.grades)
            sum += g;
        wcout << s.name << L": " << sum / s.grades.size() << L"\n";
    }
}

// ===== Вывод =====
void displayStudents()
{
    for (size_t i = 0; i < students.size(); ++i)
    {
        wcout << i << L": " << students[i].name << L" | ";
        for (int g : students[i].grades)
            wcout << g << L" ";
        wcout << L"\n";
    }
}

// ===== MAIN =====
int main()
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);

    wstring wfilename;
    wcout << L"Введите имя файла: ";
    getline(wcin, wfilename);

    if (wfilename.empty())
        filename = "ocenki.txt";
    else
        filename = wstring_to_utf8(wfilename);

    loadFromFile();

    int choice;
    do
    {
        wcout << L"\n1.Добавить ученика\n2.Добавить оценки\n3.Удалить ученика\n"
              << L"4.Удалить оценки\n5.Изменить оценку\n6.Средний балл\n"
              << L"7.Показать всех\n8.Сохранить\n0.Выход\n> ";
        wcin >> choice;
        wcin.ignore(numeric_limits<streamsize>::max(), L'\n');

        switch (choice)
        {
        case 1:
            addStudent();
            break;
        case 2:
            addGrades();
            break;
        case 3:
            deleteStudent();
            break;
        case 4:
            deleteGrades();
            break;
        case 5:
            changeGrade();
            break;
        case 6:
            calculateAverage();
            break;
        case 7:
            displayStudents();
            break;
        case 8:
            saveToFile();
            break;
        }
    } while (choice != 0);

    wcout << L"Выход из программы.\n";
    system("pause");
    return 0;
}
