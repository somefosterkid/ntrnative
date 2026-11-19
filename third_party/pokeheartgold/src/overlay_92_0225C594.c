#include "global.h"

#include "overlay_92.h"

typedef struct WiFiPlazaQuestion {
    u32 questionNumber;
    u32 selectedAnswer;
} WiFiPlazaQuestion;

typedef int (*UnkFuncPtr_02032868)(void);
typedef u8 *(*UnkFuncPtr_0203290C)(int, void *, int);
typedef void (*UnkFuncPtr_020F8E60)(int, int, void *, void *);
typedef struct CommCmdTable {
    UnkFuncPtr_020F8E60 unk_00;
    UnkFuncPtr_02032868 unk_04;
    UnkFuncPtr_0203290C unk_08;
} CommCmdTable;
extern const CommCmdTable Unk_ov116_02267740[5];
int ov92_0225C5A4(void);

int ov92_0225C594(void) {
    return sizeof(WiFiPlazaQuestion);
}

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

int ov92_0225C598(void) {
    return sizeof(BattleRegulation);
}

extern s32 _0226383C;

s32 *ov92_0225C59C(void);

s32 *ov92_0225C59C(void) {
    return &_0226383C;
}

int ov92_0225C5A4(void) {
    return sizeof(Unk_ov116_02267740) / sizeof(CommCmdTable);
}
