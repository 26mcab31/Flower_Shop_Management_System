
#include <iostream>
#include "OrderQueue.h"

using namespace std;

// Constructor
OrderQueue::OrderQueue()
{
    front = nullptr;
    rear = nullptr;
}

// Destructor: release all remaining queue nodes
OrderQueue::~OrderQueue()
{
    while (front != nullptr)
    {
        OrderNode* temp = front;
        front = front->next;
        delete temp;
    }

    rear = nullptr;
}

// Add an order from the website or backend
bool OrderQueue::addOrder(const Order& order)
{
    if (order.orderId <= 0 ||
        order.customerName.empty() ||
        order.flowerName.empty() ||
        order.quantity <= 0)
    {
        return false;
    }

    // Prevent duplicate order IDs among pending orders
    OrderNode* current = front;

    while (current != nullptr)
    {
        if (current->data.orderId == order.orderId)
        {
            return false;
        }

        current = current->next;
    }

    OrderNode* newNode = new OrderNode;
    newNode->data = order;
    newNode->next = nullptr;

    if (rear == nullptr)
    {
        front = rear = newNode;
    }
    else
    {
        rear->next = newNode;
        rear = newNode;
    }

    return true;
}

// Return all pending orders for the website
vector<Order> OrderQueue::getPendingOrders() const
{
    vector<Order> orders;
    OrderNode* current = front;

    while (current != nullptr)
    {
        orders.push_back(current->data);
        current = current->next;
    }

    return orders;
}

// Process the next order using FIFO
bool OrderQueue::processNextOrder(Order& processedOrder)
{
    if (front == nullptr)
    {
        return false;
    }

    OrderNode* temp = front;

    processedOrder = temp->data;
    front = front->next;

    if (front == nullptr)
    {
        rear = nullptr;
    }

    delete temp;
    return true;
}

// Existing console function: place an order
void OrderQueue::placeOrder()
{
    Order order;

    cout << "\nEnter Order ID: ";
    cin >> order.orderId;

    cout << "Enter Customer Name: ";
    cin >> ws;
    getline(cin, order.customerName);

    cout << "Enter Flower Name: ";
    getline(cin, order.flowerName);

    cout << "Enter Quantity: ";
    cin >> order.quantity;

    if (!addOrder(order))
    {
        cout << "\nInvalid order or duplicate pending Order ID.\n";
        return;
    }

    cout << "\nOrder placed successfully!\n";
}

// Existing console function: display pending orders
void OrderQueue::displayOrders()
{
    vector<Order> orders = getPendingOrders();

    if (orders.empty())
    {
        cout << "\nNo pending orders.\n";
        return;
    }

    cout << "\n========== CUSTOMER ORDERS ==========\n";

    for (const Order& order : orders)
    {
        cout << "\nOrder ID: " << order.orderId;
        cout << "\nCustomer: " << order.customerName;
        cout << "\nFlower: " << order.flowerName;
        cout << "\nQuantity: " << order.quantity;
        cout << "\n-----------------------------\n";
    }
}

// Existing console function: process the next order
void OrderQueue::processOrder()
{
    Order order;

    if (!processNextOrder(order))
    {
        cout << "\nNo orders to process.\n";
        return;
    }

    cout << "\nProcessing Order...";
    cout << "\nOrder ID: " << order.orderId;
    cout << "\nCustomer: " << order.customerName;
    cout << "\nFlower: " << order.flowerName;
    cout << "\nQuantity: " << order.quantity;
    cout << "\n\nOrder processed successfully!\n";
}