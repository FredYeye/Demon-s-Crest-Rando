pub enum Area {
    SomuloArena,
    SomuloExit,

    S1_1,
    S1_2,
    S1_3,

    S2Town,
    S2_1,
    S2_2a, //right path
    S2_2b,
    S2_3b,
    S2Ovnunu, //ovnunu path

    S3_1,
    S3_2a,
    S3_3a, // right path
    S3_3a2,
    S3_3a3, // already burning forest & forest burnt down (right path)
    S3_2b,
    S3_3b, // cave path -> scula
    S3HpCave,
    S3SkullCave,

    S4_1,
    S4_2,
    S4_3,
    S4_4,
    S4_5,
    S4Crown,

    S5_1,
    S5_2a, // right path
    S5_2a2, // post-crawler appearance
    S5CrawlerAlcove,
    S5_2b,
    S5_3b, // water path

    S6_1,
    S6_2a, //bottom path
    S6_2b, //upper path
    S6_3b,
    S6_4b,
    S6_5b,
    S6_6b,
    S6HpRoom,

    S7_2,
    S7_3,
    S7_4,
    S7_5,

    TrioThePago3,
}

impl Area {
    pub fn offset(&self) -> u8 {
        use Area::*;

        match self {
            SomuloArena => 0,
            SomuloExit => 17,

            S1_1 => 1,
            S1_2 => 2,
            S1_3 => 3,

            S2Town => 4,
            S2_1 => 5,
            S2_2a => 9,
            S2_2b => 6,
            S2_3b => 7,
            S2Ovnunu => 8,

            S3_1 => 10,
            S3_2a => 12,
            S3_3a => 14,
            S3_3a2 => 51,
            S3_3a3 => 50,
            S3_2b => 11,
            S3_3b => 13,
            S3HpCave => 15,
            S3SkullCave => 16,

            S4_1 => 18,
            S4_2 => 19,
            S4_3 => 20,
            S4_4 => 21,
            S4_5 => 23,
            S4Crown => 22,

            S5_1 => 24,
            S5_2a => 27,
            S5_2a2 => 59,
            S5CrawlerAlcove => 28,
            S5_2b => 25,
            S5_3b => 26,

            S6_1 => 29,
            S6_2a => 30,
            S6_2b => 31,
            S6_3b => 33,
            S6_4b => 34,
            S6_5b => 35,
            S6_6b => 36,
            S6HpRoom => 32,

            S7_2 => 38,
            S7_3 => 39,
            S7_4 => 40,
            S7_5 => 41,

            TrioThePago3 => 53,
        }
    }
}
