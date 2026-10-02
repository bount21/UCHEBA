#include <iostream>
#include <windows.h>
#include <map>
using namespace std;
// структура для задачи
struct task
{
    int id;
    int value;
    task *prev;
    task *next;
};

// голову и хвост храним глобально
task *head = nullptr;
task *tail = nullptr;

// реестр задач
map<int, task *> registry;

void add(int id, int value)
{

    task *newTask = new task{id, value, tail, nullptr};

    if (tail)
    {
        tail->next = newTask; // tail теперь указывает на newTask
    }
    else
    {
        head = newTask; // если список пуст
    }

    tail = newTask;
    registry[id] = newTask; // добавляем в реестр
}

void insert_after(int target_id, int id, int value)
{

    auto target_it = registry.find(target_id); // берем итератор на задачу

    if (target_it == registry.end())
    {
        cout << "target не существует" << endl;
        system("pause");
        exit(0);
    }

    task *target = target_it->second; // превращаем итератор в указатель

    task *newTask = new task{id, value, target, target->next};
    target->next = newTask;

    if (newTask->next)
    {
        newTask->next->prev = newTask;
        // У узла, который раньше был следующим после target,теперь предыдущим становится newTask
    }
}

void move_to_front(int id)
{

    auto it = registry.find(id); // итератор на task
    if (it == registry.end())
        return;

    task *task = it->second; // указатель на task
    if (task == head)
        return;

    // вырезаем task из текущей позиции
    if (task->prev)
        task->prev->next = task->next;
    if (task->next)
        task->next->prev = task->prev;

    // если task был хвостом, обновляем хвост
    if (task == tail)
        tail = task->prev;

    // слева никого, справа бывшая голова
    task->prev = nullptr;
    task->next = head;

    if (head)
        head->prev = task;
    head = task;
}

void splice_range(int id_from, int id_to)
{

    auto it_start = registry.find(id_from);
    auto it_end = registry.find(id_to);

    if (it_start == registry.end() || it_end == registry.end())
        return;

    task *task_start = it_start->second;
    task *task_end = it_end->second;

    if (task_start == task_end)
    {
        move_to_front(id_from);
        return;
    }

    // соединяем то что было до и после вырезаемого диапазона
    if (task_start->prev)
        task_start->prev->next = task_end->next;
    if (task_end->next)
        task_end->next->prev = task_start->prev;

    // если вырезаем хвост, обновляем tail
    if (task_end == tail)
        tail = task_end->prev;

    // вставляем вырезанный диапазон в начало
    task_start->prev = nullptr;
    task_end->next = head;

    // бывшая голова теперь ссылается на начало диапазона
    if (head)
        head->prev = task_start;
    head = task_start;
}

void get_value(int id)
{
    auto it = registry.find(id);
    if (it != registry.end())
    {
        cout << "Значение:" << it->second->value << endl;
    }
    else
    {
        cout << "Задачи не существует" << endl;
    }
}

void remove(int id)
{
    auto it = registry.find(id);
    if (it == registry.end())
        return;
    task *task = it->second;

    // вырезаем из списка
    if (task->prev)
        task->prev->next = task->next;
    if (task->next)
        task->next->prev = task->prev;

    // обновляем голову и хвост
    if (task == head)
        head = task->next;
    if (task == tail)
        tail = task->prev;

    registry.erase(it); // удаляем из реестра
    delete task;        // удаляем из памяти
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    int n;
    cout << "Введите количество операций (от 1 до 100000)" << endl;
    while (true)
    {
        cin >> n;
        if (n <= 0 || n > 100000)
        {
            cout << "Введите от 1 до 100000" << endl;
            n = 0;
        }

        else
        {
            for (int i = 0; i < n; i++)
            {
                add(i, i * 10);
                cout << "Added id:" << i << " value:" << i * 10 << endl;
            }
            break;
        }
    }

    cout << "Выберите следующее действие:" << endl
         << "1 - add <id> <value> - добавить новую задачу в конец" << endl
         << "2 - insert_after <target_id> <new_id> <value> - вставить новую задачу после <target id>" << endl
         << "3 - move_to_front <id> - переместить задачу в начало" << endl
         << "4 - splice_range <id_from> <id_to> - переставить диапазон задач в начало" << endl
         << "5 - get_value <id> - вывести значение задачи" << endl
         << "6 - remove <id> - удалить задачу" << endl
         << "7 - список задач (в порядке их выполнения)" << endl
         << "8 - выход из программы" << endl;

    int choose = 0;
    while (true)
    {
        cin >> choose;
        switch (choose)
        {

        case 1:
        {
            int id, value = 0;
            cout << "Введите <id>: ";
            cin >> id;
            cout << "Введите <value>: ";
            cin >> value;

            if (registry.find(id) == registry.end())
            {
                add(id, value);
                cout << "Задача с id = " << id << " и value = " << value << " успешно добавлена!" << endl;
            }

            else
                cout << "Задача с таким id уже существует!" << endl;
            break;
        }

        case 2:
        {
            int target_id, new_id, value = 0;
            cout << "Введите <target_id>: ";
            cin >> target_id;
            cout << "Введите <new_id>: ";
            cin >> new_id;
            cout << "Введите <value>: ";
            cin >> value;

            insert_after(target_id, new_id, value);
            break;
        }

        case 3:
        {
            int id = 0;
            cout << "Введите <id>: ";
            cin >> id;

            move_to_front(id);
            break;
        }

        case 4:
        {
            int id_from, id_to = 0;
            cout << "Введите <id_from>: ";
            cin >> id_from;
            cout << "Введите <id_to>: ";
            cin >> id_to;

            splice_range(id_from, id_to);
            break;
        }

        case 5:
        {
            int id = 0;
            cout << "Введите <id>: ";
            cin >> id;
            get_value(id);
            break;
        }

        case 6:
        {
            int id = 0;
            cout << "Введите <id>: ";
            cin >> id;
            remove(id);
            break;
        }

        case 7:
        {
            cout << "=== СПИСОК (порядок выполнения) ===" << endl;
            task *cur = head;
            while (cur)
            {
                cout << "id=" << cur->id << " val=" << cur->value;
                if (cur->next)
                    cout << " -> ";
                cur = cur->next;
            }
            cout << endl;
            break;
        }

        case 8:
            cout << "Программа завершена" << endl;
            system("pause");
            return 0;

        default:
            cout << "Неверный ввод" << endl;
            break;
        }
    }
}
