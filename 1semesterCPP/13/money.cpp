#include <iostream>
#include <vector>
#include <climits>
#include <windows.h>
#include <string>

using namespace std;

// 1 фунт = 20 шиллингов/240 пенсов    1 шилинг = 12 пенсов

struct Money
{
    int pounds;
    int shillings;
    int pence;
};

void normalize(Money &m)
{
    if (m.pence >= 12)
    {
        m.shillings += m.pence / 12;
        m.pence %= 12;
    };

    if (m.shillings >= 20)
    {
        m.pounds += m.shillings / 20;
        m.shillings %= 20;
    };
};

int toPence(const Money &m)
{
    return m.pounds * 240 + m.shillings * 12 + m.pence;
};

void printMoney(const Money &m)
{
    cout << m.pounds << "-" << m.shillings << "-" << m.pence;
};

Money addMoney(Money a, Money b)
{
    Money res{
        a.pounds + b.pounds,
        a.shillings + b.shillings,
        a.pence + b.pence};

    normalize(res);

    return (res);
};

Money writeoff(Money a, Money b)
{
    int diff = toPence(a) - toPence(b);
    Money res;
    res.pounds = diff / 240;
    diff %= 240;
    res.shillings = diff / 12;
    res.pence = diff % 12;
    return (res);
}

int main()
{
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    Money balance;
    cout << "Введите начальный баланс в формате X Y Z: ";
    cin >> balance.pounds >> balance.shillings >> balance.pence;

    while (balance.pounds < 0 || balance.shillings < 0 || balance.pence < 0)
    {
        cout << "Отрицательные значения недопустимы. Попробуйте снова: ";
        cin >> balance.pounds >> balance.shillings >> balance.pence;
    };

    normalize(balance);

    int opers = 0;
    cout << "Введите количество операций по счёту: ";
    cin >> opers;

    while (opers <= 0)
    {
        cout << "Отрицательные значения недопустимы. Попробуйте снова: ";
        cin >> opers;
    };

    for (int i = 0; i < opers; i++)
    {

        string znak;

        while (true)
        {
            cout << "Если списание введите -, если пополнение введите +: ";
            cin >> znak;
            if (znak == "-" || znak == "+")
            {
                break;
            }
            else
            {
                cout << "Пожалуйста, введите + или - \n";
            };
        };

        Money op;
        cout << "Введите сумму операции (фунты шиллинги пенсы): ";
        cin >> op.pounds >> op.shillings >> op.pence;

        while (op.pounds < 0 || op.shillings < 0 || op.pence < 0)
        {
            cout << "Отрицательные значения недопустимы. Попробуйте снова: ";
            cin >> op.pounds >> op.shillings >> op.pence;
        }

        normalize(op);

        if (znak == "+")
        {
            balance = addMoney(balance, op);
            cout << "Баланс после пополнения:   ";
            printMoney(balance);
            cout << endl;
        }
        else
        {
            while (toPence(balance) < toPence(op))
            {
                cout << "Недостаточно средств для списания. Попробуйте снова: ";
                cin >> op.pounds >> op.shillings >> op.pence;

                while (op.pounds < 0 || op.shillings < 0 || op.pence < 0)
                {
                    cout << "Отрицательные значения недопустимы. Попробуйте снова: ";
                    cin >> op.pounds >> op.shillings >> op.pence;
                };

                normalize(op);
            }

            balance = writeoff(balance, op);
            cout << "Баланс после списания: ";
            printMoney(balance);
            cout << endl;
        }
    }
    cout << endl;
    int n;
    cout << "Введите количество сумм: ";
    cin >> n;

    while (n <= 0)
    {
        cout << "Ошибка: количество должно быть больше, чем 0, введите заново:     ";
        cin >> n;
    }

    vector<Money> arr(n);

    for (int i = 0; i < n; i++)
    {
        cout << "Сумма номер " << i + 1 << "(фунты шилинги пенсы)";
        cin >> arr[i].pounds >> arr[i].shillings >> arr[i].pence;

        while (arr[i].pounds < 0 || arr[i].shillings < 0 || arr[i].pence < 0)
        {
            cout << "Отрицательные значения недопустимы. Попробуйте снова: ";
            cin >> arr[i].pounds >> arr[i].shillings >> arr[i].pence;
        }

        normalize(arr[i]);
    }

    int totalPence = 0;

    for (int i = 0; i < n; i++)
    {
        totalPence += toPence(arr[i]);
    };

    int averagePence = totalPence / n;

    Money average;
    average.pounds = averagePence / 240;
    averagePence %= 240;
    average.shillings = averagePence / 12;
    average.pence = averagePence % 12;

    cout << "\nСреднее значение: ";
    printMoney(average);
    cout << endl;

    int minDiff = INT_MAX;
    int maxDiff = 0;

    int min_i = 0, min_j = 1;
    int max_i = 0, max_j = 1;

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            int diff = abs(toPence(arr[i]) - toPence(arr[j]));

            if (diff < minDiff)
            {
                minDiff = diff;
                min_i = i;
                min_j = j;
            }

            if (diff > maxDiff)
            {
                maxDiff = diff;
                max_i = i;
                max_j = j;
            }
        }
    }

    cout << "\nНаиболее близкие суммы:\n";
    printMoney(arr[min_i]);
    cout << " и ";
    printMoney(arr[min_j]);
    cout << " (разность = " << minDiff << " пенсов)\n";

    cout << "\nНаиболее дальние суммы:\n";
    printMoney(arr[max_i]);
    cout << " и ";
    printMoney(arr[max_j]);
    cout << " (разность = " << maxDiff << " пенсов)\n";

    system("pause");
    return 0;
}