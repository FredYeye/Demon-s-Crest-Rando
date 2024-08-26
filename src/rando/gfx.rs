pub struct SpriteData {
    loop_count: u8,
    unknown: u16,
    sprite_set_overrides: Vec<u16>,
    pub tile_sets: Option<Vec<u16>>, // predend u8(1) if Some, u8(0) if None. append u16(0)
    palette_and_offset: Option<Vec<[u8; 2]>>, // prepend u8(vector len)
}

pub struct Gfx {
    pub tile_set_index: [u8; 116],
    pub tile_set: [Vec<u16>; 85],

    pub sprite_set_index: [u8; 116],
    pub sprite_set: [Vec<u8>; 90],

    pub mid_stage_index: [u8; 116],
    pub mid_stage: [Option<SpriteData>; 49],
}

impl Gfx {
    pub fn init() -> Self {
        let tiles = Self::get_tile_list();
        let sprites = Self::get_sprite_list();
        let mid_stage = Self::get_mid_stage_list();

        Self {
            tile_set_index: tiles.0,
            tile_set: tiles.1,

            sprite_set_index: sprites.0,
            sprite_set: sprites.1,

            mid_stage_index: mid_stage.0,
            mid_stage: mid_stage.1,
        }
    }

    fn get_tile_list() -> ([u8; 116], [Vec<u16>; 85]) {
        let tile_set_index = [
            00, 02, 03, 04, 05, 06, 07, 08,
            09, 10, 11, 12, 13, 14, 15, 16,
            16, 01, 17, 18, 19, 20, 21, 22,
            23, 24, 25, 26, 27, 28, 29, 30,
            31, 32, 33, 34, 35, 36, 37, 38,
            39, 40, 41, 42, 43, 44, 45, 46,
            46, 46, 15, 15, 47, 47, 47, 46,
            48, 48, 48, 26, 48, 48, 48, 48,
            48, 49, 50, 51, 52, 53, 54, 55,
            56, 56, 57, 58, 59, 59, 59, 59,
            60, 61, 62, 60, 63, 63, 63, 63,
            63, 63, 64, 65, 66, 67, 68, 69,
            70, 71, 72, 70, 73, 74, 75, 76,
            77, 78, 79, 80, 80, 80, 81, 82,
            83, 84, 84, 84,
        ];

        let tile_set = [
            vec![0x001C, 0x0028, 0x00CC, 0xFFFF], // somulo arena
            vec![0x001C, 0x0028, 0x00D4, 0x0118, 0xFFFF], // somulo exit
            vec![0x0138, 0x0004, 0x0134, 0x00A8, 0xFFFF],
            vec![0x00F8, 0x00EC, 0x00A4, 0x0088, 0xFFFF],
            vec![0x0084, 0x0088, 0x0184, 0xFFFF],
            vec![0x017C, 0x018C, 0xFFFF],
            vec![0x00F8, 0x0148, 0x0080, 0x0044, 0x0120, 0x009C, 0xFFFF], // hand
            vec![0x0138, 0x0018, 0x0008, 0x004C, 0xFFFF],
            vec![0x0054, 0x004C, 0x0058, 0xFFFF],
            vec![0x0108, 0x0108, 0x0108, 0x00DC,0x00E0, 0x0104, 0x00FC, 0xFFFF],
            vec![0x0014, 0x0040, 0x00A4, 0xFFFF],
            vec![0x00F8, 0x002C, 0x0008, 0x0098, 0x010C, 0xFFFF],
            vec![0x0138, 0x0080, 0x0074, 0x0038, 0x0070, 0xFFFF],
            vec![0x0080, 0x0120, 0x0068, 0x0008, 0x010C, 0xFFFF],
            vec![0x0080, 0x0074, 0x0008, 0x0070, 0xFFFF],
            vec![0x0064, 0xFFFF],
            vec![0x0140, 0x0080, 0xFFFF], // skull
            vec![0x00F8, 0x0090, 0x007C, 0x0030, 0xFFFF],
            vec![0x0090, 0x006C, 0x00A4, 0xFFFF],
            vec![0x003C, 0xFFFF],
            vec![0x00A8, 0x0044, 0x005C, 0x0144, 0xFFFF],
            vec![0x0144, 0xFFFF], // crown
            vec![0x0138, 0x0150, 0x0134, 0xFFFF],
            vec![0x0080, 0x0120, 0x006C, 0xFFFF],
            vec![0x0080, 0x0120, 0x0074, 0x0038, 0xFFFF],
            vec![0x0120, 0x0198, 0x0080, 0xFFFF],
            vec![0x0080, 0x00A8, 0xFFFF],
            vec![0xFFFF],
            vec![0x00F8, 0x002C, 0x006C, 0x0014, 0xFFFF],
            vec![0x0138, 0x002C, 0x00A4, 0x00FC, 0x0014, 0xFFFF], // demon fire
            vec![0x0088, 0x0018, 0xFFFF],
            vec![0xFFFF],
            vec![0x0008, 0x0068, 0x00A8, 0xFFFF],
            vec![0xFFFF],
            vec![0x0100, 0x000C, 0x0054, 0x0040, 0x0058, 0xFFFF],
            vec![0x00F4, 0x0124, 0x0128, 0x012C, 0xFFFF],
            vec![0x018C, 0x015C, 0x01E8, 0x0030, 0xFFFF],
            vec![0x019C, 0x00A8, 0x0044, 0xFFFF],
            vec![0x013C, 0x006C, 0x0134, 0xFFFF], // fang
            vec![0x019C, 0x00A8, 0x0150, 0xFFFF],
            vec![0x018C, 0x0048, 0x004C, 0xFFFF],
            vec![0x0130, 0xFFFF],
            vec![0x0190, 0x0194, 0xFFFF],
            vec![0x0080, 0xFFFF],
            vec![0x011C, 0x01F8, 0x01F0, 0x01F4, 0xFFFF],
            vec![0x0168, 0x0158, 0xFFFF],
            vec![0x0158, 0x0160, 0x0164, 0xFFFF],
            vec![0x0180, 0x0188, 0x0058, 0xFFFF],
            vec![0x008C, 0x00A0, 0x00E8, 0xFFFF],
            vec![0x0108, 0x0108, 0x0108, 0x00DC, 0x00E0, 0x0104, 0xFFFF],
            vec![0x00F0, 0xFFFF],
            vec![0x0124, 0x0128, 0x012C, 0xFFFF],
            vec![0x0064, 0x00D8, 0xFFFF],
            vec![0x0114, 0xFFFF],
            vec![0x0174, 0x016C, 0x0170, 0x0178, 0xFFFF],
            vec![0x014C, 0xFFFF],
            vec![0x003C, 0xFFFF],
            vec![0x00D0, 0x00C4, 0xFFFF],
            vec![0x0130, 0xFFFF],
            vec![0xFFFF],
            vec![0x00AC, 0x00B0, 0x00B4, 0x00B8, 0x00BC, 0x00C0, 0x00E4, 0xFFFF],
            vec![0xFFFF],
            vec![0x01DC, 0xFFFF],
            vec![0xFFFF],
            vec![0x00F4, 0xFFFF],
            vec![0x00BC, 0x01AC, 0xFFFF],
            vec![0x01B4, 0xFFFF],
            vec![0x01E4, 0xFFFF],
            vec![0x0154, 0xFFFF],
            vec![0x01D8, 0xFFFF],
            vec![0x01BC, 0x01EC, 0xFFFF],
            vec![0x01AC, 0x01C0, 0x01B0, 0xFFFF],
            vec![0x01D4, 0x01CC, 0x01D8, 0xFFFF],
            vec![0x01B4, 0xFFFF],
            vec![0x01B8, 0xFFFF],
            vec![0x01C4, 0xFFFF],
            vec![0x01D0, 0xFFFF],
            vec![0x01C8, 0xFFFF],
            vec![0x01CC, 0xFFFF],
            vec![0x01D4, 0xFFFF],
            vec![0x0130, 0x01E0, 0xFFFF],
            vec![0xFFFF],
            vec![0x01AC, 0xFFFF],
            vec![0x01AC, 0xFFFF],
            vec![0xFFFF],
        ];

        (tile_set_index, tile_set)
    }

    pub fn write_tile_list(&self, base: u16) -> Vec<u8> {
        let mut current_offset = self.tile_set_index.len() as u16 * 2;
        let mut calculated_offsets = Vec::new();

        for set in &self.tile_set {
            calculated_offsets.push(base + current_offset);
            current_offset += set.len() as u16 * 2;
        }

        let mut tile_set_index2 = [0; 116];
        for (idx, &id) in self.tile_set_index.iter().enumerate() {
            tile_set_index2[idx] = calculated_offsets[id as usize];
        }

        let mut tile_serialize = Vec::new();

        for tile_s in tile_set_index2 {
            tile_serialize.extend_from_slice(&tile_s.to_le_bytes());
        }

        for set in &self.tile_set {
            set.iter().for_each(|data| tile_serialize.extend_from_slice(&data.to_le_bytes()));
        }

        tile_serialize
    }

    fn get_sprite_list() -> ([u8; 116], [Vec<u8>; 90]) {
        let sprite_set_index = [
            00, 02, 03, 04, 05, 06, 07, 08,
            09, 10, 11, 12, 13, 14, 15, 16,
            16, 01, 17, 18, 19, 20, 21, 22,
            23, 24, 25, 26, 89, 27, 28, 29,
            30, 31, 32, 33, 34, 35, 36, 37,
            38, 39, 40, 41, 42, 43, 44, 45,
            45, 45, 15, 15, 46, 46, 46, 45,
            47, 47, 47, 26, 47, 47, 47, 47,
            48, 49, 50, 51, 52, 53, 54, 55,
            56, 56, 57, 58, 89, 89, 89, 89,
            59, 60, 61, 59, 62, 63, 64, 65,
            66, 67, 68, 69, 70, 71, 72, 73,
            74, 75, 76, 77, 78, 79, 80, 81,
            82, 83, 84, 85, 85, 85, 86, 87,
            89, 89, 89, 89,
        ];

        // todo: consider removing the trailing 0 and append during serialization
        let sprite_set = [
            vec![0x0D, 0x06, 0x11, 0x08, 0x3D, 0x0A, 0x09, 0x02, 0x0A, 0x02, 0x0B, 0x02, 0x0C, 0x02, 0x00],
            vec![0x0D, 0x06, 0x11, 0x08, 0x3F, 0x0A, 0x57, 0x06, 0x09, 0x02, 0x0A, 0x02, 0x00],
            vec![0x4E, 0x02, 0x03, 0x04, 0x61, 0x06, 0x33, 0x08, 0x0E, 0x08, 0x16, 0x08, 0x00],
            vec![0x4E, 0x02, 0x48, 0x04, 0x32, 0x06, 0x29, 0x08, 0x33, 0x0A, 0x77, 0x0A, 0x00],
            vec![0x28, 0x02, 0x29, 0x04, 0x77, 0x06, 0x5A, 0x02, 0x5C, 0x04, 0x4D, 0x06, 0x00],
            vec![0x75, 0x02, 0x79, 0x04, 0x00],
            vec![0x4E, 0x02, 0x50, 0x02, 0x27, 0x04, 0x18, 0x06, 0x59, 0x08, 0x30, 0x0A, 0x00],
            vec![0x4E, 0x02, 0x08, 0x04, 0x04, 0x06, 0x1A, 0x08, 0x00],
            vec![0x1C, 0x02, 0x1A, 0x04, 0x1D, 0x06, 0x00],
            vec![0x52, 0x02, 0x52, 0x04, 0x52, 0x06, 0x43, 0x06, 0x44, 0x08, 0x51, 0x0A, 0x4F, 0x08, 0x42, 0x06, 0x00],
            vec![0x07, 0x02, 0x17, 0x06, 0x32, 0x08, 0x2B, 0x04, 0x2C, 0x04, 0x2A, 0x04, 0x31, 0x0A, 0x47, 0x04, 0x00],
            vec![0x4E, 0x02, 0x12, 0x04, 0x04, 0x06, 0x2F, 0x08, 0x53, 0x0A, 0x00],
            vec![0x4E, 0x02, 0x27, 0x04, 0x24, 0x06, 0x15, 0x08, 0x23, 0x0A, 0x00],
            vec![0x27, 0x02, 0x59, 0x04, 0x21, 0x06, 0x04, 0x08, 0x53, 0x0A, 0x00],
            vec![0x27, 0x02, 0x24, 0x04, 0x04, 0x06, 0x23, 0x08, 0x3B, 0x04, 0x3E, 0x06, 0x3A, 0x04, 0x00],
            vec![0x20, 0x02, 0x40, 0x04, 0x41, 0x06, 0x4F, 0x0A, 0x1F, 0x08, 0x32, 0x0A, 0x00],
            vec![0x50, 0x02, 0x27, 0x04, 0x00],
            vec![0x4E, 0x02, 0x2D, 0x04, 0x26, 0x06, 0x13, 0x08, 0x00],
            vec![0x2D, 0x02, 0x22, 0x04, 0x32, 0x06, 0x4C, 0x08, 0x4A, 0x08, 0x49, 0x08, 0x4F, 0x0A, 0x00],
            vec![0x16, 0x08, 0x0E, 0x08, 0x00],
            vec![0x33, 0x02, 0x18, 0x04, 0x1E, 0x06, 0x00],
            vec![0x50, 0x02, 0x00],
            vec![0x4E, 0x02, 0x64, 0x04, 0x61, 0x06, 0x5A, 0x04, 0x5C, 0x06, 0x5D, 0x08, 0x4D, 0x02, 0x00],
            vec![0x27, 0x02, 0x59, 0x08, 0x22, 0x04, 0x00],
            vec![0x27, 0x02, 0x59, 0x04, 0x24, 0x06, 0x15, 0x08, 0x00],
            vec![0x59, 0x04, 0x7E, 0x06, 0x27, 0x02, 0x54, 0x0A, 0x55, 0x0A, 0x56, 0x08, 0x00],
            vec![0x27, 0x02, 0x33, 0x04, 0x73, 0x04, 0x71, 0x02, 0x6F, 0x06, 0x70, 0x06, 0x72, 0x04, 0x4D, 0x0A, 0x00],
            vec![0x4E, 0x02, 0x12, 0x04, 0x22, 0x06, 0x07, 0x08, 0x00],
            vec![0x4E, 0x02, 0x12, 0x04, 0x32, 0x06, 0x4F, 0x08, 0x07, 0x0A, 0x62, 0x04, 0x63, 0x06, 0x00],
            vec![0x29, 0x02, 0x08, 0x04, 0x00],
            vec![0x00],
            vec![0x04, 0x02, 0x21, 0x04, 0x33, 0x06, 0x00],
            vec![0x4C, 0x08, 0x4A, 0x08, 0x49, 0x08, 0x00],
            vec![0x50, 0x02, 0x05, 0x04, 0x1C, 0x06, 0x17, 0x08, 0x1D, 0x0A, 0x00],
            vec![0x4D, 0x02, 0x5C, 0x04, 0x5D, 0x06, 0x5E, 0x08, 0x5B, 0x0A, 0x00],
            vec![0x79, 0x02, 0x6A, 0x04, 0x98, 0x04, 0x13, 0x06, 0x0E, 0x08, 0x16, 0x08, 0x00],
            vec![0x7F, 0x02, 0x33, 0x04, 0x18, 0x06, 0x00],
            vec![0x50, 0x02, 0x22, 0x04, 0x61, 0x06, 0x00],
            vec![0x7F, 0x02, 0x33, 0x04, 0x64, 0x06, 0x00],
            vec![0x79, 0x02, 0x19, 0x04, 0x1A, 0x06, 0x62, 0x04, 0x63, 0x06, 0x00],
            vec![0x60, 0x06, 0x5F, 0x08, 0x5F, 0x0A, 0x82, 0x06, 0x4D, 0x04, 0x81, 0x02, 0x27, 0x04, 0x00],
            vec![0x7A, 0x02, 0x7D, 0x08, 0x7B, 0x02, 0x7C, 0x02, 0x00],
            vec![0x27, 0x02, 0x00],
            vec![0x58, 0x08, 0x9C, 0x06, 0x9A, 0x02, 0x9B, 0x04, 0x00],
            vec![0x6E, 0x02, 0x69, 0x04, 0x00],
            vec![0x69, 0x02, 0x6C, 0x04, 0x6D, 0x06, 0x00],
            vec![0x76, 0x02, 0x78, 0x04, 0x1D, 0x06, 0x00],
            vec![0x00],
            vec![0x2A, 0x04, 0x31, 0x0A, 0x47, 0x04, 0x2B, 0x04, 0x2C, 0x04, 0x00],
            vec![0x52, 0x02, 0x52, 0x04, 0x52, 0x06, 0x43, 0x06, 0x44, 0x08, 0x51, 0x0A, 0x42, 0x06, 0x00],
            vec![0x49, 0x08, 0x4C, 0x08, 0x4A, 0x08, 0x00],
            vec![0x5C, 0x04, 0x5D, 0x04, 0x5E, 0x04, 0x5B, 0x08, 0x00],
            vec![0x20, 0x02, 0x41, 0x06, 0x40, 0x04, 0x00],
            vec![0x56, 0x08, 0x54, 0x0A, 0x55, 0x0A, 0x00],
            vec![0x71, 0x02, 0x6F, 0x06, 0x70, 0x06, 0x72, 0x04, 0x73, 0x04, 0x00],
            vec![0x63, 0x06, 0x62, 0x04, 0x00],
            vec![0x16, 0x08, 0x0E, 0x08, 0x00],
            vec![0x3E, 0x06, 0x3A, 0x04, 0x3B, 0x04, 0x00],
            vec![0x60, 0x06, 0x5F, 0x08, 0x5F, 0x0A, 0x00],
            vec![0x34, 0x02, 0x35, 0x04, 0x36, 0x06, 0x37, 0x08, 0x38, 0x0A, 0x39, 0x0C, 0x46, 0x04, 0x00],
            vec![0x00],
            vec![0x95, 0x02, 0x00],
            vec![0x00],
            vec![0x00],
            vec![0x00],
            vec![0x00],
            vec![0x00],
            vec![0x00],
            vec![0x4D, 0x02, 0x00],
            vec![0x38, 0x08, 0x83, 0x02, 0x00],
            vec![0x85, 0x06, 0x45, 0x02, 0x4B, 0x04, 0x00],
            vec![0x97, 0x02, 0x00],
            vec![0x65, 0x02, 0x4B, 0x04, 0x00],
            vec![0x00],
            vec![0x87, 0x02, 0x99, 0x08, 0x45, 0x06, 0x4B, 0x04, 0x00],
            vec![0x83, 0x02, 0x88, 0x08, 0x84, 0x06, 0x00],
            vec![0x8D, 0x08, 0x8B, 0x0A, 0x8E, 0x0C, 0x00],
            vec![0x87, 0x02, 0x99, 0x06, 0x4B, 0x04, 0x00],
            vec![0x85, 0x02, 0x00],
            vec![0x86, 0x02, 0x00],
            vec![0x89, 0x08, 0x00],
            vec![0x8C, 0x08, 0x00],
            vec![0x8A, 0x08, 0x00],
            vec![0x8B, 0x08, 0x00],
            vec![0x8D, 0x08, 0x00],
            vec![0x60, 0x02, 0x96, 0x04, 0x8F, 0x06, 0x4B, 0x08, 0x00],
            vec![0x4B, 0x02, 0x00],
            vec![0x83, 0x08, 0x00],
            vec![0x83, 0x08, 0x00],
            vec![0x00],
        ];

        (sprite_set_index, sprite_set)
    }

    pub fn write_sprite_list(&self, base: u16) -> Vec<u8> {
        let mut current_offset = self.sprite_set_index.len() as u16 * 2;
        let mut calculated_offsets = Vec::new();

        for set in &self.sprite_set {
            calculated_offsets.push(base + current_offset);
            current_offset += set.len() as u16;
        }

        let mut sprite_set_index2 = [0; 116];
        for (idx, &id) in self.sprite_set_index.iter().enumerate() {
            sprite_set_index2[idx] = calculated_offsets[id as usize];
        }

        let mut sprite_serialize = Vec::new();

        for sprite_s in sprite_set_index2 {
            sprite_serialize.extend_from_slice(&sprite_s.to_le_bytes());
        }

        for set in &self.sprite_set {
            sprite_serialize.extend_from_slice(&set);
        }

        sprite_serialize
    }

    fn get_mid_stage_list() -> ([u8; 116], [Option<SpriteData>; 49]) {
        let mid_stage_index = [
            00, 02, 04, 03, 04, 04, 04, 04,
            05, 06, 07, 07, 07, 08, 09, 10,
            10, 01, 10, 11, 12, 13, 13, 14,
            15, 15, 16, 17, 18, 19, 20, 21,
            21, 21, 22, 23, 24, 25, 26, 26,
            26, 27, 28, 29, 30, 30, 30, 30,
            30, 30, 09, 09, 30, 30, 30, 30,
            30, 30, 30, 17, 30, 30, 30, 30,
            30, 31, 32, 33, 34, 35, 36, 37,
            38, 39, 40, 41, 42, 42, 42, 42,
            43, 43, 43, 43, 43, 43, 43, 43,
            43, 43, 43, 43, 44, 45, 45, 46,
            44, 46, 46, 45, 46, 46, 46, 46,
            46, 46, 46, 47, 47, 47, 45, 46,
            48, 48, 48, 48,
        ];

        let mid_stage = [
            Some( SpriteData {
                loop_count: 0x13, unknown: 0x0000, sprite_set_overrides: vec![0x0918, 0x0A1C, 0x0B10, 0x0C14],
                tile_sets: None, palette_and_offset: None,
            }),

            Some( SpriteData {
                loop_count: 0x0A, unknown: 0x0000, sprite_set_overrides: vec![0x0918, 0x0A1C],
                tile_sets: None, palette_and_offset: None,
            }),

            Some( SpriteData {
                loop_count: 0x08, unknown: 0x00BE, sprite_set_overrides: vec![0x0E1C],
                tile_sets: Some(vec![0x003C]), palette_and_offset: Some(vec![[0x14, 0x80], [0x15, 0xA0]]),
            }),

            Some( SpriteData {
                loop_count: 0x0F, unknown: 0x0366, sprite_set_overrides: vec![0x5A1C],
                tile_sets: Some(vec![0x0124, 0x00F4]), palette_and_offset: Some(vec![[0x56, 0x20], [0x59, 0x40], [0x45, 0x60]]),
            }),

            None,

            Some( SpriteData {
                loop_count: 0x10, unknown: 0x01F4, sprite_set_overrides: vec![0x421E],
                tile_sets: None, palette_and_offset: None,
            }),

            Some( SpriteData {
                loop_count: 0x05, unknown: 0x010E, sprite_set_overrides: vec![0x2B18, 0x2C1C],
                tile_sets: Some(vec![0x008C, 0x00A0, 0x00E8]), palette_and_offset: Some(vec![]),
            }),

            None,

            Some( SpriteData {
                loop_count: 0x06, unknown: 0x0140, sprite_set_overrides: vec![0x3B1C],
                tile_sets: Some(vec![0x00D0, 0x00C4]), palette_and_offset: Some(vec![[0x39, 0x40], [0x39, 0x60]]),
            }),

            Some( SpriteData {
                loop_count: 0x0C, unknown: 0x017C, sprite_set_overrides: vec![0x401C],
                tile_sets: None, palette_and_offset: Some(vec![[0x47, 0xA0]]),
            }),

            None,

            Some( SpriteData {
                loop_count: 0x09, unknown: 0x0294, sprite_set_overrides: vec![0x4C18, 0x4A1C],
                tile_sets: Some(vec![0x00F0, 0x00FC]), palette_and_offset: Some(vec![]),
            }),

            Some( SpriteData {
                loop_count: 0x08, unknown: 0x00BE, sprite_set_overrides: vec![0x0E1C],
                tile_sets: None, palette_and_offset: Some(vec![]),
            }),

            None,

            Some( SpriteData {
                loop_count: 0x0F, unknown: 0x0366, sprite_set_overrides: vec![0x5A1C],
                tile_sets: Some(vec![0x0124, 0x0128, 0x00F4]), palette_and_offset: Some(vec![[0x57, 0x40], [0x59, 0x60], [0x5A, 0x80]]),
            }),

            None,

            Some( SpriteData {
                loop_count: 0x0C, unknown: 0x02EE, sprite_set_overrides: vec![0x5418, 0x551C],
                tile_sets: Some(vec![0x0114]), palette_and_offset: Some(vec![[0x4E, 0x80], [0x4D, 0xA0]]),
            }),

            Some( SpriteData {
                loop_count: 0x07, unknown: 0x0474, sprite_set_overrides: vec![0x731C],
                tile_sets: Some(vec![0x0174, 0x016C, 0x0170, 0x0178, 0x00F4]), palette_and_offset: Some(vec![[0x76, 0x20], [0x6F, 0x40], [0x6F, 0x60], [0x70, 0x80], [0x47, 0xA0]]),
            }),

            None,

            None,

            Some( SpriteData {
                loop_count: 0x11, unknown: 0x053C, sprite_set_overrides: vec![0x621C],
                tile_sets: Some(vec![0x014C]), palette_and_offset: Some(vec![[0x61, 0x40], [0x62, 0x60]]),
            }),

            None,

            Some( SpriteData {
                loop_count: 0x09, unknown: 0x0294, sprite_set_overrides: vec![0x4C18, 0x4A1C],
                tile_sets: Some(vec![0x00F0]), palette_and_offset: Some(vec![]),
            }),

            None,

            Some( SpriteData {
                loop_count: 0x0F, unknown: 0x0366, sprite_set_overrides: vec![0x5B1C],
                tile_sets: Some(vec![]), palette_and_offset: Some(vec![]),
            }),

            Some( SpriteData {
                loop_count: 0x08, unknown: 0x00BE, sprite_set_overrides: vec![0x0E1C],
                tile_sets: Some(vec![0x003C]), palette_and_offset: Some(vec![[0xA2, 0x80], [0x15, 0xA0]]),
            }),

            None,

            Some( SpriteData {
                loop_count: 0x11, unknown: 0x053C, sprite_set_overrides: vec![0x621C],
                tile_sets: Some(vec![0x014C]), palette_and_offset: Some(vec![[0xAA, 0x40], [0xAB, 0x60]]),
            }),

            Some( SpriteData {
                loop_count: 0x0C, unknown: 0x03FC, sprite_set_overrides: vec![0x5F1C, 0x5F1C],
                tile_sets: None, palette_and_offset: Some(vec![]),
            }),

            Some( SpriteData {
                loop_count: 0x0A, unknown: 0x05E6, sprite_set_overrides: vec![0x7B18, 0x7C1C],
                tile_sets: None, palette_and_offset: Some(vec![]),
            }),

            Some( SpriteData {
                loop_count: 0x05, unknown: 0x010E, sprite_set_overrides: vec![0x2B18, 0x2C1C],
                tile_sets: None, palette_and_offset: Some(vec![]),
            }),

            Some( SpriteData {
                loop_count: 0x10, unknown: 0x01F4, sprite_set_overrides: vec![0x421E],
                tile_sets: None, palette_and_offset: Some(vec![]),
            }),

            Some( SpriteData {
                loop_count: 0x09, unknown: 0x0294, sprite_set_overrides: vec![0x4C18, 0x4A1C],
                tile_sets: None, palette_and_offset: Some(vec![]),
            }),

            Some( SpriteData {
                loop_count: 0x0F, unknown: 0x0366, sprite_set_overrides: vec![0x5B1C],
                tile_sets: None, palette_and_offset: Some(vec![]),
            }),

            Some( SpriteData {
                loop_count: 0x0C, unknown: 0x017C, sprite_set_overrides: vec![0x401C],
                tile_sets: None, palette_and_offset: Some(vec![]),
            }),

            Some( SpriteData {
                loop_count: 0x0C, unknown: 0x02EE, sprite_set_overrides: vec![0x5418, 0x551C],
                tile_sets: Some(vec![0x0114]), palette_and_offset: Some(vec![]),
            }),

            Some( SpriteData {
                loop_count: 0x07, unknown: 0x0474, sprite_set_overrides: vec![0x731C],
                tile_sets: None, palette_and_offset: Some(vec![]),
            }),

            Some( SpriteData {
                loop_count: 0x11, unknown: 0x053C, sprite_set_overrides: vec![0x621C],
                tile_sets: None, palette_and_offset: Some(vec![]),
            }),

            None,

            Some( SpriteData {
                loop_count: 0x08, unknown: 0x00BE, sprite_set_overrides: vec![0x0E1C],
                tile_sets: None, palette_and_offset: Some(vec![]),
            }),

            Some( SpriteData {
                loop_count: 0x06, unknown: 0x0140, sprite_set_overrides: vec![0x3B1C],
                tile_sets: None, palette_and_offset: Some(vec![]),
            }),

            Some( SpriteData {
                loop_count: 0x0C, unknown: 0x03FC, sprite_set_overrides: vec![0x5F1C, 0x5F1C],
                tile_sets: None, palette_and_offset: Some(vec![]),
            }),

            None,

            None,

            Some( SpriteData {
                loop_count: 0x0D, unknown: 0x04BA, sprite_set_overrides: vec![0x4514, 0x4B1C],
                tile_sets: None, palette_and_offset: Some(vec![]),
            }),

            Some( SpriteData {
                loop_count: 0x0D, unknown: 0x04BA, sprite_set_overrides: vec![0x4B1C],
                tile_sets: None, palette_and_offset: Some(vec![]),
            }),

            None,

            Some( SpriteData {
                loop_count: 0x0D, unknown: 0x04BA, sprite_set_overrides: vec![0x8F18, 0x4B1C],
                tile_sets: None, palette_and_offset: Some(vec![]),
             }),

            None,
        ];

        (mid_stage_index, mid_stage)
    }

    pub fn write_mid_stage_list(&self, base: u16) -> Vec<u8> {
        let mut current_offset = self.mid_stage_index.len() as u16 * 2;
        let mut calculated_offsets = Vec::new();

        for set in &self.mid_stage {
            calculated_offsets.push(base + current_offset);

            let set_size = match set {
                Some(spr_data) => {
                    let mut temp = 1 + 2 + spr_data.sprite_set_overrides.len() * 2 + 1;

                    if let Some(tile_sets) = &spr_data.tile_sets {
                        temp += (tile_sets.len() + 1) * 2;
                    }

                    if let Some(palette_and_offset) = &spr_data.palette_and_offset {
                        temp += 1 + palette_and_offset.len() * 2;
                    }

                    temp
                }

                None => 1,
            };

            current_offset += set_size as u16;
        }

        let mut mid_stage_index2 = [0; 116];
        for (idx, &id) in self.mid_stage_index.iter().enumerate() {
            mid_stage_index2[idx] = calculated_offsets[id as usize];
        }

        let mut mid_stage_serialize = Vec::new();

        for mid_s in mid_stage_index2 {
            mid_stage_serialize.extend_from_slice(&mid_s.to_le_bytes());
        }

        for set in &self.mid_stage {
            if let Some(set) = set {
                mid_stage_serialize.push(set.loop_count);
                mid_stage_serialize.extend_from_slice(&set.unknown.to_le_bytes());

                for &sso in &set.sprite_set_overrides {
                    mid_stage_serialize.extend_from_slice(&sso.to_le_bytes())
                }

                if let Some(tile_set) = &set.tile_sets {
                    mid_stage_serialize.push(1);

                    for &ts in tile_set {
                        mid_stage_serialize.extend_from_slice(&ts.to_le_bytes());
                    }

                    mid_stage_serialize.extend_from_slice(&[0, 0]);
                } else {
                    mid_stage_serialize.push(0);
                }

                if let Some(palette_and_offset) = &set.palette_and_offset {
                    mid_stage_serialize.push(palette_and_offset.len() as u8);
                    mid_stage_serialize.extend_from_slice(palette_and_offset.as_flattened());
                }
            } else {
                mid_stage_serialize.push(0);
            }
        }

        mid_stage_serialize
    }
}
