#pragma once

#include <map>

#include "random.hpp"


namespace Item
{
    enum Item : uint16_t
    {
        buster     = 0x0048,
        tornado    = 0x0248,
        earthCrest = 0x0848,
        airCrest   = 0x0A48,

        hp1 = 0x0149,
        hp2 = 0x0249,
        hp7 = 0x0749,

        stage1_Vellum = 0x002D,
        stage1_Potion = 0x0A2D,
        stage3_Potion = 0x0E2D,
    };
}

namespace Location
{
    enum Locations : uint32_t
    {
        stage1_Vellum = 0x00B4FD,
        stage1_Hp     = 0x0251A3,
        stage1_Potion = 0x02511E,
        somulo        = 0x0196D3,
        hippogriff1   = 0x011999,
        arma1         = 0x00F0A2,

        ovnunu        = 0x01C6D4,

        stage3_Potion = 0x02558E,
        flameLord     = 0x014CFD,

        arma2         = 0x00F0A4,
    };
}

struct ItemData
{
    uint8_t completionCheckOffset;
    uint8_t completionCheckBit;
    std::string name;
};

struct LocationData
{
    bool shouldExit;
    bool noBounce;
    uint32_t bossDefeatedOffset; //name? check if boss already defeated
    std::string name;
};


void AsmAndData();
void PrintLocations();

std::vector<uint8_t> rom;
Random rng;
std::vector<uint16_t> itemList;
std::vector<uint32_t> locationList;

const std::map<uint16_t, ItemData> itemData
{
    {Item::buster,        {0, 0b0000'0001, "Buster"}},
    {Item::tornado,       {0, 0b0000'0010, "Tornado"}},
    {Item::earthCrest,    {0, 0b0001'0000, "Earth crest"}},
    {Item::airCrest,      {0, 0b0010'0000, "Air Crest"}},

    {Item::hp1,           {3, 0b0000'0001, "Hp 1"}},
    {Item::hp2,           {3, 0b0000'0010, "Hp 2"}},
    {Item::hp7,           {3, 0b0100'0000, "Hp 7"}},

    {Item::stage1_Vellum, {5, 0b0000'0001, "Vellum 1"}},
    {Item::stage1_Potion, {5, 0b0000'0010, "Potion 1"}},
    {Item::stage3_Potion, {5, 0b1000'0000, "Potion 2"}},
};

const std::map<uint32_t, LocationData> locationData
{
    {Location::stage1_Vellum, {0, 0,      0, "Stage 1 vellum"}},
    {Location::stage1_Hp,     {0, 1,      0, "Stage 1 hp"}},
    {Location::stage1_Potion, {0, 1,      0, "Stage 1 potion"}},
    {Location::somulo,        {1, 0,      0, "Somulo"}},
    {Location::hippogriff1,   {1, 0, 0x8123, "Hippogriff 1"}},
    {Location::arma1,         {1, 0, 0x8125, "Arma 1"}},

    {Location::ovnunu,        {1, 1,      0, "Ovnunu"}},

    {Location::stage3_Potion, {0, 1,      0, "Stage 3 potion"}},
    {Location::flameLord,     {1, 1,      0, "Flame Lord"}},

    {Location::arma2,         {1, 0,      0, "Arma 2"}},
};

const std::map<uint32_t, std::vector<uint8_t>> customAsm
{
    {0x016B22, {0xAA, 0xBF, 0xFF, 0xD4, 0xFF, 0xD0, 0x9F, 0xEA}}, //HP exits area/stage check
};
