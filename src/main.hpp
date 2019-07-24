#pragma once

#include <map>

#include "random.hpp"


namespace Item
{
    enum Item : uint16_t
    {
        buster     = 0x0048,
        tornado    = 0x0248,
        claw       = 0x0448,
        earthCrest = 0x0848,
        airCrest   = 0x0A48,
        waterCrest = 0x0C48,

        hp1  = 0x0149, //somulo
        hp2  = 0x0249,
        hp3  = 0x0349, //belth
        hp4  = 0x0449, //scula
        hp5  = 0x0549,
        hp7  = 0x0749,
        hp9  = 0x0949, //stage 2 bone pile
        hp10 = 0x0A49,
        hp14 = 0x0E49,

        stage1_Vellum = 0x002D,
        stage3_Vellum = 0x042D,
        stage4_Vellum = 0x062D,
        stage1_Potion = 0x0A2D,
        stage3_Potion = 0x0E2D,
        stage4_Potion = 0x102D,

        crown = 0x002E,
        skull = 0x022E,
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

        stage2_Hp     = 0x00B509,
        belth         = 0x01E8D8,
        ovnunu        = 0x01C6D4,

        stage3_Potion = 0x02558E,
        stage3_Vellum = 0x00D866,
        stage3_Hp     = 0x026163,
        skull         = 0x0257B1,
        flameLord     = 0x014CFD,
        scula         = 0x1E8B99,

        stage4_Potion = 0x025813,
        stage4_Vellum = 0x0259BC,
        crown         = 0x00B505,
        flier         = 0x02DBA0,
        arma2         = 0x00F0A4,

        stage5_Hp1    = 0x00B50D,
        stage5_Hp2    = 0x00B50B,
        crawler       = 0x013A12,
    };
}

namespace Requirement
{
    enum Requirements : uint8_t
    {
        buster = 0b0000'0001, //buster & legendary
        ground = 0b0000'0010, //dash into breakable objects
        flight = 0b0000'0100, //air crest & tornado
        swim   = 0b0000'1000, //water crest
    };
}

struct ItemData
{
    uint8_t completionCheckOffset;
    uint8_t completionCheckBit;
    uint8_t ability;
    std::string name;
};

struct LocationData
{
    bool shouldExit;
    bool noBounce;
    uint8_t requirement;
    uint32_t bossDefeatedOffset;
    std::string name;
};


void AsmAndData();
void PrintLocations();

uint64_t seed = 0;
Random rng;
std::vector<uint8_t> rom;
std::vector<uint16_t> itemList;
std::vector<uint32_t> locationList;

uint8_t crawlerOffset, crawlerBit;

const std::map<uint16_t, ItemData> itemData
{
    {Item::buster,        {0, 0b0000'0001, 0b0001, "Buster"}},
    {Item::tornado,       {0, 0b0000'0010, 0b0100, "Tornado"}},
    {Item::claw,          {0, 0b0000'0100,      0, "Claw"}},
    // {Item::demonFire,     {0, 0b0000'1000,       , "Demon fire"}},
    {Item::earthCrest,    {0, 0b0001'0000, 0b0010, "Earth crest"}},
    {Item::airCrest,      {0, 0b0010'0000, 0b0100, "Air Crest"}},
    {Item::waterCrest,    {0, 0b0100'0000, 0b1000, "Water Crest"}},
    // {Item::legendary,     {0, 0b1000'0000,       , "Legendary Gargoyle"}}

    // {Item::ultimate,      {1, 0b0000'0001,       , "Ultimate gargoyle"}},

    {Item::crown,         {2, 0b0000'1000,      0, "Crown"}},
    {Item::skull,         {2, 0b0001'0000,      0, "Skull"}},

    {Item::hp1,           {3, 0b0000'0001,      0, "Hp 1"}},
    {Item::hp2,           {3, 0b0000'0010,      0, "Hp 2"}},
    {Item::hp3,           {3, 0b0000'0100,      0, "Hp 3"}},
    {Item::hp4,           {3, 0b0000'1000,      0, "Hp 4"}},
    {Item::hp5,           {3, 0b0001'0000,      0, "Hp 5"}},
    {Item::hp7,           {3, 0b0100'0000,      0, "Hp 7"}},
    {Item::hp9,           {4, 0b0000'0001,      0, "Hp 9"}},
    {Item::hp10,          {4, 0b0000'0010,      0, "Hp 10"}},
    {Item::hp14,          {4, 0b0010'0000,      0, "Hp 14"}},

    {Item::stage1_Vellum, {5, 0b0000'0001,      0, "Vellum (s1)"}},
    {Item::stage3_Vellum, {5, 0b0000'0100,      0, "Vellum (s3)"}},
    {Item::stage4_Vellum, {5, 0b0000'1000,      0, "Vellum (s4)"}},
    {Item::stage1_Potion, {5, 0b0000'0010,      0, "Potion (s1)"}},
    {Item::stage3_Potion, {5, 0b1000'0000,      0, "Potion (s3)"}},
    {Item::stage4_Potion, {6, 0b0000'0001,      0, "Potion (s4)"}},
};

const std::map<uint32_t, LocationData> locationData
{
    {Location::stage1_Vellum, {0, 0,      0,        0, "Stage 1 Vellum"}},
    {Location::stage1_Hp,     {0, 1, 0b0001,        0, "Stage 1 hp"}},
    {Location::stage1_Potion, {0, 1,      0,        0, "Stage 1 potion"}},
    {Location::somulo,        {1, 0,      0,        0, "Somulo"}}, //could see if somulo does a check just for fun
    {Location::hippogriff1,   {1, 0,      0,   0x8123, "Hippogriff 1"}},
    {Location::arma1,         {1, 0,      0,   0x8125, "Arma 1"}},

    {Location::stage2_Hp,     {0, 0, 0b0010,        0, "Stage 2 hp (1)"}},
    {Location::belth,         {1, 0,      0,   0x8127, "Belth"}},
    {Location::ovnunu,        {1, 1, 0b0010,        0, "Ovnunu"}}, //todo

    {Location::stage3_Potion, {0, 1,      0,        0, "Stage 3 potion"}},
    {Location::stage3_Hp,     {0, 1,      0,        0, "Stage 3 hp"}},
    {Location::skull,         {0, 1, 0b0001,        0, "Skull"}},
    {Location::flameLord,     {1, 1,      0,   0x812B, "Flame Lord"}},
    {Location::stage3_Vellum, {0, 0, 0b0001,        0, "Stage 3 vellum"}},
    {Location::scula,         {1, 0,      0,   0x8129, "Scula"}},

    {Location::stage4_Potion, {0, 1, 0b0001,        0, "Stage 4 potion"}},
    {Location::stage4_Vellum, {0, 1,      0,        0, "Stage 4 vellum"}},
    {Location::crown,         {0, 0,      0,        0, "Crown"}},
    {Location::flier,         {1, 0,      0,   0x812D, "Flier"}},
    {Location::arma2,         {1, 0,      0,   0x812F, "Arma 2"}},

    {Location::stage5_Hp1,    {0, 0, 0b0010,        0, "Stage 5 hp (1)"}},
    {Location::stage5_Hp2,    {0, 0, 0b0010,        0, "Stage 5 hp (2)"}},
    {Location::crawler,       {1, 0, 0b0010, 0x1FD5B1, "Crawler"}},
};

const std::map<uint32_t, std::vector<uint8_t>> customAsm
{
    //HP exits area/stage check
    {0x016B22, {0xAA, 0xBF, 0xFF, 0xD4, 0xFF, 0xD0, 0x9F, 0xEA}},

    //item exits area/stage check
    {0x016148, {0x22, 0x30, 0xD5, 0xBF}},
    {0x1FD530, {0x22, 0x5A, 0x9A, 0x80, 0xE2, 0x30, 0xA5, 0x03, 0x29, 0xBF, 0x4A, 0xAA, 0xBF, 0x47, 0xD5, 0xBF, 0xF0, 0x04, 0x5C, 0x58, 0xBB, 0x80, 0x6B}},

    //power exits area/stage check
    {0x016AD6, {0x22, 0x80, 0xD5, 0xBF}},
    {0x1FD580, {0xA5, 0x02, 0xC9, 0x49, 0xD0, 0x04, 0xA9, 0x02, 0x80, 0x0A, 0xA5, 0x03, 0x29, 0xBF, 0x4A, 0xAA, 0xBF, 0x97, 0xD5, 0xBF, 0x85, 0x05, 0x6B}},

    //crawler item check
    {0x021977 , {0x22, 0xB0, 0xD5, 0xBF, 0xEA}},
    {0x1FD5B0 , {0xA2, 0x00, 0x00, 0xBD, 0x51, 0x1E, 0x29, 0x00, 0x00, 0x6B}},

    //talisman check
    {0x01616A ,{0x22, 0xC0, 0xD5, 0xBF}},
    {0x1FD5C0 ,{0x49, 0x06, 0x85, 0x00, 0xA5, 0x03, 0x29, 0xBF, 0x4A, 0xAA, 0xBF, 0xD5, 0xD5, 0xBF, 0xF0, 0x04, 0x5C, 0x58, 0xBB, 0x80, 0x6B}},
};

const std::vector<std::vector<uint32_t>> printOrder
{
    {
        Location::stage1_Vellum,
        Location::stage1_Hp,
        Location::stage1_Potion,
        Location::somulo,
        Location::hippogriff1,
        Location::arma1,
    },
    {
        Location::stage2_Hp,
        Location::belth,
        Location::ovnunu,
    },
    {
        Location::stage3_Potion,
        Location::stage3_Hp,
        Location::skull,
        Location::flameLord,
    },
    {
        Location::stage4_Potion,
        Location::stage4_Vellum,
        Location::crown,
        Location::flier,
        Location::arma2,
    },
    {
        Location::stage5_Hp1,
        Location::stage5_Hp2,
        Location::crawler,
    },
};
