#include <iostream>
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

class LCG
{
private:
    unsigned long long a;
    unsigned long long c;
    unsigned long long m;
    unsigned long long seedValue;

public:
    LCG(unsigned long long seed)
    {
        a = 1664525;
        c = 1013904223;
        m = 4294967296;
        seedValue = seed;
    }

    unsigned long long next()
    {
        seedValue = (a * seedValue + c) % m;
        return seedValue;
    }
};

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    int N;
    cout << "Введите количество столбцов: ";
    cin >> N;
    vector<vector<int>> columns(N, vector<int>(N));

    LCG gen(time(0));

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            columns[i][j] = gen.next() % 100;
        }
    }
    cout << endl
         << "Исходная матрица:" << endl;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            cout << setw(3) << columns[i][j] << " ";
        cout << endl;
    }
    vector<int> order;
    int left = 0, right = N - 1;
    while (left <= right)
    {
        order.push_back(left++);
        if (left <= right)
            order.push_back(right--);
    }
    cout << endl
         << "Матрица после перестановки столбцов:" << endl;
    for (int i = 0; i < N; i++)
    {
        vector<int> temp(N);
        for (int j = 0; j < N; j++)
            temp[j] = columns[i][order[j]];

        for (int j = 0; j < N; j++)
            columns[i][j] = temp[j];
    }
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            cout << setw(3) << columns[i][j] << " ";
        cout << endl;
    }
    system("pause");
    return 0;
}
