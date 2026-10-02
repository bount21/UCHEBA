#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <windows.h>
#include <iomanip>
#include <fstream>
#include <codecvt>
#include <locale>
#include <fcntl.h>
#include <io.h>
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
using namespace std;

// функция подсчета частот
vector<pair<wchar_t, int>> count(const wstring &stroka)
{
    vector<pair<wchar_t, int>> freaks;

    for (wchar_t x : stroka)
    {
        bool found = false;
        for (auto &freak : freaks)
        {
            if (freak.first == x)
            {
                freak.second++;
                found = true;
                break;
            }
        }

        if (!found)
        {
            freaks.emplace_back(x, 1);
        }
    }

    return freaks;
}

// структура дерева Хаффмана
struct Node
{
    wchar_t symbol;
    int freak;
    Node *left;
    Node *right;

    // конструктор
    Node(wchar_t s, int f) : symbol(s), freak(f), left(nullptr), right(nullptr) {}
};

// генерация кодов Хаффмана
void generateCodes(Node *node, const wstring &code, vector<pair<wchar_t, wstring>> &codes)
{
    if (!node)
        return; // если пусто

    // если дошли до листа (нет детей и там символ), добавляем код в вектор
    if (!node->left && !node->right && node->symbol != L'\0')
    {
        codes.emplace_back(node->symbol, code);
        return;
    }

    // влево - добавляем 0, вправо - добавляем 1
    generateCodes(node->left, code + L"0", codes);
    generateCodes(node->right, code + L"1", codes);
}

// построение дерева (снизу вверх)
Node *buildHuffmanTree(const vector<pair<wchar_t, int>> &freaks)
{
    vector<Node *> nodes;

    for (const auto &freak : freaks)
    {
        nodes.push_back(new Node(freak.first, freak.second));
    }

    // сортируем по частоте (через лямбда функцию)
    sort(nodes.begin(), nodes.end(), [](const Node *a, const Node *b)
         { return a->freak < b->freak; });

    while (nodes.size() > 1)
    {
        // берем 2 самых редких символа
        Node *left = nodes[0];
        Node *right = nodes[1];

        // удаляем их из вектора
        nodes.erase(nodes.begin(), nodes.begin() + 2);

        // создаем родителя, дети у которого - те самые 2 символа, а частота - сумма частот этих символов
        auto parent = new Node(L'\0', left->freak + right->freak);
        parent->left = left;
        parent->right = right;

        nodes.push_back(parent);

        // опять
        sort(nodes.begin(), nodes.end(),
             [](const Node *a, const Node *b)
             { return a->freak < b->freak; });
    }

    return nodes[0];
}

// вывод таблицы
void printTable(const vector<pair<wchar_t, int>> &freaks,
                const vector<pair<wchar_t, wstring>> &codes)
{
    wcout << "\n";
    wcout << setw(10) << left << L"Символ "
          << setw(12) << left << L"Частота "
          << L"Код Хаффмана " << endl;
    wcout << wstring(35, L'-') << endl;

    for (const auto &freak : freaks)
    {
        const wchar_t symbol = freak.first;
        const int current_freak = freak.second;
        wstring code;

        // находим код для этого символа
        for (const auto &cod : codes)
        {
            if (cod.first == symbol)
            {
                code = cod.second;
                break;
            }
        }
        wcout << setw(10) << left << (L"'" + wstring(1, symbol) + L"'")
              << setw(12) << left << current_freak
              << code << endl;
    }
    wcout << wstring(35, L'-') << endl;
}

void writeNodeRecursive(const Node *node, int id, ofstream &file)
{
    if (!node)
        return;

    // если лист
    if (!node->left && !node->right)
    {
        // конвертируем wchar_t в обычную строку
        string sym;
        if (node->symbol == L' ')
            sym = "space";

        else if (node->symbol == L'\n')
            sym = "\\\\n";

        else
        {
            // Широкие символы в утф8 конвертируем
            wstring_convert<codecvt_utf8<wchar_t>> converter;
            sym = converter.to_bytes(node->symbol);
        }

        file << "    node" << id << " [shape=box, label=\"" << sym
             << "\\nf=" << node->freak << "\"];\n";
    }
    else
    {
        file << "    node" << id << " [shape=circle, label=\""
             << node->freak << "\"];\n";
    }

    static int nextId = 1; // переменная не сбрасывается между вызовами функции
    const int leftId = nextId++;
    const int rightId = nextId++;

    if (node->left)
    {
        writeNodeRecursive(node->left, leftId, file);
        file << "    node" << id << " -> node" << leftId
             << " [label=\"0\"];\n";
    }
    if (node->right)
    {
        writeNodeRecursive(node->right, rightId, file);
        file << "    node" << id << " -> node" << rightId
             << " [label=\"1\"];\n";
    }
}

void exportToDot(const Node *root, const string &filename)
{
    ofstream file(filename);
    if (!file.is_open())
    {
        wcout << L"Ошибка создания файла!" << endl;
        return;
    }

    // заголовок .dot файла (стандартный для Graphviz)
    file << "digraph HuffmanTree {\n";                  // digraph = направленный граф (стрелки)
    file << "    node [fontname=\"Arial Black\"];\n\n"; // шрифт

    writeNodeRecursive(root, 0, file);

    file << "}\n"; // закрывающая скобка
    file.close();

    // экспорт
    wcout << L"Дерево экспортировано в " << wstring(filename.begin(), filename.end()) << endl;

    const string pngFile = filename.substr(0, filename.find_last_of('.')) + ".png";
    const string command = "dot -Tpng \"" + filename + "\" -o \"" + pngFile + "\"";
    system(command.c_str()); // выполняет в консоли команду(Graphviz открывает)
}

int main()
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);

    wcout << L"Введите строку: ";
    wstring input;
    getline(wcin, input);

    if (input.empty())
    {
        wcout << L"Введите пожалуйста" << endl;
        return 0;
    }

    // считаем частоты
    const auto freaks = count(input);

    // строим дерево Хаффмана
    Node *root = buildHuffmanTree(freaks);

    // генерируем коды
    vector<pair<wchar_t, wstring>> codes;
    generateCodes(root, L"", codes);

    // выводим таблицу
    printTable(freaks, codes);

    exportToDot(root, "huffman_tree.dot");

    // сравнение iso и кодов Хаффмана
    int weight_huff = 0;
    for (int i = 0; i < freaks.size(); i++)
    {
        const wchar_t symbol = freaks[i].first;
        const int freak = freaks[i].second;
        for (int j = 0; j < codes.size(); j++)
        {
            if (symbol == codes[j].first)
            {
                weight_huff += static_cast<int>(codes[i].second.length()) * freak;
            }
        }
    }
    const int weight_in_iso = static_cast<int>(input.length()) * 8;
    const float ratio = (static_cast<float>(weight_huff) / static_cast<float>(weight_in_iso)) * 100;
    cout << endl;
    wcout << L"---------Сравнение iso, кодов Хаффмана---------------" << endl
          << L"Вес строки в iso = " << weight_in_iso << L" бит" << endl
          << L"Вес строки после сжатия алгоритмом Хаффмана = " << weight_huff << L" бит" << endl
          << L"Коэффициент сжатия (между iso и huff): " << ratio << L" %" << endl
          << L"Экономия = " << weight_in_iso - weight_huff << L" бит" << endl;

    system("pause");
    return 0;
}