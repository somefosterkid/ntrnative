// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov168/btlv_gauge.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The battle view's gauges. The name is the ROM's string, from GFL_HeapAllocate's asserts. The names are ours; swan has
// none for this file

#include "battle/btlv_gauge.h"
#include "types.h"
#include "battle/btl_main.h"
#include "battle/btl_pokeparam.h"
#include "battle/btlv_effect.h"
#include "battle/btlv_input.h"
#include "constants/battle.h"
#include "constants/pokemon.h"
#include "constants/sound.h"
#include "constants/species.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bmp.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/net.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/gx.h"
#include "nitro/math.h"
#include "pml/personal.h"
#include "pml/poke_party.h"
#include "save/pokedex.h"
#include "system/app_menu_common.h"
#include "system/gf_font.h"
#include "system/hp_gauge.h"
#include "system/palanm.h"
#include "system/printsys.h"
#include "system/str_tool.h"

// A gauge's animated value before its first step
#define GAUGE_ANIM_START ((s32)0x80000000)

// Tiles of the gauges' characters, as byte offsets
#define GAUGE_TILE_DIGITS 0x520
#define GAUGE_TILE_BLANK 0x660

// Archive 11's palettes of the gauges and of the rotation's back mons
#ifdef BLACK2
#define GAUGE_PALETTE_FILE 0x1af
#define GAUGE_PALETTE_FILE_BACK 0x1b1
#define GAUGE_CHAR_FILE 0x1b2
#else
#define GAUGE_PALETTE_FILE 0xa2
#define GAUGE_PALETTE_FILE_BACK 0xa3
#define GAUGE_CHAR_FILE 0xa4
#endif

// The status icon's sequence for no status
#define GAUGE_STATUS_NONE 8

// What a gauge shows of its mon, cleared when a mon is set, 0x40 bytes
typedef struct {
    u32 type;       // 0x00  the battle's style, BTL_STYLE_*
    u32 side;       // 0x04  1 for the opponent's
    s32 hp;         // 0x08
    s32 maxHp;      // 0x0c
    s32 hpAnim;     // 0x10  the HP shown, in 1/256 when maxHp is below the bar's 48 pixels
    s32 exp;        // 0x14  since the level started
    s32 expMax;     // 0x18  the level's EXP
    s32 nextExpMax; // 0x1c  the next level's, after a level up
    s32 expDelta;   // 0x20  the EXP to take away, so negated
    s32 expAnim;    // 0x24
    s32 hpDelta;    // 0x28  the HP to take away
    u8 level;       // 0x2c
    u8 sex;         // 0x2d  2 for no sex mark
    u8 unk2e;
    u8 caught; // 0x2f
    // 0x30
    u32 hpMoving : 1;     // bit 0
    u32 expMoving : 1;    // bit 1
    u32 levelUp : 1;      // bit 2
    u32 shown : 1;        // bit 3
    u32 levelUpSeq : 4;   // bits 4-7
    u32 expSeFrames : 8;  // bits 8-15: frames since the EXP sound started
    u32 slideFrames : 4;  // bits 16-19
    u32 shaking : 1;      // bit 20
    u32 blinkCount : 5;   // bits 21-25: the lost HP's blinks left
    u32 blinking : 1;     // bit 26
    u32 changed : 1;      // bit 27
    u32 expSe : 1;        // bit 28: the EXP sound plays
    u32 visible : 1;      // bit 29
    u32 statusHidden : 1; // bit 30
    u32 unk30_31 : 1;
    u32 hpSpeed;    // 0x34  0 to change at once
    s32 hpDots;     // 0x38  the HP bar's pixels
    TCB *slideTask; // 0x3c
} BtlvGaugeMon;

// A battler's gauge, 0x80 bytes
typedef struct {
    ClActor *nameActor;   // 0x00  the box with the name, sex, level and caught mark
    ClActor *hpNumActor;  // 0x04
    ClActor *hpBarActor;  // 0x08
    ClActor *damageActor; // 0x0c  the HP the bar lost, which blinks
    ClActor *expActor;    // 0x10  the player's side in single and double battles only
    ClActor *statusActor; // 0x14
    u32 nameChars;        // 0x18
    u32 nameCellAnims;    // 0x1c
    u32 hpNumChars;       // 0x20
    u32 hpNumCellAnims;   // 0x24
    u32 hpBarChars;       // 0x28
    u32 hpBarCellAnims;   // 0x2c
    u32 damageChars;      // 0x30
    u32 damageCellAnims;  // 0x34
    u32 expChars;         // 0x38
    u32 expCellAnims;     // 0x3c
    BtlvGaugeMon mon;     // 0x40
} BtlvGaugeEntry;

// The gauges' work, 0x44c bytes
struct BtlvGauge {
    ClActUnit *unit;           // 0x000
    ArcTool *arc;              // 0x004  archive 11
    Font *font;                // 0x008
    TCB *flashTask;            // 0x00c
    void *charFile;            // 0x010
    u8 *chars;                 // 0x014  the tiles the gauges are drawn from
    u32 palettes[6];           // 0x018  by view position, the positions from 2 on taking 0 and 1's
    u32 statusChars;           // 0x030
    u32 statusCellAnims;       // 0x034
    u32 statusPalette;         // 0x038
    u32 statusPaletteDark;     // 0x03c  for the rotation's back mons
    BtlvGaugeEntry entries[8]; // 0x040  by view position
    // 0x440
    u32 unk440_0 : 1;
    u32 bgmFading : 1;     // bit 1
    u32 pinch : 1;         // bit 2: the pinch music plays
    u32 bgmReplayed : 1;   // bit 3
    u32 showNumbers : 1;   // bit 4: the player's HP numbers instead of the bar, in triple and rotation battles
    u32 shakeAngle : 16;   // bits 5-20
    u32 noPinchBgm : 1;    // bit 21
    u32 toggleRequest : 1; // bit 22
    u32 loaded : 1;        // bit 23
    u32 mode : 2;          // bits 24-25
    u32 unk440_26 : 6;
    u32 bgm;       // 0x444  the battle's music
    HeapID heapId; // 0x448
}; // 0x44c

// The red flash of the gauges losing HP
typedef struct {
    BtlvGauge *gauge;
    u32 seq;
    u32 paletteMask;
    u32 count;
} BtlvGaugeFlashTask;

static void func_ov094_02207430(BtlvGauge *gauge, u32 type, int index);
static void func_ov094_0220778c(BtlvGauge *gauge, int index);
static void func_ov094_0220785c(BtlvGauge *gauge, u32 type, int index, PartyPkm *pkm);
static void func_ov094_0220793c(BtlvGaugeEntry *entry);
static void func_ov094_02207da0(BtlvGauge *gauge, BtlvGaugeEntry *entry, s32 hp);
static void func_ov094_02207e1c(BtlvGauge *gauge, int index);
static void func_ov094_02207ea8(BtlvGaugeEntry *entry, s32 exp);
static void func_ov094_02207ef0(BtlvGauge *gauge, int index);
static s32 func_ov094_02207f6c(s32 max, s32 cur, s32 delta, s32 *anim, u8 size, u16 speed);
static u8 func_ov094_02208034(s32 max, s32 cur, s32 delta, s32 *anim, u8 *tiles, u8 size);
static s32 func_ov094_022080b0(s32 value, s32 delta, s32 max, u8 size);
static void func_ov094_022080f4(BtlvGauge *gauge, BtlvGaugeEntry *entry, PartyPkm *pkm);
static void func_ov094_02208268(BtlvGauge *gauge, BtlvGaugeEntry *entry);
static void func_ov094_022082fc(BtlvGauge *gauge, int index, int bar);
static void func_ov094_02208420(BtlvGauge *gauge, BtlvGaugeEntry *entry, s32 hp);
static void func_ov094_02208550(BtlvGauge *gauge, BtlvGaugeEntry *entry);
static void func_ov094_02208650(BtlvGauge *gauge, BtlvGaugeEntry *entry);
static void func_ov094_0220868c(BtlvGauge *gauge, BtlvGaugeEntry *entry, s32 dots);
static void func_ov094_022086f8(BtlvGauge *gauge, int index);
static void func_ov094_022087c4(BtlvGauge *gauge, int index);
static void func_ov094_022089d4(BtlvGauge *gauge);
static void func_ov094_02208b10(TCB *tcb, void *data);
static void func_ov094_02208bfc(TCB *tcb, void *data);
static void func_ov094_02208d24(BOOL paused);
static u32 func_ov094_02208d38(u32 value, u32 max, u32 width);
static void func_ov094_02208d50(BtlvGauge *gauge, int index, BOOL visible);

void func_ov094_02206b58(BtlvInput *input) {
    if (input->unk54 != 3 || input->screen == 6) {
        GFL_SndSEPlay(SEQ_SE_CANCEL2);
    }
}

void func_ov094_02206b74(BtlvInput *input) {
    if (input->unk54 != 3 || input->screen == 6) {
        GFL_SndSEPlay(SEQ_SE_ROTATION_S);
    }
}

void func_ov094_02206b90(BtlvInput *input) {
    if (input->unk54 != 3 || input->screen == 6) {
        GFL_SndSEPlay(SEQ_SE_DECIDE2);
    }
}

void func_ov094_02206bac(BtlvInput *input) {
    if (input->unk54 != 3 || input->screen == 6) {
        GFL_SndSEPlay(SEQ_SE_BEEP);
    }
}

BtlvGauge *func_ov094_02206bc8(Font *font, HeapID heapId) {
    BtlvGauge *gauge;
    BtlvGaugeFlashTask *flash;
    ArcTool *arc;
    NNSG2dCharacterData *character;
    int i;
    int start;
    int end;
    u32 type;

    gauge = GFL_HeapAllocate(heapId, sizeof(BtlvGauge), TRUE, "btlv_gauge.c", 0x14b);
    gauge->heapId = heapId;
    gauge->arc = GFL_ArcSysCreateFileHandle(11, gauge->heapId);
    gauge->unit = func_0204b100(40, 0, gauge->heapId);
    for (i = 0; i < 6; i++) {
        gauge->palettes[i] = func_0204abf0(gauge->arc, GAUGE_PALETTE_FILE, CLACT_VRAM_MAIN, i * 0x20, gauge->heapId);
        func_020216d4(func_ov094_021f86e0(), 2, func_0204aef4(gauge->palettes[i], FALSE) / 2, 0x20);
    }
    arc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), HEAPID_TAIL(gauge->heapId));
    gauge->statusChars = func_0204a6c8(arc, func_020276b8(), FALSE, CLACT_VRAM_MAIN, gauge->heapId);
    gauge->statusCellAnims = func_0204af28(arc, func_020276bc(1), func_020276c0(1), gauge->heapId);
    gauge->statusPalette = func_0204abf0(arc, func_020276b4(), CLACT_VRAM_MAIN, 0x100, gauge->heapId);
    gauge->statusPaletteDark = func_0204abf0(arc, func_020276b4(), CLACT_VRAM_MAIN, 0x120, gauge->heapId);
    func_020216d4(func_ov094_021f86e0(), 2, func_0204aef4(gauge->statusPalette, FALSE) / 2, 0x20);
    func_020216d4(func_ov094_021f86e0(), 2, func_0204aef4(gauge->statusPaletteDark, FALSE) / 2, 0x20);
    func_02022004(func_ov094_021f86e0(), 2, 1 << 9, 8, 0);
    GFL_ArcToolFree(arc);
    gauge->charFile = GFL_G2DIOReadOBJNCGRArc(gauge->arc, GAUGE_CHAR_FILE, FALSE, &character, gauge->heapId);
    gauge->chars = character->pRawData;
    gauge->font = font;
    gauge->bgm = GFL_SndBGMGetID();
    flash = GFL_HeapAllocate(heapId, sizeof(BtlvGaugeFlashTask), TRUE, "btlv_gauge.c", 0x185);
    flash->gauge = gauge;
    gauge->flashTask = GFL_TCBMgrAddTask(func_ov094_021f86d4(), func_ov094_02208bfc, flash, 0);
    switch (func_ov094_021f8710()) {
    case BTL_STYLE_SINGLE:
        start = 0;
        end = 1;
        type = BTL_STYLE_SINGLE;
        break;
    case BTL_STYLE_DOUBLE:
        start = 2;
        end = 5;
        type = BTL_STYLE_DOUBLE;
        break;
    case BTL_STYLE_TRIPLE:
    default:
        start = 2;
        end = 7;
        type = BTL_STYLE_TRIPLE;
        break;
    case BTL_STYLE_ROTATION:
        start = 2;
        end = 7;
        type = BTL_STYLE_ROTATION;
        break;
    }
    for (i = start; i <= end; i++) {
        ClActorPos pos = { 128, 0 };

        if (i & 1) {
            pos.x *= -1;
        }
        func_ov094_02207430(gauge, type, i);
        gauge->entries[i].mon.type = type;
        func_ov094_022079c0(gauge, i, &pos);
    }
    return gauge;
}

// pokebw2's BtlvGauge_Delete
void func_ov094_02206e28(BtlvGauge *gauge) {
    BtlvGaugePos pos;
    int i;

    for (pos = 0; pos < BTLV_GAUGE_POS_MAX; pos++) {
        func_ov094_02207960(gauge, pos);
        func_ov094_0220778c(gauge, pos);
    }
    if (gauge->pinch) {
        GFL_SndBGMPop();
        func_ov094_02208d24(FALSE);
    }
    for (i = 0; i < 6; i++) {
        func_0204ada4(gauge->palettes[i]);
    }
    func_0204a8d4(gauge->statusChars);
    func_0204afd8(gauge->statusCellAnims);
    func_0204ada4(gauge->statusPalette);
    func_0204ada4(gauge->statusPaletteDark);
    func_0204b1cc(gauge->unit);
    GFL_HeapFree(gauge->charFile);
    GFL_ArcToolFree(gauge->arc);
    GFL_HeapFree(GFL_TCBGetData(gauge->flashTask));
    GFL_TCBRemove(gauge->flashTask);
    GFL_HeapFree(gauge);
}

// pokebw2's BtlvGauge_Update
void func_ov094_02206eb4(BtlvGauge *gauge) {
    int i;

    GCTX_HIDGetPressedKeys();
    if (gauge->toggleRequest) {
        u32 rule = func_ov094_021f8710();

        gauge->toggleRequest = FALSE;
        if (rule != BTL_STYLE_SINGLE && rule != BTL_STYLE_DOUBLE) {
            BOOL moving = FALSE;

            for (i = 0; i < 8; i++) {
                if (gauge->entries[i].mon.hpMoving) {
                    moving = TRUE;
                    break;
                }
            }
            if (!moving) {
                gauge->showNumbers ^= 1;
                for (i = 0; i < 8; i++) {
                    if (!(i & 1) && gauge->entries[i].mon.visible == TRUE && gauge->entries[i].mon.shown) {
                        func_ov094_02208d50(gauge, i, TRUE);
                        func_ov094_02207f6c(gauge->entries[i].mon.maxHp, gauge->entries[i].mon.hp, 0,
                                            &gauge->entries[i].mon.hpAnim, 6, 1);
                        func_ov094_022082fc(gauge, i, 0);
                    }
                }
            }
        }
    }
    for (i = 0; i < 8; i++) {
        if (gauge->entries[i].mon.hpMoving) {
            func_ov094_02207e1c(gauge, i);
        }
        if (gauge->entries[i].mon.expMoving) {
            if (gauge->entries[i].mon.expSeFrames == 0) {
                GFL_SndSEPlay(SEQ_SE_EXP);
            }
            func_ov094_02207ef0(gauge, i);
        }
        if (gauge->entries[i].mon.expSe) {
            if (gauge->entries[i].mon.expSeFrames < 16) {
                gauge->entries[i].mon.expSeFrames++;
            }
            if (!gauge->entries[i].mon.expMoving && gauge->entries[i].mon.expSeFrames == 16) {
                GFL_SndStop();
                gauge->entries[i].mon.expSeFrames++;
                gauge->entries[i].mon.expSe = FALSE;
            }
        }
        if (gauge->entries[i].mon.levelUp) {
            func_ov094_022086f8(gauge, i);
        }
        if (gauge->entries[i].mon.shaking) {
            func_ov094_022087c4(gauge, i);
        }
        if (gauge->entries[i].damageActor != NULL && gauge->entries[i].mon.blinking) {
            if (gauge->entries[i].mon.blinkCount == 0 || !gauge->entries[i].mon.shown ||
                !gauge->entries[i].mon.visible) {
                func_0204b3dc(gauge->entries[i].damageActor, FALSE);
                gauge->entries[i].mon.blinking = FALSE;
            } else {
                func_0204b3dc(gauge->entries[i].damageActor, gauge->entries[i].mon.blinkCount & 1);
                gauge->entries[i].mon.blinkCount--;
            }
            if (gauge->showNumbers && !(i & 1)) {
                func_0204b3dc(gauge->entries[i].damageActor, FALSE);
            }
        }
    }
    if (!gauge->noPinchBgm) {
        func_ov094_022089d4(gauge);
    }
}

void func_ov094_022070c0(BtlvGauge *gauge, BtlMainModule *mainModule, BattleMon *mon, u32 type, int index) {
    u32 species;
    u32 status;

    MIi_CpuClear16(0, &gauge->entries[index].mon, sizeof(BtlvGaugeMon));
    gauge->entries[index].mon.hp = GetBattleMonStat(mon, BATTLEMON_HP);
    gauge->entries[index].mon.maxHp = GetBattleMonStat(mon, 14);
    gauge->entries[index].mon.hpAnim = GAUGE_ANIM_START;
    gauge->entries[index].mon.hpDelta = 0;
    gauge->entries[index].mon.level = GetBattleMonStat(mon, BATTLEMON_LEVEL);
    gauge->entries[index].mon.type = type;
    gauge->entries[index].mon.side = index & 1;
    gauge->entries[index].mon.changed = TRUE;
    if (gauge->entries[index].mon.level < 100) {
        u16 baseSpecies = GetBattleMonSpecies(mon);
        u32 form = GetBattleMonStat(mon, BATTLEMON_FORM);
        u32 exp = PML_UtilGetPkmLvExp(baseSpecies, form, GetBattleMonStat(mon, BATTLEMON_LEVEL));

        gauge->entries[index].mon.exp = GetBattleMonStat(mon, 20) - exp;
        baseSpecies = GetBattleMonSpecies(mon);
        form = GetBattleMonStat(mon, BATTLEMON_FORM);
        gauge->entries[index].mon.expMax =
            PML_UtilGetPkmLvExp(baseSpecies, form, GetBattleMonStat(mon, BATTLEMON_LEVEL) + 1) - exp;
    } else {
        gauge->entries[index].mon.exp = 0;
        gauge->entries[index].mon.expMax = 0;
    }
    gauge->entries[index].mon.expAnim = GAUGE_ANIM_START;
    gauge->entries[index].mon.expDelta = 0;
    gauge->entries[index].mon.unk2e = 0;
    gauge->entries[index].mon.caught = func_ov093_021b8724(mainModule, mon);
    species = GetBattleMonSpecies(mon);
    if (species != SPECIES_NIDORAN_M && species != SPECIES_NIDORAN_F) {
        gauge->entries[index].mon.sex = PokeParty_GetSex(func_ov093_021d5864(mon));
    } else {
        gauge->entries[index].mon.sex = 2;
    }
    if (Condition_IsBadlyPoisoned(GetConditionContinuationParam(mon, 5)) == TRUE && GetBattleMonStatus(mon) == 5) {
        status = 6;
    } else {
        status = GetBattleMonStatus(mon);
    }
    func_ov094_02208814(gauge, status, index);
    func_ov094_022079c0(gauge, index, NULL);
    func_ov094_02208d50(gauge, index, TRUE);
    func_ov094_0220793c(&gauge->entries[index]);
    func_ov094_0220785c(gauge, type, index, func_ov093_021d5864(mon));
}

void func_ov094_02207288(BtlvGauge *gauge, PokeDexSave *pokedex, PartyPkm *pkm, u32 type, int index) {
    u16 species;

    MIi_CpuClear16(0, &gauge->entries[index].mon, sizeof(BtlvGaugeMon));
    gauge->entries[index].mon.hp = PokeParty_GetParam(pkm, PKM_PARAM_HP, NULL);
    gauge->entries[index].mon.maxHp = PokeParty_GetParam(pkm, PKM_PARAM_MAX_HP, NULL);
    gauge->entries[index].mon.hpAnim = GAUGE_ANIM_START;
    gauge->entries[index].mon.hpDelta = 0;
    gauge->entries[index].mon.level = PokeParty_GetParam(pkm, PKM_PARAM_LEVEL, NULL);
    gauge->entries[index].mon.type = type;
    gauge->entries[index].mon.side = index & 1;
    gauge->entries[index].mon.changed = TRUE;
    if (gauge->entries[index].mon.level < 100) {
        u32 baseSpecies = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
        u32 form = PokeParty_GetParam(pkm, PKM_PARAM_FORM, NULL);
        u32 exp = PML_UtilGetPkmLvExp(baseSpecies, form, gauge->entries[index].mon.level);

        gauge->entries[index].mon.exp = PokeParty_GetParam(pkm, PKM_PARAM_EXP, NULL) - exp;
        gauge->entries[index].mon.expMax =
            PML_UtilGetPkmLvExp(baseSpecies, form, gauge->entries[index].mon.level + 1) - exp;
    } else {
        gauge->entries[index].mon.exp = 0;
        gauge->entries[index].mon.expMax = 0;
    }
    gauge->entries[index].mon.expAnim = GAUGE_ANIM_START;
    gauge->entries[index].mon.expDelta = 0;
    gauge->entries[index].mon.unk2e = 0;
    species = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
    gauge->entries[index].mon.caught = PokeDex_IsCaught(pokedex, species);
    if (species != SPECIES_NIDORAN_M && species != SPECIES_NIDORAN_F) {
        gauge->entries[index].mon.sex = PokeParty_GetSex(pkm);
    } else {
        gauge->entries[index].mon.sex = 2;
    }
    func_ov094_02208814(gauge, GetStatusCond(pkm), index);
    func_ov094_022079c0(gauge, index, NULL);
    func_ov094_02208d50(gauge, index, TRUE);
    func_ov094_0220793c(&gauge->entries[index]);
    func_ov094_0220785c(gauge, type, index, pkm);
}

static void func_ov094_02207430(BtlvGauge *gauge, u32 type, int index) {
    u32 charsFile;
    u32 cellFile;
    u32 animFile;

    switch (type) {
    case BTL_STYLE_SINGLE:
    case BTL_STYLE_DOUBLE:
    default:
        charsFile = index & 1 ? 0xa5 : 0xa8;
        cellFile = index & 1 ? 0xa6 : 0xa9;
        animFile = index & 1 ? 0xa7 : 0xaa;
        break;
    case BTL_STYLE_TRIPLE:
    case BTL_STYLE_ROTATION:
        charsFile = index & 1 ? 0xab : 0xae;
        cellFile = index & 1 ? 0xac : 0xaf;
        animFile = index & 1 ? 0xad : 0xb0;
        break;
    case 4:
        break;
    }
    if (type == BTL_STYLE_ROTATION) {
        u32 paletteFile;
        int slot = index;

        if (index >= 2) {
            slot = index - 2;
        }
        paletteFile = index < 4 ? GAUGE_PALETTE_FILE : GAUGE_PALETTE_FILE_BACK;
        func_0204ada4(gauge->palettes[slot]);
        gauge->palettes[slot] = func_0204ac18(gauge->arc, paletteFile, CLACT_VRAM_MAIN, slot * 0x20, 0, 1, gauge->heapId);
        func_020216d4(func_ov094_021f86e0(), 2, func_0204aef4(gauge->palettes[slot], FALSE) / 2, 0x20);
    }
    gauge->entries[index].nameChars = func_0204a6c8(gauge->arc, charsFile, FALSE, CLACT_VRAM_MAIN, gauge->heapId);
    gauge->entries[index].nameCellAnims = func_0204af28(gauge->arc, cellFile, animFile, gauge->heapId);
    gauge->entries[index].hpNumChars = func_0204a6c8(gauge->arc, 0xba, FALSE, CLACT_VRAM_MAIN, gauge->heapId);
    gauge->entries[index].hpNumCellAnims = func_0204af28(gauge->arc, 0xbb, 0xbc, gauge->heapId);
    gauge->entries[index].hpBarChars = func_0204a6c8(gauge->arc, 0xb1, FALSE, CLACT_VRAM_MAIN, gauge->heapId);
    gauge->entries[index].hpBarCellAnims = func_0204af28(gauge->arc, 0xb2, 0xb3, gauge->heapId);
    gauge->entries[index].damageChars = func_0204a6c8(gauge->arc, 0xb4, FALSE, CLACT_VRAM_MAIN, gauge->heapId);
    gauge->entries[index].damageCellAnims = func_0204af28(gauge->arc, 0xb5, 0xb3, gauge->heapId);
    if (!(index & 1)) {
        gauge->entries[index].expChars = func_0204a6c8(gauge->arc, 0xb7, FALSE, CLACT_VRAM_MAIN, gauge->heapId);
        gauge->entries[index].expCellAnims = func_0204af28(gauge->arc, 0xb8, 0xb9, gauge->heapId);
    }
    {
        ClActorSetup setup = { 0, 0, 0, 0, 1 };
        int slot = index;
        int priority;

        if (index >= 2) {
            slot = index - 2;
        }
        priority = (7 - index) * 3;
        setup.priority = priority + 2;
        gauge->entries[index].nameActor =
            func_0204b294(gauge->unit, gauge->entries[index].nameChars, gauge->palettes[slot],
                          gauge->entries[index].nameCellAnims, &setup, CLACT_SURFACE_MAIN, gauge->heapId);
        setup.priority = priority + 1;
        gauge->entries[index].hpNumActor =
            func_0204b294(gauge->unit, gauge->entries[index].hpNumChars, gauge->palettes[slot],
                          gauge->entries[index].hpNumCellAnims, &setup, CLACT_SURFACE_MAIN, gauge->heapId);
        gauge->entries[index].hpBarActor =
            func_0204b294(gauge->unit, gauge->entries[index].hpBarChars, gauge->palettes[slot],
                          gauge->entries[index].hpBarCellAnims, &setup, CLACT_SURFACE_MAIN, gauge->heapId);
        if (func_ov094_021f8710() == BTL_STYLE_ROTATION && index >= 4) {
            gauge->entries[index].statusActor = func_0204b294(gauge->unit, gauge->statusChars, gauge->statusPaletteDark,
                                                              gauge->statusCellAnims, &setup, CLACT_SURFACE_MAIN,
                                                              gauge->heapId);
        } else {
            gauge->entries[index].statusActor = func_0204b294(gauge->unit, gauge->statusChars, gauge->statusPalette,
                                                              gauge->statusCellAnims, &setup, CLACT_SURFACE_MAIN,
                                                              gauge->heapId);
        }
        func_0204b92c(gauge->entries[index].statusActor, TRUE);
        setup.priority = priority;
        gauge->entries[index].damageActor =
            func_0204b294(gauge->unit, gauge->entries[index].damageChars, gauge->palettes[slot],
                          gauge->entries[index].damageCellAnims, &setup, CLACT_SURFACE_MAIN, gauge->heapId);
        func_0204b3dc(gauge->entries[index].damageActor, FALSE);
        if (!(index & 1) && type != BTL_STYLE_TRIPLE && type != BTL_STYLE_ROTATION) {
            gauge->entries[index].expActor =
                func_0204b294(gauge->unit, gauge->entries[index].expChars, gauge->palettes[slot],
                              gauge->entries[index].expCellAnims, &setup, CLACT_SURFACE_MAIN, gauge->heapId);
        }
    }
}

// pokebw2's BtlvGauge_DeleteActors
static void func_ov094_0220778c(BtlvGauge *gauge, int index) {
    if (gauge->entries[index].nameActor != NULL) {
        func_0204a8d4(gauge->entries[index].nameChars);
        func_0204a8d4(gauge->entries[index].hpNumChars);
        func_0204a8d4(gauge->entries[index].hpBarChars);
        func_0204a8d4(gauge->entries[index].damageChars);
        func_0204afd8(gauge->entries[index].nameCellAnims);
        func_0204afd8(gauge->entries[index].hpNumCellAnims);
        func_0204afd8(gauge->entries[index].hpBarCellAnims);
        func_0204afd8(gauge->entries[index].damageCellAnims);
        func_0204b3b4(gauge->entries[index].nameActor);
        func_0204b3b4(gauge->entries[index].hpNumActor);
        func_0204b3b4(gauge->entries[index].hpBarActor);
        func_0204b3b4(gauge->entries[index].damageActor);
        func_0204b3b4(gauge->entries[index].statusActor);
        gauge->entries[index].nameActor = NULL;
        gauge->entries[index].hpNumActor = NULL;
        gauge->entries[index].hpBarActor = NULL;
        gauge->entries[index].damageActor = NULL;
        gauge->entries[index].statusActor = NULL;
        if (!(index & 1)) {
            func_0204a8d4(gauge->entries[index].expChars);
            func_0204afd8(gauge->entries[index].expCellAnims);
        }
        if (gauge->entries[index].expActor != NULL) {
            func_0204b3b4(gauge->entries[index].expActor);
            gauge->entries[index].expActor = NULL;
        }
    }
}

static void func_ov094_0220785c(BtlvGauge *gauge, u32 type, int index, PartyPkm *pkm) {
    func_ov094_02207f6c(gauge->entries[index].mon.maxHp, gauge->entries[index].mon.hp, 0,
                        &gauge->entries[index].mon.hpAnim, 6, 1);
    func_ov094_022082fc(gauge, index, 0);
    if (gauge->entries[index].expActor != NULL) {
        func_ov094_02207f6c(gauge->entries[index].mon.expMax, gauge->entries[index].mon.exp, 0,
                            &gauge->entries[index].mon.expAnim, 10, 1);
        func_ov094_022082fc(gauge, index, 1);
    }
    func_ov094_022080f4(gauge, &gauge->entries[index], pkm);
    func_ov094_02208268(gauge, &gauge->entries[index]);
    func_ov094_02208420(gauge, &gauge->entries[index], gauge->entries[index].mon.hp);
    func_ov094_02208550(gauge, &gauge->entries[index]);
    if (gauge->entries[index].mon.side && !func_ov094_021f8720()) {
        func_ov094_02208650(gauge, &gauge->entries[index]);
    }
    gauge->entries[index].mon.shown = TRUE;
    gauge->entries[index].mon.slideFrames = 2;
    gauge->entries[index].mon.slideTask =
        GFL_TCBMgrAddTask(func_ov094_021f86d4(), func_ov094_02208b10, &gauge->entries[index], 0);
}

// pokebw2's BtlvGauge_CalcHpDots
static void func_ov094_0220793c(BtlvGaugeEntry *entry) {
    if (entry->mon.maxHp < 48) {
        entry->mon.hpDots = func_ov094_02208d38(entry->mon.hp << 8, entry->mon.maxHp, 48) >> 8;
    } else {
        entry->mon.hpDots = func_ov094_02208d38(entry->mon.hp, entry->mon.maxHp, 48);
    }
}

// pokebw2's BtlvGauge_Hide
void func_ov094_02207960(BtlvGauge *gauge, int index) {
    if (gauge->entries[index].mon.shown) {
        ClActorPos pos = { 128, 0 };

        if (index & 1) {
            pos.x *= -1;
        }
        func_ov094_022079c0(gauge, index, &pos);
        gauge->entries[index].mon.shown = FALSE;
    }
    if (gauge->entries[index].mon.slideTask != NULL) {
        GFL_TCBRemove(gauge->entries[index].mon.slideTask);
        gauge->entries[index].mon.slideTask = NULL;
    }
}

// pokebw2's BtlvGauge_SetPos
void func_ov094_022079c0(BtlvGauge *gauge, int index, const ClActorPos *offset) {
    ClActorPos pos;
    ClActorPos hpBarOffsets[2] = { { 8, 7 }, { 0, 7 } };
    ClActorPos statusOffsets[2] = { { -30, 8 }, { -38, 8 } };
    ClActorPos basePositions[6] = { { 216, 120 }, { 44, 40 }, { 216, 98 }, { 48, 28 }, { 220, 128 }, { 44, 54 } };
    ClActorPos triplePositions[8] = {
        { 216, 120 }, { 44, 40 }, { 216, 95 }, { 48, 19 }, { 220, 116 }, { 44, 40 }, { 224, 137 }, { 40, 61 },
    };
    ClActorPos rotationPositions[8] = {
        { 216, 120 }, { 44, 40 }, { 220, 116 }, { 44, 40 }, { 216, 95 }, { 48, 19 }, { 224, 137 }, { 40, 61 },
    };
    int x;
    int y;

    if (gauge->entries[index].mon.type == BTL_STYLE_TRIPLE) {
        x = triplePositions[index].x;
        y = triplePositions[index].y;
    } else if (gauge->entries[index].mon.type == BTL_STYLE_ROTATION) {
        x = rotationPositions[index].x;
        y = rotationPositions[index].y;
    } else {
        x = basePositions[index].x;
        y = basePositions[index].y;
    }
    if (offset != NULL) {
        x += offset->x;
        y += offset->y;
    }
    pos.x = x;
    pos.y = y;
    func_0204b404(gauge->entries[index].nameActor, &pos, CLACT_SURFACE_MAIN);
    if (!(index & 1) && gauge->entries[index].mon.type == BTL_STYLE_SINGLE) {
        pos.x = x + 16;
        pos.y = y + 13;
        func_0204b404(gauge->entries[index].hpNumActor, &pos, CLACT_SURFACE_MAIN);
    } else if (!(index & 1) && gauge->entries[index].mon.type != BTL_STYLE_DOUBLE) {
        pos.x = x + 16;
        pos.y = y + 7;
        func_0204b404(gauge->entries[index].hpNumActor, &pos, CLACT_SURFACE_MAIN);
    }
    pos.x = x + hpBarOffsets[index & 1].x;
    pos.y = y + hpBarOffsets[index & 1].y;
    func_0204b404(gauge->entries[index].hpBarActor, &pos, CLACT_SURFACE_MAIN);
    pos.x = x + statusOffsets[index & 1].x;
    pos.y = y + statusOffsets[index & 1].y;
    func_0204b404(gauge->entries[index].statusActor, &pos, CLACT_SURFACE_MAIN);
    if (gauge->entries[index].expActor != NULL) {
        pos.x = x + 8;
        pos.y = y + 21;
        func_0204b404(gauge->entries[index].expActor, &pos, CLACT_SURFACE_MAIN);
    }
}

// pokebw2's BtlvGauge_StartHpChange
void func_ov094_02207b4c(BtlvGauge *gauge, int index, s32 hp) {
    func_ov094_02207da0(gauge, &gauge->entries[index], hp);
    gauge->entries[index].mon.hpSpeed = 1;
}

// pokebw2's BtlvGauge_SetHpAtOnce
void func_ov094_02207b68(BtlvGauge *gauge, int index, s32 hp) {
    func_ov094_02207da0(gauge, &gauge->entries[index], hp);
    gauge->entries[index].mon.hpSpeed = 0;
}

// pokebw2's BtlvGauge_StartExpGain
void func_ov094_02207b84(BtlvGauge *gauge, int index, s32 exp) {
    gauge->entries[index].mon.expSeFrames = 0;
    gauge->entries[index].mon.expSe = TRUE;
    func_ov094_02207ea8(&gauge->entries[index], exp);
}

// pokebw2's BtlvGauge_StartLevelUp
void func_ov094_02207bac(BtlvGauge *gauge, BattleMon *mon, int index) {
    s32 exp = gauge->entries[index].mon.expMax - gauge->entries[index].mon.exp;

    gauge->entries[index].mon.hp = GetBattleMonStat(mon, BATTLEMON_HP);
    gauge->entries[index].mon.maxHp = GetBattleMonStat(mon, 14);
    gauge->entries[index].mon.hpAnim = GAUGE_ANIM_START;
    gauge->entries[index].mon.hpDelta = 0;
    gauge->entries[index].mon.level = GetBattleMonStat(mon, BATTLEMON_LEVEL);
    gauge->entries[index].mon.nextExpMax =
        PML_UtilGetPkmLvExp(GetBattleMonSpecies(mon), GetBattleMonStat(mon, BATTLEMON_FORM),
                            GetBattleMonStat(mon, BATTLEMON_LEVEL) + 1) -
        PML_UtilGetPkmLvExp(GetBattleMonSpecies(mon), GetBattleMonStat(mon, BATTLEMON_FORM),
                            GetBattleMonStat(mon, BATTLEMON_LEVEL));
    gauge->entries[index].mon.levelUpSeq = 0;
    gauge->entries[index].mon.levelUp = TRUE;
    gauge->entries[index].mon.expSeFrames = 0;
    gauge->entries[index].mon.expSe = TRUE;
    func_ov094_02207ea8(&gauge->entries[index], exp);
}

// pokebw2's BtlvGauge_IsBusy
BOOL func_ov094_02207c94(BtlvGauge *gauge) {
    int i;

    for (i = 0; i < 8; i++) {
        if (gauge->entries[i].mon.hpMoving || gauge->entries[i].mon.expMoving || gauge->entries[i].mon.levelUp ||
            gauge->entries[i].mon.expSe) {
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's BtlvGauge_SetSideVisible
void func_ov094_02207cc4(BtlvGauge *gauge, BOOL visible, u32 side) {
    BtlvGaugePos i;

    for (i = 0; i < BTLV_GAUGE_POS_MAX; i++) {
        if ((side == 2 || side == gauge->entries[i].mon.side) && gauge->entries[i].mon.shown) {
            func_ov094_02207d04(gauge, visible, i);
        }
    }
}

void func_ov094_02207d04(BtlvGauge *gauge, BOOL visible, int index) {
    gauge->entries[index].mon.visible = visible;
    func_0204b3dc(gauge->entries[index].nameActor, visible);
    func_ov094_02208d50(gauge, index, visible);
    if (gauge->entries[index].expActor != NULL) {
        func_0204b3dc(gauge->entries[index].expActor, visible);
    }
    if (gauge->entries[index].statusActor != NULL && func_0204b898(gauge->entries[index].statusActor) != 0) {
        func_0204b3dc(gauge->entries[index].statusActor, visible);
    }
}

// pokebw2's BtlvGauge_IsShown
BOOL func_ov094_02207d60(BtlvGauge *gauge, int index) {
    if (gauge->entries[index].mon.shown) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's BtlvGauge_CheckChanged
BOOL func_ov094_02207d78(BtlvGauge *gauge, int index) {
    BOOL changed = FALSE;

    if (gauge->entries[index].nameActor != NULL) {
        changed = gauge->entries[index].mon.changed;
        gauge->entries[index].mon.changed = FALSE;
    }
    return changed;
}

// pokebw2's BtlvGauge_SetHpTarget
static void func_ov094_02207da0(BtlvGauge *gauge, BtlvGaugeEntry *entry, s32 hp) {
    ClActorPos pos;

    entry->mon.hpAnim = GAUGE_ANIM_START;
    entry->mon.hpDelta = entry->mon.hp - hp;
    if (entry->mon.hp < 0) {
        entry->mon.hp = 0;
    }
    if (entry->mon.hp > entry->mon.maxHp) {
        entry->mon.hp = entry->mon.maxHp;
    }
    entry->mon.hpMoving = TRUE;
    if (entry->mon.hpDelta > 0) {
        func_ov094_0220793c(entry);
        func_0204b454(entry->hpBarActor, &pos, CLACT_SURFACE_MAIN);
        pos.x -= 48 - entry->mon.hpDots;
        func_0204b404(entry->damageActor, &pos, CLACT_SURFACE_MAIN);
        func_0204b3dc(entry->damageActor, TRUE);
        func_ov094_0220868c(gauge, entry, entry->mon.hpDots);
    }
}

// pokebw2's BtlvGauge_UpdateHp
static void func_ov094_02207e1c(BtlvGauge *gauge, int index) {
    BtlvGaugeEntry *entry = &gauge->entries[index];
    s32 hp = func_ov094_02207f6c(entry->mon.maxHp, entry->mon.hp, entry->mon.hpDelta, &entry->mon.hpAnim, 6,
                                 entry->mon.hpSpeed);

    func_ov094_022082fc(gauge, index, 0);
    if (hp == -1) {
        entry->mon.hp -= entry->mon.hpDelta;
        func_ov094_02208420(gauge, entry, entry->mon.hp);
        entry->mon.hpMoving = FALSE;
        entry->mon.hpAnim = 0;
        if (entry->mon.hpDelta > 0) {
            entry->mon.blinkCount = 18;
            entry->mon.blinking = TRUE;
        }
    } else {
        func_ov094_02208420(gauge, entry, hp);
    }
}

// pokebw2's BtlvGauge_SetExpTarget
static void func_ov094_02207ea8(BtlvGaugeEntry *entry, s32 exp) {
    if (entry->mon.level == 100) {
        return;
    }
    entry->mon.expAnim = GAUGE_ANIM_START;
    if (entry->mon.exp + exp < 0) {
        exp -= entry->mon.exp + exp;
    }
    if (entry->mon.exp + exp > entry->mon.expMax) {
        exp -= entry->mon.exp + exp - entry->mon.expMax;
    }
    entry->mon.expDelta = -exp;
    if (entry->mon.exp < 0) {
        entry->mon.exp = 0;
    }
    if (entry->mon.exp > entry->mon.expMax) {
        entry->mon.exp = entry->mon.expMax;
    }
    entry->mon.expMoving = TRUE;
}

// pokebw2's BtlvGauge_UpdateExp
static void func_ov094_02207ef0(BtlvGauge *gauge, int index) {
    BtlvGaugeEntry *entry = &gauge->entries[index];
    s32 dots = func_ov094_022080b0(entry->mon.exp, entry->mon.expDelta, entry->mon.expMax, 10);
    s32 speed;
    s32 exp;

    if (dots == 0) {
        dots = 1;
    }
    speed = entry->mon.expDelta / dots;
    if (speed < 0) {
        speed = -speed;
    }
    if (speed == 0) {
        speed = 1;
    }
    exp = func_ov094_02207f6c(entry->mon.expMax, entry->mon.exp, entry->mon.expDelta, &entry->mon.expAnim, 10, speed);
    func_ov094_022082fc(gauge, index, 1);
    if (exp == -1) {
        entry->mon.exp -= entry->mon.expDelta;
        entry->mon.expMoving = FALSE;
        entry->mon.expAnim = 0;
    }
}

// Moves anim a step from cur toward cur - delta, returning the value it shows or -1 once there. A gauge size tiles
// long with fewer points than pixels counts in 1/256 and moves a pixel a step, the others speed points
// pokebw2's BtlvGauge_StepGauge
static s32 func_ov094_02207f6c(s32 max, s32 cur, s32 delta, s32 *anim, u8 size, u16 speed) {
    s32 target;
    s32 value;
    u8 dots = size * 8;

    if (*anim == GAUGE_ANIM_START) {
        if (max < dots) {
            *anim = cur << 8;
        } else {
            *anim = cur;
        }
    }
    target = cur - delta;
    if (target < 0) {
        target = 0;
    } else if (target > max) {
        target = max;
    }
    if (max < dots) {
        if (target == *anim >> 8 && (*anim & 0xff) == 0) {
            return -1;
        }
    } else {
        if (target == *anim) {
            return -1;
        }
    }
    if (max < dots) {
        s32 step = (max << 8) / dots;

        if (delta < 0) {
            *anim += step;
            value = *anim >> 8;
            if (value >= target || speed == 0) {
                *anim = target << 8;
                value = target;
            }
        } else {
            *anim -= step;
            value = *anim >> 8;
            if ((*anim & 0xff) > 0) {
                value++;
            }
            if (value <= target || speed == 0) {
                *anim = target << 8;
                value = target;
            }
        }
    } else {
        if (delta < 0) {
            *anim += speed;
            if (*anim > target || speed == 0) {
                *anim = target;
            }
        } else {
            *anim -= speed;
            if (*anim < target || speed == 0) {
                *anim = target;
            }
        }
        value = *anim;
    }
    return value;
}

// Fills tiles with the pixels of each of a bar's size tiles for anim, returning the bar's pixels
// pokebw2's BtlvGauge_CalcBarTiles
static u8 func_ov094_02208034(s32 max, s32 cur, s32 delta, s32 *anim, u8 *tiles, u8 size) {
    s32 target = cur - delta;
    int i;
    u32 dots;
    u32 fill;
    u32 rest;

    if (target < 0) {
        target = 0;
    } else if (target > max) {
        target = max;
    }
    dots = size * 8;
    for (i = 0; i < size; i++) {
        tiles[i] = 0;
    }
    if (max < dots) {
        rest = *anim * dots / max >> 8;
    } else {
        rest = *anim * dots / max;
    }
    fill = rest;
    if (rest == 0 && target > 0) {
        tiles[0] = 1;
        fill = 1;
    } else {
        for (i = 0; i < size; i++) {
            if (rest >= 8) {
                tiles[i] = 8;
                rest -= 8;
            } else {
                tiles[i] = rest;
                break;
            }
        }
    }
    return fill;
}

// The pixels a bar of size tiles moves going from value to value - delta
// pokebw2's BtlvGauge_DotDistance
static s32 func_ov094_022080b0(s32 value, s32 delta, s32 max, u8 size) {
    s32 target;
    u8 dots = size * 8;
    s8 now;
    s8 next;
    s8 diff;

    target = value - delta;
    if (target < 0) {
        target = 0;
    } else if (target > max) {
        target = max;
    }
    now = value * dots / max;
    next = target * dots / max;
    diff = now - next;
    return MATH_ABS(diff);
}

// pokebw2's BtlvGauge_DrawName
static void func_ov094_022080f4(BtlvGauge *gauge, BtlvGaugeEntry *entry, PartyPkm *pkm) {
    StrBuf *name = GFL_StrBufCreate(12, HEAPID_TAIL(gauge->heapId));
    GFLBitmap *bitmap = GFL_BitmapCreate(8, 2, 0x20, HEAPID_TAIL(gauge->heapId));
    u8 letter;
    u8 shadow;
    u8 background;
    NNSG2dImageProxy proxy;
    u8 *pixels;
    void *vram = G2_GetOBJCharPtr();

    GFL_BitmapFill(bitmap, 0);
    PokeParty_GetParam(pkm, PKM_PARAM_NICKNAME, name);
    GFL_TextRndGetGlobalColors(&letter, &shadow, &background);
    GFL_TextRndUpdateColorIndexLUT(1, 4, 0);
    if ((s32)GFL_FontGetBlockWidth(name, gauge->font, 0) > 48) {
        GFL_TextRendererDrawToBitmap(bitmap, 2, 5, name, gauge->font);
    } else {
        GFL_TextRendererDrawToBitmap(bitmap, 8, 5, name, gauge->font);
    }
    GFL_TextRndUpdateColorIndexLUT(letter, shadow, background);
    pixels = GFL_BitmapGetPixelData(bitmap);
    func_0204b7c8(entry->nameActor, &proxy);
    if (entry->mon.side) {
        MI_CpuCopy16(pixels,
                     (void *)((uintptr_t)vram + 0x40 + proxy.vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]),
                     0xc0);
        MI_CpuCopy16(pixels + 0x100,
                     (void *)((uintptr_t)vram + 0x140 + proxy.vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]),
                     0xc0);
        MI_CpuCopy16(pixels + 0xc0,
                     (void *)((uintptr_t)vram + 0x400 + proxy.vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]),
                     0x40);
        MI_CpuCopy16(pixels + 0x1c0,
                     (void *)((uintptr_t)vram + 0x500 + proxy.vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]),
                     0x40);
    } else {
        MI_CpuCopy16(pixels,
                     (void *)((uintptr_t)vram + 0x20 + proxy.vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]),
                     0xe0);
        MI_CpuCopy16(pixels + 0x100,
                     (void *)((uintptr_t)vram + 0x120 + proxy.vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]),
                     0xe0);
        MI_CpuCopy16(pixels + 0xe0,
                     (void *)((uintptr_t)vram + 0x400 + proxy.vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]),
                     0x20);
        MI_CpuCopy16(pixels + 0x1e0,
                     (void *)((uintptr_t)vram + 0x500 + proxy.vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]),
                     0x20);
    }
    GFL_StrBufFree(name);
    GFL_BitmapFree(bitmap);
}

// pokebw2's BtlvGauge_DrawSex
static void func_ov094_02208268(BtlvGauge *gauge, BtlvGaugeEntry *entry) {
    NNSG2dImageProxy proxy;
    void *vram = G2_GetOBJCharPtr();
    u32 top = entry->mon.sex * 0x40 + 0x380;
    u32 bottom = entry->mon.sex * 0x40 + 0x3a0;

    if (entry->mon.sex == 2) {
        top = GAUGE_TILE_BLANK;
        bottom = GAUGE_TILE_BLANK;
    }
    func_0204b7c8(entry->nameActor, &proxy);
    if (entry->mon.side) {
        MI_CpuCopy16(gauge->chars + top,
                     (void *)((uintptr_t)vram + 0x440 + proxy.vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]),
                     0x20);
        MI_CpuCopy16(gauge->chars + bottom,
                     (void *)((uintptr_t)vram + 0x540 + proxy.vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]),
                     0x20);
    } else {
        MI_CpuCopy16(gauge->chars + top,
                     (void *)((uintptr_t)vram + 0x420 + proxy.vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]),
                     0x20);
        MI_CpuCopy16(gauge->chars + bottom,
                     (void *)((uintptr_t)vram + 0x520 + proxy.vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]),
                     0x20);
    }
}

// bar 0 is the HP bar, 1 the EXP bar
// pokebw2's BtlvGauge_DrawBar
static void func_ov094_022082fc(BtlvGauge *gauge, int index, int bar) {
    BtlvGaugeEntry *entry = &gauge->entries[index];
    u8 dots;
    u8 tiles[10];
    NNSG2dImageProxy proxy;
    u32 color;
    s32 hp;
    u8 i;
    void *vram = G2_GetOBJCharPtr();

    switch (bar) {
    case 0:
        if (gauge->showNumbers && !(index & 1)) {
            return;
        }
        func_0204b7c8(entry->hpBarActor, &proxy);
        dots =
            func_ov094_02208034(entry->mon.maxHp, entry->mon.hp, entry->mon.hpDelta, &entry->mon.hpAnim, tiles, 6);
        hp = entry->mon.hpAnim;
        if (entry->mon.maxHp < 48) {
            hp >>= 8;
        }
        switch (func_0202d034(hp, entry->mon.maxHp)) {
        case HP_GAUGE_COLOR_GREEN:
            color = 0;
            break;
        case HP_GAUGE_COLOR_YELLOW:
            color = 0x120;
            break;
        case HP_GAUGE_COLOR_RED:
        default:
            color = 0x240;
            break;
        }
        for (i = 0; i < 6; i++) {
            MI_CpuCopy16(
                gauge->chars + (color + tiles[i] * 0x20),
                (void *)((uintptr_t)vram + (i + 1) * 0x20 +
                         proxy.vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]),
                0x20);
        }
        if (entry->mon.hpDelta > 0) {
            func_ov094_0220868c(gauge, entry, dots);
        }
        break;
    case 1:
        if (entry->expActor == NULL) {
            return;
        }
        func_0204b7c8(entry->expActor, &proxy);
        func_ov094_02208034(entry->mon.expMax, entry->mon.exp, entry->mon.expDelta, &entry->mon.expAnim, tiles, 10);
        if (entry->mon.level == 100) {
            for (i = 0; i < 10; i++) {
                tiles[i] = 0;
            }
        }
        for (i = 0; i < 10; i++) {
            MI_CpuCopy16(
                gauge->chars + (tiles[i] * 0x20 + 0x400),
                (void *)((uintptr_t)vram + (i + 1) * 0x20 +
                         proxy.vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]),
                0x20);
        }
        break;
    }
}

// pokebw2's BtlvGauge_DrawHpNumbers
static void func_ov094_02208420(BtlvGauge *gauge, BtlvGaugeEntry *entry, s32 hp) {
    u16 hpTiles[3];
    u16 maxTiles[3];
    NNSG2dImageProxy proxy;
    int i;
    s32 maxHp;
    int n;
    void *vram = G2_GetOBJCharPtr();

    if (entry->hpNumActor == NULL) {
        return;
    }
    func_0204b7c8(entry->hpNumActor, &proxy);
    for (i = 0; i < 3; i++) {
        hpTiles[i] = GAUGE_TILE_BLANK;
        maxTiles[i] = GAUGE_TILE_BLANK;
    }
    if (hp >= 100) {
        hpTiles[0] = hp / 100 * 0x20 + GAUGE_TILE_DIGITS;
        hp %= 100;
    }
    if (hp >= 10 || hpTiles[0] != GAUGE_TILE_BLANK) {
        hpTiles[1] = hp / 10 * 0x20 + GAUGE_TILE_DIGITS;
        hp %= 10;
    }
    hpTiles[2] = hp * 0x20 + GAUGE_TILE_DIGITS;
    maxHp = entry->mon.maxHp;
    n = 0;
    if (maxHp >= 100) {
        maxTiles[n] = maxHp / 100 * 0x20 + GAUGE_TILE_DIGITS;
        maxHp %= 100;
        n++;
    }
    if (maxHp >= 10 || n != 0) {
        maxTiles[n] = maxHp / 10 * 0x20 + GAUGE_TILE_DIGITS;
        maxHp %= 10;
        n++;
    }
    maxTiles[n] = maxHp * 0x20 + GAUGE_TILE_DIGITS;
    for (i = 0; i < 3; i++) {
        MI_CpuCopy16(gauge->chars + hpTiles[i],
                     (void *)((uintptr_t)vram + i * 0x20 + proxy.vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]),
                     0x20);
        MI_CpuCopy16(gauge->chars + maxTiles[i],
                     (void *)((uintptr_t)vram + (i + 4) * 0x20 +
                              proxy.vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]),
                     0x20);
    }
}

static void func_ov094_02208550(BtlvGauge *gauge, BtlvGaugeEntry *entry) {
    StrBuf *str = GFL_StrBufCreate(4, HEAPID_TAIL(gauge->heapId));
    GFLBitmap *bitmap = GFL_BitmapCreate(3, 2, 0x20, HEAPID_TAIL(gauge->heapId));
    u8 letter;
    u8 shadow;
    u8 background;
    NNSG2dImageProxy proxy;
    u8 *pixels;
    void *vram = G2_GetOBJCharPtr();

    GFL_BitmapFill(bitmap, 0);
    GFL_WordSetFormatNumber(str, entry->mon.level, 3, 0, FALSE);
    GFL_TextRndGetGlobalColors(&letter, &shadow, &background);
    GFL_TextRndUpdateColorIndexLUT(1, 4, 0);
    GFL_TextRendererDrawToBitmap(bitmap, 0, 5, str, gauge->font);
    GFL_TextRndUpdateColorIndexLUT(letter, shadow, background);
    pixels = GFL_BitmapGetPixelData(bitmap);
    func_0204b7c8(entry->nameActor, &proxy);
    if (entry->mon.side) {
        MI_CpuCopy16(pixels,
                     (void *)((uintptr_t)vram + 0x480 + proxy.vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]),
                     0x60);
        MI_CpuCopy16(pixels + 0x60,
                     (void *)((uintptr_t)vram + 0x580 + proxy.vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]),
                     0x60);
    } else {
        MI_CpuCopy16(pixels,
                     (void *)((uintptr_t)vram + 0x460 + proxy.vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]),
                     0x60);
        MI_CpuCopy16(pixels + 0x60,
                     (void *)((uintptr_t)vram + 0x560 + proxy.vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]),
                     0x60);
    }
    GFL_StrBufFree(str);
    GFL_BitmapFree(bitmap);
}

static void func_ov094_02208650(BtlvGauge *gauge, BtlvGaugeEntry *entry) {
    NNSG2dImageProxy proxy;
    void *vram = G2_GetOBJCharPtr();
    u32 offset;

    offset = 0x360;
    if (entry->mon.caught == 0) {
        offset = 0x660;
    }
    func_0204b7c8(entry->nameActor, &proxy);
    MIi_CpuCopy16(gauge->chars + offset, (void *)((uintptr_t)vram + 0x120 + proxy.vramLocation.baseAddrOfVram[1]),
    0x20);
}

// The HP bar's lost part, from the bar's dots before the hit down to dots
static void func_ov094_0220868c(BtlvGauge *gauge, BtlvGaugeEntry *entry, s32 dots) {
    u8 tiles[6];
    NNSG2dImageProxy proxy;
    s32 lost = entry->mon.hpDots - dots;
    int i;
    void *vram = G2_GetOBJCharPtr();

    for (i = 0; i < 6; i++) {
        tiles[i] = 8;
    }
    for (i = 5; i >= 0; i--) {
        if (lost >= 8) {
            tiles[i] = 0;
            lost -= 8;
        } else {
            tiles[i] = 8 - lost;
            break;
        }
    }
    func_0204b7c8(entry->damageActor, &proxy);
    for (i = 0; i < 6; i++) {
        MI_CpuCopy16(gauge->chars + (tiles[i] * 0x20 + 0x6a0),
                     (void *)((uintptr_t)vram + i * 0x20 + proxy.vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]),
                     0x20);
    }
}

static void func_ov094_022086f8(BtlvGauge *gauge, int index) {
    BtlvGaugeEntry *entry = &gauge->entries[index];

    if (!entry->mon.expMoving) {
        switch (entry->mon.levelUpSeq) {
        case 0:
            GFL_SndSEPlay(SEQ_SE_EXPMAX);
            entry->mon.levelUpSeq++;
            break;
        case 1:
            entry->mon.levelUpSeq++;
            break;
        case 2:
            entry->mon.exp = 0;
            entry->mon.expMax = entry->mon.nextExpMax;
            entry->mon.expAnim = GAUGE_ANIM_START;
            entry->mon.expDelta = 0;
            func_ov094_02207f6c(entry->mon.maxHp, entry->mon.hp, 0, &entry->mon.hpAnim, 6, 1);
            func_ov094_02207f6c(entry->mon.expMax, entry->mon.exp, 0, &entry->mon.expAnim, 10, 1);
            func_ov094_022082fc(gauge, index, 0);
            func_ov094_022082fc(gauge, index, 1);
            func_ov094_02208420(gauge, entry, entry->mon.hp);
            func_ov094_02208550(gauge, entry);
            entry->mon.levelUp = FALSE;
            break;
        }
    }
}

static void func_ov094_022087c4(BtlvGauge *gauge, int index) {
    ClActorPos offset;

    offset.x = -16;
    offset.y = FX_SinIdx(gauge->shakeAngle) / 0x800;
    gauge->shakeAngle += 0x400;
    func_ov094_022079c0(gauge, index, &offset);
}

void func_ov094_02208814(BtlvGauge *gauge, u32 status, int index) {
    const u32 sequences[7] = { GAUGE_STATUS_NONE, 1, 3, 2, 5, 4, 7 };

    if (gauge->entries[index].statusActor != NULL) {
        if (status == 0) {
            func_0204b3dc(gauge->entries[index].statusActor, FALSE);
            func_0204b878(gauge->entries[index].statusActor, 0);
        } else {
            func_0204b3dc(gauge->entries[index].statusActor, TRUE);
            func_0204b878(gauge->entries[index].statusActor, sequences[status]);
        }
    }
}

void func_ov094_02208870(BtlvGauge *gauge, int index) {
    int i;

    for (i = 0; i < 8; i++) {
        if (gauge->entries[i].nameActor != NULL && gauge->entries[i].mon.shaking) {
            ClActorPos pos = { -16, 0 };

            gauge->entries[i].mon.shaking = FALSE;
            func_ov094_022079c0(gauge, i, &pos);
        }
    }
    if (index != 8) {
        gauge->entries[index].mon.shaking = TRUE;
        gauge->shakeAngle = 0;
    }
}

u32 func_ov094_022088f4(BtlvGauge *gauge) {
    return gauge->pinch;
}

void func_ov094_02208900(BtlvGauge *gauge, u32 value) {
    gauge->pinch = value;
}

void func_ov094_02208918(BtlvGauge *gauge, u32 value) {
    gauge->bgm = value;
}

void func_ov094_02208924(BtlvGauge *gauge, u32 value) {
    gauge->bgmReplayed = value;
}

void func_ov094_0220893c(BtlvGauge *gauge, u32 value) {
    gauge->noPinchBgm = value;
}

BOOL func_ov094_02208958(BtlvGauge *gauge, int index, u32 *color, u32 *status) {
    if (gauge->entries[index].nameActor == NULL || !gauge->entries[index].mon.shown ||
        gauge->entries[index].mon.statusHidden) {
        *color = 0;
        *status = GAUGE_STATUS_NONE;
        return FALSE;
    }
    *color = func_0202d034(gauge->entries[index].mon.hp, gauge->entries[index].mon.maxHp);
    *status = func_0204b898(gauge->entries[index].statusActor);
    if (*status == 0) {
        *status = GAUGE_STATUS_NONE;
    }
    return TRUE;
}

void func_ov094_022089ac(BtlvGauge *gauge) {
    gauge->toggleRequest = TRUE;
}

void func_ov094_022089bc(BtlvGauge *gauge, int index) {
    if (gauge->entries[index].nameActor != NULL) {
        gauge->entries[index].mon.statusHidden = TRUE;
    }
}

void func_ov094_022089d4(BtlvGauge *gauge) {
    if (gauge->bgmFading) {
        if (GFL_SndBGMIsFading()) {
            return;
        }
        gauge->bgmFading = FALSE;
        if (gauge->pinch) {
            GFL_SndBGMPop();
            if (!gauge->bgmReplayed && func_ov094_021f882c()) {
                GFL_SndBGMPlay(gauge->bgm, 0xffff);
                gauge->bgmReplayed = TRUE;
            } else {
                func_ov094_02208d24(FALSE);
            }
            GFL_SndBGMFadeIn(24);
        } else {
            func_ov094_02208d24(TRUE);
            GFL_SndBGMPush();
            GFL_SndBGMPlay(SEQ_BGM_BATTLEPINCH, 0xffff);
        }
        gauge->pinch ^= 1;
    } else {
        BOOL busy = FALSE;
        BOOL low = FALSE;
        int i;

        for (i = 0; i < 8; i++) {
            if (gauge->entries[i].mon.hpMoving || gauge->entries[i].mon.expMoving || gauge->entries[i].mon.levelUp) {
                busy = TRUE;
            } else if (gauge->entries[i].mon.side == 0 && gauge->entries[i].mon.shown &&
                       gauge->entries[i].mon.type != 4 &&
                       func_0202d034(gauge->entries[i].mon.hp, gauge->entries[i].mon.maxHp) == HP_GAUGE_COLOR_RED) {
                low = TRUE;
            }
        }
        if (gauge->pinch || GFL_SndBGMGetID() == gauge->bgm) {
            if (!busy && ((gauge->pinch && !low) || (!gauge->pinch && low == TRUE))) {
                gauge->bgmFading = TRUE;
                GFL_SndBGMFadeOut(8);
            }
        }
    }
}

static void func_ov094_02208b10(TCB *tcb, void *data) {
    BtlvGaugeEntry *entry = data;
    ClActorPos pos;
    int dx;

    if (entry->mon.side) {
        dx = 8;
    } else {
        dx = -8;
    }
    func_0204b454(entry->nameActor, &pos, CLACT_SURFACE_MAIN);
    pos.x += dx;
    func_0204b404(entry->nameActor, &pos, CLACT_SURFACE_MAIN);
    func_0204b454(entry->hpBarActor, &pos, CLACT_SURFACE_MAIN);
    pos.x += dx;
    func_0204b404(entry->hpBarActor, &pos, CLACT_SURFACE_MAIN);
    func_0204b454(entry->statusActor, &pos, CLACT_SURFACE_MAIN);
    pos.x += dx;
    func_0204b404(entry->statusActor, &pos, CLACT_SURFACE_MAIN);
    if (!entry->mon.side) {
        func_0204b454(entry->hpNumActor, &pos, CLACT_SURFACE_MAIN);
        pos.x += dx;
        func_0204b404(entry->hpNumActor, &pos, CLACT_SURFACE_MAIN);
    }
    if (entry->expActor != NULL) {
        func_0204b454(entry->expActor, &pos, CLACT_SURFACE_MAIN);
        pos.x += dx;
        func_0204b404(entry->expActor, &pos, CLACT_SURFACE_MAIN);
    }
    entry->mon.slideFrames--;
    if (entry->mon.slideFrames == 0) {
        GFL_TCBRemove(tcb);
        entry->mon.slideTask = NULL;
    }
}

static void func_ov094_02208bfc(TCB *tcb, void *data) {
    BtlvGaugeFlashTask *work = data;
    const u32 masks[8] = { 1 << 0, 1 << 1, 1 << 0, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5 };
    int i;

    switch (work->seq) {
    case 0:
        work->paletteMask = 0;
        for (i = 0; i < 8; i++) {
            if (work->gauge->entries[i].mon.hpMoving && work->gauge->entries[i].mon.hpDelta > 0) {
                work->paletteMask |= masks[i];
            }
        }
        if (work->paletteMask != 0) {
            func_0202173c(func_ov094_021f86e0(), 1 << PALFADE_BUFFER_MAIN_OBJ, work->paletteMask, 0, 0, 8, 0x10,
                          func_ov094_021f86d4());
            work->seq = 1;
        }
        break;
    case 1:
        if (func_02021ed8(func_ov094_021f86e0()) == 0) {
            func_0202173c(func_ov094_021f86e0(), 1 << PALFADE_BUFFER_MAIN_OBJ, work->paletteMask, 0, 8, 0, 0x10,
                          func_ov094_021f86d4());
            work->seq = 2;
        }
        break;
    case 2:
        if (func_02021ed8(func_ov094_021f86e0()) == 0) {
            if (++work->count == 3) {
                work->count = 0;
                work->seq = 3;
            } else {
                work->seq = 0;
            }
        }
        break;
    case 3:
        for (i = 0; i < 8; i++) {
            if (work->gauge->entries[i].mon.hpMoving) {
                break;
            }
        }
        if (i == 8) {
            work->seq = 0;
        }
        break;
    }
}

// pokebw2's BtlvGauge_PauseBgm
static void func_ov094_02208d24(BOOL paused) {
    if (func_0200f300() != TRUE) {
        GFL_SndBGMSetPaused(paused);
    }
}

// pokebw2's BtlvGauge_CalcFill
static u32 func_ov094_02208d38(u32 value, u32 max, u32 width) {
    u32 fill = value * width / max;

    if (fill == 0 && value != 0) {
        fill = 1;
    }
    return fill;
}

// pokebw2's BtlvGauge_ShowHpParts
static void func_ov094_02208d50(BtlvGauge *gauge, int index, BOOL visible) {
    if ((gauge->entries[index].mon.type == BTL_STYLE_TRIPLE || gauge->entries[index].mon.type == BTL_STYLE_ROTATION) &&
        visible == TRUE && !(index & 1)) {
        func_0204b3dc(gauge->entries[index].hpNumActor, gauge->showNumbers);
        func_0204b3dc(gauge->entries[index].hpBarActor, !gauge->showNumbers);
    } else {
        func_0204b3dc(gauge->entries[index].hpNumActor, visible);
        func_0204b3dc(gauge->entries[index].hpBarActor, visible);
    }
}
