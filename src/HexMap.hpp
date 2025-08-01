#ifndef __HEX_MAP__
#define __HEX_MAP__

#include <cmath>
#include <vector>

namespace Hex
{
    const double Scale = std::sqrt(3.0);

    const double Offset = 3.0 / 2.0;

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

        int Sum() { return (this->Q + this->R + this->S); }

        bool IsValid() { return this->Sum() == 0; }
    };

    class Tile
    {
    public:
        // square grid coordinates
        Point Point;

        // cube coordinates
        Cube Hex;

        void Initialize(int x, int y, bool flat)
        {
            this->Point = Hex::Point(x, y);

            auto parity = (flat ? this->Point.X : this->Point.Y) & 1;

            if (flat)
            {
                this->Hex = Cube(this->Point.X, this->Point.Y - (this->Point.X - parity) / 2);
            }
            else
            {
                this->Hex = Cube(this->Point.X - (this->Point.Y - parity) / 2, this->Point.Y);
            }
        }

        Tile(int x, int y, bool flat)
        {
            this->Initialize(x, y, flat);
        }

        Tile(int x, int y) : Tile(x, y, true) {}

        Tile() {}

        bool IsValid() { return this->Hex.IsValid(); }
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

        // size of viewable region
        Point Limit = Point(0, 0);

        // flat or pointed orientation
        bool Flat = false;

        // size of the tiles in pixels
        int Size;

        // tiles comprising the map
        std::vector<std::vector<Tile>> Tiles = {};

        Map(int width, int height, int size, bool flat) : Dimensions(width, height), Flat(flat), Size(size)
        {
            this->Tiles = std::vector(height, std::vector(width, Hex::Tile()));

            for (auto y = 0; y < height; y++)
            {
                for (auto x = 0; x < width; x++)
                {
                    this->Tiles[y][x].Initialize(x, y, this->Flat);
                }
            }

            this->Draw = Point(0, 0);

            this->View = Point(0, 0);

            this->Limit = Point(width, height);
        }

        Map(int width, int height) : Dimensions(width, height) {}

        Map() {}

        // access location on the map
        Tile &operator()(int x, int y)
        {
            return this->Tiles[y][x];
        }

        // access location on the map
        Tile &operator()(const Point &point)
        {
            return (*this)(point.X, point.Y);
        }

        Tile &operator[](const Point &point)
        {
            return (*this)(point);
        }

        // access location to map using cube (axial) coordinates
        Tile &operator()(int q, int r, int s)
        {
            auto parity = (this->Flat ? q : r) & 1;

            if (this->Flat)
            {
                return (*this)(q, r + (q - parity) / 2);
            }
            else
            {
                return (*this)(q + (r - parity) / 2, r);
            }
        }

        // access location to map using cube coordinates
        Tile &operator()(const Cube &cube)
        {
            return (*this)(cube.Q, cube.R, cube.S);
        }
    };
}

#endif
