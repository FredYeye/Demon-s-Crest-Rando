#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <chrono>

#include "main.hpp"
#include "file.hpp"


int main(int argc, char* argv[]) {
    if(itemData.size() != locationData.size()) { //should never happen on release
        std::cout << "item and location counts do not match (" << itemData.size() << " items, " << locationData.size() << " locations)\nPress any key to exit\n";
        std::cin.get();
        exit(0);
    }

    std::string fileName = "Demon's Crest (USA).sfc";
    uint64_t seed = 0;

    if(argc >= 3) {
        for(int x = 1; x < argc - 1; ++x) {
            if(std::string(argv[x]) == "-s") {
                seed = std::hash<std::string>{}(argv[x+1]);
            } else if(std::string(argv[x]) == "-i") {
                fileName = argv[x + 1];
            }
        }
    }

    std::cout << "\nDemon's Crest Randomizer\n\nOptions:\n  -s [any characters]: custom seed. If none is supplied, one will be generated.\n  -i [name]: file to look for. If no name is suppied, \"Demon's Crest (USA).sfc\" will be used.\n\n";
    std::cout << "Example:\n  DCRando -s \"Give me the hand talisman in stage 1 pls!\"\n\n==========\n\n";

    rom = FileToU8Vec(fileName);

    if(rom[0x007FC8] != 0x63) { //crude check to make sure this is a valid rom
        std::cout << "needs a US rom to work for now\nPress any key to exit\n";
        std::cin.get();
        exit(0);
    }

    if(!seed) {
        std::cout << "No custom seed supplied, generating seed.\n";
        std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
        seed = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
    }
    std::cout << "Seed: " << seed << "\n";
    rng.Init(seed);

    PlaceItems();
    StoreNewItemPlacements();
    AsmAndData();
    FixGfx();
    U8vecToFile(rom, "DCRando.sfc");

    PrintLocations(seed);

    std::cout << "\nDone!\n";
}


void PlaceItems() {
    //create lists of all items and locations and shuffle them
    std::vector<Item> itemList;
    std::vector<Location> locationList;

    for(const auto [key, value] : itemData) {
        itemList.push_back(key);
    }
    for(const auto [key, value] : locationData) {
        locationList.push_back(key);
    }

    rng.Shuffle(itemList); //shuffle just once or every loop?
    rng.Shuffle(locationList);

    //pairing/loop variables
    uint8_t currentAbilities = 0;
    bool reqLocationsFilled = false;

    int iLoc = 0, iItem = 0;
    int loopCounter = 0; //break loop if it takes too long; something went wrong

    while(locationList.size() > 0) {
        const Location &location = locationList[iLoc];
        const Item &item = itemList[iItem];

        const uint8_t &locRequirement = locationData.at(location).requirement;
        const uint8_t &itemAbility = uint8_t(itemData.at(item).ability);

        const bool abilityReqMet = (locRequirement & currentAbilities == locRequirement);

        if(
            locRequirement && (abilityReqMet || !itemAbility) || //if location has a requirement and is met or ability isn't needed
            reqLocationsFilled //lastly, fill locations without requirements
        ) {
            //an item and location has successfully been paired. remove from lists and start over
            locItemPair.insert({location, item});
            locationList.erase(locationList.begin() + iLoc);
            itemList.erase(itemList.begin() + iItem);

            iLoc = 0;
            iItem = 0;

            currentAbilities |= itemAbility;
        } else {
            if(++iItem >= itemList.size()) { //try next item, unless we've tried every item
                iItem = 0;

                if(++iLoc >= locationList.size()) { //try next location, until we've tried all
                    iLoc = 0;

                    //check if all locations with requirements are filled. enable free spots if so
                    bool stillReqStages = false;
                    for(const Location loc : locationList) {
                        stillReqStages |= locationData.at(loc).requirement;
                    }
                    reqLocationsFilled = !stillReqStages;

                    if(++loopCounter > 2500) { //loop has stalled. abort
                        std::cout << "Item-location pairing process has locked!\nRemaining items:\n";
                        for(const auto v : itemList)     {std::cout << "  " << itemData.at(v).name     << "\n";}
                        std::cout << "Remaining locations:\n";
                        for(const auto v : locationList) {std::cout << "  " << locationData.at(v).name << "\n";}
                        std::cout << "Press any key to exit\n";
                        std::cin.get();
                        exit(0);
                    }
                }
            }
        }
    }
}


void StoreNewItemPlacements() {
    for(const auto [loc, item] : locItemPair) {
        if(loc == Location::crawler || loc == Location::grewon) {
            rom[uint32_t(loc)    ] = uint16_t(item) >> 8;
            rom[uint32_t(loc) + 3] = uint16_t(item);
        } else { //default
            WriteU16(uint16_t(item), uint32_t(loc));

            if(locationData.at(loc).noBounce) {
                rom[uint32_t(loc) + 1] |= 0x40; // or item value with 0x4000 to stop item bounce
            }
        }

        if(const uint32_t &offset = locationData.at(loc).bossDefeatedOffset; offset != 0) {
            if(loc == Location::crawler) {
                rom[offset    ] = itemData.at(item).completionCheckOffset;
                rom[offset + 6] = itemData.at(item).completionCheckBit;
                crawlerOffset = rom[offset];
                crawlerBit = rom[offset + 6];
            } else if(loc == Location::ovnunu || loc == Location::trioThePago) {
                rom[offset    ] = 0x51 + itemData.at(item).completionCheckOffset;
                rom[offset + 3] = itemData.at(item).completionCheckBit;
            } else { //default
                rom[offset    ] = itemData.at(item).completionCheckOffset;
                rom[offset + 1] = itemData.at(item).completionCheckBit;

                if(loc == Location::arma3) { //needs additional location fix
                    rom[0x1F63E2] = 0x51 + itemData.at(item).completionCheckOffset;
                    rom[0x1F63E5] = itemData.at(item).completionCheckBit;
                }
            }
        }
    }

    //clear upper byte of trio the pago completion check bit
    rom[uint32_t(Location::trioThePago) + 4] = 0;

    //scula head & body are considered different entities and have different drops. copy to other part
    rom[uint32_t(Location::scula2)    ] = rom[uint32_t(Location::scula)    ];
    rom[uint32_t(Location::scula2) + 1] = rom[uint32_t(Location::scula) + 1];
}


void AsmAndData() {
    for(const auto [offset, code] : customAsm) { //inject custom asm
        std::copy(code.begin(), code.end(), rom.begin() + offset);
    }

    rom[0x016ADF] = 0x40; //reduce wait time on picking up crest powers
    rom[0x018A3D] = 0x04; //reduce somulo hp, 7 -> 4
    rom[0x1E2045] = 0xB8; //change trio the pago timer, 40s -> 50s
    rom[0x1E2046] = 0x0B; //^

    //crawler's item data gets overwritten by the custom asm injection. re-add
    const uint32_t &crawlerFix = locationData.at(Location::crawler).bossDefeatedOffset;
    rom[crawlerFix] = crawlerOffset;
    rom[crawlerFix + 6] = crawlerBit;

    for(const auto [loc, locData] : locationData) {
        if(locData.shouldExit) { //update item to exit area/stage if necessary
            uint8_t offset  = (loc == Location::grewon || loc == Location::crawler) ? 3 : 0;
            uint8_t offset2 = (loc == Location::grewon || loc == Location::crawler) ? 0 : 1;

            switch(rom[uint32_t(loc) + offset]) {
                case 0x2D: rom[ 0x1FD547      + ((rom[uint32_t(loc) + offset2] & ~0x40) >> 1)] = 1; break;
                case 0x2E: rom[ 0x1FD5D5      + ((rom[uint32_t(loc) + offset2] & ~0x40) >> 1)] = 1; break;
                case 0x48: rom[ 0x1FD597      + ((rom[uint32_t(loc) + offset2] & ~0x40) >> 1)] = 2; break;
                case 0x49: rom[(0x1FD500 - 1) +  (rom[uint32_t(loc) + offset2] & ~0x40      )] = 1; break;

                default: std::cout << "Updating exit offset failed at location " << locData.name << "\n";
            }
        } else if(rom[uint32_t(loc)] == 0x48) { //set powerups to not exit
            rom[0x1FD597 + ((rom[uint32_t(loc) + 1] & ~0x40) >> 1)] = 6;
        }
    }
}


void PrintLocations(const uint64_t seed) {
    std::ofstream logFile("log.txt", std::ios::out | std::ios::binary);

    logFile << "Seed: " << seed << "\n\n";

    for(const auto stage : printOrder) {
        for(const auto loc : stage) {
            logFile << std::setw(27) << std::left
                    << locationData.at(loc).name << " | "
                    << itemData.at(locItemPair.at(loc)).name << "\n";
        }

        logFile << "\n";
    }

    logFile.close();
}

void FixGfx() {
    const std::map<ItemType, ItemGfx> spriteData {
        {ItemType::fire,          {0x0FC, 0x4F}},
        {ItemType::crest,         {0x0F4, 0x4D}},
        {ItemType::vellum,        {0x138, 0x4E}},
        {ItemType::potion,        {0x0F8, 0x4E}},
        {ItemType::talismanArmor, {0x100, 0x50}},
        {ItemType::talismanHand,  {0x148, 0x50}},
    };

    const std::map<Location, GraphicsOffset> graphicsOffset {
        {Location::stage1_Vellum, {0x1E9A4D, 0xAF15}},
        {Location::stage1_Potion, {0x1E9A57, 0xAF22}},
        {Location::arma1,         {0x1E9D92, 0xAF39}},
    };

    for(const auto [loc, addr] : graphicsOffset) {
        const ItemType itemType = ItemToItemType(locItemPair.at(loc));

        if(spriteData.contains(itemType)) { //todo: remove .contains eventually
            const ItemGfx &itemGfx = spriteData.at(itemType);
            WriteU16(itemGfx.tileSet, addr.tileSet);
            rom[addr.tileType] = itemGfx.spriteSlot;

            std::cout << itemData.at(locItemPair.at(loc)).name << " fixed at " << locationData.at(loc).name << "\n";
        }
    }
}

void WriteU16(const uint16_t value, const uint32_t addr) {
    rom[addr    ] = value;
    rom[addr + 1] = value >> 8;
}

ItemType ItemToItemType(const Item item) {
    ItemType itemType = ItemType::unknown;

    if(uint8_t(item) == 0x48) {
        itemType = (uint16_t(item) < 0x0800) ? ItemType::fire : ItemType::crest;
    } else if(uint16_t(item) == 0x042E) {
        itemType = ItemType::talismanArmor;
    } else if(uint16_t(item) == 0x082E) {
        itemType = ItemType::talismanHand;
    }

    return itemType;
}
