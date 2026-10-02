#include <windows.h>
#include <iostream>

using namespace std;

int main()
{
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    int n;
    cout << "Количество вещей: ";
    cin >> n;

    double weights[100];
    int indexes[100];

    for (int i = 0; i < n; i++)
    {
        cout << "Вес вещи " << i + 1 << ": ";
        cin >> weights[i];
        indexes[i] = i;
    }

    // сортировка по убыванию веса
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (weights[j] < weights[j + 1])
            {
                swap(weights[j], weights[j + 1]);
                swap(indexes[j], indexes[j + 1]);
            }
        }
    }

    double w1 = 0, w2 = 0;

    int bag1[100], bag2[100];
    double bag1w[100], bag2w[100];
    int c1 = 0, c2 = 0;

    cout << "\nРаспределение:\n";

    for (int i = 0; i < n; i++)
    {
        if (w1 <= w2)
        {
            cout << "Вещь " << indexes[i] + 1 << " (" << weights[i] << " кг) -> Рюкзак 1\n";
            bag1[c1] = indexes[i] + 1;
            bag1w[c1] = weights[i];
            c1++;
            w1 += weights[i];
        }
        else
        {
            cout << "Вещь " << indexes[i] + 1 << " (" << weights[i] << " кг) -> Рюкзак 2\n";
            bag2[c2] = indexes[i] + 1;
            bag2w[c2] = weights[i];
            c2++;
            w2 += weights[i];
        }
    }

    cout << "\nИтог:\n";

    cout << "Рюкзак 1: " << w1 << " кг (";
    for (int i = 0; i < c1; i++)
    {
        if (i > 0)
            cout << ", ";
        cout << "вещь " << bag1[i] << " - " << bag1w[i] << " кг";
    }
    cout << ")\n";

    cout << "Рюкзак 2: " << w2 << " кг (";
    for (int i = 0; i < c2; i++)
    {
        if (i > 0)
            cout << ", ";
        cout << "вещь " << bag2[i] << " - " << bag2w[i] << " кг";
    }
    cout << ")\n";

    double diff = abs(w1 - w2);
    cout << "Разница: " << diff << " кг\n";

    system("pause");
    return 0;
}
