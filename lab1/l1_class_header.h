#pragma once
#include <iostream>

using namespace std;

class Clothing;

class Customer {
private:
    string name;
    string phone_number;
    string address;
    int card_id;

public:
    Customer();
    Customer(string name, string phone_number, string address, int card_id);
    void print_info(ostream& os);
    ~Customer();
};

class Order {
private:
    Customer customer;
    Clothing** items;
    string order_date;
    int order_id;
    double total_price;
    int item_count;
    int capacity;

public:
    Order();
    Order(Customer customer, string order_date, int order_id);
    ~Order();

    void add_item(Clothing* item);
    void remove_item(int index);
    void edit_item(int index, Clothing* new_item);
    double calculate_total();
    void print_receipt(ostream& os);
    bool save_to_file(const string& filename);
    void clear();

    int get_item_count() const { return item_count; }
    Clothing* get_item(int index) const { return items[index]; }
};

class Clothing {
protected:
    string name;
    string brand;
    string size;
    string color;
    double price;
    int quantity;

public:
    Clothing();
    Clothing(string name, string brand, string size, string color, double price, int quantity);

    virtual void print_info();
    virtual string get_category() = 0;
    virtual ~Clothing();

    string get_name() const { return name; }
    string get_brand() const { return brand; }
    string get_size() const { return size; }
    string get_color() const { return color; }
    double get_price() const { return price; }
    int get_quantity() const { return quantity; }

    void set_name(const string& set) { name = set; }
    void set_brand(const string& set) { brand = set; }
    void set_size(const string& set) { size = set; }
    void set_color(const string& set) { color = set; }
    void set_price(double set) { price = set; }
    void set_quantity(int set) { quantity = set; }
};

class Jacket : public Clothing {
private:
    string season;

public:
    Jacket();
    Jacket(string name, string brand, string size, string color,
        double price, int quantity, string season);

    void print_info() override;
    string get_category() override;
    ~Jacket();

    string get_season() const { return season; }
    void set_season(const string& v) { season = v; }
};

class Trousers : public Clothing {
private:
    string fit;

public:
    Trousers();
    Trousers(string name, string brand, string size, string color,
        double price, int quantity, string fit);

    void print_info() override;
    string get_category() override;
    ~Trousers();

    string get_fit() const { return fit; }
    void set_fit(const string& v) { fit = v; }
};

class Shirt : public Clothing {
private:
    string sleeve_type;

public:
    Shirt();
    Shirt(string name, string brand, string size, string color,
        double price, int quantity, string sleeve_type);

    void print_info() override;
    string get_category() override;
    ~Shirt();

    string get_sleeve_type() const { return sleeve_type; }
    void set_sleeve_type(const string& v) { sleeve_type = v; }
};

class Shop {
private:
    string shop_name;
    Clothing** catalog;
    Order** orders;
    int catalog_count;
    int catalog_capacity;
    int orders_count;
    int orders_capacity;

public:
    Shop();
    Shop(string shop_name);
    ~Shop();

    void add_to_catalog(Clothing* item);
    void remove_from_catalog(int index);
    void edit_catalog_item(int index, Clothing* newItem);

    void add_order(const Customer& customer);
    void find_item(const string& name);
    void show_catalog(ostream& os);
    void show_orders(ostream& os);
    bool save_all_to_file(const string& filename);

    void set_shop_name(const string& name) { shop_name = name; }

    int get_catalog_count() const { return catalog_count; }
    Clothing* get_catalog_item(int index) const { return catalog[index]; }

    int get_orders_count() const { return orders_count; }
    Order& get_order(int index) const { return *orders[index]; }
};