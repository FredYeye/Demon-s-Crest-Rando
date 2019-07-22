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
		// // todo: convert secondary value to int and use as seed
		// seed = 
		// // todo: use param to override default rom name
		// fileName = argv[];
	// }

	rom = FileToU8Vec(fileName);

	if(rom[0x007FC8] != 0x63)
	{
		std::cout << "needs a US rom to work for now";
	}

    if(!seed)
	{
		std::cout << "No custom seed supplied, generating seed.\n\n";
		std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
		seed = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
	}

	for(const auto kv : itemData)
	{
		itemList.push_back(kv.first);
	}

	for(const auto kv : locationData)
	{
		locationList.push_back(kv.first);
	}

	rng.Init(seed);
	rng.Randomize(itemList);

	for(int x = 0; x < itemList.size(); ++x)
	{
		rom[locationList[x]    ] = itemList[x];
		rom[locationList[x] + 1] = itemList[x] >> 8;

		if(locationData.at(locationList[x]).noBounce)
		{
			rom[locationList[x] + 1] |= 0x40; // or item value with 0x4000 to stop item bounce
		}

		if(const uint32_t &offset = locationData.at(locationList[x]).bossDefeatedOffset; offset != 0)
		{
			rom[offset    ] = itemData.at(itemList[x]).completionCheckOffset;
			rom[offset + 1] = itemData.at(itemList[x]).completionCheckBit;
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
		if(rom[locationList[x]] == 0x49 && locationData.at(locationList[x]).shouldExit)
		{
			//update hp pickup type to exit area/stage if necessary)
			rom[(0x1FD500 - 1) + (rom[locationList[x] + 1] & ~0x4000)] = 1;
		}
		else if(rom[locationList[x]] == 0x2D && locationData.at(locationList[x]).shouldExit)
		{
			//update item pickup list to exit area/stage if necessary
			rom[0x1FD542 + ((rom[locationList[x] + 1] & ~0x4000) >> 1)] = 1;
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
		const std::string &locationName = kv.second.name;
		const uint16_t item = (rom[kv.first] | (rom[kv.first + 1] << 8)) & ~0x4000;
		const std::string &itemName = itemData.at(item).name;

		logFile << std::setw(14) << std::left << locationName << " | " << itemName << "\n";
	}

	logFile.close();
}
