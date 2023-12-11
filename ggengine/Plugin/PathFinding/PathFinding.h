#pragma once
#include <vector>

namespace PathFinding {
    
    class Grid; // forward declaration

    // Node structure for pathfinding
    struct Node {

        Node() = default;

        Node(const Node & pNode) : parent(pNode.parent), x(pNode.x), y(pNode.y), z(pNode.z), g_cost(0), h_cost(0), isObstacle(pNode.isObstacle) {}

        Node(float x, float y, float z, bool isObstacle = false)
            : parent(nullptr), x(x), y(y), z(z), g_cost(0), h_cost(0), isObstacle(isObstacle) {}

        constexpr float f_cost() const { return g_cost + h_cost; }

        inline bool operator== (const Node& b) noexcept 
        {
			return (std::abs(x - b.x) < 0.5f) && (std::abs(y - b.y) < 0.5f) && (std::abs(z - b.z) < 0.5f);
		}

        Node*   parent;             // parent Node
        float   x, y, z;            // location of the Node
        float   g_cost;             // cost from start Node to 'this' Node
        float   h_cost;             // cost from 'this' Node to Target/End Node
        bool    isObstacle;         // this can be removed, if there is a collision detection system in the engine
    };

    // APIs for PathFinding

    // Get an optimal path while avoiding obstacles from start to end.
    std::vector<Node*>  GetPath         (Node* start, Node* end) noexcept;

    // get non-obstacle neighbors of the given node
    void                GetNeighbors    (const Node* node) noexcept;
    
    // get the distance between nodeA and nodeB
    float               GetDistance     (const Node * nodeA, const Node * nodeB) noexcept;

    // custom comparison operators for Nodes
    constexpr bool      operator==      (const Node& a, const Node& b) noexcept;
    constexpr bool      operator!=      (const Node& a, const Node& b) noexcept;
    constexpr bool      operator<       (const Node& a, const Node& b) noexcept;

};

constexpr bool PathFinding::operator==(const Node& a, const Node& b) noexcept
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

constexpr bool PathFinding::operator!=(const Node& a, const Node& b) noexcept
{
    return !(a == b);
}

constexpr bool PathFinding::operator<(const Node& a, const Node& b) noexcept
{
    return (a.f_cost() < b.f_cost() || (a.f_cost() == b.f_cost() && a.h_cost < b.h_cost));
}