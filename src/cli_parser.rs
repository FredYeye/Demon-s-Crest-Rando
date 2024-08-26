use clap::Parser;

#[derive(Parser, Debug)]
pub struct Args {
    /// Path to rom
    #[arg(short, default_value = "Demon's Crest (USA).sfc")]
    pub in_file: String,

    /// Path to rando output
    #[arg(short, default_value = "Demon's Crest Rando.sfc")]
    pub out_file: String,

    /// Custom seed
    #[arg(short)]
    pub seed: Option<String>,
}
