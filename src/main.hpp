#pragma once

#include <map>

#include "random.hpp"


enum ItemsAndEvents : uint16_t
{
    //items
    hp1 = 0x0149,
    hp2 = 0x0249,

    vellum1 = 0x002D,
    potion1 = 0x0A2D,

    tornado    = 0x0248,
    earthCrest = 0x0848,
    airCrest   = 0x0A48,

    //events
    stage1_Statue,
    stage1_Potion,
};

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

std::vector<uint8_t> rom;
Random rng;


std::vector<uint16_t> itemList
{
    hp1, hp2,
    vellum1, potion1,
    tornado, earthCrest, airCrest,
};

const std::vector<ItemOffsets> itemOffsets
{
    {0x00B4FD, 0}, //Stage 1 vellum
    {0x00F0A2, 1}, //Arma 1
    {0x00F0A4, 1}, //Arma 2
    {0x011999, 1}, //Hippogriff 1
    {0x014CFD, 1}, //Flame Lord (| 0x4000)
    {0x0196D3, 1}, //Somulo
    {0x02511E, 0}, //Stage 1 potion (| 0x4000)
};

const std::map<uint16_t, CompletionEvent> completionEvent
{
    {tornado   , {0, 0b0000'0010}},
    {earthCrest, {0, 0b0001'0000}},
    {airCrest  , {0, 0b0010'0000}},

    {vellum1, {1, 0b0000'0010}},
    {potion1, {1, 0b0100'0000}},

    {hp1, {3, 0b01}},
    {hp2, {3, 0b10}},

    {stage1_Statue, {5, 0b01}},
    {stage1_Potion, {5, 0b10}},
};

//checks against the completion list to determine if boss is defeated
const std::map<uint32_t, uint32_t> completionCheckOffsets
{
    {0x011999, 0x8123}, //Hippogriff 1
    {0x00F0A2, 0x8125}, //Arma 1
};

const std::vector<Asm> customAsm
{
    {0x016B22, {0xAA, 0xBF, 0xFF, 0xD4, 0xFF, 0xD0, 0x9F, 0xEA}}, //HP exits area/stage check
};
