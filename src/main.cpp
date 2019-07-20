#include <cstdint>
#include <vector>
#include <iostream>
#include <iomanip>
#include <chrono>

#include "main.hpp"
#include "random.hpp"
#include "file.hpp"


int main(int argc, char* argv[])
{
	// if(argc >= 2)
	// {
		// todo: convert secondary value to int and use as seed
		// seed = 
	// }

    if(!seed)
	{
		std::cout << "No custom seed supplied. Generating new seed.\n";
		std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
		seed = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
	}

	Random rng(seed);

	std::vector<uint16_t> items;
	for(const GameData data : gameData)
	{
		items.push_back(data.item);
	}

	rng.Randomize(items);

	std::vector<uint8_t> rom = FileToU8Vec("Demon's Crest (USA).sfc");

	for(int x = 0; x < gameData.size(); ++x)
	{
		rom[gameData[x].offset] = items[x];
		rom[gameData[x].offset + 1] = items[x] >> 8;
	}

	U8vecToFile(rom, "DCRando.sfc"); //hmm. need to change checksum? investigate

	for(const uint16_t &v : items)
	{
		std::cout << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << v << "\n";
	}
}
