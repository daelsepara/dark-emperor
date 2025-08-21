#ifndef __TYPES_HPP__
#define __TYPES_HPP__

#include <vector>

#include "Templates.hpp"

namespace DarkEmperor
{
    // terrain
    enum class TerrainType
    {
        NONE = -1,
        CITY,
        CAPITAL_CITY,
        MOUNTAIN,
        RIVER,
        COASTAL,
        SEA,
        PORT,
        HOLY_PLACE,
        SETTLED,
        MAGIC_HEX,
        BATTLEFIELD
    };

    enum class UnitType
    {
        NONE = -1,
        GROUND,
        AIR,
        NAVAL,
        SIEGE,
        LEADER,
        MONSTER,
        BATTLEFIELD,
        WEAPON,
        MAGICAL_DEVICE,
        TRENCH,
        FLAME
    };

    enum class RuneType
    {
        NONE = -1,
        TERROR,
        WOOD,
        DEATH,
        LIFE,
        AIR,
        METAL,
        FIRE,
        WATER,
        EARTH,
        SERENITY
    };

    enum class Kingdom
    {
        NONE = -1,
        NECROMANCER,
        TAL_PLETOR,
        LAMMARECH,
        LOYMARECH,
        KELARON_OIRET,
        FERLARIE,
        STAVROR,
        THE_SCYTHE,
        STARKEEP,
        AHAUTSIERON,
        ZOLAHAURESLOR,
        MULTIPLE
    };

    enum class Leader
    {
        NONE = -1,
        PADRECH_DAR_CHOIM,
        TOL_MORN,
        MEZAL,
        STOMMAREN,
        SILWER_FLAGRIEL,
        FERNAN_CONNIVER,
        SILFARON,
        NARMOREN,
        FERIOR,
        PADRON_PLETY_Y,
        GRIP_NUD,
        ZELARA_DO_FORNOY,
        LORDOLAM,
        PAISACH,
        THE_KEEPER,
        SKYDOR,
        STAV_FARREN,
        NIKON_V,
        KEBIR_WHITEHAND,
        FARNON,
        COS_DOL_COS,
        LORD_MONTOY,
        SAAR,
        HOUNDMASTER
    };

    enum class Mercenary
    {
        NONE = -1,
        COS_DOL_COS,
        LORD_MONTOY,
        HOUNDMASTER,
        SAAR,
        SILWER_FLAGRIEL,
        FERNAN_CONNIVER
    };

    // individual unit id (id, type, kingdom)
    struct UnitId
    {
        int Id = -1;

        UnitType Type = UnitType::NONE;

        Kingdom Kingdom = Kingdom::NONE;

        Mercenary Mercenary = Mercenary::NONE;
    };

    // unit id collection
    typedef List<UnitId> Stack;

    // for unit-stacking in the map
    struct UnitStack
    {
        // kingdoms
        Kingdom Kingdom = Kingdom::NONE;

        // lowest unit type denominator (e.g. GROUND, AIR, NAVAL)
        UnitType Type = UnitType::NONE;

        // actual unit composition
        Stack Units = {};
    };

    // default terrain stack limits
    UnorderedMap<TerrainType, int> TerrainStackLimits = {
        {TerrainType::NONE, 0},
        {TerrainType::CITY, 4},
        {TerrainType::CAPITAL_CITY, 4},
        {TerrainType::MOUNTAIN, 2},
        {TerrainType::RIVER, 4},
        {TerrainType::COASTAL, 4},
        {TerrainType::SEA, 4},
        {TerrainType::PORT, 4},
        {TerrainType::HOLY_PLACE, 4},
        {TerrainType::SETTLED, 4},
        {TerrainType::MAGIC_HEX, 4},
        {TerrainType::BATTLEFIELD, 4}};

    // player (control) type
    enum class Player
    {
        NONE = -1,
        NECROMANCER,
        KINGDOM
    };

    // game features
    enum class Feature
    {
        NONE = -1,
        NO_ATTRITION,
        UNLIMITED_STACK_SIZE,
        NO_STACKING
    };

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

    typedef List<Point> Points;

    UnorderedMap<TerrainType, List<UnitType>> AllowedPassage = {
        {TerrainType::CITY, {UnitType::AIR, UnitType::GROUND, UnitType::LEADER}},
        {TerrainType::CAPITAL_CITY, {UnitType::AIR, UnitType::GROUND, UnitType::LEADER}},
        {TerrainType::MOUNTAIN, {UnitType::AIR, UnitType::GROUND, UnitType::LEADER}},
        {TerrainType::RIVER, {UnitType::AIR, UnitType::GROUND, UnitType::LEADER}},
        {TerrainType::COASTAL, {UnitType::AIR, UnitType::GROUND, UnitType::LEADER, UnitType::NAVAL}},
        {TerrainType::SEA, {UnitType::AIR, UnitType::LEADER, UnitType::NAVAL}},
        {TerrainType::PORT, {UnitType::AIR, UnitType::GROUND, UnitType::LEADER, UnitType::NAVAL}},
        {TerrainType::HOLY_PLACE, {UnitType::AIR, UnitType::GROUND, UnitType::LEADER}},
        {TerrainType::SETTLED, {UnitType::AIR, UnitType::GROUND, UnitType::LEADER}},
        {TerrainType::MAGIC_HEX, {UnitType::AIR, UnitType::GROUND, UnitType::LEADER}},
        {TerrainType::BATTLEFIELD, {UnitType::AIR, UnitType::GROUND, UnitType::LEADER}}};

    Points Add(Points &points, Point origin)
    {
        auto translated = Points();

        for (auto point : points)
        {
            translated.push_back(point + origin);
        }

        return points;
    }

    struct ColorScheme
    {
        Uint32 Square = 0;

        Uint32 Circle = 0;
    };
}

#endif
