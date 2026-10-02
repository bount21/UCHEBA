#include <iostream>
#include <vector>
#include <windows.h>
#include <iomanip>
#include <fstream>
using namespace std;

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    int N;
    cout << "Введите размеры змейки(N x N): ";
    cin >> N;
    cout << "Выберите с какого угла начать заполнение: " << endl;
    cout << "1 - С левого верхнего " << endl;
    cout << "2 - C правого верхнего " << endl;
    cout << "3 - C левого нижнего " << endl;
    cout << "4 - С правого нижнего " << endl;
    int corner;
    cin >> corner;

    vector<vector<int>> zmei(N, vector<int>(N));
    int val = 1;

    for (int d = 0; d < 2 * N - 1; d++)
    {
        int start_i, start_j, end_i, end_j;

        if (d < N)
        {
            start_i = d;
            start_j = 0;
        }
        else
        {
            start_i = N - 1;
            start_j = d - N + 1;
        }

        if (d < N)
        {
            end_i = 0;
            end_j = d;
        }
        else
        {
            end_i = d - N + 1;
            end_j = N - 1;
        }

        if (d % 2 == 0)
        {
            for (int i = end_i, j = end_j; i <= start_i && j >= start_j; i++, j--)
            {
                zmei[i][j] = val++;
            }
        }
        else
        {
            for (int i = start_i, j = start_j; i >= end_i && j <= end_j; i--, j++)
            {
                zmei[i][j] = val++;
            }
        }
    }

    switch (corner)
    {
    case 1:
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                cout << setw(3) << zmei[i][j] << " ";
            }
            cout << endl;
        }
        break;
    case 2:
        for (int i = 0; i < N; i++)
        {
            for (int j = N - 1; j >= 0; j--)
            {
                cout << setw(3) << zmei[i][j] << " ";
            }
            cout << endl;
        }
        break;
    case 3:
        for (int i = N - 1; i >= 0; i--)
        {
            for (int j = 0; j < N; j++)
            {
                cout << setw(3) << zmei[i][j] << " ";
            }
            cout << endl;
        }
        break;
    case 4:
        for (int i = N - 1; i >= 0; i--)
        {
            for (int j = N - 1; j >= 0; j--)
            {
                cout << setw(3) << zmei[i][j] << " ";
            }
            cout << endl;
        }
        break;
    }

    ofstream save("output.txt");
    switch (corner)
    {
    case 1:
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                save << setw(3) << zmei[i][j] << " ";
            }
            save << endl;
        }
        break;
    case 2:
        for (int i = 0; i < N; i++)
        {
            for (int j = N - 1; j >= 0; j--)
            {
                save << setw(3) << zmei[i][j] << " ";
            }
            save << endl;
        }
        break;
    case 3:
        for (int i = N - 1; i >= 0; i--)
        {
            for (int j = 0; j < N; j++)
            {
                save << setw(3) << zmei[i][j] << " ";
            }
            save << endl;
        }
        break;
    case 4:
        for (int i = N - 1; i >= 0; i--)
        {
            for (int j = N - 1; j >= 0; j--)
            {
                save << setw(3) << zmei[i][j] << " ";
            }
            save << endl;
        }
        break;
    }
    save.close();
    cout << "Змейка сохранена в файл output.txt " << endl;
    system("pause");
    return 0;
}