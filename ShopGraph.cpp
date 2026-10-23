#include "ShopGraph.h"
#include <iostream>
#include <queue>

using namespace std;

ShopGraph::ShopGraph()
{
    // Create 5 shop locations
    graph[0].push_back(1);
    graph[0].push_back(2);

    graph[1].push_back(0);
    graph[1].push_back(3);

    graph[2].push_back(0);
    graph[2].push_back(3);

    graph[3].push_back(1);
    graph[3].push_back(2);
    graph[3].push_back(4);

    graph[4].push_back(3);
}

void ShopGraph::displayGraph()
{
    cout << "\n========== FLOWER SHOP GRAPH ==========\n";

    for (int i = 0; i < 5; i++)
    {
        cout << "Location " << i << " -> ";

        for (int neighbour : graph[i])
        {
            cout << neighbour << " ";
        }

        cout << endl;
    }
}

void ShopGraph::BFS(int start)
{
    bool visited[5] = {false};

    queue<int> q;

    visited[start] = true;
    q.push(start);

    cout << "\n========== BFS TRAVERSAL ==========\n";
    cout << "Starting from Location " << start << ": ";

    while (!q.empty())
    {
        int current = q.front();
        q.pop();

        cout << current << " ";

        for (int neighbour : graph[current])
        {
            if (!visited[neighbour])
            {
                visited[neighbour] = true;
                q.push(neighbour);
            }
        }
    }

    cout << endl;
}