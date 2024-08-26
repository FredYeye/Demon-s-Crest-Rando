# Demon's Crest Randomizer

## How to use
Simply running DCRando with a rom named "Demon's Crest (USA).sfc" in the same folder will generate a randomized rom. However, you can also run it from a command line prompt and use the following options:

```
  -s [any characters]: custom seed. If none is supplied, one will be generated.
  -i [name]: file to look for. If no name is supplied, "Demon's Crest (USA).sfc" will be used.
```

Example:
```
  DCRando -s "Give me the hand talisman in stage 1 pls!" -i path/to/rom.sfc
```

## Issues / todo
* Replacement items have incorrect colors.
* Some areas will get bugged sprites. This is beca TODO
* The hp item doesn't have a disable toggle, so you can keep picking up the replacement item. (custom code needed?)
* Resolve remaining softlocks (stage 6 statues for example).
* Include the phalanx stage items (fang talisman & HP) in the randomization.
