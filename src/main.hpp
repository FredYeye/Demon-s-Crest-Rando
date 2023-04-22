#pragma once

#include <map>

#include "random.hpp"


enum class Item : uint16_t {
    buster     = 0x0048,
    tornado    = 0x0248,
    claw       = 0x0448,
    demonFire  = 0x0648,
    earthCrest = 0x0848,
    airCrest   = 0x0A48,
    waterCrest = 0x0C48,
    timeCrest  = 0x0E48,

    hp1  = 0x0149, //somulo
    hp2  = 0x0249,
    hp3  = 0x0349, //belth
    hp4  = 0x0449, //scula
    hp5  = 0x0549,
    hp6  = 0x0649, //holothurion
    hp7  = 0x0749,
    hp8  = 0x0849, //stage 2 pot
    hp9  = 0x0949, //stage 2 bone pile
    hp10 = 0x0A49,
    hp11 = 0x0B49, //stage 3 water route
    hp12 = 0x0C49, //trio the pago
    hp13 = 0x0D49, //stage 5 water route
    hp14 = 0x0E49,
    hp15 = 0x0F49, //stage 6

    stage1_Vellum = 0x002D,
    stage2_Vellum = 0x022D,
    stage3_Vellum = 0x042D,
    stage4_Vellum = 0x062D,
    stage6_Vellum = 0x082D,
    stage1_Potion = 0x0A2D,
    stage2_Potion = 0x0C2D,
    stage3_Potion = 0x0E2D,
    stage4_Potion = 0x102D,
    stage6_Potion = 0x122D,

    crown = 0x002E,
    skull = 0x022E,
    armor = 0x042E,
    hand  = 0x082E,
};

enum class Location : uint32_t {
    stage1_Vellum = 0x00B4FD,
    stage1_Hp     = 0x0251A3,
    stage1_Potion = 0x02511E,
    somulo        = 0x0196D3,
    hippogriff1   = 0x011999,
    arma1         = 0x00F0A2,

    stage2_Potion = 0x025259,
    hand          = 0x0252D1,
    stage2_Vellum = 0x00D862,
    stage2_Hp2    = 0x00D864,
    stage2_Hp1    = 0x00B509,
    belth         = 0x01E8D8,
    ovnunu        = 0x01C6D4,

    stage3_Potion = 0x02558E,
    stage3_Vellum = 0x00D866,
    stage3_Hp1    = 0x026163,
    stage3_Hp2    = 0x00D868,
    skull         = 0x0257B1,
    flameLord     = 0x014CFD,
    scula         = 0x1E8B99, //head
    scula2        = 0x1E85AD, //body

    stage4_Potion = 0x025813,
    stage4_Vellum = 0x0259BC,
    crown         = 0x00B505,
    flier         = 0x02DBA0,
    arma2         = 0x00F0A4,

    stage5_Hp1    = 0x00B50D,
    stage5_Hp2    = 0x00B50B,
    stage5_Hp3    = 0x00D86A,
    crawler       = 0x013A12,
    holothurion   = 0x01D7B7,

    stage6_Potion = 0x025CBC,
    stage6_Hp     = 0x00D86C,
    armor         = 0x00B511,
    stage6_Vellum = 0x00B50F,
    arma3         = 0x017009,
    grewon        = 0x1F1E23,

    trioThePago   = 0x1E213E,
};

enum class Requirement : uint8_t {
    buster = 0b0000'0001, //buster & legendary
    ground = 0b0000'0010, //dash into breakable objects
    flight = 0b0000'0100, //air crest & tornado
    swim   = 0b0000'1000, //water crest
};

struct ItemData {
    uint8_t completionCheckOffset;
    uint8_t completionCheckBit;
    uint8_t ability;
    std::string name;
};

struct LocationData {
    bool shouldExit;
    bool noBounce;
    uint8_t requirement;
    uint32_t bossDefeatedOffset;
    std::string name;
};

std::map<Location, Item> locItemPair;

void PlaceItems();
void StoreNewItemPlacements();
void AsmAndData();
void FixGfx();
void PrintLocations(uint64_t seed);

Random rng;
std::vector<uint8_t> rom;
uint8_t crawlerOffset, crawlerBit;

const std::map<Item, ItemData> itemData {
    {Item::buster,        {0, 0b0000'0001, 0b0001, "Buster"}},
    {Item::tornado,       {0, 0b0000'0010, 0b0100, "Tornado"}},
    {Item::claw,          {0, 0b0000'0100,      0, "Claw"}},
    {Item::demonFire,     {0, 0b0000'1000,      0, "Demon fire"}},
    {Item::earthCrest,    {0, 0b0001'0000, 0b0010, "Earth crest"}},
    {Item::airCrest,      {0, 0b0010'0000, 0b0100, "Air Crest"}},
    {Item::waterCrest,    {0, 0b0100'0000, 0b1000, "Water Crest"}},
    {Item::timeCrest,     {0, 0b1000'0000, 0b0001, "Time Crest"}},

    // {Item::ultimate,      {1, 0b0000'0001,       , "Ultimate gargoyle"}},

    {Item::crown,         {2, 0b0000'1000,      0, "Crown"}},
    {Item::skull,         {2, 0b0001'0000,      0, "Skull"}},
    {Item::armor,         {2, 0b0010'0000,      0, "Armor"}},
    {Item::hand,          {2, 0b1000'0000,      0, "Hand"}},

    {Item::hp1,           {3, 0b0000'0001,      0, "Hp 1"}},
    {Item::hp2,           {3, 0b0000'0010,      0, "Hp 2"}},
    {Item::hp3,           {3, 0b0000'0100,      0, "Hp 3"}},
    {Item::hp4,           {3, 0b0000'1000,      0, "Hp 4"}},
    {Item::hp5,           {3, 0b0001'0000,      0, "Hp 5"}},
    {Item::hp6,           {3, 0b0010'0000,      0, "Hp 6"}},
    {Item::hp7,           {3, 0b0100'0000,      0, "Hp 7"}},
    {Item::hp8,           {3, 0b1000'0000,      0, "Hp 8"}},

    {Item::hp9,           {4, 0b0000'0001,      0, "Hp 9"}},
    {Item::hp10,          {4, 0b0000'0010,      0, "Hp 10"}},
    {Item::hp11,          {4, 0b0000'0100,      0, "Hp 11"}},
    {Item::hp12,          {4, 0b0000'1000,      0, "Hp 12"}},
    {Item::hp13,          {4, 0b0001'0000,      0, "Hp 13"}},
    {Item::hp14,          {4, 0b0010'0000,      0, "Hp 14"}},
    {Item::hp15,          {4, 0b0100'0000,      0, "Hp 15"}},

    {Item::stage1_Vellum, {5, 0b0000'0001,      0, "Vellum (s1)"}},
    {Item::stage2_Vellum, {5, 0b0000'0010,      0, "Vellum (s2)"}},
    {Item::stage3_Vellum, {5, 0b0000'0100,      0, "Vellum (s3)"}},
    {Item::stage4_Vellum, {5, 0b0000'1000,      0, "Vellum (s4)"}},
    {Item::stage6_Vellum, {5, 0b0001'0000,      0, "Vellum (s6)"}},
    {Item::stage1_Potion, {5, 0b0010'0000,      0, "Potion (s1)"}},
    {Item::stage2_Potion, {5, 0b0100'0000,      0, "Potion (s2)"}},
    {Item::stage3_Potion, {5, 0b1000'0000,      0, "Potion (s3)"}},

    {Item::stage4_Potion, {6, 0b0000'0001,      0, "Potion (s4)"}},
    {Item::stage6_Potion, {6, 0b0000'0010,      0, "Potion (s6)"}},
};

const std::map<Location, LocationData> locationData {
    {Location::stage1_Vellum, {0, 0,      0,        0, "Vellum (s1)"}},
    {Location::stage1_Hp,     {0, 1, 0b0001,        0, "Stage 1 hp"}},
    {Location::stage1_Potion, {0, 1,      0,        0, "Potion (s1)"}},
    {Location::somulo,        {1, 0,      0,        0, "Somulo"}}, //could see if somulo does a check just for fun
    {Location::hippogriff1,   {1, 0,      0,   0x8123, "Hippogriff 1"}},
    {Location::arma1,         {1, 0,      0,   0x8125, "Arma 1"}},

    {Location::stage2_Potion, {0, 1,      0,        0, "Potion (s2)"}},
    {Location::hand,          {0, 1, 0b1000,        0, "Hand"}},        //water or hp. add secondary req later
    {Location::stage2_Vellum, {0, 0, 0b0011,        0, "Vellum (s2)"}},
    {Location::stage2_Hp2,    {0, 0, 0b0011,        0, "Stage 2 hp (pot)"}},
    {Location::stage2_Hp1,    {0, 0, 0b0010,        0, "Stage 2 hp (bone pile)"}},
    {Location::belth,         {1, 0,      0,   0x8127, "Belth"}},
    {Location::ovnunu,        {1, 1, 0b0010, 0x0209F8, "Ovnunu"}},

    {Location::stage3_Potion, {0, 1,      0,        0, "Potion (s3)"}},
    {Location::stage3_Vellum, {0, 0, 0b0001,        0, "Vellum (s3)"}},
    {Location::stage3_Hp1,    {0, 1,      0,        0, "Stage 3 hp (forest)"}},
    {Location::stage3_Hp2,    {0, 0, 0b1000,        0, "Stage 3 hp (water)"}},
    {Location::skull,         {0, 1, 0b0001,        0, "Skull"}},
    {Location::flameLord,     {1, 1,      0,   0x812B, "Flame Lord"}},
    {Location::scula,         {1, 0,      0,   0x8129, "Scula"}},

    {Location::stage4_Potion, {0, 1, 0b0001,        0, "Potion (s4)"}},
    {Location::stage4_Vellum, {0, 1,      0,        0, "Vellum (s4)"}},
    {Location::crown,         {0, 0,      0,        0, "Crown"}},
    {Location::flier,         {1, 0,      0,   0x812D, "Flier"}},
    {Location::arma2,         {1, 0,      0,   0x812F, "Arma 2"}},

    {Location::stage5_Hp1,    {0, 0, 0b0010,        0, "Stage 5 hp (before crawler)"}},
    {Location::stage5_Hp2,    {0, 0, 0b0010,        0, "Stage 5 hp (above crawler)"}},
    {Location::stage5_Hp3,    {0, 0, 0b1000,        0, "Stage 5 hp (water route)"}},
    {Location::crawler,       {1, 0, 0b0010, 0x1FD5B1, "Crawler"}}, //custom location
    {Location::holothurion,   {1, 1, 0b1000,   0x8131, "Holothurion"}},

    {Location::stage6_Potion, {0, 1, 0b0100,        0, "Potion (s6)"}},
    {Location::stage6_Hp,     {0, 0, 0b0100,        0, "Stage 6 hp"}},
    {Location::armor,         {0, 0, 0b0110,        0, "Armor"}},
    {Location::stage6_Vellum, {0, 0, 0b0010,        0, "Vellum (s6)"}},
    {Location::arma3,         {1, 1, 0b0100,   0x8137, "Arma 3"}},
    {Location::grewon,        {1, 0,      0,   0x8135, "Grewon"}},

    {Location::trioThePago,   {0, 1,      0, 0x1E2AED, "Trio the Pago"}},
};

const std::map<uint32_t, std::vector<uint8_t>> customAsm {
    //HP exits area/stage check
    {0x016B22, {0xC9, 0x1F, 0xF0, 0xBD, 0xAA, 0xBF, 0xFF, 0xD4, 0xBF, 0xD0, 0x9B, 0xEA}},

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

    //stage unlock check
    {0x02A1F6 ,{0xD0, 0x15, 0xA0, 0x03, 0xAD, 0x51, 0x1E, 0x89, 0x00, 0x01, 0xD0, 0x0D, 0xA0, 0x00, 0xAD, 0x56, 0x1E, 0x29, 0x1F, 0x00, 0xC9, 0x1F, 0x00, 0xF0, 0x02, 0xA0, 0x01, 0xE2, 0x20, 0x6B}},
};

const std::vector<std::vector<Location>> printOrder {
    {
        Location::stage1_Vellum,
        Location::stage1_Hp,
        Location::stage1_Potion,
        Location::somulo,
        Location::hippogriff1,
        Location::arma1,
    }, {
        Location::stage2_Potion,
        Location::hand,
        Location::stage2_Vellum,
        Location::stage2_Hp2,
        Location::stage2_Hp1,
        Location::belth,
        Location::ovnunu,
    }, {
        Location::stage3_Potion,
        Location::stage3_Vellum,
        Location::stage3_Hp1,
        Location::stage3_Hp2,
        Location::skull,
        Location::flameLord,
        Location::scula,
    }, {
        Location::stage4_Potion,
        Location::stage4_Vellum,
        Location::crown,
        Location::flier,
        Location::arma2,
    }, {
        Location::stage5_Hp1,
        Location::stage5_Hp2,
        Location::stage5_Hp3,
        Location::crawler,
        Location::holothurion,
    }, {
        Location::stage6_Potion,
        Location::stage6_Hp,
        Location::armor,
        Location::stage6_Vellum,
        Location::arma3,
        Location::grewon,
    }, {
        Location::trioThePago,
    },
};
