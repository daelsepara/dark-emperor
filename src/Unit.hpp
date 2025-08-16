#ifndef __UNIT_HPP__
#define __UNIT_HPP__

#include "Asset.hpp"
#include "Types.hpp"

namespace DarkEmperor::Unit
{
    class Base
    {
    public:
        UnitType Type = UnitType::NONE;

        Kingdom Kingdom = Kingdom::NONE;

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
    UnorderedMap<Kingdom, ColorScheme> ColorSchemes = {};

    // set kingdom's color scheme
    void SetColor(Kingdom kingdom, ColorScheme scheme)
    {
        ColorSchemes[kingdom] = scheme;
    }

    // get kingdom's color scheme
    ColorScheme GetColors(Kingdom kingdom)
    {
        return DarkEmperor::Has(ColorSchemes, kingdom) ? ColorSchemes[kingdom] : ColorScheme{0, 0};
    }
}

#endif
