#include <iostream>
#include <vector>
#include <sstream>
#include <windows.h>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <algorithm>

using namespace std;
using namespace std::chrono;
// Теоретическая сложность:1
//  - Средняя: O(n log n) — при хорошем выборе опорного элемента (как можно ближе к медиане).
//  - Худшая: O(n^2) — если опорный элемент минимальный или максимальный.
void quicksort(vector<int> &arr, int left, int right, long long &comparisons, long long &swaps)
{
    int i = left;
    int j = right;

    int mid = (left + right) / 2;

    // медиана трёх
    // Точки учета: сравнения и обмены для выбора медианы
    comparisons++;
    if (arr[left] > arr[mid])
    {
        swap(arr[left], arr[mid]);
        swaps++;
    }
    comparisons++;
    if (arr[left] > arr[right])
    {
        swap(arr[left], arr[right]);
        swaps++;
    }
    comparisons++;
    if (arr[mid] > arr[right])
    {
        swap(arr[mid], arr[right]);
        swaps++;
    }

    int pivot = arr[mid];
    // Точка учета: основной цикл разделения O(n)
    while (i <= j)
    {
        // Движение i
        while (arr[i] < pivot)
        {
            comparisons++; // Точка учёта: сравнение
            i++;
        }
        // Движение j
        while (arr[j] > pivot)
        {
            comparisons++; // Точка учёта: сравнение
            j--;
        }
        comparisons++; // Точка учёта: сравнение
        if (i <= j)
        {
            swap(arr[i], arr[j]);
            swaps++; // Точка учёта: обмен элементов
            i++;
            j--;
        }
    }
    // Точка учёта: рекурсивные вызовы
    if (left < j)
    {
        quicksort(arr, left, j, comparisons, swaps);
    }
    if (right > i)
    {
        quicksort(arr, i, right, comparisons, swaps);
    }
}
// Теоретическая сложность:
//   - Средняя/Худшая: O(n^2) — отсортировано в обратном порядке.
//   - Лучшая: O(n) — если массив уже отсортирован (внутренний цикл не выполняется).
void insert(vector<int> &arr, long long &comparisons, long long &swaps)
{

    for (int i = 1; i < arr.size(); i++)
    {
        int j = i - 1;
        int current = arr[i];

        while (j >= 0 && arr[j] > current)
        {
            comparisons++; // Точка учёта: сравнение в while
            arr[j + 1] = arr[j];
            j--;
        }
        comparisons++; // Учёт ложного сравнения while
        arr[j + 1] = current;
        swaps++; // Точка учёта: сдвиг
    }
}
// Теоретическая сложность: O(d * (n + k)), где d — разрядность, k — диапазон цифр.
// Не является сортировкой сравнения.
void countingSortRadix(vector<int> &arr, int exp)
{
    int n = arr.size();
    vector<int> output(n);
    int count[10] = {0};

    // Точка учёта: подсчёт количества каждой цифры в массиве
    for (int i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;

    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];
    // Точка учёта: перенос элементов в выходной массив
    for (int i = n - 1; i >= 0; i--)
    {
        int digit = (arr[i] / exp) % 10;
        output[count[digit] - 1] = arr[i];
        count[digit]--;
    }

    for (int i = 0; i < n; i++)
        arr[i] = output[i];
}
void radix(vector<int> &arr)
{
    int max = *max_element(arr.begin(), arr.end());

    // Точка учёта: цикл по количеству разрядов
    for (int exp = 1; max / exp > 0; exp *= 10)
        countingSortRadix(arr, exp);
}
// Теоретическая сложность: O(n + k), где k — диапазон значений.
// Эффективна только при малом k.
void countingSort(vector<int> &arr)
{
    int max = *max_element(arr.begin(), arr.end());
    vector<int> count(max + 1, 0);
    int n = arr.size();
    // Точка учёта: подсчёт частоты каждого элемента массива
    for (int i = 0; i < n; i++)
    {
        int num = arr[i];
        count[num]++;
    }
    int index = 0;
    // Точка учёта: формирование итогового массива
    for (int i = 0; i <= max; i++)
    {
        while (count[i] > 0)
        {
            arr[index++] = i;
            count[i]--;
        }
    }
}
// Теоретическая сложность:
// - Средняя/Худшая: O(n^2) — массив отсортирован в обратном порядке.
// - Лучшая: O(n) — если массив уже отсортирован.
void shakerSort(vector<int> &arr, long long &comparisons, long long &swaps)
{
    int left = 0;
    int right = arr.size() - 1;
    int last, temp;

    while (left < right)
    {
        last = -1;
        // Точка учёта: проход слева направо
        for (int i = left; i < right; i++)
        {
            comparisons++;
            if (arr[i] > arr[i + 1])
            {
                temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
                swaps++;
                last = i;
            }
        }
        right = last;
        if (last == -1)
        {
            break;
        }
        last = arr.size();
        // Точка учёта: проход справа налево
        for (int i = right - 1; i >= left; i--)
        {
            comparisons++;
            if (arr[i] > arr[i + 1])
            {
                temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
                swaps++;
                last = i;
            }
        }
        left = last + 1;
    }
}
int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    srand(time(0));

    long long comparisons = 0;
    long long swaps = 0;
    vector<int> arr;

    while (true)
    {
        arr.clear();

        cout << "1.Ввести массив вручную \n2.Сгенерировать случайный массив \n3.Импортировать массив из файла\n";

        int fill;
        cin >> fill;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Ошибка! Введите число.\n";
            continue;
        }

        bool menuError = false;
        switch (fill)
        {
        case 1:
        {
            cout << "Введите элементы массива: ";
            string line;
            getline(cin, line);

            stringstream ss(line);
            int number;

            while (ss >> number)
            {
                arr.push_back(number);
            }
            break;
        }
        case 2:
        {
            int n;
            cout << "Укажите количество элементов массива: ";
            cin >> n;
            int a;
            cout << "Укажите до какого числа генерировать элементы: ";
            while (true)
            {
                cin >> a;
                if (a > 0)
                {
                    break;
                }
                else
                {
                    cout << "Введите число БОЛЬШЕ 0\n";
                }
            }
            arr.resize(n);
            for (int i = 0; i < n; i++)
            {
                arr[i] = rand() % a;
            }
            break;
        }
        case 3:
        {
            string filename;
            ifstream file;
            while (true)
            {
                cout << "Введите имя файла: ";
                cin >> filename;
                file.clear();
                file.open(filename);
                if (file.is_open())
                    break;
                cout << "Файл не найден, попробуйте снова.\n";
            }
            string line;
            while (getline(file, line))
            {
                stringstream ss(line);
                string token;
                while (ss >> token)
                {
                    stringstream tokenStream(token);
                    int number;
                    if (tokenStream >> number && tokenStream.eof())
                        arr.push_back(number);
                    else
                    {
                        menuError = true;
                        break;
                    }
                }
                if (menuError)
                    break;
            }
            file.close();
            if (menuError)
                cout << "Ошибка: файл содержит некорректные данные.\n";
            break;
        }
        default:
        {
            cout << "Неверный ввод\n";
            continue;
        }
        }
        if (arr.empty())
        {
            cout << "Ошибка: не введено ни одного элемента\n";
            continue;
        }
        else if (arr.size() == 1)
        {
            cout << "Ошибка: массив должен содержать >= 2 элементов\n";
            continue;
        }
        break;
    }

    vector<int> original = arr;
    int menu;
    high_resolution_clock::time_point start;

    while (true)
    {
        cout << "Выберите тип сортировки: "
             << "\n1.Быстрая \n2.Вставками \n3.Цифровая \n4.Шейкер \n5.Подсчётом" << endl;
        cin >> menu;
        bool valid = true;

        switch (menu)
        {
        case 1:
        {
            start = high_resolution_clock::now();
            quicksort(arr, 0, arr.size() - 1, comparisons, swaps);
            break;
        }
        case 2:
        {
            start = high_resolution_clock::now();
            insert(arr, comparisons, swaps);
            break;
        }
        case 3:
        {
            for (int x : arr)
            {
                if (x < 0)
                {
                    cout << "Ошибка: цифровая сортировка не поддерживает отрицательные числа\n";
                    valid = false;
                    break;
                }
            }
            if (valid == false)
            {
                continue;
            }
            start = high_resolution_clock::now();
            radix(arr);
            break;
        }
        case 4:
        {
            start = high_resolution_clock::now();
            shakerSort(arr, comparisons, swaps);
            break;
        }
        case 5:
        {
            for (int x : arr)
            {
                if (x < 0)
                {
                    cout << "Ошибка: сортировка подсчётом не поддерживает отрицательные числа\n";
                    valid = false;
                    break;
                }
            }
            if (valid == false)
            {
                continue;
            }
            start = high_resolution_clock::now();
            countingSort(arr);
            break;
        }
        default:
        {
            cout << "Неверный ввод\n";
            continue;
        }
        }
        break;
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Изначальный массив: ";
    for (int x : original)
    {
        cout << x << " ";
    }
    cout << "\nИтог сортировки: ";
    for (int x : arr)
    {
        cout << x << " ";
    }
    cout << "\nВремя сортировки: " << duration.count() << "миллисекунд" << endl;
    cout << "Количество операций сравнения: " << comparisons << "\nКоличество операций обмена: " << swaps << endl;
    system("pause");
    return 0;
}