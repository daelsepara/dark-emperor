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
        // type of unit (see Types.hpp)
        UnitType Type = UnitType::NONE;

        // leader (see Types.hpp)
        Leader Leader = Leader::NONE;

        // kingdom affiliation (see Types.hpp)
        Kingdom Kingdom = Kingdom::NONE;

        // mercenary affiliation (see Types.hpp)
        Mercenary Mercenary = Mercenary::NONE;

        // magic rune (see Types.hpp)
        RuneType Rune = RuneType::NONE;

        // unit id
        int Id = -1;

        // combat strength
        int Combat = -1;

        // undead combat strength
        int Undead = -1;

        // hero rating
        int Hero = -1;

        // magic strength
        int Magic = -1;

        // asset (texture) id
        int Asset = Asset::NONE;

        // flag if the unit is dead and uses undead combat strength
        bool IsUndead = false;

        // for magical devices / monsters / weapons
        bool Revealed = false;

        std::string Name = std::string();

        // super-constructor
        Base(UnitType unit_type, DarkEmperor::Leader leader, DarkEmperor::Kingdom kingdom, DarkEmperor::Mercenary mercenary, RuneType rune_type, int combat, int undead, int hero, int magic, int asset, bool is_undead, std::string name) : Type(unit_type), Leader(leader), Kingdom(kingdom), Mercenary(mercenary), Rune(rune_type), Combat(combat), Undead(undead), Hero(hero), Magic(magic), Asset(asset), IsUndead(is_undead), Name(name) {}

        // leader constructor
        Base(DarkEmperor::Leader leader, DarkEmperor::Kingdom kingdom, DarkEmperor::Mercenary mercenary, RuneType rune_type, int hero, int magic, int asset, std::string name) : Leader(leader), Kingdom(kingdom), Mercenary(mercenary), Rune(rune_type), Hero(hero), Magic(magic), Asset(asset), Name(name) { this->Type = UnitType::LEADER; }

        // air / ground / naval unit constuctor (non-mercenary)
        Base(UnitType unit_type, DarkEmperor::Kingdom kingdom, int combat, int undead, int asset, std::string name) : Type(unit_type), Kingdom(kingdom), Combat(combat), Undead(undead), Asset(asset), Name(name) {}

        // mercenary (non-leader)
        Base(UnitType unit_type, DarkEmperor::Mercenary mercenary, int combat, int undead, int asset, std::string name) : Type(unit_type), Mercenary(mercenary), Combat(combat), Undead(undead), Asset(asset), Name(name) {}

        // magical device / monster
        Base(UnitType unit_type, RuneType rune_type, int hero, int magic, int asset, std::string name) : Type(unit_type), Rune(rune_type), Hero(hero), Magic(magic), Asset(asset), Name(name) {}

        // weapon / entrenchments / flame marker
        Base(UnitType unit_type, int asset, std::string name) : Type(unit_type), Asset(asset), Name(name) {}

        // battlefield
        Base(UnitType unit_type, int combat, int asset, std::string name) : Type(unit_type), Combat(combat), Asset(asset), Name(name) {}

        // copy constructor
        Base(const Base &unit, int id)
        {
            if (id < 0)
            {
                std::string error_message = "Invalid unit id: " + std::to_string(id);

                throw std::invalid_argument(error_message);
            }
            else
            {
                // copy unit
                *this = unit;

                // assign id
                this->Id = id;
            }
        }

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
