/*18. Разработать набор классов (минимум 5 классов, связи между классами: 
агрегация, композиция, наследование) по предметной области «Магазин одежды». 
Функционал программы должен позволить собрать данные о заказе. */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <windows.h>
#include "l1_class_header.h"

using namespace std;

Customer::Customer() : name(""), phone_number(""), address(""), card_id(0) {}

Customer::Customer(string name, string phone_number, string address, int card_id)
    : name(name), phone_number(phone_number), address(address), card_id(card_id) {
}

void Customer::print_info(ostream& os) {
    os << "Имя: " << name
        << ", Телефон: " << phone_number
        << ", Адрес: " << address
        << ", Карта: " << card_id;
}

Customer::~Customer() {}

Order::Order()
    : customer(), items(nullptr), order_date(""), order_id(0),
    total_price(0), item_count(0), capacity(0) {
}

Order::Order(Customer customer, string order_date, int order_id)
    : customer(customer), items(nullptr), order_date(order_date),
    order_id(order_id), total_price(0), item_count(0), capacity(0) {
}

Order::~Order() {
    delete[] items;
}

void Order::add_item(Clothing* item) {
    if (item_count >= capacity) {
        int new_cap = (capacity == 0) ? 4 : capacity * 2;
        Clothing** new_items = new Clothing * [new_cap];
        for (int i = 0; i < item_count; ++i) new_items[i] = items[i];
        delete[] items;
        items = new_items;
        capacity = new_cap;
    }
    items[item_count++] = item;
}

void Order::remove_item(int index) {
    if (index < 0 || index >= item_count) return;
    for (int i = index; i < item_count - 1; ++i) items[i] = items[i + 1];
    --item_count;
}

void Order::edit_item(int index, Clothing* new_item) {
    if (index < 0 || index >= item_count || !new_item) return;
    items[index] = new_item;
}

double Order::calculate_total() {
    total_price = 0;
    for (int i = 0; i < item_count; ++i)
        if (items[i]) total_price += items[i]->get_price();
    return total_price;
}

void Order::print_receipt(ostream& os) {
    os << "Заказ №" << order_id << " от " << order_date << "\n";
    customer.print_info(os);
    os << "\nТовары:\n";

    os.setf(ios::fixed, ios::floatfield);
    os << setprecision(2);
    os.setf(ios::showpoint);
    os.setf(ios::right, ios::adjustfield);

    os << setw(71) << setfill('-') << "" << setfill(' ') << endl
        << setw(7) << "№" << " | "
        << setw(22) << "Название" << " | "
        << setw(17) << "Категория" << " | "
        << setw(14) << "Цена" << " | " << endl
        << setw(71) << setfill('-') << "" << setfill(' ') << endl;
    
    for (int i = 0; i < item_count; ++i) {
        os << setw(7) << i + 1 << " | "
            << setw(22) << items[i]->get_name() << " | "
            << setw(17) << items[i]->get_category() << " | "
            << setw(14) << items[i]->get_price() << " | " << endl
            << setw(71) << setfill('-') << "" << setfill(' ') << endl;
    }

    os << "Итого: " << calculate_total() << endl;

    os.unsetf(ios::fixed);
    os.unsetf(ios::showpoint);
    os.unsetf(ios::right);
}

bool Order::save_to_file(const string& filename) {
    ofstream out(filename);
    if (!out) return false;
    print_receipt(out);
    out.close();
    return true;
}

void Order::clear() {
    item_count = 0;
    total_price = 0;
}

Clothing::Clothing()
    : name(""), brand(""), size(""), color(""), price(0), quantity(0) {
}

Clothing::Clothing(string name, string brand, string size, string color,
    double price, int quantity)
    : name(name), brand(brand), size(size), color(color),
    price(price), quantity(quantity) {
}

void Clothing::print_info() {
    cout << "Название: " << name
        << ", Бренд: " << brand
        << ", Размер: " << size
        << ", Цвет: " << color
        << ", Цена: " << price
        << ", Количество: " << quantity;
}

Clothing::~Clothing() {}

Jacket::Jacket() : Clothing(), season("") {}

Jacket::Jacket(string name, string brand, string size, string color,
    double price, int quantity, string season)
    : Clothing(name, brand, size, color, price, quantity), season(season) {
}

void Jacket::print_info() { Clothing::print_info(); cout << ", Сезон: " << season; }
string Jacket::get_category() { return "Куртка"; }
Jacket::~Jacket() {}

Trousers::Trousers() : Clothing(), fit("") {}

Trousers::Trousers(string name, string brand, string size, string color,
    double price, int quantity, string fit)
    : Clothing(name, brand, size, color, price, quantity), fit(fit) {
}

void Trousers::print_info() { Clothing::print_info(); cout << ", Посадка: " << fit; }
string Trousers::get_category() { return "Брюки"; }
Trousers::~Trousers() {}

Shirt::Shirt() : Clothing(), sleeve_type("") {}

Shirt::Shirt(string name, string brand, string size, string color,
    double price, int quantity, string sleeve_type)
    : Clothing(name, brand, size, color, price, quantity),
    sleeve_type(sleeve_type) {
}

void Shirt::print_info() { Clothing::print_info(); cout << ", Рукав: " << sleeve_type; }
string Shirt::get_category() { return "Рубашка"; }
Shirt::~Shirt() {}

Shop::Shop()
    : shop_name(""), catalog(nullptr), orders(nullptr),
    catalog_count(0), catalog_capacity(0),
    orders_count(0), orders_capacity(0) {
}

Shop::Shop(string shop_name)
    : shop_name(shop_name), catalog(nullptr), orders(nullptr),
    catalog_count(0), catalog_capacity(0),
    orders_count(0), orders_capacity(0) {
}

Shop::~Shop() {
    for (int i = 0; i < catalog_count; ++i) delete catalog[i];
    delete[] catalog;
    for (int i = 0; i < orders_count; ++i) delete orders[i];
    delete[] orders;
}

void Shop::add_to_catalog(Clothing* item) {
    if (catalog_count >= catalog_capacity) {
        int new_cap = (catalog_capacity == 0) ? 4 : catalog_capacity * 2;
        Clothing** new_catalog = new Clothing * [new_cap];
        for (int i = 0; i < catalog_count; ++i) new_catalog[i] = catalog[i];
        delete[] catalog;
        catalog = new_catalog;
        catalog_capacity = new_cap;
    }
    catalog[catalog_count++] = item;
}

void Shop::remove_from_catalog(int index) {
    if (index < 0 || index >= catalog_count) return;
    delete catalog[index];
    for (int i = index; i < catalog_count - 1; ++i) catalog[i] = catalog[i + 1];
    --catalog_count;
}

void Shop::edit_catalog_item(int index, Clothing* new_item) {
    if (index < 0 || index >= catalog_count || !new_item) return;
    delete catalog[index];
    catalog[index] = new_item;
}

void Shop::add_order(const Customer& customer) {
    if (orders_count >= orders_capacity) {
        int new_cap = (orders_capacity == 0) ? 4 : orders_capacity * 2;
        Order** new_orders = new Order * [new_cap];
        for (int i = 0; i < orders_count; ++i) new_orders[i] = orders[i];
        delete[] orders;
        orders = new_orders;
        orders_capacity = new_cap;
    }
    string date;
    getline(cin, date);
    orders[orders_count] = new Order(customer, date, orders_count + 1);
    ++orders_count;
}

void Shop::find_item(const string& name) {
    bool found = false;
    for (int i = 0; i < catalog_count; ++i) {
        if (catalog[i]->get_name() == name) {
            catalog[i]->print_info();
            cout << endl;
            found = true;
        }
    }
    if (!found) cout << "Товар не найден.\n";
}

void Shop::show_catalog(ostream& os) {
    os.setf(ios::fixed, ios::floatfield);
    os << setprecision(2);
    os.setf(ios::showpoint);
    os.setf(ios::right, ios::adjustfield);

    os << setw(120) << setfill('-') << "" << setfill(' ') << endl
        << setw(8) << "№ | "
        << setw(23) << "Название | "
        << setw(18) << "Бренд | "
        << setw(13) << "Размер | "
        << setw(13) << "Цвет | "
        << setw(15) << "Цена | "
        << setw(14) << "Кол-во | "
        << setw(16) << "Категория |" << endl
        << setw(120) << setfill('-') << "" << setfill(' ') << endl;

    for (int i = 0; i < catalog_count; ++i) {
        os << setw(5) << i + 1 << " | "
            << setw(20) << catalog[i]->get_name() << " | "
            << setw(15) << catalog[i]->get_brand() << " | "
            << setw(10) << catalog[i]->get_size() << " | "
            << setw(10) << catalog[i]->get_color() << " | "
            << setw(12) << catalog[i]->get_price() << " | "
            << setw(10) << catalog[i]->get_quantity() << " | "
            << setw(15) << catalog[i]->get_category() << " |" << endl
            << setw(120) << setfill('-') << "" << setfill(' ') << endl;
    }

    os.unsetf(ios::fixed);
    os.unsetf(ios::showpoint);
    os.unsetf(ios::right);
}

void Shop::show_orders(ostream& os) {
    if (orders_count == 0) { os << "Заказов нет.\n"; return; }
    for (int i = 0; i < orders_count; ++i) {
        orders[i]->print_receipt(os);
        os << "-------------------\n";
    }
}

bool Shop::save_all_to_file(const string& filename) {
    ofstream out(filename);
    if (!out) return false;
    out << "Магазин: " << shop_name << "\n\n";
    out << "Каталог:\n";
    show_catalog(out);
    out << "\nЗаказы:\n";
    show_orders(out);
    out.close();

    ifstream in(filename);
    if (!in) return false;
    string line;
    while (getline(in, line)) {}
    if (in.bad())            cout << "Проверка потока: bad.\n";
    else if (in.eof())       cout << "Проверка потока: eof.\n";
    else if (in.fail())      cout << "Проверка потока: fail.\n";
    else if (in.good())      cout << "Проверка потока: good.\n";
    in.clear();
    return true;
}