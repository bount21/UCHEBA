#ifndef _WIN32_WINNT // Чтоб на винде работало
#define _WIN32_WINNT 0x0600 // Виста и выше
#endif

#include <iostream>
#include <windows.h>
#include <string>
using namespace std;

// Функция для создания ключа и записи значения
bool createRegistryKey(HKEY hKeyParent, const wstring& subKey, const wstring& valueName, const wstring& data) {
    HKEY hKey;
    // Создаем или открываем ключ реестра
    LONG result = RegCreateKeyExW(hKeyParent, subKey.c_str(), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, &hKey, nullptr);

    if (result == ERROR_SUCCESS) {
        // Записываем значение (размер в байтах, поэтому умножаем на sizeof(wchar_t))
        result = RegSetValueExW(hKey, valueName.c_str(), 0, REG_SZ,
                                reinterpret_cast<const BYTE*>(data.c_str()),
                                (data.length() + 1) * sizeof(wchar_t));
        RegCloseKey(hKey);
    }
    return result == ERROR_SUCCESS;
}
// Функция удаления из реестра
bool deleteRegistryKey(HKEY hKeyParent, const wstring& subKey) {
    LONG result = RegDeleteTreeW(hKeyParent, subKey.c_str()); // Сносим папку вместе со всем содержимым
    return result == ERROR_SUCCESS || result == ERROR_FILE_NOT_FOUND; // Даже если этой папки не было, то всё норм
}

void install() {
    cout << "\n Installing registry keys...\n\n";

    // Регистрация расширения .hor и привязка к типу Horse.Image.1
    if (!createRegistryKey(HKEY_CURRENT_USER, L"Software\\Classes\\.hor", L"", L"Horse.Image.1")) return;
    if (!createRegistryKey(HKEY_CURRENT_USER, L"Software\\Classes\\.hor\\ShellNew", L"FileName", L"horse.bmp")) return;

    // Описание типа файла
    if (!createRegistryKey(HKEY_CURRENT_USER, L"Software\\Classes\\Horse.Image.1", L"", L"Horse Image")) return;

    // Иконка
    if (!createRegistryKey(HKEY_CURRENT_USER, L"Software\\Classes\\Horse.Image.1\\DefaultIcon", L"",
                           L"D:\\INFPROGC++\\2semester\\6.registry\\horse_icon.ico")) return;

    // Открывать в Paint
    if (!createRegistryKey(HKEY_CURRENT_USER, L"Software\\Classes\\Horse.Image.1\\shell\\open\\command", L"",
                           L"\"C:\\Windows\\System32\\mspaint.exe\" \"%1\"")) return;

    cout << " Success! All keys added to registry\n\n";
}

void uninstall() {
    cout << "\n Uninstalling from registry...\n\n";

    bool res1 = deleteRegistryKey(HKEY_CURRENT_USER, L"Software\\Classes\\.hor");
    bool res2 = deleteRegistryKey(HKEY_CURRENT_USER, L"Software\\Classes\\Horse.Image.1");

    if (res1 && res2) {
        cout << " [SUCCESS] Registry is clean from .hor!\n\n";
    } else {
        cout << " [ERROR] Failed to clean the registry.\n\n";
    }
}

int main() {
    int choice = 0;
    while (true) {
        cout << "======================================\n";
        cout << " 1. Install (.hor)\n";
        cout << " 2. Uninstall\n";
        cout << " 3. Exit\n";
        cout << "======================================\n";
        cout << "\n Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            install();
        } else if (choice == 2) {
            uninstall();
        } else if (choice == 3) {
            break;
        } else {
            cout << "\n Invalid choice. Try again.\n";
        }
    }
    return 0;
}