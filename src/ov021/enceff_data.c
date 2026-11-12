// The table of the encounter effects that EncEff_StartEvent (enceff.c) reads: for each effect the function that
// creates its event, the one that draws it, its overlay, a parameter and the most memory it needs. Black's .rodata
// has it ahead of enceff.c's two tables, which one file can't give (a table of 64 bytes or more gets a section of
// its own after the file's shared one), so it is a file of its own, as pokebw2's enceff_data.c (file name from
// pokebw2). The overlays are numbers, as pokebw2 writes them: Black's table has no relocation there.
#include "types.h"
#include "field/encounter_effect.h"
#include "field/encounter_effect_model.h"
#include "field/encounter_effect_ripple.h"
#include "field/encounter_effect_spread.h"
#include "field/event_encounter_cutin.h"

const EncEffEntry data_ov021_021db4fc[37] = {
    { func_ov076_021f5260, func_ov076_021f5324, 76, 1, 0x13010 },
    { func_ov076_021f5270, func_ov076_021f5324, 76, 1, 0x131a4 },
    { func_ov078_021f5620, func_ov078_021f57d4, 78, 1, 0x0 },
    { func_ov073_021f5380, NULL, 73, 0, 0x1e04 },
    { func_ov073_021f538c, NULL, 73, 1, 0x14bc8 },
    { func_ov076_021f5280, func_ov076_021f5324, 76, 1, 0x12fbc },
    { func_ov076_021f5290, func_ov076_021f5324, 76, 1, 0x13b20 },
    { func_ov079_021f5a80, func_ov077_021f57f8, 79, 0, 0x0 },
    { func_ov081_021f5a80, func_ov077_021f57f8, 81, 1, 0x0 },
    { func_ov080_021f5a80, func_ov077_021f57f8, 80, 1, 0x0 },
    { func_ov073_021f5260, NULL, 73, 0, 0xe570 },
    { func_ov073_021f526c, NULL, 73, 0, 0xe570 },
    { func_ov073_021f5278, NULL, 73, 0, 0xe570 },
    { func_ov073_021f5284, NULL, 73, 0, 0xe570 },
    { func_ov073_021f5290, NULL, 73, 0, 0xe570 },
    { func_ov073_021f529c, NULL, 73, 0, 0xe570 },
    { func_ov073_021f52a8, NULL, 73, 0, 0xe570 },
    { func_ov073_021f52b4, NULL, 73, 0, 0xe570 },
    { func_ov073_021f52c0, NULL, 73, 0, 0xe570 },
    { func_ov073_021f52cc, NULL, 73, 0, 0xe570 },
    { func_ov073_021f52d8, NULL, 73, 0, 0xe570 },
    { func_ov073_021f52e4, NULL, 73, 0, 0xe570 },
    { func_ov073_021f52f0, NULL, 73, 0, 0xe570 },
    { func_ov073_021f52fc, NULL, 73, 0, 0x12a68 },
    { func_ov073_021f5308, NULL, 73, 0, 0x12a68 },
    { func_ov073_021f5314, NULL, 73, 0, 0x12a68 },
    { func_ov073_021f5320, NULL, 73, 0, 0x12a68 },
    { func_ov073_021f532c, NULL, 73, 0, 0x12a68 },
    { func_ov073_021f5338, NULL, 73, 0, 0xd530 },
    { func_ov073_021f5344, NULL, 73, 0, 0xd530 },
    { func_ov073_021f5350, NULL, 73, 0, 0xcf20 },
    { func_ov076_021f52a0, func_ov076_021f5324, 76, 1, 0x132e8 },
    { func_ov073_021f535c, NULL, 73, 1, 0x80e0 },
    { func_ov073_021f5368, NULL, 73, 1, 0x619c },
    { func_ov073_021f5374, NULL, 73, 1, 0xac88 },
    { func_ov076_021f52b0, func_ov076_021f5324, 76, 1, 0x12dc0 },
    { func_ov078_021f565c, func_ov078_021f57d4, 78, 0, 0x0 },
};
