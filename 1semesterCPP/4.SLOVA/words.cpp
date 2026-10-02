#include <iostream>
#include <string>
#include <vector>
#include <cwctype>
#include <io.h>
#include <fcntl.h>
#include <set>

using namespace std;

vector<wstring> splitWords(const wstring &line)
{
    vector<wstring> words;
    wstring current;

    for (wchar_t ch : line)
    {
        if (iswalpha(ch))
        {
            current += ch;
        }
        else
        {
            if (!current.empty())
            {
                words.push_back(current);
                current.clear();
            }
        }
    }
    if (!current.empty())
        words.push_back(current);
    return words;
}

int main()
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);

    while (true)
    {
        wcout << L"Введите фразу: ";
        wstring line;
        getline(wcin, line);

        vector<wstring> words = splitWords(line);

        if (words.empty())
        {
            wcout << L"Ошибка: не найдено ни одного слова.\n";
            continue;
        }

        if (line.size() > 1000)
        {
            wcout << L"Слишком длинная строка, максимальное количество символов - 1000.\n";
            continue;
        }

        size_t minLen = words[0].size();
        size_t maxLen = words[0].size();

        for (auto &word : words)
        {
            minLen = min(minLen, word.size());
            maxLen = max(maxLen, word.size());
        }

        vector<wstring> shortest, longest;
        set<wstring> usedShortest, usedLongest;

        for (auto &word : words)
        {
            wstring long_words;
            for (wchar_t ch : word)
            {
                long_words += towlower(ch);
            }

            if (word.size() == minLen && usedShortest.find(long_words) == usedShortest.end())
            {
                shortest.push_back(word);
                usedShortest.insert(long_words);
            }

            if (word.size() == maxLen && usedLongest.find(long_words) == usedLongest.end())
            {
                longest.push_back(word);
                usedLongest.insert(long_words);
            }
        }

        wcout << L"Самое короткое слово(а) (" << minLen << L" символа): ";
        for (size_t i = 0; i < shortest.size(); i++)
        {
            wcout << shortest[i];
            if (i < shortest.size() - 1)
                wcout << L", ";
        }

        wcout << L"\nСамое длинное слово(а) (" << maxLen << L" символов): ";
        for (size_t i = 0; i < longest.size(); i++)
        {
            wcout << longest[i];
            if (i < longest.size() - 1)
                wcout << L", ";
        }
        wcout << endl;

        break;
    }

    system("pause");
    return 0;
}