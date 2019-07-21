#include <vector>
#include <iostream>
#include <iomanip>
#include <chrono>

#include "main.hpp"
#include "random.hpp"
#include "file.hpp"


int main(int argc, char* argv[])
{
	std::string fileName = "Demon's Crest (USA).sfc";
	uint64_t seed = 0;

	// if(argc >= 2)
	// {

		// todo: convert secondary value to int and use as seed
		// seed = 
		// if(argc >= 3)
		// {
		// 	//todo: use 2nd param to override default rom name
		// 	fileName = argv[2];
		// }
	// }

	std::vector<uint8_t> rom = FileToU8Vec(fileName);

    if(!seed)
	{
		std::cout << "No custom seed supplied, generating seed.\n";
		std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
		seed = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
	}

	Random rng(seed);

	rng.Randomize(itemList);

	for(int x = 0; x < itemList.size(); ++x)
	{
		rom[itemOffsets[x]    ] = itemList[x];
		rom[itemOffsets[x] + 1] = itemList[x] >> 8;

		if(completionCheckOffsets.count(itemOffsets[x]))
		{
			const uint32_t &offset2 = completionCheckOffsets.at(itemOffsets[x]);
			rom[offset2    ] = completionEvent.at(itemList[x]).completionCheckOffset;
			rom[offset2 + 1] = completionEvent.at(itemList[x]).completionCheckBit;
		}
	}

	U8vecToFile(rom, "DCRando.sfc"); //hmm. need to change checksum or anything? investigate

	for(const uint16_t &v : itemList)
	{
		std::cout << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << v << "\n";
	}
}
