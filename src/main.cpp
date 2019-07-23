#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <chrono>

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

	for(int x = 0; x < itemList.size(); ++x)
	{
		rom[locationList[x]] = itemList[x];

		if(locationList[x] == Location::crawler)
		{
			rom[locationList[x] + 3] = itemList[x] >> 8;

			crawlerItem = itemList[x];
		}
		else //default case
		{
			rom[locationList[x] + 1] = itemList[x] >> 8;
		}

		if(locationData.at(locationList[x]).noBounce)
		{
			rom[locationList[x] + 1] |= 0x40; // or item value with 0x4000 to stop item bounce
		}

		if(const uint32_t &offset = locationData.at(locationList[x]).bossDefeatedOffset; offset != 0)
		{
			rom[offset] = itemData.at(itemList[x]).completionCheckOffset;

			if(locationList[x] == Location::crawler) //crawler special case
			{
				rom[offset + 6] = itemData.at(itemList[x]).completionCheckBit;

				crawlerOffset = rom[offset];
				crawlerBit = rom[offset + 6];
			}
			else //default case
			{
				rom[offset + 1] = itemData.at(itemList[x]).completionCheckBit;
			}
		}
	}

	AsmAndData();

	U8vecToFile(rom, "DCRando.sfc"); //hmm. need to change checksum or anything? investigate

	PrintLocations();
}


void AsmAndData()
{
	//inject custom asm
	for(const auto kv : customAsm)
	{
		for(int x = 0; x < kv.second.size(); ++x)
		{
			rom[kv.first + x] = kv.second[x];
		}
	}

	for(int x = 0; x < locationList.size(); ++x)
	{
		if(locationList[x] == Location::crawler)
		{
			//crawler's item data gets overwritten by the custom asm injection. re-add
			const uint32_t &crawlerFix = locationData.at(Location::crawler).bossDefeatedOffset;
			rom[crawlerFix] = crawlerOffset;
			rom[crawlerFix] = crawlerBit;
			continue;
		}

		if(rom[locationList[x]] == 0x49 && locationData.at(locationList[x]).shouldExit)
		{
			//update hp pickup type to exit area/stage if necessary)
			rom[(0x1FD500 - 1) + (rom[locationList[x] + 1] & ~0x40)] = 1;
		}
		else if(rom[locationList[x]] == 0x2D && locationData.at(locationList[x]).shouldExit)
		{
			//update item pickup list to exit area/stage if necessary
			rom[0x1FD547 + ((rom[locationList[x] + 1] & ~0x40) >> 1)] = 1;
		}
		else if(rom[locationList[x]] == 0x48)
		{
			//update power pickup list to exit/not exit area/stage
			rom[0x1FD597 + ((rom[locationList[x] + 1] & ~0x40) >> 1)] = (locationData.at(locationList[x]).shouldExit) ? 2 : 6;
		}
		else if(rom[locationList[x]] == 0x2E && locationData.at(locationList[x]).shouldExit)
		{
			rom[0x1FD5D5 + ((rom[locationList[x] + 1] & ~0x40) >> 1)] = 1;
		}
	}
}


void PrintLocations()
{
	std::string outfile = "log.txt";
	std::ofstream logFile(outfile, std::ios::out | std::ios::binary);

	logFile << "seed: " << seed << "\n\n";

	for(const auto kv : locationData)
	{
		uint16_t item;
		if(kv.first == Location::crawler)
		{
			item = crawlerItem;
		}
		else
		{
			item = (rom[kv.first] | (rom[kv.first + 1] << 8)) & ~0x4000;
		}

		const std::string &locationName = kv.second.name;
		const std::string &itemName = itemData.at(item).name;

		logFile << std::setw(14) << std::left << locationName << " | " << itemName << "\n";
	}

	logFile.close();
}
