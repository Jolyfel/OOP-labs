#include <iostream>
#include <fstream>
#include <iomanip>
#include <windows.h>
#include <string>
#include "l1_class_header.h"
#define FILE_NAME "lab1_file.txt"

using namespace std;

Clothing* create_clothing_from_input() {
    int type;
    cout << "Тип: 1-Куртка 2-Брюки 3-Рубашка: ";
    cin >> type;
    cin.ignore(10000, '\n');

    string name, brand, size, color, extra;
    double price;
    int quantity;

    cout << "Название: ";  
    getline(cin, name);
    cout << "Бренд: ";     
    getline(cin, brand);
    cout << "Размер: ";    
    getline(cin, size);
    cout << "Цвет: ";      
    getline(cin, color);
    cout << "Цена: ";      
    while (!(cin >> price)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Неверный ввод. Введите целое число: ";
    }
    cin.ignore(10000, '\n');
    cout << "Количество: "; 
    while (!(cin >> quantity)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Неверный ввод. Введите целое число: ";
    }
    cin.ignore(10000, '\n');

    if (type == 1) {
        cout << "Сезон: "; getline(cin, extra);
        return new Jacket(name, brand, size, color, price, quantity, extra);
    }
    else if (type == 2) {
        cout << "Посадка: "; getline(cin, extra);
        return new Trousers(name, brand, size, color, price, quantity, extra);
    }
    else {
        cout << "Тип рукава: "; getline(cin, extra);
        return new Shirt(name, brand, size, color, price, quantity, extra);
    }
}

void edit_clothing_fields(Clothing* item) {
    cout << "Что редактировать?\n";
    cout << "1. Название\n2. Бренд\n3. Размер\n4. Цвет\n5. Цену\n6. Количество\n";
    cout << "0. Отмена\n";
    cout << "Выбор: ";
    int c; cin >> c; cin.ignore(10000, '\n');

    string s; double p; int q;
    switch (c) {
    case 1: cout << "Новое название: "; 
        getline(cin, s); item->set_name(s); break;
    case 2: cout << "Новый бренд: ";    
        getline(cin, s); item->set_brand(s); break;
    case 3: cout << "Новый размер: ";   
        getline(cin, s); item->set_size(s); break;
    case 4: cout << "Новый цвет: ";     
        getline(cin, s); item->set_color(s); break;
    case 5: cout << "Новая цена: ";     
        cin >> p; cin.ignore(10000, '\n'); item->set_price(p); break;
    case 6: cout << "Новое кол-во: ";   
        cin >> q; cin.ignore(10000, '\n'); item->set_quantity(q); break;
    default: cout << "Отмена.\n"; 
        break;
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Shop shop("Одежда/by");

    shop.add_to_catalog(new Jacket("Зимняя куртка", "Nike", "L", "Черный", 12000, 5, "Зима"));
    shop.add_to_catalog(new Trousers("Джинсы", "Levi's", "32", "Синий", 5000, 10, "Regular"));
    shop.add_to_catalog(new Shirt("Рубашка", "H&M", "M", "Белый", 2500, 20, "Длинный"));

    int choice = 0;
    bool sort_default = true;

    do {
        system("cls");
        cout << "==========Меню магазина одежды===========\n";
        cout << "1. Сделать заказ\n";
        cout << "2. Просмотреть каталог\n";
        cout << "3. Просмотреть информацию о заказах\n";
        cout << "4. Изменить название магазина\n";
        cout << "5. Управление каталогом\n";
        cout << "6. Управление заказом\n";
        cout << "7. Сортировать по цене\n";
        cout << "0. Сохранить и выйти\n";
        cout << "=========================================\n\n";
        cout << "Ваш выбор: ";

        cin >> choice;
        if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); choice = -1; }

        switch (choice) {
        case 1: {
            system("cls");
            cin.ignore(10000, '\n');

            string name, phone, address;
            int card_id;

            cout << "Введите имя: ";        
            getline(cin, name);
            cout << "Введите телефон: ";    
            getline(cin, phone);
            cout << "Введите адрес: ";      
            getline(cin, address);
            cout << "Введите номер карты: "; 
            while (!(cin >> card_id)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Неверный ввод. Введите целое число: ";
            }
            cin.ignore(10000, '\n');

            Customer customer(name, phone, address, card_id);
            shop.add_order(customer);

            int order_idx = shop.get_orders_count() - 1;
            Order& order = shop.get_order(order_idx);

            char more = 'y';
            while (more == 'y' || more == 'Y') {
                shop.show_catalog(cout);
                cout << "Выберите номер товара (Выход - 0): ";
                int item_idx; cin >> item_idx;

                if (item_idx == 0) {
                    break;
                }
                else if (item_idx > 0 && item_idx <= shop.get_catalog_count()) {
                    Clothing* item = shop.get_catalog_item(item_idx - 1);
                    if (item->get_quantity() > 0) {
                        order.add_item(item);
                        item->set_quantity(item->get_quantity() - 1);
                        cout << "Товар добавлен.\n";
                    }
                    else {
                        cout << "Товара нет на складе.\n";
                    }
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
                cout << setprecision(2) << "Заказ оформлен. Итоговая сумма: "
                    << order.calculate_total() << endl;
                cout.unsetf(ios::fixed);
            }
            else {
                cout << "Заказ пуст. Отмена";
            }

            system("pause");
            break;
        }

        case 2: {
            system("cls");
            shop.show_catalog(cout);
            system("pause");
            break;
        }

        case 3: {
            system("cls");
            shop.show_orders(cout);
            system("pause");
            break;
        }

        case 4: {
            system("cls");
            cin.ignore(10000, '\n');
            string shopName;
            cout << "Введите название магазина: ";
            getline(cin, shopName);
            shop.set_shop_name(shopName);
            cout << "Название магазина обновлено.\n";
            system("pause");
            break;
        }

        case 5: {
            int sub = 0;
            do {
                system("cls");
                cout << "---Управление каталогом---\n";
                cout << "1. Добавить товар\n";
                cout << "2. Редактировать товар\n";
                cout << "3. Удалить товар\n";
                cout << "0. Назад\n";
                cout << "Выбор: ";
                cin >> sub;
                if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); sub = -1; }

                if (sub == 1) {
                    system("cls");
                    cin.ignore(10000, '\n');
                    Clothing* item = create_clothing_from_input();
                    shop.add_to_catalog(item);
                    cout << "Товар добавлен в каталог.\n";
                    system("pause");
                }
                else if (sub == 2) {
                    system("cls");
                    shop.show_catalog(cout);
                    if (shop.get_catalog_count() == 0) { system("pause"); continue; }
                    cout << "Введите номер товара для редактирования: ";
                    int idx; cin >> idx;
                    cin.ignore(10000, '\n');
                    if (idx > 0 && idx <= shop.get_catalog_count()) {
                        edit_clothing_fields(shop.get_catalog_item(idx - 1));
                        cout << "Товар изменён.\n";
                    }
                    else {
                        cout << "Неверный номер.\n";
                    }
                    system("pause");
                }
                else if (sub == 3) {
                    system("cls");
                    shop.show_catalog(cout);
                    if (shop.get_catalog_count() == 0) { system("pause"); continue; }
                    cout << "Введите номер товара для удаления: ";
                    int idx; cin >> idx;
                    if (idx > 0 && idx <= shop.get_catalog_count()) {
                        shop.remove_from_catalog(idx - 1);
                        cout << "Товар удалён.\n";
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
            if (shop.get_orders_count() == 0) {
                cout << "Заказов нет.\n";
                system("pause");
                break;
            }
            shop.show_orders(cout);
            cout << "Введите номер заказа: ";
            int order_idx; cin >> order_idx;
            if (order_idx < 1 || order_idx > shop.get_orders_count()) {
                cout << "Неверный номер.\n";
                system("pause");
                break;
            }
            Order& order = shop.get_order(order_idx - 1);

            int sub = 0;
            do {
                system("cls");
                cout << "---Управление заказом №" << order_idx << "---\n";
                order.print_receipt(cout);
                cout << "\n1. Добавить товар в заказ\n";
                cout << "2. Редактировать товар в заказе\n";
                cout << "3. Удалить товар из заказа\n";
                cout << "0. Назад\n";
                cout << "Выбор: ";
                cin >> sub;
                if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); sub = -1; }

                if (sub == 1) {
                    system("cls");
                    shop.show_catalog(cout);
                    if (shop.get_catalog_count() == 0) { system("pause"); continue; }
                    cout << "Введите номер товара: ";
                    int idx; cin >> idx;
                    if (idx > 0 && idx <= shop.get_catalog_count()) {
                        order.add_item(shop.get_catalog_item(idx - 1));
                        order.calculate_total();
                        cout << "Товар добавлен в заказ.\n";
                    }
                    else {
                        cout << "Неверный номер.\n";
                    }
                    system("pause");
                }
                else if (sub == 2) {
                    system("cls");
                    order.print_receipt(cout);
                    if (order.get_item_count() == 0) { system("pause"); continue; }
                    cout << "Введите номер позиции заказа для редактирования: ";
                    int idx; cin >> idx;
                    if (idx > 0 && idx <= order.get_item_count()) {
                        shop.show_catalog(cout);
                        cout << "Введите номер нового товара из каталога: ";
                        int new_idx; cin >> new_idx;
                        if (new_idx > 0 && new_idx <= shop.get_catalog_count()) {
                            order.edit_item(idx - 1, shop.get_catalog_item(new_idx - 1));
                            order.calculate_total();
                            cout << "Позиция заказа обновлена.\n";
                        }
                        else {
                            cout << "Неверный номер товара.\n";
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
                    if (order.get_item_count() == 0) { system("pause"); continue; }
                    cout << "Введите номер позиции для удаления: ";
                    int idx; cin >> idx;
                    if (idx > 0 && idx <= order.get_item_count()) {
                        order.remove_item(idx - 1);
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

        case 7: {
            system("cls");
            shop.sort_catalog_by_price(sort_default);
            cout << "Каталог отсортирован по цене (по возрастанию).\n\n";
            shop.show_catalog(cout);
            system("pause");
            break;
        }

        case 0: {
            system("cls");
            if (shop.save_all_to_file(FILE_NAME))
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