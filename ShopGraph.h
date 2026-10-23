#ifndef SHOPGRAPH_H
#define SHOPGRAPH_H

#include <vector>

using namespace std;

class ShopGraph
{
private:
    vector<int> graph[5];

public:
    ShopGraph();

    void displayGraph();

    void BFS(int start);
};

#endif