#include <vector>
#include <iostream>
#include <iomanip>
#include <chrono>

#include "main.hpp"
#include "file.hpp"


int main(int argc, char* argv[])
{
	std::string fileName = "Demon's Crest (USA).sfc";
	uint64_t seed = 0;

	// if(argc >= 2)
	// {
		// // todo: convert secondary value to int and use as seed
		// seed = 
		// // todo: use param to override default rom name
		// fileName = argv[];
	// }

	rom = FileToU8Vec(fileName);

    if(!seed)
	{
		std::cout << "\nNo custom seed supplied, generating seed.\n\n";
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

		if(const uint32_t &offset = locationData.at(locationList[x]).completionCheckOffset; offset != 0)
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

	//update hp pickup type to exit area/stage or not)
	for(int x = 0; x < locationList.size(); ++x)
	{
		if(rom[locationList[x]] == 0x49 && locationData.at(locationList[x]).shouldExit)
		{
			rom[(0x1FD500 - 1) + rom[locationList[x] + 1]] = 1;
		}
	}
}


void PrintLocations()
{
	for(const auto kv : locationData)
	{
		const std::string &locationName = kv.second.name;
		const uint16_t item = rom[kv.first] | (rom[kv.first + 1] << 8);
		const std::string &itemName = itemData.at(item).name;
		std::cout << std::setw(14) << std::left << locationName << " | " << itemName << "\n";
	}
}
