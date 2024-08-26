use std::collections::HashMap;

use ability::Ability;
use gfx::Gfx;
use item::Item;
use location::{Location, TileList};
use rand::Rand;

mod item;
mod location;
mod rand;
mod ability;
mod gfx;
mod area;

pub struct Rando {
    rom: Vec<u8>,
    rng: Rand,
}

impl Rando {
    pub fn new(in_file: &str, seed: u64) -> Result<Self, ()> {
        let rom = std::fs::read(in_file).expect("File not found");

        // check rom size and checksum to verify rom
        if rom.len() == (1 << 21) {
            let checksum = u32::from_le_bytes(rom[0x7FDC .. 0x7FE0].try_into().unwrap());
            if checksum == 0xC47D3B82 {
                return Ok ( Self {
                    rom: rom,
                    rng: Rand::init(seed),
                });
            }
        }

        Err(())
    }

    pub fn patch_rom(&mut self) {
        let patch = include_bytes!("dcrando.pat");

        let mut x = 0;
        while x < patch.len() {
            let offset = u32::from_le_bytes(patch[x .. x + 4].try_into().unwrap()) as usize;
            let len = u16::from_le_bytes(patch[x + 4 .. x + 4 + 2].try_into().unwrap()) as usize;
            self.rom[offset .. offset + len].copy_from_slice(&patch[x + 4 + 2 .. x + 4 + 2 + len]);
            x += 4 + 2 + len;
        }
    }

    pub fn output_rom(&self, out_file: &str) {
        std::fs::write(out_file, &self.rom).expect("unable to write file");
    }

    pub fn randomize(&mut self) {
        let loc_item_pairs: HashMap<Location, Item> = self.assumed_fill();
        self.write_items_to_rom(&loc_item_pairs);
        self.asm_and_data();
        self.adjust_tile_sets(&loc_item_pairs);
        Self::write_log(&loc_item_pairs);
    }

    fn assumed_fill(&mut self) -> HashMap<Location, Item> {
        let mut items = Item::get_all(); // inventory
        let mut locations = Location::get_all();

        if items.len() != locations.len() {
            todo!("item and location counts do not match")
        }

        self.rng.shuffle(&mut items);
        self.rng.shuffle(&mut locations);

        let mut loc_item: HashMap<Location, Item> = HashMap::new();

        while let Some(item) = items.pop() {
            let mut current_items = items.clone();
            let mut current_abilities2 = Ability::new();

            for i in &current_items {
                current_abilities2.update_ability(i);
            }

            loop {
                let mut picked_up_something = false;

                for (loc, &i) in &loc_item {
                    if !current_items.contains(&i) {
                        if loc.is_reachable(&current_abilities2) {
                            current_abilities2.update_ability(&i);

                            current_items.push(i);
                            picked_up_something = true;
                        }
                    }
                }

                if !picked_up_something { break; }
            }

            for x in 0 .. locations.len() {
                if locations[x].is_reachable(&current_abilities2) {
                    loc_item.insert(locations[x], item);
                    locations.remove(x);
                    break;
                }
            }
        }

        loc_item
    }

    fn write_items_to_rom(&mut self, loc_item_pairs: &HashMap<Location, Item>) {
        for (loc, item) in loc_item_pairs {
            let offset = loc.rom_offset();
            let item_id = item.id();

            match loc {
                Location::Crawler | Location::Grewon => {
                    self.rom[offset    ] = item_id.to_le_bytes()[1];
                    self.rom[offset + 3] = item_id.to_le_bytes()[0];
                }

                _ => {
                    self.rom[offset ..= offset + 1].copy_from_slice(&item_id.to_le_bytes());

                    if loc.remove_bounce() {
                        self.rom[offset + 1] |= 0x40; // 0x4000 = prevent bounce flag
                    }
                }
            }

            if let Some(d_offset) = loc.boss_defeat_offset() {
                match loc {
                    Location::Somulo => (),

                    Location::Ovnunu => {
                        self.rom[d_offset    ] = item.completion_data().0 + 0x51;
                        self.rom[d_offset + 3] = item.completion_data().1;
                    }

                    Location::Crawler => {
                        self.rom[d_offset    ] = item.completion_data().0;
                        self.rom[d_offset + 6] = item.completion_data().1;
                    }

                    Location::Scula => {
                        self.rom[d_offset    ] = item.completion_data().0;
                        self.rom[d_offset + 1] = item.completion_data().1;

                        // scula head & body are considered different entities and have different drops. copy to other part
                        let location_scula2 = Self::snes_to_effective(0xBD85AD); //body
                        self.rom[location_scula2 ..= location_scula2 + 1].copy_from_slice(&item_id.to_le_bytes());
                    }

                    Location::Arma3 => {
                        self.rom[d_offset    ] = item.completion_data().0;
                        self.rom[d_offset + 1] = item.completion_data().1;

                        self.rom[0x1F63E2] = item.completion_data().0 + 0x51;
                        self.rom[0x1F63E5] = item.completion_data().1;
                    }

                    _ => {
                        self.rom[d_offset    ] = item.completion_data().0;
                        self.rom[d_offset + 1] = item.completion_data().1;
                    }
                }
            }

            if let Location::TrioThePago = loc {
                let trio_de_pago_offset = Self::snes_to_effective(0xBCAAED);
                self.rom[trio_de_pago_offset    ] = item.completion_data().0 + 0x51;
                self.rom[trio_de_pago_offset + 3 ..= trio_de_pago_offset + 4].copy_from_slice(&[item.completion_data().1, 0]);
            }
        }
    }

    fn asm_and_data(&mut self) {
        // todo: this function no longer patches in custom asm, so rename. what does it do, though? LOL
        // set exit status for items?

        for loc in Location::get_all() {
            if loc.boss_defeat_offset().is_some() {
                let offset = match loc {
                    Location::Grewon | Location::Crawler => (3, 0),
                    _ => (0, 1),
                };

                let temp = (self.rom[loc.rom_offset() + offset.1] & !0x40) as usize;

                match self.rom[loc.rom_offset() + offset.0] {
                    0x2D => self.rom[ 0x1FD547      + (temp >> 1)] = 1,
                    0x2E => self.rom[ 0x1FD5D5      + (temp >> 1)] = 1,
                    0x48 => self.rom[ 0x1FD597      + (temp >> 1)] = 2,
                    0x49 => self.rom[(0x1FD500 - 1) +  temp      ] = 1,
                    _ => todo!(),
                }
            } else if self.rom[loc.rom_offset()] == 0x48 { // set powerups to not exit
                let temp = (self.rom[loc.rom_offset() + 1] & !0x40) >> 1;
                self.rom[0x1FD597 + temp as usize] = 6;
            }
        }
    }

    fn write_log(loc_item_pairs: &HashMap<Location, Item>) {
        let mut str_vec = Vec::new();

        let log_order = Location::log_order();

        for section in log_order {
            for loc in section {
                str_vec.push(format!("Location: {:27} | Item: {}", loc.name(), loc_item_pairs.get(&loc).unwrap().name()));
            }

            str_vec.push("".to_owned());
        }

        std::fs::write("log.txt", str_vec.join("\n")).expect("unable to create log");
    }

    fn adjust_tile_sets(&mut self, loc_item_pairs: &HashMap<Location, Item>) {
        // todo: maybe move most of this into gfx.rs?

        let mut gfx = Gfx::init();

        for (loc, item) in loc_item_pairs {
            // if new item needs to add a tile set
            if let Some((tile_set, sprite_set1, sprite_set2)) = item.tile_sprite_set() {

                let (tile_list, original_item) = loc.location_gfx();
                let area_id = loc.area().offset() as usize;

                let sprite_set_idx = gfx.sprite_set_index[area_id] as usize;

                match original_item {
                    Item::Hp(_) => {
                        // hp can't be replaced. insert new tile / sprite set at beginning of associated list
                        // todo: also check if gfx already happens to exist?
                        match tile_list {
                            TileList::TileSet => {
                                let tile_set_idx = gfx.tile_set_index[area_id] as usize;
                                gfx.tile_set[tile_set_idx].insert(0, tile_set);

                                gfx.sprite_set[sprite_set_idx].insert(0, sprite_set1);
                                gfx.sprite_set[sprite_set_idx].insert(1, sprite_set2);
                            }

                            TileList::MidStage => {
                                let mid_stage_idx = gfx.mid_stage_index[area_id] as usize;
                                if let Some(spr_data) = &mut gfx.mid_stage[mid_stage_idx] {
                                    if let Some(tile_sets) = &mut spr_data.tile_sets {
                                        tile_sets.push(tile_set);
                                    } else {
                                        todo!()
                                    }
                                } else {
                                    todo!()
                                }

                                // remove ending 0 and add back. should maybe be removed from vec in the first place
                                gfx.sprite_set[sprite_set_idx].pop();
                                gfx.sprite_set[sprite_set_idx].extend_from_slice(&[sprite_set1, sprite_set2, 0]);
                            }
                        }
                    }

                    _ => {
                        // replace old item's tile / sprite set slot
                        let original_data = original_item.tile_sprite_set().unwrap();
                        match tile_list {
                            TileList::TileSet => {
                                let tile_set_idx = gfx.tile_set_index[area_id] as usize;

                                for x in &mut gfx.tile_set[tile_set_idx] {
                                    if *x == original_data.0 {
                                        *x = tile_set;
                                    }
                                }
                            }

                            TileList::MidStage => {
                                let mid_stage_idx = gfx.mid_stage_index[area_id] as usize;

                                if let Some(spr_data) = &mut gfx.mid_stage[mid_stage_idx] {
                                    if let Some(ms_tile_set) = &mut spr_data.tile_sets {
                                        ms_tile_set.iter_mut().for_each(|x| if *x == original_data.0 {*x = tile_set});
                                    } else {
                                        spr_data.tile_sets = Some(vec![tile_set]);
                                    }
                                } else {
                                    todo!()
                                }
                            }
                        }

                        let mut x = 0;

                        while x < gfx.sprite_set[sprite_set_idx].len() {
                            // todo: make this look better
                            if gfx.sprite_set[sprite_set_idx][x] == original_data.1 && gfx.sprite_set[sprite_set_idx][x + 1] == original_data.2 {
                                gfx.sprite_set[sprite_set_idx][x    ] = sprite_set1;
                                gfx.sprite_set[sprite_set_idx][x + 1] = sprite_set2;
                            }

                            x += 2;
                        }
                    }
                }
            }
        }

        let tile_base = Self::snes_to_effective(0xBD9953);
        let sprite_base = Self::snes_to_effective(0x81AE11);
        let mid_stage_base = Self::snes_to_effective(0xBD9C7D + 0x80);

        let tile_serialize = gfx.write_tile_list(tile_base as u16);
        self.rom[tile_base .. tile_base + tile_serialize.len()].copy_from_slice(&tile_serialize);

        let sprite_serialize = gfx.write_sprite_list(sprite_base as u16);
        self.rom[sprite_base .. sprite_base + sprite_serialize.len()].copy_from_slice(&sprite_serialize);

        let mid_stage_serialize = gfx.write_mid_stage_list(mid_stage_base as u16);
        self.rom[mid_stage_base .. mid_stage_base + mid_stage_serialize.len()].copy_from_slice(&mid_stage_serialize);
    }

    fn snes_to_effective(address: u32) -> usize {
        if address & 0xFFFF < 0x8000 { panic!("sa->ea: bad address!") }

        let bank = ((address >> 16) & 0x7F) * 0x8000;
        let offset = (address - 0x8000) & 0xFFFF;
        (bank + offset) as usize
    }
}
