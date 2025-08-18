#ifndef __UNIT_HPP__
#define __UNIT_HPP__

#include "Asset.hpp"
#include "Types.hpp"

namespace DarkEmperor::Unit
{
    // size of the square sides in pixels
    const int Side = 64;

    // size of the radius
    const int Radius = 32;

    class Base
    {
    public:
        UnitType Type = UnitType::NONE;

        Kingdom Kingdom = Kingdom::NONE;

        Mercenary Mercenary = Mercenary::NONE;

        int Id = -1;

        int Combat = -1;

        int Undead = -1;

        int Hero = -1;

        int Magic = -1;

        int Asset = Asset::NONE;

        bool IsUndead = false;

        RuneType Rune = RuneType::NONE;

        std::string Name = std::string();

        Base() {}
    };

    // kingdom color scheme mapping
    UnorderedMap<Kingdom, ColorScheme> KingdomColors = {
        {Kingdom::NECROMANCER, {Color::Grey, Color::Grey}},
        {Kingdom::TAL_PLETOR, {Color::Grey, Color::Yellow}},
        {Kingdom::ZOLAHAURESLOR, {Color::Purple, Color::Purple}},
        {Kingdom::STAVROR, {Color::Green, Color::Orange}},
        {Kingdom::THE_SCYTHE, {Color::Green, Color::White}},
        {Kingdom::STARKEEP, {Color::Red, Color::Green}},
        {Kingdom::KELARON_OIRET, {Color::Red, Color::Yellow}},
        {Kingdom::FERLARIE, {Color::Blue, Color::White}},
        {Kingdom::AHAUTSIERON, {Color::Blue, Color::Yellow}},
        {Kingdom::LAMMARECH, {Color::Brown, Color::Orange}},
        {Kingdom::LOYMARECH, {Color::Brown, Color::Yellow}}};

    // mercenary color scheme mapping
    UnorderedMap<Mercenary, ColorScheme> MercenaryColors = {
        {Mercenary::COS_DOL_COS, {Color::White, Color::Yellow}},
        {Mercenary::LORD_MONTOY, {Color::White, Color::Red}},
        {Mercenary::FERNAN_CONNIVER, {Color::White, Color::Green}},
        {Mercenary::SILWER_FLAGRIEL, {Color::White, Color::Orange}},
        {Mercenary::SAAR, {Color::White, Color::Blue}},
        {Mercenary::HOUNDMASTER, {Color::White, Color::Purple}}};

    UnorderedMap<UnitType, ColorScheme> UnitColors = {
        {UnitType::BATTLEFIELD, {Color::Grey, Color::White}},
        {UnitType::MAGICAL_DEVICE, {Color::Black, Color::Black}},
        {UnitType::MAGICAL_DEVICE, {Color::Black, Color::Black}},
        {UnitType::TRENCH, {Color::White, Color::White}},
        {UnitType::FLAME, {Color::White, Color::White}},
        {UnitType::WEAPON, {Color::White, Color::White}},
        {UnitType::MONSTER, {Color::White, Color::White}}};

    // set kingdom's color scheme
    void SetColor(Kingdom kingdom, ColorScheme scheme)
    {
        KingdomColors[kingdom] = scheme;
    }

    // get kingdom's color scheme
    ColorScheme GetColors(Kingdom kingdom)
    {
        return DarkEmperor::Has(KingdomColors, kingdom) ? KingdomColors[kingdom] : ColorScheme{0, 0};
    }

    // set mercenary's color scheme
    void SetColor(Mercenary Mercenary, ColorScheme scheme)
    {
        MercenaryColors[Mercenary] = scheme;
    }

    // get mercenary's color scheme
    ColorScheme GetColors(Mercenary mercenary)
    {
        return DarkEmperor::Has(MercenaryColors, mercenary) ? MercenaryColors[mercenary] : ColorScheme{0, 0};
    }

    // set unit color scheme (non-KINGDOM/MERCENARY)
    void SetColor(UnitType unit, ColorScheme scheme)
    {
        UnitColors[unit] = scheme;
    }

    // get unit color scheme (non-KINGDOM/MERCENARY)
    ColorScheme GetColors(UnitType unit)
    {
        return DarkEmperor::Has(UnitColors, unit) ? UnitColors[unit] : ColorScheme{0, 0};
    }

    // get color scheme (KINGDOM/MERCENARY/UNIT)
    ColorScheme GetColors(Unit::Base &unit)
    {
        if (unit.Kingdom != Kingdom::NONE)
        {
            return Unit::GetColors(unit.Kingdom);
        }
        else if (unit.Mercenary != Mercenary::NONE)
        {
            return Unit::GetColors(unit.Mercenary);
        }
        else if (unit.Type != UnitType::NONE && unit.Type != UnitType::AIR && unit.Type != UnitType::GROUND && unit.Type != UnitType::NAVAL && unit.Type != UnitType::LEADER)
        {
            return Unit::GetColors(unit.Type);
        }
        else
        {
            return {0, 0};
        }
    }
}

#endif
