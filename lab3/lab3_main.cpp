#include <cstdlib>
#include <iostream>
#include <string>
#include <windows.h>
#include "l3_class_header.h"

#define FILE_NAME "lab3_file.txt"

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
static void result(bool ok) {
    cout << (ok ? "Готово.\n" : "Ошибка: объект не найден или данные некорректны.\n");
}

static void elements_menu(UIManager& m) {
    int c;
    do {
        cout << "\n--- ЭЛЕМЕНТЫ UI (vector) ---\n"
            "1. Показать все\n2. Добавить\n3. Удалить\n4. Редактировать\n"
            "5. Поиск по названию\n6. Фильтр по типу\n7. Фильтр по окну\n"
            "8. Сортировка\n9. Сохранить в файл\n0. Назад\n";
        c = read_int("Выбор: ");
        switch (c) {
        case 1: m.print(cout, 1); break;
        case 2: {
            int t = read_int("Тип (1-Button, 2-TextField, 3-Label): ");
            string n = read_str("Название: ");
            int w = read_int("Ширина: "), h = read_int("Высота: ");
            string ex = read_str("Текст/подсказка: ");
            string st = read_str("Стиль (название из списка стилей): ");
            result(m.add_element(t, n, w, h, ex, st));
            break;
        }
        case 3: result(m.remove_element(read_int("ID: "))); break;
        case 4: {
            int id = read_int("ID: ");
            cout << "(Enter - оставить без изменений)\n";
            string n = read_str("Новое название: ");
            int w = read_int("Ширина: "), h = read_int("Высота: ");
            string ex = read_str("Текст/подсказка: ");
            string st = read_str("Стиль: ");
            result(m.edit_element(id, n, w, h, ex, st));
            break;
        }
        case 5: UIManager::print_element_table(cout, m.find_elements(read_str("Часть названия: "))); break;
        case 6: UIManager::print_element_table(cout, m.filter_by_type(read_int("Тип (1-Button, 2-TextField, 3-Label): "))); break;
        case 7: UIManager::print_element_table(cout, m.filter_by_window(read_str("Имя окна: "))); break;
        case 8:
            m.sort_elements(read_int("По: 1-ID 2-название 3-тип 4-площадь: "));
            m.print(cout, 1);
            break;
        case 9: result(m.save_to_file(read_str("Имя файла: "), 1)); break;
        case 0: break;
        default: cout << "Неверный пункт.\n";
        }
    } while (c != 0);
}

static void windows_menu(UIManager& m) {
    int c;
    do {
        cout << "\n--- ОКНА (list) ---\n"
            "1. Показать все\n2. Добавить\n3. Удалить\n4. Редактировать\n"
            "5. Добавить элемент в окно\n6. Убрать элемент из окна\n"
            "7. Поиск по имени\n8. Сортировка\n9. Сохранить в файл\n0. Назад\n";
        c = read_int("Выбор: ");
        switch (c) {
        case 1: m.print(cout, 2); break;
        case 2: {
            string n = read_str("Имя: ");
            result(m.add_window(n, read_str("Заголовок: ")));
            break;
        }
        case 3: result(m.remove_window(read_str("Имя окна: "))); break;
        case 4: {
            string n = read_str("Имя окна: ");
            cout << "(Enter - оставить без изменений)\n";
            string nn = read_str("Новое имя: ");
            result(m.edit_window(n, nn, read_str("Новый заголовок: ")));
            break;
        }
        case 5: {
            string n = read_str("Имя окна: ");
            result(m.attach(n, read_int("ID элемента: ")));
            break;
        }
        case 6: {
            string n = read_str("Имя окна: ");
            result(m.detach(n, read_int("ID элемента: ")));
            break;
        }
        case 7: m.print_windows(cout, read_str("Часть имени: ")); break;
        case 8:
            m.sort_windows(read_int("По: 1-имя 2-кол-во элементов: "));
            m.print(cout, 2);
            break;
        case 9: result(m.save_to_file(read_str("Имя файла: "), 2)); break;
        case 0: break;
        default: cout << "Неверный пункт.\n";
        }
    } while (c != 0);
}

static void styles_menu(UIManager& m) {
    int c;
    do {
        cout << "\n--- СТИЛИ (array, 5 слотов) ---\n"
            "1. Показать все\n2. Добавить\n3. Удалить\n4. Редактировать\n"
            "5. Поиск по названию\n6. Сортировка\n7. Сохранить в файл\n0. Назад\n";
        c = read_int("Выбор: ");
        switch (c) {
        case 1: m.print(cout, 3); break;
        case 2: {
            string n = read_str("Название: "), col = read_str("Цвет: ");
            result(m.add_style(n, col, read_int("Размер шрифта: ")));
            break;
        }
        case 3: result(m.remove_style(read_int("Номер слота: "))); break;
        case 4: {
            int i = read_int("Номер слота: ");
            cout << "(Enter - оставить без изменений)\n";
            string n = read_str("Новое название: "), col = read_str("Новый цвет: ");
            result(m.edit_style(i, n, col, read_int("Размер шрифта: ")));
            break;
        }
        case 5: m.print_styles(cout, read_str("Часть названия: ")); break;
        case 6:
            m.sort_styles(read_int("По: 1-название 2-размер шрифта: "));
            m.print(cout, 3);
            break;
        case 7: result(m.save_to_file(read_str("Имя файла: "), 3)); break;
        case 0: break;
        default: cout << "Неверный пункт.\n";
        }
    } while (c != 0);
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    UIManager m;
    int c;
    do {
        cout << "\n===== УЧЁТ ЭЛЕМЕНТОВ UI =====\n"
            "1. Элементы (vector)\n2. Окна (list)\n3. Стили (array)\n"
            "4. Показать все данные\n5. Сохранить все данные в файл\n0. Выход\n";
        c = read_int("Выбор: ");
        switch (c) {
        case 1: elements_menu(m); break;
        case 2: windows_menu(m); break;
        case 3: styles_menu(m); break;
        case 4: m.print(cout, 4); break;
        case 5: result(m.save_to_file(FILE_NAME, 4)); break;
        case 0: break;
        default: cout << "Неверный пункт.\n";
        }
    } while (c != 0);
    return 0;
}