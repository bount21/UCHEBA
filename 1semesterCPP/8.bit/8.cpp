#include <iostream>
#include <fstream>
#include <windows.h>
#include <cstdint>
#include <limits>

using namespace std;

const string QUESTIONS[8] = {
    "ммм?",
    "Ало?",
    "Нет?",
    "Зачем?",
    "Почему?",
    "Ага?",
    "Брр?",
    "да?"};

int safeIntInput()
{
    int x;
    while (true)
    {
        cin >> x;
        if (!cin.fail())
            return x;

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Ошибка ввода! Введите число: ";
    }
}

int main()
{
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    uint8_t Voprosi = 0;
    const char *filename = "br.txt";

    ifstream fin(filename);
    if (fin)
    {
        fin.read((char *)&Voprosi, 1);
        fin.close();
    }

    while (true)
    {
        cout << "1. Ответить на 8 вопросов\n";
        cout << "2. Просмотреть текущие ответы (0/1)\n";
        cout << "3. Изменить ответ на конкретный вопрос\n";
        cout << "4. Сохранить и выйти\n";
        cout << "Ваш выбор: ";

        int choice = safeIntInput();

        if (choice == 1)
        {
            cout << "\n Ответьте на вопросы (1=да, 0=нет):\n";
            for (int i = 0; i < 8; i++)
            {
                cout << QUESTIONS[i] << " ";
                int ans;
                while (true)
                {
                    ans = safeIntInput();
                    if (ans == 0 || ans == 1)
                        break;
                    cout << "Введите 0 или 1: ";
                }

                if (ans == 1)
                    Voprosi |= (1 << i);
                else
                    Voprosi &= ~(1 << i);
            }
        }

        else if (choice == 2)
        {
            cout << "\nТекущие ответы:\n";
            for (int i = 0; i < 8; i++)
            {
                cout << QUESTIONS[i] << " — "
                     << ((Voprosi >> i) & 1) << endl;
            }
        }

        else if (choice == 3)
        {
            cout << "\nВведите номер вопроса (1–8): ";
            int q = safeIntInput();

            if (q < 1 || q > 8)
            {
                cout << "Ошибка: номер должен быть 1–8.\n";
                continue;
            }

            cout << QUESTIONS[q - 1] << " (1=да, 0=нет): ";
            int ans;
            while (true)
            {
                ans = safeIntInput();
                if (ans == 0 || ans == 1)
                    break;
                cout << "Введите 0 или 1: ";
            }

            if (ans == 1)
                Voprosi |= (1 << (q - 1));
            else
                Voprosi &= ~(1 << (q - 1));
        }

        else if (choice == 4)
        {
            ofstream fout(filename);
            fout.write((char *)&Voprosi, 1);
            fout.close();
            cout << "\nСохранено. Выход.\n";
            break;
        }

        else
        {
            cout << "Нет такого пункта!\n";
        }
    }

    system("pause");
    return 0;
}
