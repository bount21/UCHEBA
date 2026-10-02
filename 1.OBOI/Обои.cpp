#include <iostream>
#include <cmath>
#include <iomanip>
#include <windows.h>

using namespace std;

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    float dlina, shirina, visota;
    float dlina_rulona, shirina_rulona;
    float shirina_okna, visota_okna;

    cout << "РАСЧЕТ ОБОЕВ ДЛЯ ПОКЛЕЙКИ" << endl;
    cout << "========================================================================================================================" << endl;

    while (1)
    {
        cout << "Введите длину комнаты(м): ";
        cin >> dlina;
        if (cin.fail() || dlina <= 0)
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "ОШИБКА! Введите положительное ЧИСЛО" << endl;
        }
        else
            break;
    }

    while (1)
    {
        cout << "Введите ширину комнаты(м): ";
        cin >> shirina;
        if (cin.fail() || shirina <= 0)
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "ОШИБКА! Введите положительное ЧИСЛО" << endl;
        }
        else
            break;
    }

    while (1)
    {
        cout << "Введите высоту комнаты(м): ";
        cin >> visota;
        if (cin.fail() || visota <= 0)
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "ОШИБКА! Введите положительное ЧИСЛО" << endl;
        }
        else
            break;
    }

    cout << endl;

    while (1)
    {
        cout << "Введите ширину окна(м): ";
        cin >> shirina_okna;
        if (cin.fail() || shirina_okna <= 0)
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "ОШИБКА! Введите положительное ЧИСЛО" << endl;
        }
        else
            break;
    }
    while (1)
    {
        cout << "Введите высоту окна(м): ";
        cin >> visota_okna;
        if (cin.fail() || visota_okna <= 0)
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "ОШИБКА! Введите положительное ЧИСЛО" << endl;
        }
        else
            break;
    }

    while (1)
    {
        cout << endl
             << "Введите длину рулона(м): ";
        cin >> dlina_rulona;
        if (cin.fail() || dlina_rulona <= 0)
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "ОШИБКА! Введите положительное ЧИСЛО" << endl;
        }
        else if (dlina_rulona < visota)
        {
            cout << "ОШИБКА! Рулон слишком короткий " << endl;
        }
        else
            break;
    }

    while (1)
    {
        cout << "Введите ширину рулона(м): ";
        cin >> shirina_rulona;
        if (cin.fail() || shirina_rulona <= 0)
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "ОШИБКА! Введите положительное ЧИСЛО" << endl;
        }

        else
            break;
    }

    float perimetr = 2 * (dlina + shirina);
    float ploshad_sten = perimetr * visota;
    float ploshad_okna = shirina_okna * visota_okna;
    float poleznaya_ploshad = ploshad_sten - ploshad_okna;

    int polos_iz_rulona = dlina_rulona / visota;

    float shirina_polosi = shirina_rulona;
    float nuzhno_polos = poleznaya_ploshad / (visota * shirina_polosi);

    int nuzhno_rulonov = ceil(nuzhno_polos / polos_iz_rulona);

    float obshaya_ploshad_rulonov = nuzhno_rulonov * dlina_rulona * shirina_rulona;
    float ispolzovano_oboev = poleznaya_ploshad;
    float ostatki = obshaya_ploshad_rulonov - ispolzovano_oboev;
    float procent_ostatkov = (ostatki / obshaya_ploshad_rulonov) * 100;

    float visota_okna_ot_pola = visota - visota_okna;
    float ploshad_pod_oknom = shirina_okna * visota_okna_ot_pola;

    if (ostatki >= ploshad_pod_oknom)
    {
        ostatki -= ploshad_pod_oknom;
        ispolzovano_oboev += ploshad_pod_oknom;
        procent_ostatkov = (ostatki / obshaya_ploshad_rulonov) * 100;
    }

    cout << endl
         << "РЕЗУЛЬТАТЫ:" << endl;
    cout << "========================================================================================================================" << endl;
    cout << "Периметр комнаты: " << perimetr << " м" << endl;
    cout << "Площадь стен: " << ploshad_sten << " м2" << endl;
    cout << "Площадь окна: " << ploshad_okna << " м2" << endl;

    cout << "Нужно оклеить: " << poleznaya_ploshad << " м2" << endl;

    cout << "Полос из одного рулона: " << polos_iz_rulona << endl;
    cout << "Всего нужно полос: " << ceil(nuzhno_polos) << endl;
    cout << "Нужно рулонов: " << nuzhno_rulonov << endl;
    cout << "Процент остатков: " << fixed << setprecision(2) << procent_ostatkov << "%" << endl;

    system("pause");
    return 0;
}