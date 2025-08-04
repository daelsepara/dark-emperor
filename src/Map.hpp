#ifndef __HEX_MAP__
#define __HEX_MAP__

#include <cmath>
#include <vector>

#include "Color.hpp"
#include "Types.hpp"
#include "Unit.hpp"

namespace DarkEmperor
{
    const double Scale = std::sqrt(3.0);

    const double Offset = 3.0 / 2.0;

    typedef List<Unit::Base> Units;

    const Points FlatNeighbors = {{0, -1}, {1, -1}, {1, 0}, {0, 1}, {-1, 0}, {-1, -1}};

    const Points FlatNeighborsOdd = {{0, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}};

    const Points PointyNeighbors = {{-1, -1}, {0, -1}, {1, 0}, {0, 1}, {-1, 1}, {-1, 0}};

    const Points PointyNeighborsOdd = {{0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 0}};

    Point Corner(Point center, int size, int corner, bool flat = false)
    {
        auto deg = 60.0 * corner - (flat ? 0.0 : 30.0);

        auto rad = M_PI / 180.0 * deg;

        return Point(center.X + int(std::round(size * std::cos(rad))), center.Y + int(std::round(size * std::sin(rad))));
    }

    Points Vertices(Point center, int size, bool flat = false)
    {
        // hex-tile
        auto hex = Points();

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
        // hex number
        int Id = -1;

        // square grid coordinates
        Point Point;

        // cube coordinates
        Cube Hex;

        TerrainType Terrain = TerrainType::NONE;

        // units occupying current tile (id, type, kingdom)
        Stack Units = {};

        // stacking limit
        int StackLimit = 0;

        // attrition cost
        int Attrition = 0;

        // city value
        int CityValue = 0;

        // current owner
        Kingdom Owner = Kingdom::NONE;

        // asset number
        int Asset = -1;

        Uint32 Color = 0;

        void Initialize(int x, int y, bool flat)
        {
            this->Point = DarkEmperor::Point(x, y);

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

        Tile(int id, int x, int y, bool flat) : Id(id)
        {
            this->Initialize(x, y, flat);
        }

        Tile(int id, int x, int y) : Tile(id, x, y, true) {}

        Tile(int x, int y) : Tile(x, y, true) {}

        Tile() {}

        bool IsValid() { return this->Hex.IsValid(); }

        int CurrentStack()
        {
            return (int)(Units.size());
        }

        // tile is capital city
        bool IsCapital()
        {
            return this->Terrain == TerrainType::CAPITAL_CITY;
        }

        // tile is a city
        bool IsCity()
        {
            return this->Terrain == TerrainType::CITY || this->Terrain == TerrainType::CAPITAL_CITY;
        }

        // can fit stack of size
        bool CanFitStack(int size)
        {
            auto limit = this->StackLimit + (this->IsCity() ? this->CityValue : 0);

            return size <= (limit - this->CurrentStack());
        }

        // check if location is traversable or if it is the target destination
        bool IsPassable(UnitType type)
        {
            return DarkEmperor::In(AllowedPassage[this->Terrain], type);
        }

        bool IsPassable(Stack &units)
        {
            auto passable = true;

            auto types = List<UnitType>();

            for (auto unit : units)
            {
                if (!DarkEmperor::In(types, unit.Type))
                {
                    types.push_back(unit.Type);
                }
            }

            for (auto type : types)
            {
                passable &= this->IsPassable(type);
            }

            return passable;
        }

        // check if tile is blocked
        bool IsBlocked()
        {
            return (this->Terrain == TerrainType::NONE);
        }

        // check if cell is not occupied by any units
        bool NotOccupied()
        {
            return (this->Units.size() == 0);
        }

        // count other units belonging to "kingdom"
        int Count(Kingdom kingdom)
        {
            auto count = 0;

            for (auto unit : this->Units)
            {
                count += (unit.Kingdom != Kingdom::NONE && unit.Kingdom == kingdom ? 1 : 0);
            }

            return count;
        }

        // count other units belonging to "kingdom"
        int Count()
        {
            return this->Count(this->Owner);
        }

        // count other units not belonging to "kingdom"
        int Others(Kingdom kingdom)
        {
            auto count = 0;

            for (auto unit : this->Units)
            {
                count += (unit.Kingdom != Kingdom::NONE && unit.Kingdom != kingdom ? 1 : 0);
            }

            return count;
        }

        // count other units
        int Others()
        {
            return this->Others(this->Owner);
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

        // size of viewable region
        Point Limit = Point(0, 0);

        // flat or pointed orientation
        bool Flat = false;

        // size of the tiles in pixels
        int Size;

        // tiles comprising the map
        Array<Tile> Tiles = {};

        Map(int width, int height, int size, bool flat) : Dimensions(width, height), Flat(flat), Size(size)
        {
            this->Tiles = std::vector(height, std::vector(width, DarkEmperor::Tile()));

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

        // is (x, y) within map bounds?
        bool IsValid(int x, int y)
        {
            return (x >= 0 && x < this->Dimensions.X && y >= 0 && y < this->Dimensions.Y);
        }

        // is the point within map bounds?
        bool IsValid(Point point)
        {
            return this->IsValid(point.X, point.Y);
        }

        Point operator[](int id)
        {
            auto point = Point(-1, -1);

            for (auto y = 0; y < this->Tiles.size(); y++)
            {
                for (auto x = 0; x < this->Tiles[y].size(); x++)
                {
                    if (this->Tiles[y][x].Id == id)
                    {
                        point = Point(x, y);

                        break;
                    }
                }

                if (point != Point(-1, -1))
                {
                    break;
                }
            }

            return point;
        }

        // return set of neighbors
        Points Neighbors(bool is_odd = false)
        {
            return this->Flat ? (is_odd ? DarkEmperor::FlatNeighborsOdd : DarkEmperor::FlatNeighbors) : (is_odd ? DarkEmperor::PointyNeighborsOdd : DarkEmperor::PointyNeighbors);
        }

        // get all valid neighbors of point (x, y)
        Points Neighbors(int x, int y)
        {
            auto is_odd = this->Flat ? ((x & 1) != 0) : ((y & 1) != 0);

            auto neighbors = Points();

            for (auto neighbor : this->Neighbors(is_odd))
            {
                auto point = Point(x, y) + neighbor;

                if (this->IsValid(point))
                {
                    neighbors.push_back(point);
                }
            }

            return neighbors;
        }

        // get all valid neighbors of the point
        Points Neighbors(Point point)
        {
            return this->Neighbors(point.X, point.Y);
        }

        // calculate distance between two points using cube coordinates
        int Distance(int x0, int y0, int x1, int y1)
        {
            auto distance = -1;

            if (this->IsValid(x0, y0) && this->IsValid(x1, y1))
            {
                auto &src = this->Tiles[y0][x0].Hex;

                auto &dst = this->Tiles[y1][x1].Hex;

                auto q = std::abs(src.Q - dst.Q);

                auto r = std::abs(src.R - dst.R);

                auto s = std::abs(src.S - dst.S);

                distance = (q + r + s) / 2;
            }

            return distance;
        }

        // calculate distance between two points
        int Distance(Point src, Point dst)
        {
            return this->Distance(src.X, src.Y, dst.X, dst.Y);
        }
    };

    Kingdom FirstKingdom(Stack &units)
    {
        auto kingdom = Kingdom::NONE;

        for (auto unit : units)
        {
            if (unit.Kingdom != Kingdom::NONE)
            {
                kingdom = unit.Kingdom;

                break;
            }
        }

        return kingdom;
    }

    bool Has(Stack &units, UnitType type)
    {
        bool has = false;

        for (auto unit : units)
        {
            if (unit.Type == type)
            {
                has = true;

                break;
            }
        }

        return has;
    }

    int Attrition(Map &map, Units &units, Points path)
    {
        auto attrition = 0;

        for (auto point : path)
        {
            if (map.IsValid(point))
            {
                attrition += map[point].Attrition;
            }
        }

        return attrition;
    }
}

#endif
