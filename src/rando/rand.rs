#[derive(Debug, Clone)]
pub struct Rand {
    state: [u64; 5],
    result: u64,
    unused_bits: i8,
}

impl Rand {
    pub fn init(seed: u64) -> Self {
        let mut state = Self {
            state: [
                seed.wrapping_add(0x26AA069EA2FB1A4D),
                seed.wrapping_add(0x70C72C95CD592D04),
                seed.wrapping_add(0x504F333D3AA0B359),
                seed,
                (seed.wrapping_add(0x3504F333D3AA0B37) << 1) | 1,
            ],
            result: 0,
            unused_bits: 0,
        };

        for _ in 0 .. 20 {
            state.update();
        }

        state
    }

    fn update(&mut self) { //stc64
        let s = &mut self.state;

        s[3] = s[3].wrapping_add(s[4]);
        self.result = (s[0] ^ s[3]).wrapping_add(s[1]);
        s[0] = s[1] ^ (s[1] >> 11);
        s[1] = s[2].wrapping_add(s[2] << 3);
        s[2] = s[2].rotate_left(24).wrapping_add(self.result);

        self.unused_bits = 64;
    }

    pub fn bounded_rand(&mut self, upper_bound: u64) -> u64 { //value is within 0 .. bound
        if upper_bound < 2 {
            return 0;
        }

        let leading_zeros = (upper_bound - 1).leading_zeros() as u8;
        let mask_size = u64::BITS as u8 - leading_zeros;
        let mask = u64::MAX >> leading_zeros;

        loop {
            if self.unused_bits < mask_size as i8 {
                self.update();
            }

            let x = self.result & mask;
            self.result >>= mask_size;
            self.unused_bits -= mask_size as i8;

            if x < upper_bound {
                return x;
            }
        }
    }

    pub fn shuffle<T> (&mut self, seq: &mut [T]) {
        for idx in (1 .. seq.len()).rev() {
            let rng = self.bounded_rand(idx as u64 + 1) as usize;
            seq.swap(rng, idx);
        }
    }
}
