#include <iostream>
#include <windows.h>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include <algorithm>
#include <random>
#include <ranges>

using namespace std;
using MarkovChain = unordered_map<string, unordered_map<string, int>>;

static random_device rd;
static mt19937 gen(rd());

MarkovChain translator_chain;

// токенизация
vector<string> tokenize(const string& text, const bool keep_punct) {
    vector<string> tokens;
    string token;
    istringstream stream(text);

    while (stream >> token) {
        // 1. Убираем знаки препинания
        if (!keep_punct) {
            erase_if(token, [](unsigned char c) { return ispunct(c); });
        }

        if (!token.empty())
            tokens.push_back(token);
    }

    return tokens;
}

// Обучение цепи
void train_translator(const string& uncultured_path, const string& cultured_path, MarkovChain& chain) {
    ifstream uncultured_file(uncultured_path);
    ifstream cultured_file(cultured_path);

    if (!uncultured_file.is_open() || !cultured_file.is_open()) {
        cerr << "Ошибка открытия файлов обучения!" << endl;
        return;
    }
    string uncultured_text((istreambuf_iterator<char>(uncultured_file)), istreambuf_iterator<char>());
    string cultured_text((istreambuf_iterator<char>(cultured_file)), istreambuf_iterator<char>());

    vector<string> bad_words = tokenize(uncultured_text, false);
    vector<string> good_words = tokenize(cultured_text, false);

    string bad_word, good_word;


    // Читаем параллельно: одно слово из сленга, одно из культуры
    size_t min_size = min(bad_words.size(), good_words.size());
    for (size_t i = 0; i < min_size; ++i) {
        chain[bad_words[i]][good_words[i]]++;
    }
}

// выбор слова
string choose_word(const unordered_map<string, int>& weights) {
    int total = 0;
    for (const auto &freq: weights | views::values)
        total += freq;

    if (total == 0) return "";

    uniform_int_distribution dist(0, total - 1);
    const int r = dist(gen);
    int cumulative = 0;

    for (const auto& [word, freq] : weights) {
        cumulative += freq;
        if (r < cumulative) return word;
    }
    return weights.begin() -> first;  // Страховка на случай если цикл ничё не вернул
}

string translate_text(const string& input_sentence, const MarkovChain& chain) {
    const vector<string> input = tokenize(input_sentence, false);

    string result;

    uniform_int_distribution<size_t> dist(0, chain.empty() ? 0 : chain.size() - 1);

    for (const auto& current_word : input) {
        if (current_word.empty()) continue;

        if (auto it = chain.find(current_word); it != chain.end()) {
            // Если нашли точное сленговое слово — переводим по рулетке
            string translated = choose_word(it->second);
            result += translated + " ";
        } else {
            // Если слова нет в словаре
            if (!chain.empty()) {

                // Прыгаем на случайную позицию в мапе
                const auto random_it = next(chain.begin(), static_cast<long long>(dist(gen)));

                // Вытаскиваем из случайной позиции культурное слово через рулетку
                string random_cultural_word = choose_word(random_it->second);
                result += random_cultural_word + " ";
            } else {
                // Если база вообще пустая, оставляем исходное слово
                result += current_word + " ";
            }
        }
    }

    return result;
}

int main() {
    system("chcp 65001 > nul");


    // Обучаем
    train_translator("uncultured.txt", "cultured.txt", translator_chain);

    cout << "Введите строку: ";
    string input;
    getline(cin, input);

    if (input.size() > 1000) {
        cerr << "[Предупреждение]: Входная строка слишком длинная. Обрезание до 1000 символов." << endl;
        input = input.substr(0, 1000);
    }

    if (input.empty()) {
        cout << "Ну, как хочешь" << endl;
        system("pause");
        return 0;
    }

    const string result = translate_text(input, translator_chain);
    cout << "Результат: " << result << endl;

    system("pause");
    return 0;
}