const GEN_PATCH: bool = false;

use std::hash::{DefaultHasher, Hash, Hasher};

use clap::Parser;
use cli_parser::Args;
use rando::Rando;

mod cli_parser;
mod rando;

fn main() {
    if GEN_PATCH {
        gen_patch();
        return;
    }

    let args = Args::parse();

    let seed = if let Some(seed) = args.seed {
        let mut s = DefaultHasher::new();
        seed.hash(&mut s);
        s.finish()
    } else {
        0x1337 // todo: generate random seed otherwise
    };

    let mut rando = Rando::new(&args.in_file, seed).expect("msg");
    rando.patch_rom();
    rando.randomize();
    rando.output_rom(&args.out_file);
}

fn gen_patch() {
    // used to generate a diff between a vanilla rom and a modified rom
    let rom1 = std::fs::read("Demon's Crest (USA).sfc").expect("cannot open file");
    let rom2 = std::fs::read("dcrando.sfc").expect("cannot open file");

    let mut offset = Vec::new();
    let mut diff = Vec::new();

    let mut offset_part = None;
    let mut diff_part = Vec::new();

    if rom1.len() == rom2.len() {
        for x in 0 .. rom1.len() {
            if rom1[x] != rom2[x] {
                if offset_part.is_none() {
                    offset_part = Some(x);
                }

                diff_part.push(rom2[x]);
            } else if let Some(y) = offset_part {
                offset.push(y as u32);
                diff.push(diff_part.clone());

                diff_part.clear();
                offset_part = None;
            }
        }
    }

    let mut result = Vec::new();

    for x in 0 .. offset.len() {
        result.extend_from_slice(&offset[x].to_le_bytes());
        result.extend_from_slice(&(diff[x].len() as u16).to_le_bytes());
        result.extend_from_slice(&diff[x]);
    }

    std::fs::write("dcrando.pat", result).expect("err");
    println!("created patch successfully");
}
