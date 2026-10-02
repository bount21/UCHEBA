#include <iostream>
#include <fcntl.h>
#include <vector>

using namespace std;

vector<wchar_t> splitDigits(const wstring &line)
{
    vector<wchar_t> digits;

    for (wchar_t ch : line)
    {
        if (iswdigit(ch))
        {
            digits.push_back(ch);
        }
    }
    return digits;
}

int main()
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
    while (true)
    {
        wcout << L"Введите фразу с числами: ";
        wstring line;
        getline(wcin, line);
        if (line.empty())
        {
            continue;
        }
        if (line.size() > 1000)
        {
            wcout << L"Превышен лимит в 1000 символов, введите что-то другое: " << endl;
            ;
            continue;
        }
        vector<wchar_t> digits = splitDigits(line);
        if (digits.empty())
        {
            wcout << L"Во фразе нет ни одного числа." << endl;
            continue;
        }
        wcout << L"Введите номер цифры или диапазон цифр которые хотите вывести(например 3 или 3-7): ";
        wstring n;
        int start, end;
        wcin >> n;
        size_t dash = n.find(L'-');

        if (dash != string::npos)
        {
            start = stoi(n.substr(0, dash));
            end = stoi(n.substr(dash + 1));

            wcout << L"Цифры в диапазоне от " << start << L" до " << end << L": ";
            for (size_t i = start; i <= end && i < digits.size(); i++)
            {
                wcout << digits[i - 1];
            }
            wcout << endl;
        }
        else
        {
            start = end = stoi(n);
            wcout << L"Цифрой номер " << start << L" является: " << digits[start - 1] << endl;
        }
        break;
    }

    system("pause");
    return 0;
}