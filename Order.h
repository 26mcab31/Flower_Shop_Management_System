#ifndef ORDER_H
#define ORDER_H

#include <string>
using namespace std;

struct Order
{
    int orderId;
    string customerName;
    string flowerName;
    int quantity;
};

#endif