#ifndef __UNIT_HPP__
#define __UNIT_HPP__

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

        bool IsUndead = false;

        RuneType Rune = RuneType::NONE;

        std::string Name = std::string();

        Base() {}
    };

    typedef List<Unit::Base> Units;
}

#endif
