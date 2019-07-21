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
		std::cout << "No custom seed supplied, generating seed.\n";
		std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
		seed = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
	}

	rng.Init(seed);
	rng.Randomize(itemList);

	for(int x = 0; x < itemList.size(); ++x)
	{
		rom[itemOffsets[x].offset    ] = itemList[x];
		rom[itemOffsets[x].offset + 1] = itemList[x] >> 8;

		if(completionCheckOffsets.count(itemOffsets[x].offset))
		{
			const uint32_t &offset2 = completionCheckOffsets.at(itemOffsets[x].offset);
			rom[offset2    ] = completionEvent.at(itemList[x]).completionCheckOffset;
			rom[offset2 + 1] = completionEvent.at(itemList[x]).completionCheckBit;
		}
	}

	AsmAndData();

	U8vecToFile(rom, "DCRando.sfc"); //hmm. need to change checksum or anything? investigate

	for(const uint16_t &v : itemList)
	{
		std::cout << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << v << "\n";
	}
}


void AsmAndData()
{
	//inject custom asm
	for(const Asm ca : customAsm)
	{
		for(int x = 0; x < ca.code.size(); ++x)
		{
			rom[ca.offset + x] = ca.code[x];
		}
	}

	//update hp pickup type to exit area/stage or not)
	for(int x = 0; x < itemOffsets.size(); ++x)
	{
		if(rom[itemOffsets[x].offset] == 0x49 && itemOffsets[x].shouldExit)
		{
			rom[(0x1FD500 - 1) + rom[itemOffsets[x].offset + 1]] = 1;
		}
	}
}
