#pragma once

#include "PathFinding.h"

namespace PathFinding {

    class Grid {

    public:

    static           Grid*   MakeGrid           (int width, int height, int depth);
                            ~Grid               ();

                    int     GetGridX            () const;
                    int     GetGridY            () const;
                    int     GetGridZ            () const;

                    Node*   GetNodeAt           (int x, int y, int z) noexcept;

    static          Grid*   GetGrid             () noexcept;

    private:
                    void    InitializeGrid      ();

    private:

        int widthX, heightY, depthZ;
        std::vector<std::vector<std::vector<Node>>> nodes;
        static Grid* grid;
    };

}