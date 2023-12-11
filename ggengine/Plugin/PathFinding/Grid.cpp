#include "Grid.h"

namespace PathFinding {

    Grid* Grid::grid = nullptr;

    Grid* Grid::MakeGrid(int width, int height, int depth) {

        if (grid)
            delete grid;

        grid = new Grid();

        grid->widthX = width;
        grid->heightY = height;
        grid->depthZ = depth;

        // Initialize the map with nodes
        grid->nodes.resize(width, std::vector<std::vector<Node>>(height, std::vector<Node>(depth)));

        // Populate nodes and mark certain locations as obstacles
        grid->InitializeGrid();

        return grid;
    }

    Grid::~Grid()
    {
        nodes.clear();
    }

    void Grid::InitializeGrid()
    {
        for (int x = 0; x < widthX; ++x) {

            for (int y = 0; y < heightY; ++y) {

                for (int z = 0; z < depthZ; ++z) {

                    bool isObstacle = false;

                    // Initialize Node with location values
                    // mark certain locations randomly as obstacles (Can be replaced with collision checks, if a collision system exists)
                    //if (rand() % 9 == 0)
                    //    isObstacle = true;

                    nodes[x][y][z] = Node((float) x, (float) y, (float) z, isObstacle);
                }
            }
        }
    }

    int Grid::GetGridX() const
    {
        return widthX;
    }

    int Grid::GetGridY() const
    {
        return heightY;
    }

    int Grid::GetGridZ() const
    {
        return depthZ;
    }

    Node* Grid::GetNodeAt(int x, int y, int z) noexcept
    {
        if (x >= widthX || y >= heightY || z >= depthZ)
            return nullptr;

        return &nodes[x][y][z];
    }

    Grid* Grid::GetGrid() noexcept
    {
		return grid;
	}
}