/* So i used c++ for the task because it's the most natural language to me specially for using oop
because i solved lots of problems with it and am used to it the most. But i can also use Java. I have created
some desktop applications with java and used it with sql alot. but for this task i wanted to do my best so i used c++.
 */

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

class Product {
public:
    string name;
    double price;
    int quantity;



    Product(string n, double p, int q) {
        name = n;
        price = p;
        quantity = q;
    }

    virtual bool isExpired() {
        return false;
    }

    virtual bool needsShipping() {
        return false;
    }

    virtual double getWeight() {
        return 0.0;
    }
};

class ExpirableProduct : public Product {
public:
    bool expired;

    ExpirableProduct(string n, double p, int q, bool e)
        : Product(n, p, q), expired(e) {}

    bool isExpired() override {
        return expired;
    }
};

class ShippableProduct : public Product {
public:
    double weight;

    ShippableProduct(string n, double p, int q, double w)
        : Product(n, p, q), weight(w) {}

    bool needsShipping() override {
        return true;
    }

    double getWeight() override {
        return weight;
    }
};

class ExpirableShippableProduct : public ShippableProduct {
public:
    bool expired;

    ExpirableShippableProduct(string n, double p, int q, double w, bool e)
        : ShippableProduct(n, p, q, w), expired(e) {}

    bool isExpired() override {
        return expired;
    }
};

class CartItem {
public:
    Product* product;
    int quantity;

    CartItem(Product* p, int q) {
        product = p;
        quantity = q;
    }
};

class Cart {
public:
    vector<CartItem> items;

    void add(Product* p, int qty) {
        if (qty <= 0 || qty > p->quantity) {
            cout << "Invalid quantity for " << p->name << endl;
            return;
        }

        for (auto& item : items) {
            if (item.product == p) {
                item.quantity += qty;
                return;
            }
        }
        items.push_back(CartItem(p, qty));
    }

    bool isEmpty() {
        return items.empty();
    }

    void clear() {
        items.clear();
    }
};

void checkout(string customerName, string paymentType, Cart& cart) {
    if (cart.isEmpty()) {
        cout << "Error: Cart is empty." << endl;
        return;
    }

    double subtotal = 0.0;
    double shippingFee = 0.0;
    double shippingRate = 10.0; // like 10 dollar for a kg
    double totalWeight = 0.0;

    cout << "** Shipment notice **" << endl;

    for (auto& item : cart.items) {
        Product* p = item.product;

        if (p->isExpired()) {
            cout << "Error: " << p->name << " is expired." << endl;
            return;
        }

        if (item.quantity > p->quantity) {
            cout << "Error: Not enough stock for " << p->name << endl;
            return;
        }

        if (p->needsShipping()) {
            double w = p->getWeight() * item.quantity;
            totalWeight += w;
            cout << item.quantity << "x " << p->name << " "
                 << fixed << setprecision(0) << w * 1000 << "g" << endl;
        }
    }

    if (totalWeight > 0)
        cout << "Total package weight " << fixed << setprecision(1) << totalWeight << "kg" << endl;

    cout << endl << "** Checkout receipt **" << endl;

    for (auto& item : cart.items) {
        Product* p = item.product;
        double totalPrice = p->price * item.quantity;
        subtotal += totalPrice;

        cout << item.quantity << "x " << p->name << " "
             << fixed << setprecision(0) << totalPrice << endl;

        p->quantity -= item.quantity;
    }

    shippingFee = totalWeight * shippingRate;
    double total = subtotal + shippingFee;

    cout << "----------------------" << endl;
    cout << "Subtotal " << fixed << setprecision(0) << subtotal << endl;
    cout << "Shipping " << fixed << setprecision(0) << shippingFee << endl;
    cout << "Amount " << fixed << setprecision(0) << total << endl;
    cout << "Payment method: " << paymentType << endl;

    cart.clear();
}


int main() {

    ExpirableShippableProduct cheese("Cheese", 100, 5, 0.2, false);
    ExpirableShippableProduct biscuits("Biscuits", 150, 5, 0.7, false);
    ShippableProduct tv("TV", 800, 3, 10.0);
    Product scratchCard("Scratch Card", 1, 100);


    Cart cart;
    string customerName = "Heba";
    string paymentType;

    cart.add(&cheese, 2);
    cart.add(&biscuits, 1);
    cart.add(&scratchCard, 5);
    cart.add(&tv, 1);

    cout << "Enter payment method (Online or Cash on Delivery): ";
    getline(cin, paymentType);

    if (paymentType != "Online" && paymentType != "Cash on Delivery") {
        cout << "Invalid payment method." << endl;
        return 0;
    }

    checkout(customerName, paymentType, cart);

    return 0;
}
