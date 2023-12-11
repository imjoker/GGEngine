// PathFindingDemo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Plugin/PathFinding/PathFinding.h"
#include "Plugin/PathFinding/Grid.h"

using namespace PathFinding;

int main()
{
    Grid* grid = Grid::MakeGrid (200, 200, 200);

    Node*  start = grid->GetNodeAt(110, 110, 100);
    Node*  end = grid->GetNodeAt(100, 100, 100);
    std::vector<Node*> path2 = GetPath(start, end);

    int j = 0;

    while (j < 50) {

        start = grid->GetNodeAt(100, 100, 100);
        end = grid->GetNodeAt(100 * j, 101 * j, 100);
        std::vector<Node*> path = GetPath(start, end);

        int i = 1;

        for (const Node* pathNode : path) {

            std::cout << i << ": " << pathNode->x << ", " << pathNode->y << ", " << pathNode->z << std::endl;

            ++i;
        }

        for (Node* node : path)
			delete node;

        path.clear();
        path.shrink_to_fit();

        ++j;
    }
}
