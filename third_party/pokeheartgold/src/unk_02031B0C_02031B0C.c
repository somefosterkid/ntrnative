#include "global.h"
#include "save.h"
#include "unk_02031B0C.h"

u32 Save_ApricornBox_sizeof(void);

typedef struct BattleRegulation {
    u16 name[12];
    u16 maxTotalLevel;
    u8 teamSize;
    u8 maxLevel;
    s8 heightRestriction;
    s8 weightRestriction;
    u8 evolutionRestriction : 1;
    u8 specialSpeciesRestriction : 1;
    u8 speciesUniqueness : 1;
    u8 itemUniqueness : 1;
    u8 moveRestrictions : 1;
} BattleRegulation;
int sub_02031B10(void);

u32 Save_ApricornBox_sizeof(void) {
    return 0x80;
}

int sub_02031B10(void) {
    return sizeof(BattleRegulation);
}

SaveApricornBox *Save_ApricornBox_Get(SaveData *saveData) {
    return SaveArray_Get(saveData, 0x26);
}
