#[derive(Clone, Copy, PartialEq, Eq, Hash)]
pub enum Item {
    Buster,
    Tornado,
    Claw,
    DemonFire,

    EarthCrest,
    AirCrest,
    WaterCrest,
    TimeCrest,

    Hp(u8),
    //  1: somulo,          2,              3: belth,           4: scula
    //  5,                  6: holothurion, 7,                  8: s2 pot
    //  9: s2 bone pile,   10,             11: s3 water route, 12: trio the pago
    // 13: s5 water route, 14,             15: s6              16: phalanx tower

    Vellum(u8), // 1: s1, 2: s2, 3: s3, 4: s4, 5: s6
    Potion(u8), // 1: s1, 2: s2, 3: s3, 4: s4, 5: s6

    Crown,
    Skull,
    Armor,
    Fang,
    Hand,
}

impl Item {
    pub fn get_all() -> Vec<Self> {
        use Item::*;

        vec![
            Buster, Tornado, Claw, DemonFire,
            EarthCrest, AirCrest, WaterCrest, TimeCrest,
            Hp(1), Hp(2), Hp(3), Hp(4), Hp(5), Hp(6), Hp(7), Hp(8), Hp(9), Hp(10), Hp(11), Hp(12), Hp(13), Hp(14), Hp(15), Hp(16),
            Vellum(1), Vellum(2), Vellum(3), Vellum(4), Vellum(5),
            Potion(1), Potion(2), Potion(3), Potion(4), Potion(5),
            Crown, Skull, Armor, Fang, Hand,
        ]
    }

    pub fn name(&self) -> String {
        match self {
            Item::Buster => "Buster",
            Item::Tornado => "Tornado",
            Item::Claw => "Claw",
            Item::DemonFire => "Demon Fire",
            Item::EarthCrest => "Earth Crest",
            Item::AirCrest => "Air Crest",
            Item::WaterCrest => "Water Crest",
            Item::TimeCrest => "Time Crest",
            Item::Hp(_) => "Hp",
            Item::Vellum(_) => "Vellum",
            Item::Potion(_) => "Potion",
            Item::Crown => "Crown",
            Item::Skull => "Skull",
            Item::Armor => "Armor",
            Item::Fang  => "Fang",
            Item::Hand  => "Hand",
        }.to_string()
    }

    pub fn id(&self) -> u16 {
        match self {
            Item::Buster    => 0x0048,
            Item::Tornado   => 0x0248,
            Item::Claw      => 0x0448,
            Item::DemonFire => 0x0648,

            Item::EarthCrest => 0x0848,
            Item::AirCrest   => 0x0A48,
            Item::WaterCrest => 0x0C48,
            Item::TimeCrest  => 0x0E48,

            Item::Hp(x) => 0x0049 | ((*x as u16) << 8),
            Item::Vellum(x) => u16::from_le_bytes([0x2D, (*x - 1) * 2]),
            Item::Potion(x) => u16::from_le_bytes([0x2D, 0x0A + (*x - 1) * 2]),

            Item::Crown => 0x002E,
            Item::Skull => 0x022E,
            Item::Armor => 0x042E,
            Item::Fang  => 0x062E,
            Item::Hand  => 0x082E,
        }
    }

    pub fn completion_data(&self) -> (u8, u8) {
        match self {
            Item::Buster     => (0, 0b0000_0001),
            Item::Tornado    => (0, 0b0000_0010),
            Item::Claw       => (0, 0b0000_0100),
            Item::DemonFire  => (0, 0b0000_1000),
            Item::EarthCrest => (0, 0b0001_0000),
            Item::AirCrest   => (0, 0b0010_0000),
            Item::WaterCrest => (0, 0b0100_0000),
            Item::TimeCrest  => (0, 0b1000_0000),

            Item::Hp(x) => {
                match x {
                    1  => (3, 0b0000_0001),
                    2  => (3, 0b0000_0010),
                    3  => (3, 0b0000_0100),
                    4  => (3, 0b0000_1000),
                    5  => (3, 0b0001_0000),
                    6  => (3, 0b0010_0000),
                    7  => (3, 0b0100_0000),
                    8  => (3, 0b1000_0000),
                    9  => (4, 0b0000_0001),
                    10 => (4, 0b0000_0010),
                    11 => (4, 0b0000_0100),
                    12 => (4, 0b0000_1000),
                    13 => (4, 0b0001_0000),
                    14 => (4, 0b0010_0000),
                    15 => (4, 0b0100_0000),
                    16 => (4, 0b1000_0000),
                    _ => todo!(),
                }
            }

            Item::Vellum(x) => {
                match x {
                    1 => (5, 0b0000_0001),
                    2 => (5, 0b0000_0010),
                    3 => (5, 0b0000_0100),
                    4 => (5, 0b0000_1000),
                    5 => (5, 0b0001_0000),
                    _ => todo!(),
                }
            }

            Item::Potion(x) => {
                match x {
                    1 => (5, 0b0010_0000),
                    2 => (5, 0b0100_0000),
                    3 => (5, 0b1000_0000),
                    4 => (6, 0b0000_0001),
                    5 => (6, 0b0000_0010),
                    _ => todo!(),
                }
            }

            Item::Crown => (2, 0b0000_1000),
            Item::Skull => (2, 0b0001_0000),
            Item::Armor => (2, 0b0010_0000),
            Item::Fang  => (2, 0b0100_0000),
            Item::Hand  => (2, 0b1000_0000),
        }
    }

    pub fn tile_sprite_set(&self) -> Option<(u16, [u8; 2])> {
        if let Item::Hp(_) = self {
            return None;
        }

        let tile_set = match self {
            Item::Buster | Item::Tornado | Item::Claw | Item::DemonFire => 0x0FC,
            Item::EarthCrest | Item::AirCrest | Item::WaterCrest | Item::TimeCrest => 0x0F4,
            Item::Vellum(_) => 0x138,
            Item::Potion(_) => 0x0F8,
            Item::Crown => 0x144,
            Item::Skull => 0x140,
            Item::Armor => 0x100,
            Item::Fang  => 0x13C,
            Item::Hand  => 0x148,

            Item::Hp(_) => unreachable!(),
        };

        let sprite_set = match self {
            Item::Buster | Item::DemonFire => [0x4F, 0x08],
            Item::Tornado | Item::Claw    => [0x4F, 0x0A],
            Item::EarthCrest => [0x4D, 0x06],
            Item::AirCrest | Item::TimeCrest   => [0x4D, 0x02],
            Item::WaterCrest => [0x4D, 0x0A],
            Item::Vellum(_) | Item::Potion(_) => [0x4E, 0x02],
            Item::Crown | Item::Skull | Item::Armor | Item::Fang | Item::Hand => [0x50, 0x02],

            Item::Hp(_) => unreachable!(),
        };

        Some((tile_set, sprite_set))
    }
}
