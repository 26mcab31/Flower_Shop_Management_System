#ifndef FLOWER_H
#define FLOWER_H

#include <string>
using namespace std;

struct Flower
{
    int flowerId;
    string flowerName;
    string flowerType;
    double price;
    int quantity;
    bool availability;
};

#endif