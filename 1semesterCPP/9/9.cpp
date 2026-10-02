#include <iostream>
#include <windows.h>
#include <vector>
#include <io.h>
#include <fcntl.h>

using namespace std;

// Функция для получения ширины консоли
int getConsoleWidth()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

int main()
{
    // Установка русской кодировки для Windows
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    int N, height;

    cout << "Введите количество треугольников (N): ";
    cin >> N;
    cout << "Введите высоту каждого треугольника: ";
    cin >> height;

    // Создаем массив для хранения строк одного треугольника
    vector<string> triangleRows(height);
    int triangleWidth = 2 * height - 1; // Ширина одного треугольника

    // Формируем строки одного треугольника
    for (int row = 0; row < height; row++)
    {
        int stars = 1 + 2 * row;       // Количество звездочек: 1, 3, 5, 7, ...
        int spaces = height - row - 1; // Пробелы для центрирования
        triangleRows[row] = string(spaces, ' ') + string(stars, '*') + string(spaces, ' ');
    }

    // Получаем ширину консоли
    int consoleWidth = getConsoleWidth();
    int spacing = 2; // Минимальное расстояние между треугольниками
    int triangleWithSpacing = triangleWidth + spacing;

    // Рассчитываем, сколько треугольников помещается в одну строку
    int trianglesPerLine = (consoleWidth - spacing) / triangleWithSpacing;
    if (trianglesPerLine <= 0)
        trianglesPerLine = 1;

    // Выводим информацию
    cout << "\n"
         << string(60, '-') << "\n";
    cout << "Ширина консоли: " << consoleWidth << " символов\n";
    cout << "Ширина одного треугольника: " << triangleWidth << " символов\n";
    cout << "Треугольников в строке: " << trianglesPerLine << "\n";
    cout << string(60, '-') << "\n\n";

    // Выводим треугольники с переносом
    int triangleCounter = 0;

    for (int startTriangle = 0; startTriangle < N; startTriangle += trianglesPerLine)
    {
        // Определяем сколько треугольников в текущей строке
        int trianglesInCurrentLine = min(trianglesPerLine, N - startTriangle);

        // Выводим построчно для текущей строки треугольников
        for (int row = 0; row < height; row++)
        {
            for (int i = 0; i < trianglesInCurrentLine; i++)
            {
                cout << triangleRows[row] << string(spacing, ' ');
            }
            cout << "\n";
        }

        // Добавляем пустую строку между блоками треугольников
        if (startTriangle + trianglesPerLine < N)
        {
            cout << "\n";
        }
    }
    system("pause");
    return 0;
}
