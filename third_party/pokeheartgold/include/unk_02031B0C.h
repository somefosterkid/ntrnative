#ifndef POKEHEARTGOLD_UNK_02031B0C_H
#define POKEHEARTGOLD_UNK_02031B0C_H

#include "pm_string.h"
#include "save.h"

// size: 0x8; the 2-byte alignment matters (mwcc copies it with ldrh/strh)
typedef struct ApricornJuice {
    u16 fanCount;
    u8 mildness;
    u8 flavors[5];
} ApricornJuice;

typedef struct SaveApricornBox SaveApricornBox;

// size: 0x2: one juice taste entry written by sub_02031DA0
typedef struct ApricornJuiceRank {
    u16 flavor : 4;
    u16 tier : 4;
    u16 value : 8;
} ApricornJuiceRank;

// size: 0xE: the ranked flavors of an ApricornJuice (sub_02031DA0 output)
typedef struct ApricornJuiceRanks {
    ApricornJuiceRank ranks[3];
    u8 order[5];
    u8 topCount;
    u8 rankCount;
    u8 pad_0D;
} ApricornJuiceRanks;

// size: 0x20
typedef struct ApricornRecord {
    u32 trainerId;
    u8 gender;
    u8 language;
    u8 version;
    u8 padding;
    u16 name[7 + 1];
    ApricornJuice data;
} ApricornRecord;

// size: 0x80
struct SaveApricornBox {
    u8 apricorns[7];
    u8 kurtQuantity;
    u8 kurtApricorn;
    u8 juiceServings;
    union {
        u16 queuedApricorns;
        struct {
            u16 queuedSlots : 15;
            u16 mixing : 1;
        };
    };
    ApricornJuice juice;
    u32 mixStartSteps;
    u32 juiceAgingSteps;
    union {
        u32 mixWord;
        struct {
            u32 mixProgress : 30;
            u32 mixState : 2;
        };
    };
    ApricornRecord juiceRecords[3];
};

struct PlayerProfile;

SaveApricornBox *Save_ApricornBox_Get(SaveData *saveData);
void sub_02031CCC(SaveApricornBox *, int);
void sub_02031B5C(ApricornJuice *juice);
BOOL sub_02031CEC(SaveApricornBox *apricornBox, int a1, ApricornJuice *a2);
int sub_02031D80(ApricornJuice *a0);
ApricornRecord *sub_020320E0(SaveApricornBox *box, struct PlayerProfile *profile, u32 value, enum HeapID heapId);
void sub_02032058(SaveApricornBox *saveApricornBox, u32 steps);
String *sub_020322AC(SaveApricornBox *apricornBox, u16 a1, u32 a2);
String *sub_02032308(SaveApricornBox *apricornBox, u16 a1, u32 a2);
int ApricornBox_CountApricorn(SaveApricornBox *apricornBox, u32 a1);
int ApricornBox_GiveApricorn(SaveApricornBox *apricornBox, int apricorn, u8 amount);
int ApricornBox_GetKurtQuantity(SaveApricornBox *apricornBox);
void ApricornBox_SetKurtApricorn(SaveApricornBox *apricornBox, u32 a0, u32 a1);
int ApricornBox_GetKurtBall(SaveApricornBox *apricornBox);

#endif // POKEHEARTGOLD_UNK_02031B0C_H
