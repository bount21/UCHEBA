#define NOMINMAX
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <windows.h>
#include <io.h>
#include <fcntl.h>

using namespace std;

wstring utf8_to_wstring(const string &str)
{
    if (str.empty())
        return L"";
    int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    wstring result(size - 1, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &result[0], size);
    return result;
}

string wstring_to_utf8(const wstring &wstr)
{
    if (wstr.empty())
        return "";
    int size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    string result(size - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &result[0], size, nullptr, nullptr);
    return result;
}

struct Book
{
    wstring name;
    wstring author;
    wstring publisher;
    int year;
    double price;
};

vector<Book> books;
string filename;

bool fileExists(const string &path)
{
    ifstream f(path);
    return f.is_open();
}

void loadBooks()
{
    if (!fileExists(filename))
    {
        ofstream out(filename, ios::binary);
        vector<Book> test = {
            {L"Война и мир", L"Толстой Л.Н.", L"Эксмо", 2020, 850},
            {L"1984", L"Оруэлл Дж.", L"АСТ", 2019, 620},
            {L"Мастер и Маргарита", L"Булгаков М.А.", L"Азбука", 2021, 720}};

        for (auto &b : test)
        {
            out << wstring_to_utf8(b.name) << ";"
                << wstring_to_utf8(b.author) << ";"
                << wstring_to_utf8(b.publisher) << ";"
                << b.year << ";"
                << b.price << "\n";
        }
        books = test;
        return;
    }

    ifstream in(filename);
    string line;

    while (getline(in, line))
    {
        vector<string> parts;
        size_t pos;
        while ((pos = line.find(';')) != string::npos)
        {
            parts.push_back(line.substr(0, pos));
            line.erase(0, pos + 1);
        }
        parts.push_back(line);

        if (parts.size() == 5)
        {
            Book b;
            b.name = utf8_to_wstring(parts[0]);
            b.author = utf8_to_wstring(parts[1]);
            b.publisher = utf8_to_wstring(parts[2]);
            b.year = stoi(parts[3]);
            b.price = stod(parts[4]);
            books.push_back(b);
        }
    }
}

void saveBooks()
{
    ofstream out(filename, ios::binary);
    for (auto &b : books)
    {
        out << wstring_to_utf8(b.name) << ";"
            << wstring_to_utf8(b.author) << ";"
            << wstring_to_utf8(b.publisher) << ";"
            << b.year << ";"
            << b.price << "\n";
    }
}

void showBooks(const vector<Book> &list)
{
    wcout << left
          << setw(4) << L"№"
          << setw(30) << L"Название"
          << setw(20) << L"Автор"
          << setw(15) << L"Изд-во"
          << setw(6) << L"Год"
          << L"Цена\n";

    wcout << wstring(85, L'-') << L"\n";

    for (size_t i = 0; i < list.size(); ++i)
    {
        const auto &b = list[i];
        wcout << setw(4) << i + 1
              << setw(30) << b.name
              << setw(20) << b.author
              << setw(15) << b.publisher
              << setw(6) << b.year
              << b.price << L"\n";
    }
}

void filterByPrice()
{
    double min, max;
    wcout << L"Мин цена: ";
    wcin >> min;
    wcout << L"Макс цена: ";
    wcin >> max;

    vector<Book> result;
    for (auto &b : books)
        if (b.price >= min && b.price <= max)
            result.push_back(b);

    showBooks(result);
}

void addBook()
{
    Book b;
    wcin.ignore();
    wcout << L"Название: ";
    getline(wcin, b.name);
    wcout << L"Автор: ";
    getline(wcin, b.author);
    wcout << L"Издательство: ";
    getline(wcin, b.publisher);
    while (true)
    {
        wcout << L"Цена: ";
        if (wcin >> b.price && b.price >= 0)
        {
            break;
        }
        else
        {
            wcout << L"Ошибка! Введите неотрицательное число.\n";
            wcin.clear();
            wcin.ignore(1000, L'\n');
        }
    }

    while (true)
    {
        wcout << L"Год: ";
        if (wcin >> b.year && b.year >= 0)
        {
            break;
        }
        else
        {
            wcout << L"Ошибка! Введите корректный год.\n";
            wcin.clear();
            wcin.ignore(1000, L'\n');
        }
    }

    books.push_back(b);
    saveBooks();
}

void deleteBook()
{
    showBooks(books);
    wcout << L"Номер: ";
    int n;
    wcin >> n;

    if (n >= 1 && n <= (int)books.size())
    {
        books.erase(books.begin() + n - 1);
        saveBooks();
    }
}

void sortBooks()
{
    wcout << L"1-Название 2-Автор 3-Цена 4-Издательство 5-Год: ";
    int c;
    wcin >> c;

    if (c == 1)
        sort(books.begin(), books.end(), [](auto &a, auto &b)
             { return a.name < b.name; });
    if (c == 2)
        sort(books.begin(), books.end(), [](auto &a, auto &b)
             { return a.author < b.author; });
    if (c == 3)
        sort(books.begin(), books.end(), [](auto &a, auto &b)
             { return a.price < b.price; });
    if (c == 4)
        sort(books.begin(), books.end(), [](auto &a, auto &b)
             { return a.publisher < b.publisher; });
    if (c == 5)
        sort(books.begin(), books.end(), [](auto &a, auto &b)
             { return a.year < b.year; });

    saveBooks();
}

int main()
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);

    wcout << L"Введите имя файла: ";
    wstring wfname;
    getline(wcin, wfname);

    filename = wstring_to_utf8(wfname);

    loadBooks();

    int choice;
    do
    {
        wcout << L"\n1.Показать\n2.Фильтр\n3.Добавить\n4.Удалить\n5.Сортировать\n6.Выход\n>";
        wcin >> choice;

        switch (choice)
        {
        case 1:
            showBooks(books);
            break;
        case 2:
            filterByPrice();
            break;
        case 3:
            addBook();
            break;
        case 4:
            deleteBook();
            break;
        case 5:
            sortBooks();
            break;
        }
    } while (choice != 6);

    system("pause");
    return 0;
}
