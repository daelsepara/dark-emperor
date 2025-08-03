#ifndef __TYPES_HPP__
#define __TYPES_HPP__

#include <vector>

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
        SEA,
        PORT,
        HOLY_PLACE,
        SETTLED,
        MAGIC_HEX,
        BATTLEFIELD
    };

    std::vector<TerrainType> GroundPassable = {
        TerrainType::CITY,
        TerrainType::CAPITAL_CITY,
        TerrainType::MOUNTAIN,
        TerrainType::RIVER,
        TerrainType::PORT,
        TerrainType::HOLY_PLACE,
        TerrainType::SETTLED,
        TerrainType::MAGIC_HEX,
        TerrainType::BATTLEFIELD};

    std::vector<TerrainType> NavalPassable = {
        TerrainType::RIVER,
        TerrainType::PORT,
        TerrainType::SEA,
        TerrainType::HOLY_PLACE,
        TerrainType::MAGIC_HEX,
        TerrainType::BATTLEFIELD};

    std::vector<TerrainType> AirPassable = {
        TerrainType::CITY,
        TerrainType::CAPITAL_CITY,
        TerrainType::MOUNTAIN,
        TerrainType::RIVER,
        TerrainType::SEA,
        TerrainType::PORT,
        TerrainType::HOLY_PLACE,
        TerrainType::SETTLED,
        TerrainType::MAGIC_HEX,
        TerrainType::BATTLEFIELD};

    enum class UnitType
    {
        NONE = -1,
        GROUND,
        AIR,
        NAVAL,
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

    enum class Nationality
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
        ZOLAHAURESLOR
    };

    // individual unit id (id, type, nationality)
    typedef struct UnitId
    {
        int Id = -1;

        UnitType Type = UnitType::NONE;

        Nationality Nationality = Nationality::NONE;

    } UnitId;

    // unit id collection
    typedef std::vector<UnitId> Units;

    // for unit-stacking in the map
    typedef struct UnitStack
    {
        // nationality
        Nationality Nationality = Nationality::NONE;

        // lowest unit type denominator (e.g. GROUND, AIR, NAVAL)
        UnitType Type = UnitType::NONE;

        // actual unit composition
        Units Units = {};

    } UnitStack;

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

    enum class MercenaryType
    {
        NONE = -1,
        COS_DOL_COS,
        LORD_MONTOY,
        HOUNDMASTER,
        SAAR,
        SILWER_FLAGRIEL,
        FERNAN_CONNIVER
    };
}

#endif
