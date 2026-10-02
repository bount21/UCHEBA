#include <iostream>
#include <vector>
#include <numeric>
#include <fstream>
#include <ctime>
#include <windows.h>
using namespace std;

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    srand(time(0));
    vector<int> arr;

    cout << "1. Сгенерировать идеально отсортированный массив \n2. Сгенерировать случайный массив\n";
    int fill;
    cin >> fill;
    switch (fill)
    {
    case 1:
    {
        cout << "Укажите размер идеального массива: ";
        int n;
        cin >> n;
        arr.resize(n);

        iota(arr.begin(), arr.end(), 1);

        ofstream outFile("numbers.txt");

        for (int num : arr)
        {
            outFile << num << " ";
        }
        break;
    }
    case 2:
    {
        int n, a;
        cout << "Укажите размер массива: ";
        cin >> n;
        cout << "Укажите до какого числа генерировать элементы: ";
        cin >> a;
        arr.resize(n);
        for (int i = 0; i < n; i++)
        {
            arr[i] = rand() % a;
        }

        ofstream outFile("numbers.txt");

        for (int num : arr)
        {
            outFile << num << " ";
        }
        break;
    }
    }

    system("pause");
    return 0;
}
