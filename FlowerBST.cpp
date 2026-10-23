#include "FlowerBST.h"
#include <iostream>

using namespace std;

FlowerBST::FlowerBST()
{
    root = nullptr;
}

BSTNode* FlowerBST::insert(BSTNode* node, Flower flower)
{
    if (node == nullptr)
    {
        return new BSTNode(flower);
    }

    if (flower.flowerId < node->flower.flowerId)
    {
        node->left = insert(node->left, flower);
    }
    else if (flower.flowerId > node->flower.flowerId)
    {
        node->right = insert(node->right, flower);
    }

    return node;
}

void FlowerBST::insertFlower(Flower flower)
{
    root = insert(root, flower);
}

void FlowerBST::inorder(BSTNode* node)
{
    if (node == nullptr)
    {
        return;
    }

    inorder(node->left);

    cout << "ID: " << node->flower.flowerId
         << " | Name: " << node->flower.flowerName
         << " | Type: " << node->flower.flowerType
         << " | Price: " << node->flower.price
         << " | Quantity: " << node->flower.quantity
         << " | Available: "
         << (node->flower.availability ? "Yes" : "No")
         << endl;

    inorder(node->right);
}

void FlowerBST::displayInorder()
{
    if (root == nullptr)
    {
        cout << "BST is empty." << endl;
        return;
    }

    cout << "\n========== FLOWERS IN BST ORDER ==========\n";

    inorder(root);
}