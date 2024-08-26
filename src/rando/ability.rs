use super::item::Item;

pub enum AbilityType {
    Buster,
    Ground,
    Flight,
    Swim,
}

impl AbilityType {
    pub fn mask(&self) -> u8 {
        match self {
            AbilityType::Buster => 0b0001,
            AbilityType::Ground => 0b0010,
            AbilityType::Flight => 0b0100,
            AbilityType::Swim   => 0b1000,
        }
    }
}

pub struct Ability {
    hp: u8,
    armor_level: u8,
    abilities: u8, // bit flags: 0001 buster | 0010 ground | 0100 flight | 1000 swim
}

impl Ability {
    pub fn new() -> Self {
        Self { hp: 4, armor_level: 0, abilities: 0 }
    }

    pub fn update_ability(&mut self, item: &Item) {
        match item {
            Item::Buster => self.abilities |= AbilityType::Buster.mask(),
            Item::Tornado | Item::AirCrest => self.abilities |= AbilityType::Flight.mask(),
            Item::EarthCrest => self.abilities |= AbilityType::Ground.mask(),
            Item::WaterCrest => self.abilities |= AbilityType::Swim.mask(),
            Item::TimeCrest => {
                self.abilities |= AbilityType::Buster.mask();
                self.armor_level += 1;
            }
            Item::Hp(_) => self.hp += 1,
            Item::Armor => self.armor_level += 1,

            _ => (),
        }
    }

    pub fn has_ability(&self, ability_mask: u8, hp_req: u8) -> bool {
        let effective_hp = self.hp << self.armor_level;
        ability_mask & self.abilities == ability_mask && effective_hp >= hp_req
    }
}
