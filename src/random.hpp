#pragma once

#include <array>


class Random
{
    public:
        Random(uint64_t seed); //init
        void Randomize(std::vector<uint16_t> &test);

    private:
        uint32_t Xoshiro256ss();
        uint64_t Rol64(uint64_t x, int k);
        uint64_t Splitmix64();

        std::array<uint64_t, 4> s;
        uint64_t smstate;
};
