#define _WIN32_WINNT 0x0A00

#include <iostream>
#include <string>
#include <cstdlib>
#include "httplib.h"

using namespace std;

// PRODUCT MANAGEMENT

int id[100], quantity[100], price[100];
string name[100];
int productCount = 0;

// ORDER MANAGEMENT

int orderId[100];
int orderProductId[100];
int orderQuantity[100];
string orderStatus[100];

int orderCount = 0;
int nextOrderId = 1001;

// FIND PRODUCT BY ID

int findProduct(int searchId) {
    for (int i = 0; i < productCount; i++) {
        if (id[i] == searchId) {
            return i;
        }
    }
    return -1;
}

// GET INTEGER FROM WEBSITE FORM

bool getNumber(const httplib::Request& req,
               const string& key, int& value) {
    if (!req.has_param(key)) {
        return false;
    }

    try {
        value = stoi(req.get_param_value(key));
        return true;
    }
    catch (...) {
        return false;
    }
}

// ADD PRODUCT

void addProduct(const httplib::Request& req,
                httplib::Response& res) {

    if (productCount >= 100) {
        res.set_content("Product storage is full!", "text/plain");
        return;
    }

    int productChoice, newQuantity;

    if (!getNumber(req, "product", productChoice) ||
        !getNumber(req, "quantity", newQuantity)) {
        res.set_content("Please enter valid product and quantity.",
                        "text/plain");
        return;
    }

    if (productChoice < 1 || productChoice > 20) {
        res.set_content("Invalid product number!", "text/plain");
        return;
    }

    if (newQuantity <= 0) {
        res.set_content("Quantity must be greater than zero.",
                        "text/plain");
        return;
    }

    switch (productChoice) {
        case 1:  name[productCount] = "Notebook"; price[productCount] = 50; break;
        case 2:  name[productCount] = "Pen"; price[productCount] = 10; break;
        case 3:  name[productCount] = "Pencil"; price[productCount] = 5; break;
        case 4:  name[productCount] = "Eraser"; price[productCount] = 5; break;
        case 5:  name[productCount] = "Book"; price[productCount] = 40; break;
        case 6:  name[productCount] = "School Bag"; price[productCount] = 500; break;
        case 7:  name[productCount] = "Water Bottle"; price[productCount] = 150; break;
        case 8:  name[productCount] = "Scale"; price[productCount] = 10; break;
        case 9:  name[productCount] = "Sharpener"; price[productCount] = 5; break;
        case 10: name[productCount] = "Marker"; price[productCount] = 30; break;
        case 11: name[productCount] = "File"; price[productCount] = 25; break;
        case 12: name[productCount] = "Stapler"; price[productCount] = 60; break;
        case 13: name[productCount] = "Glue"; price[productCount] = 20; break;
        case 14: name[productCount] = "Scissors"; price[productCount] = 40; break;
        case 15: name[productCount] = "Calculator"; price[productCount] = 300; break;
        case 16: name[productCount] = "Register"; price[productCount] = 80; break;
        case 17: name[productCount] = "Highlighter"; price[productCount] = 25; break;
        case 18: name[productCount] = "Geometry Box"; price[productCount] = 100; break;
        case 19: name[productCount] = "Paper"; price[productCount] = 2; break;
        case 20: name[productCount] = "Clipboard"; price[productCount] = 70; break;
    }

    id[productCount] = 101 + productCount;
    quantity[productCount] = newQuantity;

    ostringstream output;
    output << "Product added successfully!\n"
           << "Product ID: " << id[productCount] << "\n"
           << "Product Name: " << name[productCount] << "\n"
           << "Price: Rs. " << price[productCount];

    productCount++;

    res.set_content(output.str(), "text/plain");
}

// VIEW PRODUCTS

void viewProducts(httplib::Response& res) {

    if (productCount == 0) {
        res.set_content("<p>No products available. Add a product first.</p>",
                        "text/html");
        return;
    }

    ostringstream output;

    output << "<table border='1' cellpadding='8' cellspacing='0'>"
           << "<tr><th>ID</th><th>Name</th><th>Price</th>"
           << "<th>Quantity</th></tr>";

    for (int i = 0; i < productCount; i++) {
        output << "<tr><td>" << id[i] << "</td>"
               << "<td>" << name[i] << "</td>"
               << "<td>Rs. " << price[i] << "</td>"
               << "<td>" << quantity[i] << "</td></tr>";
    }

    output << "</table>";

    res.set_content(output.str(), "text/html");
}

// SEARCH PRODUCT

void searchProduct(const httplib::Request& req,
                   httplib::Response& res) {

    int searchId;

    if (!getNumber(req, "id", searchId)) {
        res.set_content("Please enter a valid Product ID.",
                        "text/plain");
        return;
    }

    int index = findProduct(searchId);

    if (index == -1) {
        res.set_content("Product ID not found.", "text/plain");
        return;
    }

    ostringstream output;

    output << "Product Found!\n"
           << "Product ID: " << id[index] << "\n"
           << "Product Name: " << name[index] << "\n"
           << "Price: Rs. " << price[index] << "\n"
           << "Quantity: " << quantity[index];

    res.set_content(output.str(), "text/plain");
}

// UPDATE QUANTITY

void updateQuantity(const httplib::Request& req,
                    httplib::Response& res) {

    int searchId, newQuantity;

    if (!getNumber(req, "id", searchId) ||
        !getNumber(req, "quantity", newQuantity)) {
        res.set_content("Please enter valid details.", "text/plain");
        return;
    }

    if (newQuantity < 0) {
        res.set_content("Quantity cannot be negative.", "text/plain");
        return;
    }

    int index = findProduct(searchId);

    if (index == -1) {
        res.set_content("Product ID not found.", "text/plain");
        return;
    }

    quantity[index] = newQuantity;

    ostringstream output;
    output << "Quantity updated successfully!\n"
           << "Product: " << name[index] << "\n"
           << "New Quantity: " << quantity[index];

    res.set_content(output.str(), "text/plain");
}

// PLACE ORDER

void placeOrder(const httplib::Request& req,
                httplib::Response& res) {

    if (productCount == 0) {
        res.set_content("No products available. Add a product first.",
                        "text/plain");
        return;
    }

    if (orderCount >= 100) {
        res.set_content("Order storage is full!", "text/plain");
        return;
    }

    int productId, qty;

    if (!getNumber(req, "id", productId) ||
        !getNumber(req, "quantity", qty)) {
        res.set_content("Please enter valid order details.",
                        "text/plain");
        return;
    }

    int index = findProduct(productId);

    if (index == -1) {
        res.set_content("Product ID not found.", "text/plain");
        return;
    }

    if (qty <= 0) {
        res.set_content("Quantity must be greater than zero.",
                        "text/plain");
        return;
    }

    if (qty > quantity[index]) {
        res.set_content("Insufficient stock!", "text/plain");
        return;
    }

    orderId[orderCount] = nextOrderId;
    orderProductId[orderCount] = productId;
    orderQuantity[orderCount] = qty;
    orderStatus[orderCount] = "Placed";

    quantity[index] = quantity[index] - qty;

    ostringstream output;

    output << "Order placed successfully!\n"
           << "Order ID: " << orderId[orderCount] << "\n"
           << "Product Name: " << name[index] << "\n"
           << "Order Quantity: " << qty << "\n"
           << "Total Amount: Rs. " << price[index] * qty << "\n"
           << "Remaining Stock: " << quantity[index];

    orderCount++;
    nextOrderId++;

    res.set_content(output.str(), "text/plain");
}

// TRACK ORDERS

void trackOrders(httplib::Response& res) {

    if (orderCount == 0) {
        res.set_content("<p>No orders available.</p>", "text/html");
        return;
    }

    ostringstream output;

    output << "<table border='1' cellpadding='8' cellspacing='0'>"
           << "<tr><th>Order ID</th><th>Product ID</th>"
           << "<th>Quantity</th><th>Status</th></tr>";

    for (int i = 0; i < orderCount; i++) {
        output << "<tr><td>" << orderId[i] << "</td>"
               << "<td>" << orderProductId[i] << "</td>"
               << "<td>" << orderQuantity[i] << "</td>"
               << "<td>" << orderStatus[i] << "</td></tr>";
    }

    output << "</table>";

    res.set_content(output.str(), "text/html");
}

// MAIN HTTP SERVER

int main() {

    httplib::Server server;

    // ADD PRODUCT
    server.Post("/api/add", [](const httplib::Request& req,
                               httplib::Response& res) {
        addProduct(req, res);
    });

    // VIEW PRODUCTS
    server.Get("/api/view", [](const httplib::Request& req,
                               httplib::Response& res) {
        viewProducts(res);
    });

    // SEARCH PRODUCT
    server.Post("/api/search", [](const httplib::Request& req,
                                  httplib::Response& res) {
        searchProduct(req, res);
    });

    // UPDATE QUANTITY
    server.Post("/api/update", [](const httplib::Request& req,
                                  httplib::Response& res) {
        updateQuantity(req, res);
    });

    // PLACE ORDER
    server.Post("/api/order", [](const httplib::Request& req,
                                 httplib::Response& res) {
        placeOrder(req, res);
    });

    // TRACK ORDERS
    server.Get("/api/track", [](const httplib::Request& req,
                                httplib::Response& res) {
        trackOrders(res);
    });

    // HEALTH CHECK
    server.Get("/api/health", [](const httplib::Request& req,
                                 httplib::Response& res) {
        res.set_content("Inventory Server is running!",
                        "text/plain");
    });

    // WEBSITE FILES
    if (!server.set_mount_point("/", "./public")) {
        cout << "Error: Public folder not found!" << endl;
        return 1;
    }

    // PORT FOR LOCAL COMPUTER AND RENDER
    const char* portValue = getenv("PORT");
    int port = portValue ? atoi(portValue) : 8080;

    cout << "Inventory Management Server is starting..." << endl;

    if (!server.listen("0.0.0.0", port)) {
        cout << "Server could not start on port "
             << port << endl;
        return 1;
    }

    return 0;
}
