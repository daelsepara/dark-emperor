#ifndef __MOVE_HPP__
#define __MOVE_HPP__

#include "HexMap.hpp"
#include "Random.hpp"

namespace DarkEmperor::Move
{
    // path found by A* algorithm
    class Path
    {
    public:
        // list of coordinates of the path
        Points Points;

        Point Closest;

        Path() {}
    };

    // class representing a node in the graph
    class Node
    {
    public:
        int X = -1;

        int Y = -1;

        int Cost = 0;

        int Distance = 0;

        Smart<Move::Node> Parent = nullptr;

        Node(int x, int y, int cost, Smart<Move::Node> &parent) : X(x), Y(y), Cost(cost), Parent(parent) {}

        Node(Point point, int cost, Smart<Move::Node> &parent) : Node(point.X, point.Y, cost, parent) {}

        Node(int x, int y) : X(x), Y(y) {}

        Node(Point point) : X(point.X), Y(point.Y) {}

        Node() {}

        // total cost to traverse this node
        int CostDistance()
        {
            return (this->Cost + this->Distance);
        }

        // the distance is estimated distance, ignoring obstacles to our target:
        // how many nodes ignoring obstacles, to get there.
        //
        // computes the 2D Manhattan Distance (modified for hex)
        void SetDistance(Map &map, Smart<Move::Node> &node)
        {
            this->Distance = map.Distance(this->X, this->Y, node->X, node->Y);
        }
    };

    // list of nodes
    typedef List<Smart<Move::Node>> Moves;

    Point operator+(const Smart<Node> &node, const Point &p)
    {
        return Point(node->X + p.X, node->Y + p.Y);
    }

    // is the node equal to the point?
    bool Is(Smart<Move::Node> &a, Point &b)
    {
        return a->X == b.X && a->Y == b.Y;
    }

    // compare equality between two nodes
    bool Compare(Smart<Move::Node> &a, Smart<Move::Node> &b)
    {
        return a->X == b->X && a->Y == b->Y;
    }
}

#endif
