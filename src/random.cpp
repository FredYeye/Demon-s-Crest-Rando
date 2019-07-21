#include <cstdint>
#include <vector>

#include "random.hpp"


void Random::Init(uint64_t seed)
{
	smstate = seed;

	uint64_t tmp = Splitmix64();
	s[0] = (uint32_t)tmp;
	s[1] = (uint32_t)(tmp >> 32);

	tmp = Splitmix64();
	s[2] = (uint32_t)tmp;
	s[3] = (uint32_t)(tmp >> 32);
}


void Random::Randomize(std::vector<uint16_t> &test)
{
    for(int i = test.size() - 1; i > 0; --i)
    {
        std::swap(test[i], test[Xoshiro256ss() % (i+1)]);
    }
}


uint32_t Random::Xoshiro256ss()
{
	const uint64_t result = Rol64(s[1] * 5, 7) * 9;

	const uint64_t t = s[1] << 17;

	s[2] ^= s[0];
	s[3] ^= s[1];
	s[1] ^= s[2];
	s[0] ^= s[3];

	s[2] ^= t;

	s[3] = Rol64(s[3], 45);

	return result >> 32;
}


uint64_t Random::Rol64(uint64_t x, int k)
{
	return (x << k) | (x >> (64 - k));
}


uint64_t Random::Splitmix64()
{
	smstate += 0x9E3779B97F4A7C15;

	uint64_t result = smstate;
	result = (result ^ (result >> 30)) * 0xBF58476D1CE4E5B9;
	result = (result ^ (result >> 27)) * 0x94D049BB133111EB;
	return result ^ (result >> 31);
}
