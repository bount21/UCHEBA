#include <iostream>
#include <vector>
#include <sstream>
#include <windows.h>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <limits>
#include <queue>
#include <iomanip>

using namespace std;
using namespace std::chrono;

struct Metrics
{
    long long comparisons = 0;
    long long moves = 0;
    int runsCount = 0;
};

void mergeSort(vector<int> &arr, int left, int mid, int right, vector<int> &buffer, Metrics &metrics, bool verbose)
{
    int i = left;
    int j = mid;
    int k = left;

    if (verbose)
    {
        cout << "\nСлияние: ";
        for (int t = left; t < mid; t++)
            cout << arr[t] << " ";
        cout << "+ ";
        for (int t = mid; t < right; t++)
            cout << arr[t] << " ";
    }

    while (i < mid && j < right)
    {
        metrics.comparisons++;
        if (arr[i] <= arr[j])
        {
            buffer[k++] = arr[i++];
            metrics.moves++;
        }
        else
        {
            buffer[k++] = arr[j++];
            metrics.moves++;
        }
    }

    while (i < mid)
    {
        buffer[k++] = arr[i++];
        metrics.moves++;
    }

    while (j < right)
    {
        buffer[k++] = arr[j++];
        metrics.moves++;
    }

    if (verbose)
    {
        cout << "= [ ";
        for (int t = left; t < right; t++)
            cout << buffer[t] << " ";
        cout << "]\n";
    }
}

void directMerge(vector<int> &arr, Metrics &metrics, bool verbose)
{
    int n = arr.size();
    if (n < 2)
        return;
    vector<int> buffer(n);

    for (int width = 1; width < n; width *= 2)
    {
        for (int left = 0; left < n; left += 2 * width)
        {
            int mid = min(left + width, n);
            int right = min(left + width * 2, n);
            if (mid < right)
            {
                mergeSort(arr, left, mid, right, buffer, metrics, verbose);
            }
        }
        for (int i = 0; i < n; i++)
        {
            arr[i] = buffer[i];
            metrics.moves++;
        }
        if (verbose)
            cout << "\n";
    }
}

struct Run
{
    int l, r;
};

void mergeRuns(vector<int> &a, vector<int> &buffer, Run leftRun, Run rightRun, Metrics &metrics, bool verbose)
{
    int i = leftRun.l;
    int j = rightRun.l;
    int k = leftRun.l;

    if (verbose)
    {
        cout << "\nСлияние серий: ";
        for (int t = leftRun.l; t < leftRun.r; t++)
            cout << a[t] << " ";
        cout << "+ ";
        for (int t = rightRun.l; t < rightRun.r; t++)
            cout << a[t] << " ";
    }

    while (i < leftRun.r && j < rightRun.r)
    {
        metrics.comparisons++;
        if (a[i] <= a[j])
        {
            buffer[k++] = a[i++];
            metrics.moves++;
        }
        else
        {
            buffer[k++] = a[j++];
            metrics.moves++;
        }
    }
    while (i < leftRun.r)
    {
        buffer[k++] = a[i++];
        metrics.moves++;
    }
    while (j < rightRun.r)
    {
        buffer[k++] = a[j++];
        metrics.moves++;
    }
    for (int t = leftRun.l; t < rightRun.r; t++)
    {
        a[t] = buffer[t];
        metrics.moves++;
    }
    if (verbose)
    {
        cout << "= [ ";
        for (int t = leftRun.l; t < rightRun.r; t++)
            cout << buffer[t] << " ";
        cout << "]\n\n";
    }
}

void naturalMergeSort(vector<int> &a, Metrics &metrics, bool verbose)
{
    int n = a.size();
    if (n < 2)
        return;

    vector<int> buffer(n);
    queue<Run> runQueue;
    metrics.runsCount = 0;

    int i = 0;
    while (i < n)
    {
        int start = i;
        while (i + 1 < n)
        {
            metrics.comparisons++;
            if (a[i] <= a[i + 1])
                i++;
            else
                break;
        }

        runQueue.push({start, i + 1});
        metrics.runsCount++;

        if (verbose)
        {
            cout << "Найдена серия: ";
            for (int t = start; t <= i; t++)
                cout << a[t] << " ";
            cout << endl;
        }
        i++;
    }

    int runsInCurrentLevel = runQueue.size();

    while (runQueue.size() > 1)
    {
        if (runsInCurrentLevel == 1)
        {
            Run lonelyRun = runQueue.front();
            runQueue.pop();
            runQueue.push(lonelyRun);
            runsInCurrentLevel = runQueue.size();
            continue;
        }

        Run r1 = runQueue.front();
        runQueue.pop();
        Run r2 = runQueue.front();
        runQueue.pop();

        mergeRuns(a, buffer, r1, r2, metrics, verbose);

        runQueue.push({r1.l, r2.r});
        runsInCurrentLevel -= 2;

        if (runsInCurrentLevel <= 0)
        {
            runsInCurrentLevel = runQueue.size();
        }
    }
}

void printToFile(ostream &stream, const string &algoName, int size, const Metrics &metrics, long long durationMs)
{
    stream << "[" << algoName << "]:\n";
    stream << "Время: " << durationMs << " мкс\n";
    stream << "- Сравнений: " << metrics.comparisons << "\n";
    stream << "- Перемещений: " << metrics.moves << "\n";
    if (metrics.runsCount > 0)
    {
        stream << "- Найдено серий: " << metrics.runsCount << "\n";
    }
}

int main(int argc, char *argv[])
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    srand(time(0));

    vector<int> arr;
    Metrics directMetrics;
    Metrics naturalMetrics;

    bool verbose = false;
    string algo = "both";
    string inputFile = "";
    string outputFile = "";
    int generateSize = 0;

    long long durationDirectTime = 0;
    long long durationNaturalTime = 0;

    for (int i = 1; i < argc; i++)
    {
        string arg = argv[i];
        if (arg == "--generate" && i + 1 < argc)
            generateSize = stoi(argv[++i]);
        else if (arg == "--input" && i + 1 < argc)
            inputFile = argv[++i];
        else if (arg == "--algo" && i + 1 < argc)
            algo = argv[++i];
        else if (arg == "--verbose")
            verbose = true;
        else if (arg == "--output" && i + 1 < argc)
            outputFile = argv[++i];
        else if (arg == "--help")
        {
            cout << "--generate <size>\n--input <file>\n--algo <direct|natural|both>\n--verbose\n--output <file>\n";
            return 0;
        }
    }

    if (argc > 1)
    {
        if (generateSize > 0)
        {
            arr.resize(generateSize);
            for (int i = 0; i < generateSize; i++)
                arr[i] = rand() % 100;
        }
        else if (!inputFile.empty())
        {
            ifstream file(inputFile);
            if (!file.is_open())
            {
                cout << "Файл не найден, попробуйте снова.\n\n";
                return 0;
            }
            string line;
            bool hasGarbage = false;

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
                        hasGarbage = true;
                        break;
                    }
                }
                if (hasGarbage)
                    break;
            }
            file.close();

            if (hasGarbage)
            {
                cout << "Ошибка: файл должен содержать только числовые данные.\n\n";
                system("pause");
                return 0;
            }
        }
    }
    else
    {
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
                    cout << "Ошибка: ввод должен содержать только числа.\n";
                break;
            }
            case 2:
            {
                int n, a;
                cout << "Укажите количество элементов массива: ";
                cin >> n;
                cout << "Укажите до какого числа генерировать элементы: ";
                while (true)
                {
                    cin >> a;
                    if (a > 0)
                        break;
                    cout << "Введите число БОЛЬШЕ 0\n";
                }
                arr.resize(n);
                for (int i = 0; i < n; i++)
                    arr[i] = rand() % a;
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
                cout << "Неверный ввод\n";
                continue;
            }

            if (menuError)
                continue;
            if (arr.empty())
            {
                cout << "Ошибка: не введено ни одного элемента\n";
                continue;
            }
            if (arr.size() == 1)
            {
                cout << "Ошибка: массив должен содержать >= 2 элементов\n";
                continue;
            }
            break;
        }
    }

    if (arr.size() > 1000 && verbose)
    {
        cout << "[Предупреждение]: Массив слишком большой. --verbose отключен.\n";
        verbose = false;
    }

    vector<int> arr1 = arr;
    vector<int> arr2 = arr;

    if (algo == "direct" || algo == "both")
    {
        auto start = high_resolution_clock::now();
        directMerge(arr1, directMetrics, verbose);
        auto stop = high_resolution_clock::now();
        durationDirectTime = duration_cast<microseconds>(stop - start).count();

        if (algo == "direct")
        {
            cout << "[1] Прямое слияние: ";
            for (int x : arr1)
                cout << x << " ";
            cout << "\n\n - Время: " << durationDirectTime << " мкс\n";
            cout << " - Сравнений: " << directMetrics.comparisons << "\n";
            cout << " - Перемещений: " << directMetrics.moves << "\n\n";
        }
    }

    if (algo == "natural" || algo == "both")
    {
        auto start = high_resolution_clock::now();
        naturalMergeSort(arr2, naturalMetrics, verbose);
        auto stop = high_resolution_clock::now();
        durationNaturalTime = duration_cast<microseconds>(stop - start).count();

        if (algo == "natural")
        {
            cout << "[2] Естественное слияние: ";
            for (int x : arr2)
                cout << x << " ";
            cout << "\n\n - Время: " << durationNaturalTime << " мкс\n";
            cout << " - Сравнений: " << naturalMetrics.comparisons << "\n";
            cout << " - Перемещений: " << naturalMetrics.moves << "\n";
            cout << " - Количество серий: " << naturalMetrics.runsCount << "\n\n";
        }
    }

    if (algo == "both")
    {
        cout << "[1] Прямое слияние:\n - Время: " << durationDirectTime << " мкс\n";
        cout << " - Сравнений: " << directMetrics.comparisons << "\n";
        cout << " - Перемещений: " << directMetrics.moves << "\n\n";

        cout << "[2] Естественное слияние:\n - Время: " << durationNaturalTime << " мкс\n";
        cout << " - Сравнений: " << naturalMetrics.comparisons << "\n";
        cout << " - Перемещений: " << naturalMetrics.moves << "\n";
        cout << " - Количество серий: " << naturalMetrics.runsCount << "\n\n";

        if (durationDirectTime < durationNaturalTime)
        {
            double faster = (((double)(durationNaturalTime - durationDirectTime)) / durationNaturalTime) * 100;
            cout << fixed << setprecision(2) << "Прямое слияние быстрее на " << faster << "%\n";
        }
        else if (durationNaturalTime < durationDirectTime)
        {
            double faster = (((double)(durationDirectTime - durationNaturalTime)) / durationDirectTime) * 100;
            cout << fixed << setprecision(2) << "Естественное слияние быстрее на " << faster << "%\n";
        }
        else
            cout << "Сортировки выполнились за одинаковое время\n";
    }

    if (!outputFile.empty())
    {
        ofstream outFile(outputFile);
        if (!outFile.is_open())
            cout << "Ошибка записи в файл: " << outputFile << endl;
        else
        {
            outFile << "Размер массива: " << arr.size() << "\n\n";
            if (algo == "direct" || algo == "both")
                printToFile(outFile, "1] Прямое слияние (Direct Merge)", arr.size(), directMetrics, durationDirectTime);
            if (algo == "natural" || algo == "both")
                printToFile(outFile, "2] Естественное слияние (Natural Merge)", arr.size(), naturalMetrics, durationNaturalTime);

            if (algo == "both")
            {
                outFile << "\nВывод:\n";
                if (durationDirectTime < durationNaturalTime)
                    outFile << fixed << setprecision(2) << "Прямое слияние лучше на " << (((double)(durationNaturalTime - durationDirectTime)) / durationNaturalTime) * 100 << "%.\n";
                else if (durationNaturalTime < durationDirectTime)
                    outFile << fixed << setprecision(2) << "Естественное слияние лучше на " << (((double)(durationDirectTime - durationNaturalTime)) / durationDirectTime) * 100 << "%.\n";
                else
                    outFile << "Время выполнения одинаково.\n";
            }
            outFile.close();
            cout << "Результаты сохранены в " << outputFile << "\n";
        }
    }
    else if (argc = 1)
    {
        cout << "Введите имя файла для сохранения: ";
        cin >> outputFile;

        ofstream outFile(outputFile);

        outFile << "Размер массива: " << arr.size() << " элементов.\n\n";

        outFile << "Исходный массив:\n[ ";
        for (int x : arr)
        {
            outFile << x << " ";
        }
        outFile << "]\n\n";

        if (algo == "direct" || algo == "both")
            printToFile(outFile, "1] Прямое слияние (Direct Merge)", arr.size(), directMetrics, durationDirectTime);
        outFile << "Результат сортировки:\n[ ";
        for (int x : arr1)
        {
            outFile << x << " ";
        }
        outFile << "]\n\n";
        if (algo == "natural" || algo == "both")
            printToFile(outFile, "2] Естественное слияние (Natural Merge)", arr.size(), naturalMetrics, durationNaturalTime);
        outFile << "Результат сортировки:\n[ ";
        for (int x : arr2)
        {
            outFile << x << " ";
        }
        outFile << "]\n\n";

        if (algo == "both")
        {
            outFile << "\nВывод:\n";
            if (durationDirectTime < durationNaturalTime)
                outFile << fixed << setprecision(2) << "Прямое слияние лучше на " << (((double)(durationNaturalTime - durationDirectTime)) / durationNaturalTime) * 100 << "%.\n";
            else if (durationNaturalTime < durationDirectTime)
                outFile << fixed << setprecision(2) << "Естественное слияние лучше на " << (((double)(durationDirectTime - durationNaturalTime)) / durationDirectTime) * 100 << "%.\n";
            else
                outFile << "Время выполнения одинаково.\n";
        }
        outFile.close();
        cout << "Результаты сохранены в " << outputFile << "\n";
    }

    system("pause");
    return 0;
}