
#ifndef FLOWER_LINKED_LIST_H
#define FLOWER_LINKED_LIST_H

#include "Flower.h"
#include <vector>

struct FlowerNode
{
    Flower data;
    FlowerNode* next;
};

class FlowerLinkedList
{
private:
    FlowerNode* head;

public:
    FlowerLinkedList();

    // Existing console functions
    void addFlower();
    void displayFlowers();
    void searchFlower();
    void updateFlower();
    void deleteFlower();

    // New functions for website-backend connection
    std::vector<Flower> getAllFlowers();
    bool addFlowerData(const Flower& flower);
    bool findFlowerById(int id, Flower& result);
    bool updateFlowerData(int id, double price, int quantity);
    bool deleteFlowerById(int id);
};

#endif
