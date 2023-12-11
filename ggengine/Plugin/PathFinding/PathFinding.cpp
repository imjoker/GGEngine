
#include "PathFinding.h"
#include "Grid.h"
#include <algorithm>
#include <cmath>
#include <queue>
#include <set>

namespace PathFinding {

    static std::vector<Node*> neighbors;

    void GetNeighbors(const Node* node) noexcept
    {
        neighbors.clear();
        neighbors.reserve(8);

        for (int dx = -1; dx <= 1; ++dx) {

            for (int dy = -1; dy <= 1; ++dy) {

                if (dx == 0 && dy == 0)
                    continue; // Skip the current node

                Node* fetchedNode = Grid::GetGrid()->GetNodeAt((int)node->x + dx, (int)node->y + dy, (int)node->z);

                if (fetchedNode == nullptr)
                    continue; // Skip if the node is out of bounds

                Node* neighbor = new Node(*(fetchedNode));

                // Check if the neighbor is an obstacle
                if ((!neighbor->isObstacle))
                    neighbors.push_back(neighbor);
                else
                    delete neighbor;
            }
        }

        //return neighbors;
    }

    float PathFinding::GetDistance(const Node* nodeA, const Node* nodeB) noexcept
    {

        float distX = std::abs(nodeA->x - nodeB->x);
        float distY = std::abs(nodeA->y - nodeB->y);
        float distZ = std::abs(nodeA->z - nodeB->z);

        // Diagonal movement cost (10) for each unit along the x, y, and z axes
        // Straight movement cost (10) for each additional unit along the longer axis

        if (distX >= distY && distX >= distZ)
            return 14 * distY + 10 * (distX - distY + distZ);

        if (distY >= distX && distY >= distZ)
            return 14 * distX + 10 * (distY - distX + distZ);

        return 14 * distZ + 10 * (distX + distY - distZ);
    }

    Grid* InitializeGrid(int width, int height, int depth) noexcept
    {
        return nullptr;
    }

    std::vector<Node*> GetPath(Node* start, Node* end) noexcept {

        std::set<Node*> open_set;
        std::set<Node*> closed_set;

        if (start == nullptr || end == nullptr)
			return {};

        if (*start == *end)
            return {};

        open_set.insert(new Node(*start));

        while (!open_set.empty()) {

            Node* current_node = *open_set.begin();

            for (Node* node : open_set)
            {
                if (node->f_cost() < current_node->f_cost() || node->f_cost() == current_node->f_cost() && node->h_cost < current_node->h_cost)
                    current_node = node;
            }

            if (*current_node == *end) {

                std::vector<Node*> path;

                while (current_node != nullptr) {

                    path.push_back(current_node);
                    current_node = current_node->parent;
                }

                //std::reverse(path.begin(), path.end());
                neighbors.clear();
                neighbors.shrink_to_fit();

                open_set.clear();
                closed_set.clear();

                return path;
            }

            open_set.erase(current_node);
            closed_set.insert(current_node);

            GetNeighbors(current_node);

            for (auto neighbor : neighbors) {

                if (closed_set.find(neighbor) != closed_set.end() || neighbor->isObstacle) {

                    delete neighbor;
                    continue;
                }

                float tentative_g_cost = current_node->g_cost + GetDistance(current_node, neighbor);

                if (bool isNotFound = (open_set.find(neighbor) == open_set.end()); (isNotFound || tentative_g_cost < neighbor->g_cost)) {

                    if (isNotFound)
                        open_set.insert(neighbor);

                    neighbor->g_cost = tentative_g_cost;
                    neighbor->h_cost = GetDistance(neighbor, end);
                    neighbor->parent = current_node;
                }
				else
					delete neighbor;
            }
        }

        neighbors.clear();
        neighbors.shrink_to_fit();

        for (auto node : open_set) {
            delete node;
        }

        for (auto node : closed_set) {
            delete node;
        }

        open_set.clear();
        closed_set.clear();

        return {}; // No path found
    }
}