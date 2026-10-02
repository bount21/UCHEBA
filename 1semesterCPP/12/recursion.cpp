#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <windows.h>

using namespace std;

int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};

int dfs(int x, int y, int color, vector<vector<int>> &matrix)
{
    int N = matrix.size();
    int M = matrix[0].size();

    if (x < 0 || x >= N || y < 0 || y >= M)
        return 0;
    if (matrix[x][y] != color)
        return 0;

    matrix[x][y] = 0;

    int size = 1;
    for (int i = 0; i < 4; i++)
    {
        size += dfs(x + dx[i], y + dy[i], color, matrix);
    }
    return size;
}

int main()
{
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    srand(time(0));

    int N, M;
    cout << "Введите количество строк: ";
    cin >> N;
    cout << "Введите количество столбцов: ";
    cin >> M;

    vector<vector<int>> matrix(N, vector<int>(M));
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            matrix[i][j] = rand() % 7 + 1;
        }
    }

    cout << "Сгенерированная матрица:" << endl;
    for (const auto &row : matrix)
    {
        for (int val : row)
        {
            cout << val << " ";
        }
        cout << endl;
    }

    int max_size = 0;
    int color_of_max = 0;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            if (matrix[i][j] != 0)
            {
                int color = matrix[i][j];
                int size = dfs(i, j, color, matrix);
                if (size > max_size)
                {
                    max_size = size;
                    color_of_max = color;
                }
            }
        }
    }

    cout << "Наибольшая область: Цвет " << color_of_max << ", размер " << max_size << " ячеек" << endl;

    system("pause");
    return 0;
}