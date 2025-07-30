#ifndef __HEX_MAP__
#define __HEX_MAP__

#include <cmath>
#include <vector>

namespace Hex
{
    // cartesian coordinates
    class Point
    {
    public:
        int X = -1;

        int Y = -1;

        Point(int x, int y) : X(x), Y(y) {}

        Point() {}

        Point &operator*=(const Point &p)
        {
            this->X *= p.X;

            this->Y *= p.Y;

            return *this;
        }

        Point &operator+=(const Point &p)
        {
            this->X += p.X;

            this->Y += p.Y;

            return *this;
        }

        Point &operator-=(const Point &p)
        {
            this->X -= p.X;

            this->Y -= p.Y;

            return *this;
        }

        bool operator==(const Point &p)
        {
            return this->X == p.X && this->Y == p.Y;
        }

        bool operator!=(const Point &p)
        {
            return !(*this == p);
        }

        Point operator+(const Point &p)
        {
            return Point(this->X + p.X, this->Y + p.Y);
        }

        Point operator-(const Point &p)
        {
            return Point(this->X - p.X, this->Y - p.Y);
        }

        Point operator*(const Point &p)
        {
            return Point(this->X * p.X, this->Y * p.Y);
        }

        Point operator+(int p)
        {
            return Point(this->X + p, this->Y + p);
        }

        Point operator-(int p)
        {
            return *this + (-p);
        }

        Point operator*(int p)
        {
            return Point(this->X * p, this->Y * p);
        }

        Point operator/(const Point &p)
        {
            return Point(this->X / p.X, this->Y / p.Y);
        }

        Point operator/(int p)
        {
            return Point(this->X / p, this->Y / p);
        }

        // check if point is a valid map coordinate
        bool IsNone()
        {
            return *this == Point(-1, -1);
        }
    };

    Point Corner(Point center, int size, int corner, bool flat = false)
    {
        auto deg = 60.0 * corner - (flat ? 0.0 : 30.0);

        double rad = M_PI / 180.0 * deg;

        return Point(center.X + int(std::round(size * std::cos(rad))), center.Y + int(std::round(size * std::sin(rad))));
    }

    std::vector<Point> Vertices(Point center, int size, bool flat = false)
    {
        // hex-tile
        auto hex = std::vector<Point>();

        for (auto corner = 0; corner < 6; corner++)
        {
            hex.push_back(Corner(center, size, corner, flat));
        }

        return hex;
    }

    class Tile
    {
    public:
        // square grid coordinates
        int X = -1;

        int Y = -1;

        // cube coordinates
        int Q = -1;

        int R = -1;

        int S = -1;
    };

    class Map
    {
    public:
        // dimensions
        Point Size = Point(-1, -1);

        // location (on screen) where maps is drawn
        Point Draw = Hex::Point(-1, -1);

        // top-left coordinates of viewing region
        Point View = Hex::Point(-1, -1);

        // flat or pointed mode
        bool Flat = false;

        // tiles comprising the Hex Grid Map
        std::vector<std::vector<Hex::Tile>> Tiles = {};
    };
}

#endif
