#pragma once

struct GameData
{
	uint32_t offset;
	uint16_t item;
};

#include <cstdint>
#include <vector>


uint64_t seed = 0;

const std::vector<GameData> gameData
{
    {0x00B4FD, 0x002D}, //Stage 1      - Vellum 1
    {0x00F0A2, 0x0848}, //Arma 1 drop  - Air crest
    {0x00F0A4, 0x0A48}, //Arma 2 drop  - Earth crest
    {0x011990, 0x0249}, //Hippogriff 1 - HP
    {0x014CFC, 0x0248}, //Flame Lord   - Tornado (| 0x4000)
    {0x0196C1, 0x0149}, //Somulo       - HP
    {0x02511E, 0x4A2D}, //Stage 1      - Potion 1 (check 4000)
};
