#ifndef FLOWERBST_H
#define FLOWERBST_H

#include "Flower.h"

struct BSTNode
{
    Flower flower;
    BSTNode* left;
    BSTNode* right;

    BSTNode(Flower f)
    {
        flower = f;
        left = nullptr;
        right = nullptr;
    }
};

class FlowerBST
{
private:
    BSTNode* root;

    BSTNode* insert(BSTNode* node, Flower flower);
    void inorder(BSTNode* node);

public:
    FlowerBST();

    void insertFlower(Flower flower);
    void displayInorder();
};

#endif