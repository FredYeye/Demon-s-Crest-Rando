use super::{ability::{AbilityType, Ability}, area::Area, item::Item};

pub enum TileList {
    TileSet,
    MidStage,
}

#[derive(Clone, Copy, PartialEq, Eq, Hash)]
pub enum Location {
    Stage1Vellum,
    Stage1Hp,
    Stage1Potion,
    Somulo,
    Hippogriff1,
    Arma1,

    Stage2Potion,
    Hand,
    Stage2Vellum,
    Stage2Hp1,
    Stage2Hp2,
    Belth,
    Ovnunu,

    Stage3Potion,
    Stage3Vellum,
    Stage3Hp1,
    Stage3Hp2,
    Skull,
    FlameLord,
    Scula,

    Stage4Potion,
    Stage4Vellum,
    Crown,
    Flier,
    Arma2,

    Stage5Hp1,
    Stage5Hp2,
    Stage5Hp3,
    Crawler,
    Holothurion,

    Stage6Potion,
    Stage6Hp,
    Armor,
    Stage6Vellum,
    Arma3,
    Grewon,

    TrioThePago,

    PhalanxHp,
    PhalanxFang,
}

impl Location {
    pub fn get_all() -> Vec<Self> {
        use Location::*;

        vec![
            Stage1Vellum, Stage1Hp, Stage1Potion, Somulo, Hippogriff1, Arma1,
            Stage2Potion, Hand, Stage2Vellum, Stage2Hp1, Stage2Hp2, Belth, Ovnunu,
            Stage3Potion, Stage3Vellum, Stage3Hp1, Stage3Hp2, Skull, FlameLord, Scula,
            Stage4Potion, Stage4Vellum, Crown, Flier, Arma2,
            Stage5Hp1, Stage5Hp2, Stage5Hp3, Crawler, Holothurion,
            Stage6Potion, Stage6Hp, Armor, Stage6Vellum, Arma3, Grewon,
            TrioThePago,
            PhalanxHp, PhalanxFang,
        ]
    }

    pub fn name(&self) -> String {
        match self {
            Location::Stage1Vellum => "Vellum (s1)",
            Location::Stage1Hp     => "Stage 1 hp",
            Location::Stage1Potion => "Potion (s1)",
            Location::Somulo       => "Somulo",
            Location::Hippogriff1  => "Hippogriff 1",
            Location::Arma1        => "Arma 1",

            Location::Stage2Potion => "Potion (s2)",
            Location::Hand         => "Hand",
            Location::Stage2Vellum => "Vellum (s2)",
            Location::Stage2Hp1    => "Stage 2 hp (pot)",
            Location::Stage2Hp2    => "Stage 2 hp (bone pile)",
            Location::Belth        => "Belth",
            Location::Ovnunu       => "Ovnunu",

            Location::Stage3Potion => "Potion (s3)",
            Location::Stage3Vellum => "Vellum (s3)",
            Location::Stage3Hp1    => "Stage 3 hp (forest)",
            Location::Stage3Hp2    => "Stage 3 hp (water)",
            Location::Skull        => "Skull",
            Location::FlameLord    => "Flame Lord",
            Location::Scula        => "Scula",

            Location::Stage4Potion => "Potion (s4)",
            Location::Stage4Vellum => "Vellum (s4)",
            Location::Crown        => "Crown",
            Location::Flier        => "Flier",
            Location::Arma2        => "Arma 2",

            Location::Stage5Hp1    => "Stage 5 hp (before crawler)",
            Location::Stage5Hp2    => "Stage 5 hp (above crawler)",
            Location::Stage5Hp3    => "Stage 5 hp (water route)",
            Location::Crawler      => "Crawler",
            Location::Holothurion  => "Holothurion",

            Location::Stage6Potion => "Potion (s6)",
            Location::Stage6Hp     => "Stage 6 hp",
            Location::Armor        => "Armor",
            Location::Stage6Vellum => "Vellum (s6)",
            Location::Arma3        => "Arma 3",
            Location::Grewon       => "Grewon",

            Location::TrioThePago  => "Trio the Pago",

            Location::PhalanxHp    => "Phalanx (hp)",
            Location::PhalanxFang  => "Phalanx (fang)",
        }.to_string()
    }

    pub fn log_order() -> [Vec<Self>; 8] {
        use Location::*;

        [
            vec![Stage1Vellum, Stage1Hp, Stage1Potion, Somulo, Hippogriff1, Arma1],
            vec![Stage2Potion, Hand, Stage2Vellum, Stage2Hp1, Stage2Hp2, Belth, Ovnunu],
            vec![Stage3Potion, Stage3Vellum, Stage3Hp1, Stage3Hp2, Skull, FlameLord, Scula],
            vec![Stage4Potion, Stage4Vellum, Crown, Flier, Arma2],
            vec![Stage5Hp1, Stage5Hp2, Stage5Hp3, Crawler, Holothurion],
            vec![Stage6Potion, Stage6Hp, Armor, Stage6Vellum, Arma3, Grewon],
            vec![TrioThePago],
            vec![PhalanxHp, PhalanxFang],
        ]
    }

    pub fn requirement(&self) -> Vec<(u8, u8)> {
        match self {
            Location::Stage1Hp     => vec![(AbilityType::Buster.mask(), 0)],

            Location::Hand         => vec![(AbilityType::Swim.mask(), 0), (0, 8)],
            Location::Stage2Vellum => vec![(AbilityType::Buster.mask() | AbilityType::Ground.mask(), 0)],
            Location::Stage2Hp1    => vec![(AbilityType::Ground.mask(), 0)],
            Location::Stage2Hp2    => vec![(AbilityType::Buster.mask() | AbilityType::Ground.mask(), 0)],
            Location::Ovnunu       => vec![(AbilityType::Ground.mask(), 0)],

            Location::Stage3Vellum => vec![(AbilityType::Buster.mask(), 0), (AbilityType::Swim.mask(), 0)],
            Location::Stage3Hp2    => vec![(AbilityType::Swim.mask(), 0), (AbilityType::Buster.mask(), 7)],
            Location::Skull        => vec![(AbilityType::Buster.mask(), 0)],

            Location::Stage4Potion => vec![(AbilityType::Buster.mask(), 0)],

            Location::Stage5Hp1    => vec![(AbilityType::Ground.mask(), 0)],
            Location::Stage5Hp2    => vec![(AbilityType::Ground.mask(), 0)],
            Location::Stage5Hp3    => vec![(AbilityType::Swim.mask(), 0)],
            Location::Crawler      => vec![(AbilityType::Ground.mask(), 0)],
            Location::Holothurion  => vec![(AbilityType::Swim.mask(), 0)],

            Location::Stage6Potion => vec![(AbilityType::Flight.mask(), 0)],
            Location::Stage6Hp     => vec![(AbilityType::Flight.mask(), 0)], // needs ability that can hit low
            Location::Armor        => vec![(AbilityType::Ground.mask() | AbilityType::Flight.mask(), 0)],
            Location::Stage6Vellum => vec![(AbilityType::Ground.mask(), 0)],
            Location::Arma3        => vec![(AbilityType::Ground.mask() | AbilityType::Flight.mask(), 0)],

            Location::PhalanxHp => vec![(AbilityType::CanEnterPhalanx.mask() | AbilityType::Flight.mask(), 0)],
            Location::PhalanxFang => vec![(AbilityType::CanEnterPhalanx.mask() | AbilityType::Flight.mask(), 0)],

            _ => vec![(0, 0)],
        }
    }

    pub fn rom_offset(&self) -> u32 {
        match self {
            Location::Stage1Vellum => 0x81B4FD,
            Location::Stage1Hp     => 0x84D1A3,
            Location::Stage1Potion => 0x84D11E,
            Location::Somulo       => 0x8396D3,
            Location::Hippogriff1  => 0x829999,
            Location::Arma1        => 0x81F0A2,

            Location::Stage2Potion => 0x84D259,
            Location::Hand         => 0x84D2D1,
            Location::Stage2Vellum => 0x81D862,
            Location::Stage2Hp1    => 0x81D864,
            Location::Stage2Hp2    => 0x81B509,
            Location::Belth        => 0x83E8D8,
            Location::Ovnunu       => 0x83C6D4,

            Location::Stage3Potion => 0x84D58E,
            Location::Stage3Vellum => 0x81D866,
            Location::Stage3Hp1    => 0x84E163,
            Location::Stage3Hp2    => 0x81D868,
            Location::Skull        => 0x84D7B1,
            Location::FlameLord    => 0x82CCFD,
            Location::Scula        => 0xBD8B99, //head

            Location::Stage4Potion => 0x84D813,
            Location::Stage4Vellum => 0x84D9BC,
            Location::Crown        => 0x81B505,
            Location::Flier        => 0x85DBA0,
            Location::Arma2        => 0x81F0A4,

            Location::Stage5Hp1    => 0x81B50D,
            Location::Stage5Hp2    => 0x81B50B,
            Location::Stage5Hp3    => 0x81D86A,
            Location::Crawler      => 0x82BA12,
            Location::Holothurion  => 0x83D7B7,

            Location::Stage6Potion => 0x84DCBC,
            Location::Stage6Hp     => 0x81D86C,
            Location::Armor        => 0x81B511,
            Location::Stage6Vellum => 0x81B50F,
            Location::Arma3        => 0x82F009,
            Location::Grewon       => 0xBE9E25, // original location: BE9E23 (patch changes this location)

            Location::TrioThePago  => 0xBCA13E,

            Location::PhalanxHp    => 0x81D86E,
            Location::PhalanxFang  => 0x84E06E,
        }
    }

    pub fn remove_bounce(&self) -> bool {
        use Location::*;

        match self {
            Stage1Hp | Stage1Potion |
            Stage2Potion | Hand | Ovnunu |
            Stage3Potion | Stage3Hp1 | Skull | FlameLord |
            Stage4Potion | Stage4Vellum |
            Holothurion  |
            Stage6Potion | Arma3 |
            TrioThePago |
            PhalanxFang => true,

            _ => false,
        }
    }

    pub fn boss_defeat_offset(&self) -> Option<usize> {
        // convert to snes addresses
        match self {
            Location::Somulo      => Some(0), //could see if somulo does a check just for fun
            Location::Hippogriff1 => Some(0x8123),
            Location::Arma1       => Some(0x8125),

            Location::Belth       => Some(0x8127),
            Location::Ovnunu      => Some(0x0209F8),

            Location::FlameLord   => Some(0x812B),
            Location::Scula       => Some(0x8129),

            Location::Flier       => Some(0x812D),
            Location::Arma2       => Some(0x812F),

            Location::Crawler     => Some(0x1FD5B1),
            Location::Holothurion => Some(0x8131),

            Location::Arma3       => Some(0x8137),
            Location::Grewon      => Some(0x8135),

            _ => None,
        }
    }

    pub fn location_gfx(&self) -> (TileList, Item) { // todo: rename
        match self {
            Location::Stage1Vellum => (TileList::TileSet,  Item::Vellum(1)),
            Location::Stage1Hp     => (TileList::TileSet,  Item::Hp(1)),
            Location::Stage1Potion => (TileList::TileSet,  Item::Potion(1)),
            Location::Somulo       => (TileList::TileSet,  Item::Hp(1)),
            Location::Hippogriff1  => (TileList::MidStage, Item::Hp(1)),
            Location::Arma1        => (TileList::MidStage, Item::EarthCrest),

            Location::Stage2Potion => (TileList::TileSet,  Item::Potion(1)),
            Location::Hand         => (TileList::TileSet,  Item::Hand),
            Location::Stage2Vellum => (TileList::TileSet,  Item::Vellum(1)),
            Location::Stage2Hp1    => (TileList::TileSet,  Item::Hp(1)),
            Location::Stage2Hp2    => (TileList::TileSet,  Item::Hp(1)),
            Location::Belth        => (TileList::MidStage, Item::Hp(1)),
            Location::Ovnunu       => (TileList::TileSet,  Item::Buster),

            Location::Stage3Potion => (TileList::TileSet,  Item::Potion(1)),
            Location::Stage3Vellum => (TileList::TileSet,  Item::Vellum(1)),
            Location::Stage3Hp1    => (TileList::TileSet,  Item::Hp(1)),
            Location::Stage3Hp2    => (TileList::TileSet,  Item::Hp(1)),
            Location::Skull        => (TileList::TileSet, Item::Skull),
            Location::FlameLord    => (TileList::MidStage, Item::Tornado),
            Location::Scula        => (TileList::MidStage, Item::Hp(1)),

            Location::Stage4Potion => (TileList::TileSet,  Item::Potion(1)),
            Location::Stage4Vellum => (TileList::TileSet,  Item::Vellum(1)),
            Location::Crown        => (TileList::TileSet,  Item::Crown),
            Location::Flier        => (TileList::MidStage, Item::Claw),
            Location::Arma2        => (TileList::MidStage, Item::AirCrest),

            Location::Stage5Hp1   => (TileList::TileSet,  Item::Hp(1)),
            Location::Stage5Hp2   => (TileList::TileSet,  Item::Hp(1)),
            Location::Stage5Hp3   => (TileList::TileSet,  Item::Hp(1)),
            Location::Crawler     => (TileList::MidStage, Item::WaterCrest), // untested
            Location::Holothurion => (TileList::MidStage, Item::Hp(1)), // untested

            Location::Stage6Potion => (TileList::TileSet,  Item::Potion(1)),
            Location::Stage6Hp     => (TileList::TileSet,  Item::Hp(1)),
            Location::Armor        => (TileList::TileSet,  Item::Armor),
            Location::Stage6Vellum => (TileList::TileSet,  Item::Vellum(1)),
            Location::Arma3        => (TileList::TileSet,  Item::TimeCrest),
            Location::Grewon       => (TileList::TileSet,  Item::DemonFire),

            Location::TrioThePago => (TileList::TileSet,  Item::Hp(1)),

            Location::PhalanxHp   => (TileList::TileSet, Item::Hp(1)),
            Location::PhalanxFang => (TileList::TileSet, Item::Fang),
        }
    }

    pub fn area(&self) -> Area {
        match self {
            Location::Stage1Vellum => Area::S1_1,
            Location::Stage1Hp => Area::S1_3,
            Location::Stage1Potion => Area::S1_2,
            Location::Somulo => Area::SomuloExit,
            Location::Hippogriff1 => Area::S1_1,
            Location::Arma1 => Area::S1_3,

            Location::Stage2Potion => Area::S2_1,
            Location::Hand => Area::S2_1,
            Location::Stage2Vellum => Area::S2_2b,
            Location::Stage2Hp1 => Area::S2_3b,
            Location::Stage2Hp2 => Area::S2_2a,
            Location::Belth => Area::S2_2a,
            Location::Ovnunu => Area::S2Ovnunu,

            Location::Stage3Potion => Area::S3_1,
            Location::Stage3Vellum => Area::S3_2b,
            Location::Stage3Hp1 => Area::S3_3a,
            Location::Stage3Hp2 => Area::S3HpCave,
            Location::Skull => Area::S3SkullCave,
            Location::FlameLord => Area::S3_3a,
            Location::Scula => Area::S3_3b,

            Location::Stage4Potion => Area::S4_1,
            Location::Stage4Vellum => Area::S4_5,
            Location::Crown => Area::S4Crown,
            Location::Flier => Area::S4_2,
            Location::Arma2 => Area::S4_5,

            Location::Stage5Hp1 => Area::S5_2a,
            Location::Stage5Hp2 => Area::S5CrawlerAlcove,
            Location::Stage5Hp3 => Area::S5_2b,
            Location::Crawler => Area::S5_2a,
            Location::Holothurion => Area::S5_3b,

            Location::Stage6Potion => Area::S6_1,
            Location::Stage6Hp => Area::S6HpRoom,
            Location::Armor => Area::S6_5b,
            Location::Stage6Vellum => Area::S6_2a,
            Location::Arma3 => Area::S6_6b,
            Location::Grewon => Area::S6_2a,

            Location::TrioThePago => Area::TrioThePago3,

            Location::PhalanxHp => Area::S7_2,
            Location::PhalanxFang => Area::S7_3,
        }
    }

    pub fn is_reachable(&self, current_abilities: &Ability) -> bool {
        for (req_abilities, req_hp) in self.requirement() {
            if current_abilities.has_ability(req_abilities, req_hp) {
                return true;
            }
        }

        false
    }
}
