/*26. Разработать набор классов (минимум 5 классов, связи между классами: 
агрегация, композиция, наследование) по предметной области «Станция технического обслуживания автомобилей». 
Функционал программы должен позволить собрать данные об услуге по обслуживанию автомобиля. */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <windows.h>
#include "l1_class_header.h"

using namespace std;

CarOwner::CarOwner() : full_name(""), phone_number(""), address(""), car_number(0) {}

CarOwner::CarOwner(string full_name, string phone_number, string address, int car_number)
    : full_name(full_name), phone_number(phone_number), address(address), car_number(car_number) {
}

void CarOwner::print_info(ostream& os) {
    os << "Владелец: " << full_name
        << ", Телефон: " << phone_number
        << ", Адрес: " << address
        << ", Авто: " << car_number;
}

CarOwner::~CarOwner() {}

RepairOrder::RepairOrder()
    : owner(), services(nullptr), order_date(""), order_id(0),
    total_price(0), service_count(0), capacity(0) {
}

RepairOrder::RepairOrder(CarOwner owner, string order_date, int order_id)
    : owner(owner), services(nullptr), order_date(order_date),
    order_id(order_id), total_price(0), service_count(0), capacity(0) {
}

RepairOrder::~RepairOrder() {
    delete[] services;
}

void RepairOrder::add_service(Service* service) {
    if (service_count >= capacity) {
        int new_cap = (capacity == 0) ? 4 : capacity * 2;
        Service** new_services = new Service * [new_cap];
        for (int i = 0; i < service_count; ++i) new_services[i] = services[i];
        delete[] services;
        services = new_services;
        capacity = new_cap;
    }
    services[service_count++] = service;
}

void RepairOrder::remove_service(int index) {
    if (index < 0 || index >= service_count) return;
    for (int i = index; i < service_count - 1; ++i) services[i] = services[i + 1];
    --service_count;
}

void RepairOrder::edit_service(int index, Service* new_service) {
    if (index < 0 || index >= service_count || !new_service) return;
    services[index] = new_service;
}

double RepairOrder::calculate_total() {
    total_price = 0;
    for (int i = 0; i < service_count; ++i)
        if (services[i]) total_price += services[i]->get_price();
    return total_price;
}

void RepairOrder::print_receipt(ostream& os) {
    os << "Заказ-наряд №" << order_id << " от " << order_date << "\n";
    owner.print_info(os);
    os << "\nУслуги:\n";

    os.setf(ios::fixed, ios::floatfield);
    os << setprecision(2);
    os.setf(ios::showpoint);
    os.setf(ios::right, ios::adjustfield);

    os << setw(71) << setfill('-') << "" << setfill(' ') << endl
        << setw(7) << "№" << " | "
        << setw(22) << "НАЗВАНИЕ" << " | "
        << setw(17) << "ТИП" << " | "
        << setw(14) << "ЦЕНА" << " | " << endl
        << setw(71) << setfill('-') << "" << setfill(' ') << endl;

    for (int i = 0; i < service_count; ++i) {
        os << setw(7) << i + 1 << " | "
            << setw(22) << services[i]->get_name() << " | "
            << setw(17) << services[i]->get_type() << " | "
            << setw(14) << services[i]->get_price() << " | " << endl
            << setw(71) << setfill('-') << "" << setfill(' ') << endl;
    }

    os << "Итого: " << calculate_total() << endl;

    os.unsetf(ios::fixed);
    os.unsetf(ios::showpoint);
    os.unsetf(ios::right);
}

bool RepairOrder::save_to_file(const string& filename) {
    ofstream out(filename);
    if (!out) return false;
    print_receipt(out);
    out.close();
    return true;
}

void RepairOrder::clear() {
    service_count = 0;
    total_price = 0;
}

Service::Service()
    : name(""), category(""), duration(""), complexity(""), price(0), slots(0) {
}

Service::Service(string name, string category, string duration, string complexity,
    double price, int slots)
    : name(name), category(category), duration(duration), complexity(complexity),
    price(price), slots(slots) {
}

void Service::print_info() {
    cout << "Название: " << name
        << ", Категория: " << category
        << ", Длительность: " << duration
        << ", Сложность: " << complexity
        << ", Цена: " << price
        << ", Мест: " << slots;
}

Service::~Service() {}

Diagnostics::Diagnostics() : Service(), equipment("") {}

Diagnostics::Diagnostics(string name, string category, string duration, string complexity,
    double price, int slots, string equipment)
    : Service(name, category, duration, complexity, price, slots), equipment(equipment) {
}

void Diagnostics::print_info() { Service::print_info(); cout << ", Оборудование: " << equipment; }
string Diagnostics::get_type() { return "Диагностика"; }
Diagnostics::~Diagnostics() {}

Maintenance::Maintenance() : Service(), interval("") {}

Maintenance::Maintenance(string name, string category, string duration, string complexity,
    double price, int slots, string interval)
    : Service(name, category, duration, complexity, price, slots), interval(interval) {
}

void Maintenance::print_info() { Service::print_info(); cout << ", Интервал: " << interval; }
string Maintenance::get_type() { return "ТО"; }
Maintenance::~Maintenance() {}

Repair::Repair() : Service(), part_type("") {}

Repair::Repair(string name, string category, string duration, string complexity,
    double price, int slots, string part_type)
    : Service(name, category, duration, complexity, price, slots), part_type(part_type) {
}

void Repair::print_info() { Service::print_info(); cout << ", Деталь: " << part_type; }
string Repair::get_type() { return "Ремонт"; }
Repair::~Repair() {}

ServiceStation::ServiceStation()
    : station_name(""), catalog(nullptr), orders(nullptr),
    catalog_count(0), catalog_capacity(0),
    orders_count(0), orders_capacity(0) {
}

ServiceStation::ServiceStation(string station_name)
    : station_name(station_name), catalog(nullptr), orders(nullptr),
    catalog_count(0), catalog_capacity(0),
    orders_count(0), orders_capacity(0) {
}

ServiceStation::~ServiceStation() {
    for (int i = 0; i < catalog_count; ++i) delete catalog[i];
    delete[] catalog;
    for (int i = 0; i < orders_count; ++i) delete orders[i];
    delete[] orders;
}

void ServiceStation::add_to_catalog(Service* service) {
    if (catalog_count >= catalog_capacity) {
        int new_cap = (catalog_capacity == 0) ? 4 : catalog_capacity * 2;
        Service** new_catalog = new Service * [new_cap];
        for (int i = 0; i < catalog_count; ++i) new_catalog[i] = catalog[i];
        delete[] catalog;
        catalog = new_catalog;
        catalog_capacity = new_cap;
    }
    catalog[catalog_count++] = service;
}

void ServiceStation::remove_from_catalog(int index) {
    if (index < 0 || index >= catalog_count) return;
    delete catalog[index];
    for (int i = index; i < catalog_count - 1; ++i) catalog[i] = catalog[i + 1];
    --catalog_count;
}

void ServiceStation::edit_catalog_item(int index, Service* new_service) {
    if (index < 0 || index >= catalog_count || !new_service) return;
    delete catalog[index];
    catalog[index] = new_service;
}

void ServiceStation::add_order(const CarOwner& owner) {
    if (orders_count >= orders_capacity) {
        int new_cap = (orders_capacity == 0) ? 4 : orders_capacity * 2;
        RepairOrder** new_orders = new RepairOrder * [new_cap];
        for (int i = 0; i < orders_count; ++i) new_orders[i] = orders[i];
        delete[] orders;
        orders = new_orders;
        orders_capacity = new_cap;
    }
    string date;
    getline(cin, date);
    orders[orders_count] = new RepairOrder(owner, date, orders_count + 1);
    ++orders_count;
}

void ServiceStation::find_item(const string& name) {
    bool found = false;
    for (int i = 0; i < catalog_count; ++i) {
        if (catalog[i]->get_name() == name) {
            catalog[i]->print_info();
            cout << endl;
            found = true;
        }
    }
    if (!found) cout << "Услуга не найдена.\n";
}

void ServiceStation::show_catalog(ostream& os) {
    os.setf(ios::fixed, ios::floatfield);
    os << setprecision(2);
    os.setf(ios::showpoint);
    os.setf(ios::right, ios::adjustfield);

    os << setw(120) << setfill('-') << "" << setfill(' ') << endl
        << setw(8) << "№ | "
        << setw(23) << "НАЗВАНИЕ | "
        << setw(18) << "КАТЕГОРИЯ | "
        << setw(13) << "ДЛИТ. | "
        << setw(13) << "СЛОЖН. | "
        << setw(15) << "ЦЕНА | "
        << setw(14) << "МЕСТ | "
        << setw(16) << "ТИП |" << endl
        << setw(120) << setfill('-') << "" << setfill(' ') << endl;

    for (int i = 0; i < catalog_count; ++i) {
        os << setw(5) << i + 1 << " | "
            << setw(20) << catalog[i]->get_name() << " | "
            << setw(15) << catalog[i]->get_category() << " | "
            << setw(10) << catalog[i]->get_duration() << " | "
            << setw(10) << catalog[i]->get_complexity() << " | "
            << setw(12) << catalog[i]->get_price() << " | "
            << setw(10) << catalog[i]->get_slots() << " | "
            << setw(15) << catalog[i]->get_type() << " |" << endl
            << setw(120) << setfill('-') << "" << setfill(' ') << endl;
    }

    os.unsetf(ios::fixed);
    os.unsetf(ios::showpoint);
    os.unsetf(ios::right);
}

void ServiceStation::show_orders(ostream& os) {
    if (orders_count == 0) { os << "Заказ-нарядов нет.\n"; return; }
    for (int i = 0; i < orders_count; ++i) {
        orders[i]->print_receipt(os);
        os << "-------------------\n";
    }
}

bool ServiceStation::save_all_to_file(const string& filename) {
    ofstream out(filename);
    if (!out) return false;
    out << "СТО: " << station_name << "\n\n";
    out << "КАТАЛОГ УСЛУГ:\n";
    show_catalog(out);
    out << "\nЗАКАЗ-НАРЯДЫ:\n";
    show_orders(out);
    out.close();

    ifstream in(filename);
    if (!in) return false;
    string line;
    while (getline(in, line)) {}
    if (in.bad()) cout << "Проверка потока: bad.\n";
    else if (in.eof()) cout << "Проверка потока: eof.\n";
    else if (in.fail()) cout << "Проверка потока: fail.\n";
    else if (in.good()) cout << "Проверка потока: good.\n";
    in.clear();
    return true;
}