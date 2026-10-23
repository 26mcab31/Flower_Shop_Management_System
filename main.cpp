#include <iostream>
#include <string>

#include "FlowerLinkedList.h"
#include "OrderQueue.h"
#include "FlowerBST.h"
#include "ShopGraph.h"

using namespace std;

bool login()
{
    string username;
    string password;

    cout << "\n====================================";
    cout << "\n        FLOWER SHOP LOGIN";
    cout << "\n====================================";

    cout << "\n\nUsername: ";
    cin >> username;

    cout << "Password: ";
    cin >> password;

    if (username == "admin" && password == "1234")
    {
        cout << "\nLogin successful!\n";
        return true;
    }

    cout << "\nInvalid username or password!\n";
    return false;
}

int main()
{
    // Login
    if (!login())
    {
        return 0;
    }

    FlowerLinkedList flowerList;
    OrderQueue orderQueue;
    FlowerBST flowerBST;
    ShopGraph shopGraph;

    int choice;

    do
    {
        cout << "\n\n====================================";
        cout << "\n     FLOWER SHOP MANAGEMENT SYSTEM";
        cout << "\n====================================";

        cout << "\n\n1. Add Flower";
        cout << "\n2. Display Flowers";
        cout << "\n3. Search Flower";
        cout << "\n4. Update Flower";
        cout << "\n5. Delete Flower";

        cout << "\n\n6. Place Customer Order";
        cout << "\n7. Display Customer Orders";
        cout << "\n8. Process Next Order";

        cout << "\n\n9. Add Flower to BST";
        cout << "\n10. Display Flowers using BST";

        cout << "\n\n11. Display Shop Graph";
        cout << "\n12. BFS Traversal";

        cout << "\n\n0. Exit";

        cout << "\n\nEnter your choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                flowerList.addFlower();
                break;

            case 2:
                flowerList.displayFlowers();
                break;

            case 3:
                flowerList.searchFlower();
                break;

            case 4:
                flowerList.updateFlower();
                break;

            case 5:
                flowerList.deleteFlower();
                break;

            case 6:
                orderQueue.placeOrder();
                break;

            case 7:
                orderQueue.displayOrders();
                break;

            case 8:
                orderQueue.processOrder();
                break;

            case 9:
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

                cout << "Is the flower available? (1 = Yes, 0 = No): ";
                cin >> flower.availability;

                flowerBST.insertFlower(flower);

                cout << "\nFlower added to BST successfully!\n";
                break;
            }

            case 10:
                flowerBST.displayInorder();
                break;

            case 11:
                shopGraph.displayGraph();
                break;

            case 12:
            {
                int start;

                cout << "\nEnter starting location (0-4): ";
                cin >> start;

                if (start >= 0 && start < 5)
                {
                    shopGraph.BFS(start);
                }
                else
                {
                    cout << "\nInvalid location!\n";
                }

                break;
            }

            case 0:
                cout << "\nExiting program...\n";
                break;

            default:
                cout << "\nInvalid choice!\n";
        }

    } while (choice != 0);

    return 0;
}