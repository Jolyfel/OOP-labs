#pragma once
#include <iostream>
#include <string>

class Order {
private:
	Customer customer;
	Clothing* items;
	std::string order_date;
	int order_id;
	double total_price;
public:

};

class Clothing {
protected:
	std::string name;
	std::string brand;
	std::string size;
	std::string color;
	double price;
	int quantity;
	
	virtual void print_info();
	virtual double get_final_price();
	virtual std::string get_category() = 0;
};

class Customer {

};