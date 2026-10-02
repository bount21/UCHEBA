#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <windows.h>
using namespace std;

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    cout << "Введите комнаты в которые необходимо провести интернет: ";
    vector<int> rooms_left;
    vector<int> rooms_right;
    string line;
    getline(cin, line);

    stringstream ss(line);
    int x;
    while (ss >> x)
    {
        if (x % 2 != 0)
        {
            rooms_left.push_back(x);
        }

        else
        {
            rooms_right.push_back(x);
        }
    }
    vector<int> colonki;

    if (rooms_left.empty() && rooms_right.empty())
    {
        cout << "ОШИБКА: Не введено ни одной комнаты" << endl;
        system("pause");
        return 0;
    }
    int minRoom_left = rooms_left.empty() ? INT_MAX : *min_element(rooms_left.begin(), rooms_left.end());
    int minRoom_right = rooms_right.empty() ? INT_MAX : *min_element(rooms_right.begin(), rooms_right.end());
    int minRoom = min(minRoom_left, minRoom_right);

    int maxRoom_left = rooms_left.empty() ? INT_MIN : *max_element(rooms_left.begin(), rooms_left.end());
    int maxRoom_right = rooms_right.empty() ? INT_MIN : *max_element(rooms_right.begin(), rooms_right.end());
    int maxRoom = max(maxRoom_left, maxRoom_right);

    int bestRoom = minRoom;
    int bestLen = INT_MAX;

    for (int router = minRoom; router <= maxRoom; router++)
    {
        int totalLen = 0;

        for (int room : rooms_left)
        {
            int col_router = (router % 2 == 0 ? router / 2 : (router + 1) / 2);
            int col_room = (room + 1) / 2;
            int side_router = (router % 2 == 0 ? 1 : 0);
            int side_room = 0;

            totalLen += abs(col_router - col_room) + (side_router != side_room ? 1 : 0);
        }
        for (int room : rooms_right)
        {
            int col_router = (router % 2 == 0 ? router / 2 : (router + 1) / 2);
            int col_room = room / 2;
            int side_router = (router % 2 == 0 ? 1 : 0);
            int side_room = 1;

            totalLen += abs(col_router - col_room) + (side_router != side_room ? 1 : 0);
        }

        if (totalLen < bestLen)
        {
            bestLen = totalLen;
            bestRoom = router;
        }
    }
    cout << "Лучшее место для установки шлюза: Комната № " << bestRoom << endl;
    cout << "Суммарная длина проводов: " << bestLen << endl;
    system("pause");
    return 0;
}
