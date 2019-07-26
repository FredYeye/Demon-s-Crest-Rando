#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <algorithm>

#include "main.hpp"
#include "file.hpp"


int main(int argc, char* argv[])
{
	std::string fileName = "Demon's Crest (USA).sfc";

	// if(argc >= 2)
	// {
		// // todo: convert secondary value to uint64 and use as seed
		// seed = 
		// // todo: use param to override default rom name
		// fileName = argv[];
	// }

	rom = FileToU8Vec(fileName);

	if(rom[0x007FC8] != 0x63)
	{
		std::cout << "needs a US rom to work for now\n";
		std::cin.get();
		exit(0);
	}

    if(!seed)
	{
		std::cout << "No custom seed supplied, generating seed: ";
		std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
		seed = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
		std::cout << seed << "\n";
	}

	for(const auto kv : itemData)
	{
		itemList.push_back(kv.first);
	}

	for(const auto kv : locationData)
	{
		locationList.push_back(kv.first);
	}

	if(itemList.size() != locationList.size())
	{
		std::cout << "item and location count doesn't match (" << itemList.size() << " items, " << locationList.size() << " locations)\n";
		std::cin.get();
		exit(0);
	}

	rng.Init(seed);
	rng.Randomize(itemList);
	rng.Randomize(locationList);

	uint8_t currentAbilities = 0;
	bool reqLocationsFilled = false;

	int iLoc = 0, iItem = 0;
	while(locationList.size() > 0)
	{
		const uint32_t &location = locationList[iLoc];
		const uint16_t &item = itemList[iItem];

		const uint8_t &locRequirement = locationData.at(location).requirement;
		const uint8_t &itemAbility = itemData.at(item).ability;

		const bool abilityReqMet = (locRequirement & currentAbilities == locRequirement);

		if
		(
			locRequirement &&  //if location has a requirement, and
			abilityReqMet  ||  //requirement is met, or
			!itemAbility   ||  //ability isn't needed.
			reqLocationsFilled //lastly, fill locations without requirements
		)
		{
			if(location == Location::crawler || location == Location::grewon)
			{
				rom[location    ] = item >> 8;
				rom[location + 3] = item;
			}
			else //default
			{
				rom[location    ] = item;
				rom[location + 1] = item >> 8;

				if(locationData.at(location).noBounce)
				{
					rom[location + 1] |= 0x40; // or item value with 0x4000 to stop item bounce
				}
			}

			if(const uint32_t &offset = locationData.at(location).bossDefeatedOffset; offset != 0)
			{
				if(location == Location::crawler) //crawler special case
				{
					rom[offset    ] = itemData.at(item).completionCheckOffset;
					rom[offset + 6] = itemData.at(item).completionCheckBit;
					crawlerOffset = rom[offset];
					crawlerBit = rom[offset + 6];
				}
				else if(location == Location::ovnunu) //ovnunu special case
				{
					rom[offset    ] = 0x51 + itemData.at(item).completionCheckOffset;
					rom[offset + 3] = itemData.at(item).completionCheckBit;
				}
				else //default
				{
					rom[offset    ] = itemData.at(item).completionCheckOffset;
					rom[offset + 1] = itemData.at(item).completionCheckBit;
				}

				if(location == Location::arma3) //needs additional location fix
				{
					rom[0x1F63E2] = 0x51 + itemData.at(item).completionCheckOffset;
					rom[0x1F63E5] = itemData.at(item).completionCheckBit;
				}
			}

			currentAbilities |= itemAbility;

			locationList.erase(locationList.begin() + iLoc);
			itemList.erase(itemList.begin() + iItem);
			iLoc = 0;
			iItem = 0;
		}
		else //failed to place item
		{
			if(++iItem >= itemList.size()) //try next item, unless we've tried every item
			{
				iItem = 0;
				if(++iLoc >= locationList.size()) //try next location
				{
					iLoc = 0;

					for(const uint32_t loc : locationList)
					{
						reqLocationsFilled |= !locationData.at(loc).requirement;
					}
				}
			}
		}
	}

	//scula head & body are considered different entities and have different drops. copy to other part
	rom[Location::scula2] = rom[Location::scula];
	rom[Location::scula2 + 1] = rom[Location::scula + 1];

	AsmAndData();

	U8vecToFile(rom, "DCRando.sfc"); //hmm. need to change checksum or anything? investigate

	PrintLocations();
}


void AsmAndData()
{
	//inject custom asm
	for(const auto [offset, code] : customAsm)
	{
		std::copy(code.begin(), code.end(), rom.begin() + offset);
	}

	rom[0x016ADF] = 0x10; //reduce wait time on picking up crest powers

	rom[0x02A22E] = 0x01; //was -1, enable stages 5 & 6 from the start
	rom[0x02A246] = 0x01; //was  2, enable stages 5 & 6 instead of phalanx 1

	rom[0x1E2045] = 0xB8; //change trio the pago timer, 40s -> 50s
	rom[0x1E2046] = 0x0B; //^

	for(const auto [loc, locData] : locationData)
	{
		if(loc == Location::crawler)
		{
			//crawler's item data gets overwritten by the custom asm injection. re-add
			const uint32_t &crawlerFix = locData.bossDefeatedOffset;
			rom[crawlerFix] = crawlerOffset;
			rom[crawlerFix + 6] = crawlerBit;
			continue;
		}

		if(locationData.at(loc).shouldExit) //update item to exit area/stage if necessary
		{
			switch(rom[loc])
			{
				case 0x2D: rom[0x1FD547 + ((rom[loc + 1] & ~0x40) >> 1)] = 1; break;
				case 0x2E: rom[0x1FD5D5 + ((rom[loc + 1] & ~0x40) >> 1)] = 1; break;
				case 0x48: rom[0x1FD597 + ((rom[loc + 1] & ~0x40) >> 1)] = 2; break;
				case 0x49: rom[(0x1FD500 - 1) + (rom[loc + 1] & ~0x40)]  = 1; break;
			}
		}
		else if(rom[loc] == 0x48)
		{
			rom[0x1FD597 + ((rom[loc + 1] & ~0x40) >> 1)] = 6;
		}
	}
}


void PrintLocations()
{
	std::ofstream logFile("log.txt", std::ios::out | std::ios::binary);

	logFile << "seed: " << seed << "\n\n";

	for(const auto stage : printOrder)
	{
		for(const auto loc : stage)
		{
			uint16_t item;
			if(loc == Location::crawler || loc == Location::grewon)
			{
				item = (rom[loc] << 8) | rom[loc + 3];
			}
			else
			{
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
