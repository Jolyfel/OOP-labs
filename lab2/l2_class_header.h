#pragma once
#include <iostream> 
#include <string>
#include <fstream>

using namespace std;

class Service;

class CarOwner {
    friend ostream& operator<<(ostream& os, const CarOwner& c);
    friend istream& operator>>(istream& is, CarOwner& c);
private:
    string full_name;
    string phone_number;
    string address;
    int car_number;

public:
    CarOwner();
    CarOwner(string full_name, string phone_number, string address, int car_number);
    void print_info(ostream& os);
    ~CarOwner();
};

class RepairOrder {
private:
    CarOwner owner;
    Service** services;
    string order_date;
    int order_id;
    double total_price;
    int service_count;
    int capacity;

public:
    RepairOrder();
    RepairOrder(CarOwner owner, string order_date, int order_id);
    ~RepairOrder();

    void add_service(Service* service);
    void remove_service(int index);
    void edit_service(int index, Service* new_service);
    double calculate_total();
    void print_receipt(ostream& os);
    bool save_to_file(const string& filename);
    void clear();

    int get_service_count() const { return service_count; }
    Service* get_service(int index) const { return services[index]; }
};

class Service {
protected:
    string name;
    string category;
    string duration;
    string complexity;
    double price;
    int slots;

public:
    Service();
    Service(string name, string category, string duration, string complexity, double price, int slots);

    virtual void print_info();
    virtual string get_type() const = 0;
    virtual ~Service();

    string get_name() const { return name; }
    string get_category() const { return category; }
    string get_duration() const { return duration; }
    string get_complexity() const { return complexity; }
    double get_price() const { return price; }
    int get_slots() const { return slots; }

    void set_name(const string& set) { name = set; }
    void set_category(const string& set) { category = set; }
    void set_duration(const string& set) { duration = set; }
    void set_complexity(const string& set) { complexity = set; }
    void set_price(double set) { price = set; }
    void set_slots(int set) { slots = set; }
};

ostream& operator<<(ostream& os, const Service& s);
istream& operator>>(istream& is, Service*& s);

class Diagnostics : public Service {
private:
    string equipment;

public:
    Diagnostics();
    Diagnostics(string name, string category, string duration, string complexity,
        double price, int slots, string equipment);

    void print_info() override;
    string get_type() const override;
    ~Diagnostics();

    string get_equipment() const { return equipment; }
    void set_equipment(const string& v) { equipment = v; }
};

class Maintenance : public Service {
private:
    string interval;

public:
    Maintenance();
    Maintenance(string name, string category, string duration, string complexity,
        double price, int slots, string interval);

    void print_info() override;
    string get_type() const override;
    ~Maintenance();

    string get_interval() const { return interval; }
    void set_interval(const string& v) { interval = v; }
};

class Repair : public Service {
private:
    string part_type;

public:
    Repair();
    Repair(string name, string category, string duration, string complexity,
        double price, int slots, string part_type);

    void print_info() override;
    string get_type() const override;
    ~Repair();

    string get_part_type() const { return part_type; }
    void set_part_type(const string& v) { part_type = v; }
};

class ServiceStation {
private:
    string station_name;
    Service** catalog;
    RepairOrder** orders;
    int catalog_count;
    int catalog_capacity;
    int orders_count;
    int orders_capacity;

public:
    ServiceStation();
    ServiceStation(string station_name);
    ~ServiceStation();

    void add_to_catalog(Service* service);
    void remove_from_catalog(int index);
    void edit_catalog_item(int index, Service* new_service);

    void add_order(const CarOwner& owner);
    void find_item(const string& name);
    void show_catalog(ostream& os);
    void show_orders(ostream& os);
    bool save_all_to_file(const string& filename);

    void set_station_name(const string& name) { station_name = name; }

    int get_catalog_count() const { return catalog_count; }
    Service* get_catalog_item(int index) const { return catalog[index]; }

    int get_orders_count() const { return orders_count; }
    RepairOrder& get_order(int index) const { return *orders[index]; }

    bool load_catalog_from_file(const string& filename);
    bool save_catalog_to_file(const string& filename);
    bool add_service_to_file(const string& filename, Service* service);
    bool delete_service_from_file(const string& filename, int index);
    bool edit_service_in_file(const string& filename, int index, Service* s);
    void view_file(const string& filename);
    void search_in_file(const string& filename, const string& name);
    long find_position(const string& filename, int index);
};