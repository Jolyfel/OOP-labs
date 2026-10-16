#include <iostream>
#include <fstream>
#include <iomanip>
#include <windows.h>
#include "l1_class_header.h"
#define FILE_NAME "lab1_file.txt"

using namespace std;

Service* create_service_from_input() {
    int type;
    cout << "Тип: 1-Диагностика 2-ТО 3-Ремонт: ";
    cin >> type;
    cin.ignore(10000, '\n');

    string name, category, duration, complexity, extra;
    double price;
    int slots;

    cout << "Название: ";
    getline(cin, name);
    cout << "Категория: ";
    getline(cin, category);
    cout << "Длительность: ";
    getline(cin, duration);
    cout << "Сложность: ";
    getline(cin, complexity);
    cout << "Цена: ";
    cin >> price;
    cout << "Количество мест: ";
    cin >> slots;
    cin.ignore(10000, '\n');

    if (type == 1) {
        cout << "Оборудование: "; getline(cin, extra);
        return new Diagnostics(name, category, duration, complexity, price, slots, extra);
    }
    else if (type == 2) {
        cout << "Интервал: "; getline(cin, extra);
        return new Maintenance(name, category, duration, complexity, price, slots, extra);
    }
    else {
        cout << "Тип детали: "; getline(cin, extra);
        return new Repair(name, category, duration, complexity, price, slots, extra);
    }
}

void edit_service_fields(Service* item) {
    cout << "Что редактировать?\n";
    cout << "1. Название\n2. Категория\n3. Длительность\n4. Сложность\n5. Цену\n6. Количество мест\n";
    cout << "0. Отмена\n";
    cout << "Выбор: ";
    int c; cin >> c; cin.ignore(10000, '\n');

    string s; double p; int q;
    switch (c) {
    case 1: cout << "Новое название: ";
        getline(cin, s); item->set_name(s); break;
    case 2: cout << "Новая категория: ";
        getline(cin, s); item->set_category(s); break;
    case 3: cout << "Новая длительность: ";
        getline(cin, s); item->set_duration(s); break;
    case 4: cout << "Новая сложность: ";
        getline(cin, s); item->set_complexity(s); break;
    case 5: cout << "Новая цена: ";
        cin >> p;
        cin.ignore(10000, '\n'); item->set_price(p); break;
    case 6: cout << "Новое кол-во мест: ";
        cin >> q;
        cin.ignore(10000, '\n'); item->set_slots(q); break;
    default:
        cout << "Отмена.\n";
        break;
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    ServiceStation station("СТО АвтоМастер");

    station.add_to_catalog(new Diagnostics("Комп. диагностика", "Электрика", "30 мин", "Средняя", 1500, 3, "Сканер OBD-II"));
    station.add_to_catalog(new Maintenance("Замена масла", "Двигатель", "40 мин", "Низкая", 2500, 5, "каждые 10 000 км"));
    station.add_to_catalog(new Repair("Замена колодок", "Тормозная сист.", "1 час", "Средняя", 3000, 4, "колодки"));

    int choice = 0;

    do {
        system("cls");
        cout << "==========СТО: АвтоМастер===========\n";
        cout << "1. Оформить заказ-наряд\n";
        cout << "2. Просмотреть каталог услуг\n";
        cout << "3. Просмотреть информацию о заказ-нарядах\n";
        cout << "4. Изменить название СТО\n";
        cout << "5. Управление каталогом услуг\n";
        cout << "6. Управление заказ-нарядом\n";
        cout << "0. Сохранить и выйти\n";
        cout << "====================================\n\n";
        cout << "Ваш выбор: ";

        cin >> choice;
        if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); choice = -1; }

        switch (choice) {
        case 1: {
            system("cls");
            cin.ignore(10000, '\n');

            string name, phone, address;
            int car_number;

            cout << "Введите ФИО владельца: ";
            getline(cin, name);
            cout << "Введите телефон: ";
            getline(cin, phone);
            cout << "Введите адрес: ";
            getline(cin, address);
            cout << "Введите номер автомобиля: ";
            cin >> car_number;

            CarOwner owner(name, phone, address, car_number);
            station.add_order(owner);

            int order_idx = station.get_orders_count() - 1;
            RepairOrder& order = station.get_order(order_idx);

            char more = 'y';
            while (more == 'y' || more == 'Y') {
                station.show_catalog(cout);
                cout << "Выберите номер услуги (Выход - 0): ";
                int item_idx; cin >> item_idx;

                if (item_idx > 0 && item_idx <= station.get_catalog_count() && station.get_catalog_item(item_idx - 1)->get_slots() > 0) {
                    order.add_service(station.get_catalog_item(item_idx - 1));
                    station.get_catalog_item(item_idx - 1)->set_slots(station.get_catalog_item(item_idx - 1)->get_slots() - 1);
                    cout << "Услуга добавлена.\n";
                }

                else if (item_idx == 0) {
                    break;
                }

                else if (station.get_catalog_item(item_idx - 1)->get_slots() == 0) {
                    cout << "Свободных мест нет.\n";
                }

                else {
                    cout << "Неверный номер.\n";
                }

                cout << "Добавить ещё? (y/n): ";
                cin >> more;
            }

            order.calculate_total();
            if (order.calculate_total() > 0) {
                cout.setf(ios::fixed, ios::floatfield);
                cout << setprecision(2) << "Заказ-наряд оформлен. Итоговая сумма: "
                    << order.calculate_total() << endl;
                cout.unsetf(ios::fixed);
            }
            else cout << "Заказ-наряд пуст. Отмена";

            system("pause");
            break;
        }

        case 2: {
            system("cls");
            station.show_catalog(cout);
            system("pause");
            break;
        }

        case 3: {
            system("cls");
            station.show_orders(cout);
            system("pause");
            break;
        }

        case 4: {
            system("cls");
            cin.ignore(10000, '\n');
            string stationName;
            cout << "Введите название СТО: ";
            getline(cin, stationName);
            station.set_station_name(stationName);
            cout << "Название СТО обновлено.\n";
            system("pause");
            break;
        }

        case 5: {
            int sub = 0;
            do {
                system("cls");
                cout << "---Управление каталогом услуг---\n";
                cout << "1. Добавить услугу\n";
                cout << "2. Редактировать услугу\n";
                cout << "3. Удалить услугу\n";
                cout << "0. Назад\n";
                cout << "Выбор: ";
                cin >> sub;
                if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); sub = -1; }

                if (sub == 1) {
                    system("cls");
                    cin.ignore(10000, '\n');
                    Service* item = create_service_from_input();
                    station.add_to_catalog(item);
                    cout << "Услуга добавлена в каталог.\n";
                    system("pause");
                }
                else if (sub == 2) {
                    system("cls");
                    station.show_catalog(cout);
                    if (station.get_catalog_count() == 0) { system("pause"); continue; }
                    cout << "Введите номер услуги для редактирования: ";
                    int idx; cin >> idx;
                    cin.ignore(10000, '\n');
                    if (idx > 0 && idx <= station.get_catalog_count()) {
                        edit_service_fields(station.get_catalog_item(idx - 1));
                        cout << "Услуга изменена.\n";
                    }
                    else {
                        cout << "Неверный номер.\n";
                    }
                    system("pause");
                }
                else if (sub == 3) {
                    system("cls");
                    station.show_catalog(cout);
                    if (station.get_catalog_count() == 0) { system("pause"); continue; }
                    cout << "Введите номер услуги для удаления: ";
                    int idx; cin >> idx;
                    if (idx > 0 && idx <= station.get_catalog_count()) {
                        station.remove_from_catalog(idx - 1);
                        cout << "Услуга удалена.\n";
                    }
                    else {
                        cout << "Неверный номер.\n";
                    }
                    system("pause");
                }
            } while (sub != 0);
            break;
        }

        case 6: {
            system("cls");
            if (station.get_orders_count() == 0) {
                cout << "Заказ-нарядов нет.\n";
                system("pause");
                break;
            }
            station.show_orders(cout);
            cout << "Введите номер заказ-наряда: ";
            int order_idx; cin >> order_idx;
            if (order_idx < 1 || order_idx > station.get_orders_count()) {
                cout << "Неверный номер.\n";
                system("pause");
                break;
            }
            RepairOrder& order = station.get_order(order_idx - 1);

            int sub = 0;
            do {
                system("cls");
                cout << "---Управление заказ-нарядом №" << order_idx << "---\n";
                order.print_receipt(cout);
                cout << "\n1. Добавить услугу в заказ-наряд\n";
                cout << "2. Редактировать услугу в заказ-наряде\n";
                cout << "3. Удалить услугу из заказ-наряда\n";
                cout << "0. Назад\n";
                cout << "Выбор: ";
                cin >> sub;
                if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); sub = -1; }

                if (sub == 1) {
                    system("cls");
                    station.show_catalog(cout);
                    if (station.get_catalog_count() == 0) { system("pause"); continue; }
                    cout << "Введите номер услуги: ";
                    int idx; cin >> idx;
                    if (idx > 0 && idx <= station.get_catalog_count()) {
                        order.add_service(station.get_catalog_item(idx - 1));
                        order.calculate_total();
                        cout << "Услуга добавлена в заказ-наряд.\n";
                    }
                    else {
                        cout << "Неверный номер.\n";
                    }
                    system("pause");
                }
                else if (sub == 2) {
                    system("cls");
                    order.print_receipt(cout);
                    if (order.get_service_count() == 0) { system("pause"); continue; }
                    cout << "Введите номер позиции заказ-наряда для редактирования: ";
                    int idx; cin >> idx;
                    if (idx > 0 && idx <= order.get_service_count()) {
                        station.show_catalog(cout);
                        cout << "Введите номер новой услуги из каталога: ";
                        int new_idx; cin >> new_idx;
                        if (new_idx > 0 && new_idx <= station.get_catalog_count()) {
                            order.edit_service(idx - 1, station.get_catalog_item(new_idx - 1));
                            order.calculate_total();
                            cout << "Позиция заказ-наряда обновлена.\n";
                        }
                        else {
                            cout << "Неверный номер услуги.\n";
                        }
                    }
                    else {
                        cout << "Неверная позиция.\n";
                    }
                    system("pause");
                }
                else if (sub == 3) {
                    system("cls");
                    order.print_receipt(cout);
                    if (order.get_service_count() == 0) { system("pause"); continue; }
                    cout << "Введите номер позиции для удаления: ";
                    int idx; cin >> idx;
                    if (idx > 0 && idx <= order.get_service_count()) {
                        order.remove_service(idx - 1);
                        order.calculate_total();
                        cout << "Позиция удалена.\n";
                    }
                    else {
                        cout << "Неверный номер.\n";
                    }
                    system("pause");
                }
            } while (sub != 0);
            break;
        }

        case 0: {
            system("cls");
            if (station.save_all_to_file(FILE_NAME))
                cout << "Данные сохранены в " << FILE_NAME << endl;
            else
                cout << "Ошибка сохранения.\n";
            break;
        }

        default:
            cout << "Неверный выбор" << endl;
            system("pause");
            break;
        }
    } while (choice != 0);

    cout << "До свидания" << endl;
    return 0;
}