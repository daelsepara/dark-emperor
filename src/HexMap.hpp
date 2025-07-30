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

        auto rad = M_PI / 180.0 * deg;

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

    // cube coordinates
    class Cube
    {
    public:
        int Q = 0;

        int R = 0;

        int S = 0;

        Cube(int q, int r, int s) : Q(q), R(r), S(s) {}

        Cube(int q, int r) : Q(q), R(r) { this->S = -(this->Q + this->R); }

        Cube() {}
    };

    class Tile
    {
    public:
        // square grid coordinates
        Point Point;

        // cube coordinates
        Cube Hex;

        Tile(int x, int y) : Point(x, y)
        {
            auto parity = this->Point.X & 1;

            this->Hex = Cube(this->Point.X, this->Point.Y - (this->Point.X - parity) / 2);
        }
    };

    class Map
    {
    public:
        // dimensions (number of tile rows and columns)
        Point Dimensions;

        // location (on screen) where map is drawn
        Point Draw = Point(0, 0);

        // top-left coordinates of viewing region
        Point View = Point(0, 0);

        // flat or pointed orientation
        bool Flat = false;

        // size of the tiles in pixels
        int Size;

        // tiles comprising the map
        std::vector<std::vector<Tile>> Tiles = {};

        Map(int width, int height, int size) : Dimensions(width, height), Size(size) {}

        Map(int width, int height) : Dimensions(width, height) {}

        Map() {}
    };
}

#endif
