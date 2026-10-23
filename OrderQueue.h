
#ifndef ORDER_QUEUE_H
#define ORDER_QUEUE_H

#include "Order.h"
#include <vector>

struct OrderNode
{
    Order data;
    OrderNode* next;
};

class OrderQueue
{
private:
    OrderNode* front;
    OrderNode* rear;

public:
    // Constructor
    OrderQueue();

    // Destructor
    ~OrderQueue();

    // Prevent accidental copying of queue pointers
    OrderQueue(const OrderQueue&) = delete;
    OrderQueue& operator=(const OrderQueue&) = delete;

    // Existing console operations
    void placeOrder();
    void displayOrders();
    void processOrder();

    // Website and backend operations
    bool addOrder(const Order& order);
    std::vector<Order> getPendingOrders() const;
    bool processNextOrder(Order& processedOrder);
};

#endif
