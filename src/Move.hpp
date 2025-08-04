#ifndef __MOVE_HPP__
#define __MOVE_HPP__

#include "Map.hpp"
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

    bool IsPassable(Map &map, Point &location, Stack &units)
    {
        auto passable = false;

        if (map.IsValid(location))
        {
            auto tile = map[location];

            auto blocked = tile.IsBlocked();

            auto allowed = tile.IsPassable(units);

            auto fits = tile.CanFitStack(units.size());

            return !blocked && allowed && fits;
        }

        return passable;
    }

    Moves Nodes(Map &map, Smart<Move::Node> &current, Smart<Move::Node> &target, Stack &units)
    {
        auto traversable = Moves();

        auto directions = map.Neighbors(current->X, current->Y);

        if (map.Dimensions.X > 0 && map.Dimensions.Y > 0)
        {
            for (auto &next : directions)
            {
                if (Move::IsPassable(map, next, units))
                {
                    traversable.push_back(std::make_shared<Move::Node>(next, current->Cost + (map[next].Attrition == 0 ? 1 : map[next].Attrition), current));

                    traversable.back()->SetDistance(map, target);
                }
            }
        }

        return traversable;
    }

    // get index of node from a list
    Moves::const_iterator Find(Moves &nodes, Smart<Move::Node> &node)
    {
        return DarkEmperor::Find(nodes, node, Move::Compare);
    }

    // remove node from list
    void Remove(Moves &nodes, Smart<Move::Node> &node)
    {
        auto found = Move::Find(nodes, node);

        if (found != nodes.end())
        {
            nodes.erase(found);
        }
    }

    // check if node is on the list
    bool In(Moves &nodes, Smart<Move::Node> &node)
    {
        return Move::Find(nodes, node) != nodes.end();
    }

    // find path from src to dst using the A* algorithm
    Move::Path FindPath(Map &map, Point src, Point dst, Stack &units)
    {
        auto path = Move::Path();

        auto valid = map.IsValid(src) && map.IsValid(dst);

        if (map.Dimensions.X > 0 && map.Dimensions.Y > 0 && valid)
        {
            auto start = std::make_shared<Move::Node>(src);

            auto end = std::make_shared<Move::Node>(dst);

            start->SetDistance(map, end);

            // list of nodes to be checked
            auto active = Moves();

            // list of nodes already visited
            auto visited = Moves();

            active.push_back(start);

            auto min_distance = map.Distance(src, dst);

            path.Closest = src;

            while (!active.empty())
            {
                // sort based on CostDistance
                std::sort(active.begin(), active.end(), [](Smart<Move::Node> &src, Smart<Move::Node> &dst)
                          { return src->CostDistance() < dst->CostDistance(); });

                auto check = active.front();

                if (Move::Compare(check, end))
                {
                    // we found the destination and we can be sure (because of the sort order above)
                    // that it's the most low cost option.
                    auto node = check;

                    while (node)
                    {
                        path.Points.push_back(Point(node->X, node->Y));

                        node = node->Parent;
                    }

                    // reverse list of coordinates so path leads from src to dst
                    std::reverse(path.Points.begin(), path.Points.end());

                    return path;
                }

                visited.push_back(check);

                auto test = Point(check->X, check->Y);

                auto dist = map.Distance(test, dst);

                // check if this is closest point to destination
                if (dist < min_distance)
                {
                    path.Closest = test;

                    min_distance = dist;
                }

                Move::Remove(active, check);

                auto nodes = Move::Nodes(map, check, end, units);

                for (auto &node : nodes)
                {
                    // we have already visited this node so we don't need to do so again!
                    if (Move::In(visited, node))
                    {
                        continue;
                    }

                    // it's already in the active list, but that's OK, maybe this new node has a better value (e.g. We might zigzag earlier but this is now straighter).
                    if (Move::In(active, node))
                    {
                        auto existing = *Move::Find(active, node);

                        if (existing->CostDistance() > node->CostDistance())
                        {
                            Move::Remove(active, existing);

                            active.push_back(node);
                        }
                    }
                    else
                    {
                        // we've never seen this node before so add it to the list.
                        active.push_back(node);
                    }
                }
            }
        }

        return path;
    }
}

#endif
