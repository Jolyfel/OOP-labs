#include <cstdlib>
#include <iostream>
#include <string>
#include <windows.h>
#include "l4_class_header.h"

#define FILE_NAME "lab4_file.txt"

using namespace std;

static string read_str(const string& prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    return s;
}

static int read_int(const string& prompt) {
    try { return stoi(read_str(prompt)); }
    catch (...) { return -1; }
}

static void show_result(bool ok) {
    cout << (ok ? "Готово.\n" : "Ошибка: данные некорректны или объект не найден.\n");
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    BugTracker tracker;
    int choice;
    do {
        cout << "\n===== УЧЁТ ОШИБОК В UI-ЭЛЕМЕНТАХ =====\n"
            "1. Ввести данные о найденных ошибках\n"
            "2. Рейтинг тестировщиков (от большего к меньшему)\n"
            "3. Найти тестировщика\n"
            "4. Найти UI-элемент\n"
            "5. Удалить тестировщика\n"
            "6. Показать все контейнеры\n"
            "7. Сохранить в файл\n"
            "0. Выход\n";
        choice = read_int("Выбор: ");
        switch (choice) {
        case 1: {
            string tester = read_str("Тестировщик: ");
            string element = read_str("UI-элемент: ");
            string type = read_str("Тип ошибки: ");
            show_result(tracker.add_report(tester, element, type, read_int("Количество ошибок: ")));
            break;
        }
        case 2: tracker.print_ranking(cout); break;
        case 3: tracker.print_tester(cout, read_str("Имя тестировщика: ")); break;
        case 4: tracker.print_element(cout, read_str("Название UI-элемента: ")); break;
        case 5: show_result(tracker.remove_tester(read_str("Имя тестировщика: "))); break;
        case 6: tracker.print_containers(cout); break;
        case 7: show_result(tracker.save_to_file(FILE_NAME)); break;
        case 0: break;
        default: cout << "Неверный пункт.\n";
        }
    } while (choice != 0);
    return 0;
}