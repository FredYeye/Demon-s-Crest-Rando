#pragma once

#include <bit>


class Random {
    public:
        void Init(const uint64_t seed);
		uint64_t BoundedRand(const uint64_t upper_bound);

        template <typename T, typename A>
        void Shuffle(std::vector<T, A> &vec);

    private:
		void Mx3();

		const uint64_t C = 0xBEA225F9EB34556D;
		uint64_t counter;
		uint64_t result;
		int8_t unused_bits = 0;
};


void Random::Init(const uint64_t seed) {
	counter = seed + C;
	Mx3();
}

template <typename T, typename A>
void Random::Shuffle(std::vector<T, A> &vec) {
	for(int x = vec.size() - 1; x > 0; --x) {
		const uint16_t rng = BoundedRand(x + 1);
		std::swap(vec[rng], vec[x]);
	}
}

void Random::Mx3() { // author: Jon Maiga
	uint64_t x = counter++;

	x ^= x >> 32;
	x *= C;
	x ^= x >> 29;
	x *= C;
	x ^= x >> 32;
	x *= C;
	x ^= x >> 29;

	unused_bits = 64;
	result = x;
}

uint64_t Random::BoundedRand(const uint64_t bound) { // [0, bound)
	if(bound < 2) {
		return 0;
	}

	const uint8_t leading_zeros = std::countl_zero(bound - 1);
	const uint8_t mask_size = 64 - leading_zeros;
	const uint64_t mask = UINT64_MAX >> leading_zeros;
	uint64_t x;

	do {
		if(unused_bits < mask_size) {
			Mx3();
		}

		x = result & mask;
		result >>= mask_size;
		unused_bits -= mask_size;
	} while(x >= bound);

	return x;
}
