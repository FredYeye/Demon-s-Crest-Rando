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

    if(argc >= 3) {
        for(int x = 1; x < argc - 1; ++x) {
            if(std::string(argv[x]) == "-s") {
                seed = std::hash<std::string>{}(argv[x+1]);
            } else if(std::string(argv[x]) == "-i") {
                fileName = argv[x + 1]; //untested
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

    itemLocPairs = PlaceItems();
    StoreNewItemPlacements();
    AsmAndData();
    U8vecToFile(rom, "DCRando.sfc");
    PrintLocations();

    std::cout << "\nDone!\n";
}


std::vector<ItemLocationPair> PlaceItems() {
    std::vector<ItemLocationPair> itemLocPairs;

    //create lists of all items and locations and shuffle them
    std::vector<uint16_t> itemList;
    std::vector<uint32_t> locationList;
    for(const auto [key, value] : itemData)     {itemList.push_back(key);    }
    for(const auto [key, value] : locationData) {locationList.push_back(key);}

    rng.Init(seed);
    rng.Shuffle(itemList);
    rng.Shuffle(locationList);

    //pairing/loop variables
    uint8_t currentAbilities = 0;
    bool reqLocationsFilled = false;

    int iLoc = 0, iItem = 0;
    int loopCounter = 0; //break loop if it takes too long; something went wrong

    while(locationList.size() > 0) {
        const uint32_t &location = locationList[iLoc];
        const uint16_t &item = itemList[iItem];

        const uint8_t &locRequirement = locationData.at(location).requirement;
        const uint8_t &itemAbility = itemData.at(item).ability;

        const bool abilityReqMet = (locRequirement & currentAbilities == locRequirement);

        if(
            locRequirement &&                    //if location has a requirement, and
            (abilityReqMet  || !itemAbility) ||  //requirement is met, or ability isn't needed.
            reqLocationsFilled                   //lastly, fill locations without requirements
        )
        {
            //an item and location has successfully been paired. remove from lists and start over
            itemLocPairs.push_back({item, location});
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
                    bool stillReqStages  = false;
                    for(const uint32_t loc : locationList) {
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

    return itemLocPairs;
}


void StoreNewItemPlacements() {
    for(const ItemLocationPair il : itemLocPairs) {
        if(il.location == Location::crawler || il.location == Location::grewon) {
            rom[il.location    ] = il.item >> 8;
            rom[il.location + 3] = il.item;
        } else { //default
            rom[il.location    ] = il.item;
            rom[il.location + 1] = il.item >> 8;

            if(locationData.at(il.location).noBounce) {
                rom[il.location + 1] |= 0x40; // or item value with 0x4000 to stop item bounce
            }
        }

        if(const uint32_t &offset = locationData.at(il.location).bossDefeatedOffset; offset != 0) {
            if(il.location == Location::crawler) {
                rom[offset    ] = itemData.at(il.item).completionCheckOffset;
                rom[offset + 6] = itemData.at(il.item).completionCheckBit;
                crawlerOffset = rom[offset];
                crawlerBit = rom[offset + 6];
            } else if(il.location == Location::ovnunu || il.location == Location::trioThePago) {
                rom[offset    ] = 0x51 + itemData.at(il.item).completionCheckOffset;
                rom[offset + 3] = itemData.at(il.item).completionCheckBit;
            } else { //default
                rom[offset    ] = itemData.at(il.item).completionCheckOffset;
                rom[offset + 1] = itemData.at(il.item).completionCheckBit;
            }

            if(il.location == Location::arma3) { //needs additional location fix
                rom[0x1F63E2] = 0x51 + itemData.at(il.item).completionCheckOffset;
                rom[0x1F63E5] = itemData.at(il.item).completionCheckBit;
            }
        }
    }

    //clear upper byte of trio the pago completion check bit
    rom[Location::trioThePago + 4] = 0;

    //scula head & body are considered different entities and have different drops. copy to other part
    rom[Location::scula2    ] = rom[Location::scula    ];
    rom[Location::scula2 + 1] = rom[Location::scula + 1];
}


void AsmAndData() {
    //inject custom asm
    for(const auto [offset, code] : customAsm) {
        std::copy(code.begin(), code.end(), rom.begin() + offset);
    }

    rom[0x016ADF] = 0x40; //reduce wait time on picking up crest powers

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

            switch(rom[loc + offset]) {
                case 0x2D: rom[ 0x1FD547      + ((rom[loc + offset2] & ~0x40) >> 1)] = 1; break;
                case 0x2E: rom[ 0x1FD5D5      + ((rom[loc + offset2] & ~0x40) >> 1)] = 1; break;
                case 0x48: rom[ 0x1FD597      + ((rom[loc + offset2] & ~0x40) >> 1)] = 2; break;
                case 0x49: rom[(0x1FD500 - 1) +  (rom[loc + offset2] & ~0x40      )] = 1; break;

                default: std::cout << "Updating exit offset failed at location " << locData.name << "\n";
            }
        } else if(rom[loc] == 0x48) { //set powerups to not exit
            rom[0x1FD597 + ((rom[loc + 1] & ~0x40) >> 1)] = 6;
        }
    }
}


void PrintLocations() {
    std::ofstream logFile("log.txt", std::ios::out | std::ios::binary);

    logFile << "seed: " << seed << "\n\n";

    for(const auto stage : printOrder) {
        for(const auto loc : stage) {
            uint16_t item;
            if(loc == Location::crawler || loc == Location::grewon) {
                item = (rom[loc] << 8) | rom[loc + 3];
            } else {
                item = (rom[loc] | (rom[loc + 1] << 8)) & ~0x4000;
            }

            const std::string &locationName = locationData.at(loc).name;
            const std::string &itemName = itemData.at(item).name;

            logFile << std::setw(27) << std::left << locationName << " | " << itemName << "\n";
        }

        logFile << "\n";
    }

    logFile.close();
}
