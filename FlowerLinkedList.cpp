
#include <iostream>
#include "FlowerLinkedList.h"

using namespace std;

FlowerLinkedList::FlowerLinkedList()
{
    head = nullptr;
}

// 1. Add Flower using console
void FlowerLinkedList::addFlower()
{
    Flower flower;

    cout << "\nEnter Flower ID: ";
    cin >> flower.flowerId;

    cout << "Enter Flower Name: ";
    cin >> flower.flowerName;

    cout << "Enter Flower Type: ";
    cin >> flower.flowerType;

    cout << "Enter Price: ";
    cin >> flower.price;

    cout << "Enter Quantity: ";
    cin >> flower.quantity;

    flower.availability = (flower.quantity > 0);

    if (addFlowerData(flower))
    {
        cout << "\nFlower added successfully!\n";
    }
    else
    {
        cout << "\nFlower ID already exists!\n";
    }
}

// 2. Display all flowers using console
void FlowerLinkedList::displayFlowers()
{
    if (head == nullptr)
    {
        cout << "\nNo flowers available.\n";
        return;
    }

    FlowerNode* temp = head;

    cout << "\n========== FLOWER LIST ==========\n";

    while (temp != nullptr)
    {
        cout << "\nFlower ID: " << temp->data.flowerId;
        cout << "\nFlower Name: " << temp->data.flowerName;
        cout << "\nFlower Type: " << temp->data.flowerType;
        cout << "\nPrice: " << temp->data.price;
        cout << "\nQuantity: " << temp->data.quantity;

        cout << "\nAvailability: "
             << (temp->data.availability
                 ? "Available"
                 : "Not Available");

        cout << "\n-----------------------------";

        temp = temp->next;
    }

    cout << endl;
}

// 3. Search flower by name using console
void FlowerLinkedList::searchFlower()
{
    if (head == nullptr)
    {
        cout << "\nNo flowers available.\n";
        return;
    }

    string name;

    cout << "\nEnter flower name to search: ";
    cin >> name;

    FlowerNode* temp = head;

    while (temp != nullptr)
    {
        if (temp->data.flowerName == name)
        {
            cout << "\nFlower Found!";
            cout << "\nID: " << temp->data.flowerId;
            cout << "\nName: " << temp->data.flowerName;
            cout << "\nType: " << temp->data.flowerType;
            cout << "\nPrice: " << temp->data.price;
            cout << "\nQuantity: " << temp->data.quantity;
            cout << "\nAvailability: "
                 << (temp->data.availability
                     ? "Available"
                     : "Not Available")
                 << endl;

            return;
        }

        temp = temp->next;
    }

    cout << "\nFlower not found.\n";
}

// 4. Update flower using console
void FlowerLinkedList::updateFlower()
{
    int id;
    double price;
    int quantity;

    cout << "\nEnter Flower ID to update: ";
    cin >> id;

    cout << "Enter new price: ";
    cin >> price;

    cout << "Enter new quantity: ";
    cin >> quantity;

    if (updateFlowerData(id, price, quantity))
    {
        cout << "\nFlower updated successfully!\n";
    }
    else
    {
        cout << "\nFlower not found.\n";
    }
}

// 5. Delete flower using console
void FlowerLinkedList::deleteFlower()
{
    int id;

    cout << "\nEnter Flower ID to delete: ";
    cin >> id;

    if (deleteFlowerById(id))
    {
        cout << "\nFlower deleted successfully!\n";
    }
    else
    {
        cout << "\nFlower not found.\n";
    }
}

// 6. Get all flowers for the website
vector<Flower> FlowerLinkedList::getAllFlowers()
{
    vector<Flower> flowers;
    FlowerNode* temp = head;

    while (temp != nullptr)
    {
        flowers.push_back(temp->data);
        temp = temp->next;
    }

    return flowers;
}

// 7. Add flower without console input
bool FlowerLinkedList::addFlowerData(const Flower& flower)
{
    FlowerNode* temp = head;

    // Prevent duplicate flower IDs
    while (temp != nullptr)
    {
        if (temp->data.flowerId == flower.flowerId)
        {
            return false;
        }

        temp = temp->next;
    }

    // Validate basic data
    if (flower.flowerId <= 0 ||
        flower.price < 0 ||
        flower.quantity < 0)
    {
        return false;
    }

    FlowerNode* newNode = new FlowerNode;

    newNode->data = flower;
    newNode->data.availability = (flower.quantity > 0);
    newNode->next = nullptr;

    if (head == nullptr)
    {
        head = newNode;
    }
    else
    {
        temp = head;

        while (temp->next != nullptr)
        {
            temp = temp->next;
        }

        temp->next = newNode;
    }

    return true;
}

// 8. Find flower by ID for the website
bool FlowerLinkedList::findFlowerById(int id, Flower& result)
{
    FlowerNode* temp = head;

    while (temp != nullptr)
    {
        if (temp->data.flowerId == id)
        {
            result = temp->data;
            return true;
        }

        temp = temp->next;
    }

    return false;
}

// 9. Update flower without console input
bool FlowerLinkedList::updateFlowerData(
    int id, double price, int quantity)
{
    if (price < 0 || quantity < 0)
    {
        return false;
    }

    FlowerNode* temp = head;

    while (temp != nullptr)
    {
        if (temp->data.flowerId == id)
        {
            temp->data.price = price;
            temp->data.quantity = quantity;
            temp->data.availability = (quantity > 0);

            return true;
        }

        temp = temp->next;
    }

    return false;
}

// 10. Delete flower without console input
bool FlowerLinkedList::deleteFlowerById(int id)
{
    FlowerNode* temp = head;
    FlowerNode* previous = nullptr;

    while (temp != nullptr)
    {
        if (temp->data.flowerId == id)
        {
            if (previous == nullptr)
            {
                head = temp->next;
            }
            else
            {
                previous->next = temp->next;
            }

            delete temp;
            return true;
        }

        previous = temp;
        temp = temp->next;
    }

    return false;
}
