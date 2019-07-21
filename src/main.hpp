#pragma once

#include <map>

#include "random.hpp"


enum ItemsAndEvents : uint16_t
{
    //items
    hp1 = 0x0149,
    hp2 = 0x0249,
    hp7 = 0x0749,

    vellum1 = 0x002D,
    potion1 = 0x0A2D,

    buster     = 0x0048,
    tornado    = 0x0248,
    earthCrest = 0x0848,
    airCrest   = 0x0A48,

    //events
    stage1_Statue,
    stage1_Potion,
};

namespace Location
{
    enum Locations : uint32_t
    {
        stage1_Vellum = 0x00B4FD,
        arma1         = 0x00F0A2,
        arma2         = 0x00F0A4,
        hippogriff1   = 0x011999,
        flameLord     = 0x014CFD,
        somulo        = 0x0196D3,
        ovnunu        = 0x01C6D4,
        stage1_Potion = 0x02511E,
        stage1_Hp     = 0x0251A3,
    };
}

struct GameData
{
	uint32_t offset;
	uint16_t item;
};

struct ItemOffsets
{
    uint32_t offset;
    bool shouldExit;
};

struct CompletionEvent
{
    uint8_t completionCheckOffset;
    uint8_t completionCheckBit;
};

struct Asm
{
    uint32_t offset;
    std::vector<uint8_t> code;
};

//-----

void AsmAndData();
void PrintLocations();

std::vector<uint8_t> rom;
Random rng;


std::vector<uint16_t> itemList
{
    hp1, hp2,
    vellum1, potion1,
    buster, tornado, earthCrest, airCrest,
};

const std::vector<ItemOffsets> itemOffsets
{
    {Location::stage1_Vellum, 0},
    {Location::arma1, 1},
    {Location::arma2, 1},
    {Location::hippogriff1, 1},
    {Location::flameLord, 1}, // | 0x4000
    {Location::somulo, 1},
    {Location::ovnunu, 1}, // | 0x4000
    {Location::stage1_Potion, 0}, // | 0x4000
    {Location::stage1_Hp, 0}, // 0x4000
};

const std::map<uint16_t, CompletionEvent> completionEvent
{
    {buster    , {0, 0b0000'0001}},
    {tornado   , {0, 0b0000'0010}},
    {earthCrest, {0, 0b0001'0000}},
    {airCrest  , {0, 0b0010'0000}},

    {vellum1, {1, 0b0000'0010}},
    {potion1, {1, 0b0100'0000}},

    {hp1, {3, 0b0000'0001}},
    {hp2, {3, 0b0000'0010}},
    {hp7, {3, 0b0100'0000}},

    {stage1_Statue, {5, 0b01}},
    {stage1_Potion, {5, 0b10}},
};

//checks against the completion list to determine if boss is defeated
const std::map<uint32_t, uint32_t> completionCheckOffsets
{
    {Location::hippogriff1, 0x8123},
    {Location::arma1, 0x8125},
};

const std::vector<Asm> customAsm
{
    {0x016B22, {0xAA, 0xBF, 0xFF, 0xD4, 0xFF, 0xD0, 0x9F, 0xEA}}, //HP exits area/stage check
};

const std::map<uint16_t, std::string> itemNames
{
    {hp1, "Hp 1"},
    {hp2, "Hp 2"},
    {hp7, "Hp 7"},

    {vellum1, "Vellum 1"},
    {potion1, "Potion 1"},

    {buster,     "Buster"},
    {tornado,    "Tornado"},
    {earthCrest, "Earth crest"},
    {airCrest,   "Air crest"},
};

const std::map<uint32_t, std::string> locationNames
{
    {Location::stage1_Vellum, "Stage 1 vellum"},
    {Location::arma1,         "Arma 1"},
    {Location::arma2,         "Arma 2"},
    {Location::hippogriff1,   "Hippogriff 1"},
    {Location::flameLord,     "Flame Lord"},
    {Location::somulo,        "Somulo"},
    {Location::ovnunu,        "Ovnunu"},
    {Location::stage1_Potion, "Stage 1 potion"},
    {Location::stage1_Hp,     "Stage 1 Hp"},
};
