#include "global.h"
struct GBABoxPokemon;
struct GBAPokemonSubstruct0;
struct GBAPokemonSubstruct1;
struct GBAPokemonSubstruct2;
struct GBAPokemonSubstruct3;
typedef struct GBAPokemonSubstruct0 {
    u16 species;
    u16 heldItem;
    u32 experience;
    u8 ppBonuses;
    u8 friendship;
    u16 padding;
} GBAPokemonSubstruct0;
typedef struct GBAPokemonSubstruct1 {
    u16 moves[4 ];
    u8 pp[4 ];
} GBAPokemonSubstruct1;
typedef struct GBAPokemonSubstruct2 {
    u8 hpEV;
    u8 attackEV;
    u8 defenseEV;
    u8 speedEV;
    u8 spAttackEV;
    u8 spDefenseEV;
    u8 cool;
    u8 beauty;
    u8 cute;
    u8 smart;
    u8 tough;
    u8 sheen;
} GBAPokemonSubstruct2;
typedef struct GBAPokemonSubstruct3 {
    u32 pokerus : 8;
    u32 metLocation : 8;
    u32 metLevel : 7;
    u32 metGame : 4;
    u32 pokeball : 4;
    u32 otGender : 1;
    u32 hpIV : 5;
    u32 attackIV : 5;
    u32 defenseIV : 5;
    u32 speedIV : 5;
    u32 spAttackIV : 5;
    u32 spDefenseIV : 5;
    u32 isEgg : 1;
    u32 abilityNum : 1;
    u32 coolRibbon : 3;
    u32 beautyRibbon : 3;
    u32 cuteRibbon : 3;
    u32 smartRibbon : 3;
    u32 toughRibbon : 3;
    u32 championRibbon : 1;
    u32 winningRibbon : 1;
    u32 victoryRibbon : 1;
    u32 artistRibbon : 1;
    u32 effortRibbon : 1;
    u32 marineRibbon : 1;
    u32 landRibbon : 1;
    u32 skyRibbon : 1;
    u32 countryRibbon : 1;
    u32 nationalRibbon : 1;
    u32 earthRibbon : 1;
    u32 worldRibbon : 1;
    u32 unusedRibbons : 4;







    u32 modernFatefulEncounter : 1;
} GBAPokemonSubstruct3;
union GBAPokemonSubstruct {
    GBAPokemonSubstruct0 type0;
    GBAPokemonSubstruct1 type1;
    GBAPokemonSubstruct2 type2;
    GBAPokemonSubstruct3 type3;
    u16 raw[((( ((sizeof(GBAPokemonSubstruct0)) > ( sizeof(GBAPokemonSubstruct1)) ? (sizeof(GBAPokemonSubstruct0)) : ( sizeof(GBAPokemonSubstruct1)))) > ( ((sizeof(GBAPokemonSubstruct2)) > ( sizeof(GBAPokemonSubstruct3)) ? (sizeof(GBAPokemonSubstruct2)) : ( sizeof(GBAPokemonSubstruct3)))) ? ( ((sizeof(GBAPokemonSubstruct0)) > ( sizeof(GBAPokemonSubstruct1)) ? (sizeof(GBAPokemonSubstruct0)) : ( sizeof(GBAPokemonSubstruct1)))) : ( ((sizeof(GBAPokemonSubstruct2)) > ( sizeof(GBAPokemonSubstruct3)) ? (sizeof(GBAPokemonSubstruct2)) : ( sizeof(GBAPokemonSubstruct3)))))) / sizeof(u16)];
};
typedef struct GBABoxPokemon {
    u32 personality;
    u32 otId;
    u8 nickname[10 ];
    u8 language;
    u8 isBadEgg : 1;
    u8 hasSpecies : 1;
    u8 isEgg : 1;
    u8 unused : 5;
    u8 otName[7];
    u8 markings;
    u16 checksum;
    u16 unknown;
    union {
        u32 raw[12];
        union GBAPokemonSubstruct substructs[4];
    } secure;
} GBABoxPokemon;
typedef struct {
    u8 currentBox;
    GBABoxPokemon boxes[14 ][(5 * 6) ];
    u8 boxNames[14 ][8 + 1];
    u8 boxWallpapers[14 ];
} GBAPokemonStorage;
typedef struct {
    u32 unk_00[1024];
    u8 saveBlock2[(3968 + 128) * 1];
    u8 saveBlock1[(3968 + 128) * 4];
    GBAPokemonStorage pokemonStorage;
} GBASaveSlot;
typedef struct {
    u8 unk_00[4084];
    u16 unk_FF4;
    u16 unk_FF6;
    u32 unk_FF8;
    u32 unk_FFC;
} UnkStruct_ov97_02235F4C;
u16 ov74_02233C08(void *param0, int *param1, u32 *param2);
extern int Unk_ov97_0223F438;
extern u32 Unk_ov97_0223F448;
#ifdef __MWERKS__
GBASaveSlot *sGBASaveSlot;  /* judge stand-in: keeps the statics' bss layout; nothing reads it, and a private definition would clash at link */
#endif
extern int Unk_ov97_0223F444;
u16 ov74_02233A88(u32 *param0, u16 param1);
void ov74_02233AB8(int param0, void *param1);
u16 ov74_02233ACC(int gbaSectorID);
void *ov74_02233B04(int gbaSectorID);

#ifdef __MWERKS__
static int sSlot;
static int sSectorID;
static GBASaveSlot *sSaveSlot;
static int sUnkC;
static u32 sCounter;
static int sState;
#else
#include "pm_agb_save.h"
/* the real record, not private copies: the DS layout of ov74_0223D33C */
#define sSlot (*(int *)&ov74_0223D33C.slot)
#define sSectorID (*(int *)&ov74_0223D33C.sectorsWritten)
#define sSaveSlot ((GBASaveSlot *)ov74_0223D33C.buffer)
#define sUnkC (*(int *)&ov74_0223D33C.firstSector)
#define sCounter (ov74_0223D33C.counter)
#define sState (*(int *)&ov74_0223D33C.writeStatus)
#endif

int ov74_02233CE4();
int ov74_02233CE4()
{
    u16 v0 = ov74_02233C08((void *)sSaveSlot->unk_00, &sSlot, &sCounter);

    if (v0 != 0x1) {
        switch (v0) {
        case 0xff:
            return 5;
        case 0x0:
            return 7;
        case 0x2:
            return 6;
        }
    }

    if ((sSlot == 0) || (sSlot == 1)) {
        int v1;
        u32 v2 = 0;
        UnkStruct_ov97_02235F4C *v3 = (UnkStruct_ov97_02235F4C *)sSaveSlot->unk_00;

        for (v1 = 0; v1 < 14; v1++) {
            ov74_02233AB8(v1 + sSlot * 14, (void *)v3);

            if (v3->unk_FF8 == 0x8012025) {
                u16 v4 = ov74_02233A88((u32 *)v3->unk_00, ov74_02233ACC(v3->unk_FF4));

                if (v3->unk_FF6 == v4) {
                    sCounter = v3->unk_FFC;
                    v2 |= (0x1 << v3->unk_FF4);

                    MI_CpuCopy32((u32 *)v3->unk_00, ov74_02233B04(v3->unk_FF4), ov74_02233ACC(v3->unk_FF4));

                    if (v3->unk_FF4 == 0) {
                        sUnkC = v1;
                    }
                }
            }
        }

        if (v2 == ((0xffffffff << 14) ^ 0xffffffff)) {
            return 0;
        } else {
            return 4;
        }
    }

    return 6;
}
