// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov207/p_status_local.h, the lines in use so
// far.
#ifndef BWPORT_OV131_P_STATUS_LOCAL_H
#define BWPORT_OV131_P_STATUS_LOCAL_H

#include "types.h"
#include "app/p_status.h"
#include "gfl/heap.h"
#include "system/mcss.h"
#include "gfl/g3d.h"
#include "gfl/clact.h"
#include "gfl/msg.h"
#include "system/printsys.h"
#include "struct_decls.h"

// The summary screen's work and the functions its files share. Field names are pokebw2's

// Cut short after param: the rest needs headers nothing here uses yet
struct PStatusWork {
    HeapID heapId;
    TCB *vblankTcb;
    PStatusParam *param;
    u32 unk0C;
    u32 unk10;
    u32 unk14;
    u16 partyIndex;
    u8 shownPartyIndex;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D[0x3];
    u32 unk20;
    u32 touchHit;
    u32 unk28;
    u32 unk2C;
    u32 unk30;
    u32 unk34;
    BOOL isEgg;
    u32 happiness;
    u32 unk40;
    u32 unk44;
    u32 unk48;
    u32 unk4C;
    u32 unk50;
    u32 unk54;
    s32 unk58;
    u32 unk5C;
    u32 unk60[0x3];
    u8 unk6C[0x4];
    G3DCamera *camera;
    u8 unk74[4];
    MCSSSystem *mcssSys;
    MsgData *msgData;
    Font *font;
    PrintQueue *printQueue;
    PStaSubWork *unk88;
    void *unk8C;
    PStaSkillWork *unk90;
    void *unk94;
    u32 unk98[0x26];
    u32 typeIconChars[0x11];
    ClActUnit *actorUnit;
    ClActor *unk178[8];
    ClActor *typeIcons[2];
    ClActor *unk1A0;
    u16 unk1A4;
    u16 unk1A6[0x4];
    u16 unk1AE[0x4];
    u16 unk1B6[0x20];
    u16 unk1F6[0x10];
    u16 unk216[0x10];
    u16 unk236[0x10];
    u8 unk256[0x2];
    PartyPkm *unk258;
    u32 unk25C;
    u8 unk260;
    u8 unk261[0x3];
};

// The Pokémon the summary screen shows, from the party or the box
BoxPkm *func_ov131_021d6064(PStatusWork *wk);
// The party Pokémon the summary screen shows, or NULL in a box
PartyPkm *func_ov131_021d60a4(PStatusWork *wk);

// pokebw2's PStatus_Init, PStatus_Exit and PStatus_Main (p_sta_sys.c)
BOOL func_ov131_021d4240(PStatusWork *wk);
BOOL func_ov131_021d4418(PStatusWork *wk);
int func_ov131_021d44c8(PStatusWork *wk);
BOOL func_ov131_021d52d4(PStatusWork *wk);
void func_ov131_021d54a8(PStatusWork *wk);
BOOL func_ov131_021d5bac(PStatusWork *wk, u8 dir, u16 *index);
void func_ov131_021d5c4c(PStatusWork *wk);
BOOL func_ov131_021d5b58(PStatusWork *wk, u8 dir);
void func_ov131_021d5c98(PStatusWork *wk);
void func_ov131_021d60d4(PStatusWork *wk, BOOL decrypted);
void func_ov131_021d5180(PStatusWork *wk);
void func_ov131_021d89fc(PStatusWork *wk, PStaRibbonWork *ribbon);
void func_ov131_021d9bd4(PStatusWork *wk, PStaSkillWork *skill);
void func_ov131_021d694c(PStatusWork *wk, PStaSubWork *sub);
void func_ov131_021d4f24(PStatusWork *wk);
void func_ov131_021d6808(PStatusWork *wk, PStaSubWork *sub);
void func_ov131_021d8904(PStatusWork *wk, PStaRibbonWork *ribbon);
void func_ov131_021d9a04(PStatusWork *wk, PStaSkillWork *skill);
void func_ov131_021d7bf0(PStatusWork *wk, PStaInfoWork *info);
void func_ov131_021d6810(PStatusWork *wk, PStaSubWork *sub);
void func_ov131_021d8938(PStatusWork *wk, PStaRibbonWork *ribbon);
void func_ov131_021d9a4c(PStatusWork *wk, PStaSkillWork *skill);
void func_ov131_021d6614(PStatusWork *wk, PStaSubWork *sub);
void func_ov131_021d7b48(PStatusWork *wk, PStaInfoWork *info);
void func_ov131_021d9740(PStatusWork *wk, PStaSkillWork *skill);
void func_ov131_021d8830(PStatusWork *wk, PStaRibbonWork *ribbon);
void func_ov131_021d67ec(PStatusWork *wk, PStaSubWork *sub, ArcTool *arc);
void func_ov131_021d7b4c(PStatusWork *wk, PStaInfoWork *info, ArcTool *arc);
void func_ov131_021d9980(PStatusWork *wk, PStaSkillWork *skill, ArcTool *arc);
void func_ov131_021d8884(PStatusWork *wk, PStaRibbonWork *ribbon, ArcTool *arc);
void func_ov131_021d6d04(PStatusWork *wk, PStaSubWork *sub);
void func_ov131_021d6a38(PStatusWork *wk, PStaSubWork *sub);
void func_ov131_021d7eac(PStatusWork *wk, PStaInfoWork *info);
void func_ov131_021d9d94(PStatusWork *wk, PStaSkillWork *skill);
void func_ov131_021d90c0(PStatusWork *wk, PStaRibbonWork *ribbon);
void func_ov131_021da8bc(PStatusWork *wk, PStaSkillWork *skill);
void func_ov131_021d7cac(PStatusWork *wk, PStaInfoWork *info);
void func_ov131_021d9cd0(PStatusWork *wk, PStaSkillWork *skill);
void func_ov131_021d8fac(PStatusWork *wk, PStaRibbonWork *ribbon);
void func_ov131_021da7a4(PStatusWork *wk, PStaSkillWork *skill);
PStaSubWork *func_ov131_021d65d8(PStatusWork *wk);
PStaInfoWork *func_ov131_021d7b00(PStatusWork *wk);
PStaSkillWork *func_ov131_021d96b0(PStatusWork *wk);
PStaRibbonWork *func_ov131_021d87e0(PStatusWork *wk);
void func_ov131_021d91ec(PStatusWork *wk, PStaRibbonWork *ribbon);
void func_ov131_021d881c(PStatusWork *wk, PStaRibbonWork *ribbon);
void func_ov131_021d9734(PStatusWork *wk, PStaSkillWork *skill);
void func_ov131_021d7b34(PStatusWork *wk, PStaInfoWork *info);
void func_ov131_021d6608(PStatusWork *wk, PStaSubWork *sub);
void func_ov131_021d6cdc(PStatusWork *wk, PStaSubWork *sub);
void func_ov131_021d697c(PStatusWork *wk, PStaSubWork *sub);
void func_ov131_021d911c(PStatusWork *wk, PStaRibbonWork *ribbon);
void func_ov131_021d7e84(PStatusWork *wk, PStaInfoWork *info);
void func_ov131_021d9d44(PStatusWork *wk, PStaSkillWork *skill);
void func_ov131_021d909c(PStatusWork *wk, PStaRibbonWork *ribbon);
void func_ov131_021da84c(PStatusWork *wk, PStaSkillWork *skill);
void func_ov131_021d7c38(PStatusWork *wk, PStaInfoWork *info);
void func_ov131_021d9c3c(PStatusWork *wk, PStaSkillWork *skill);
void func_ov131_021d8f50(PStatusWork *wk, PStaRibbonWork *ribbon);
void func_ov131_021da6a4(PStatusWork *wk, PStaSkillWork *skill);

// The directions of the Pokémon's search: the down arrow goes to the next, the up arrow to the previous
#define PSTA_DIR_UP 0
#define PSTA_DIR_DOWN 1

// ribbon.c: the table of the 80 ribbons
enum {
    RIBBON_DATA_PARAM,
    RIBBON_DATA_ICON,
    RIBBON_DATA_PALETTE,
    RIBBON_DATA_NAME,
    RIBBON_DATA_DESCRIPTION,
    RIBBON_DATA_CATEGORY,
};

#define RIBBON_COUNT 80

// The groups that number the ribbons, named after their headings in the ribbon text, which follow the descriptions
enum {
    RIBBON_CATEGORY_LEAGUE,
    RIBBON_CATEGORY_CONTEST,
    RIBBON_CATEGORY_TOWER,
    RIBBON_CATEGORY_MEMORIAL,
    RIBBON_CATEGORY_GIFT,
    RIBBON_CATEGORY_COUNT,
};

// pokebw2's Ribbon_GetData and Ribbon_GetDescription
u32 func_ov131_021dbf2c(u32 ribbon, u32 field);
u32 func_ov131_021dbf94(u32 ribbon);

// The palettes, characters and cell animations in PStatusWork's clResources
#define PSTA_RES_PLTT(i) (i)

#define PSTA_RES_CHAR(i) (12 + (i))
#define PSTA_RES_CELL(i) (25 + (i))

// The buttons of the bottom screen
enum {
    PSTA_BUTTON_INFO,
    PSTA_BUTTON_SKILL,
    PSTA_BUTTON_RIBBON,
    // Registers the page to the shortcut menu
    PSTA_BUTTON_SHORTCUT,
    PSTA_BUTTON_UP,
    PSTA_BUTTON_DOWN,
    PSTA_BUTTON_CLOSE,
    PSTA_BUTTON_BACK,
    PSTA_BUTTON_COUNT,
};

// What the proc's main function does
enum {
    PSTA_SEQ_FADE_IN,
    PSTA_SEQ_WAIT_FADE_IN,
    PSTA_SEQ_EXIT,
    PSTA_SEQ_WAIT_FADE_OUT,
    PSTA_SEQ_MAIN,
};

// PStaScreen: a screen loaded from an NSCR arc file
typedef struct {
    NNSG2dScreenData *screen;
    void *file;
} PStaScreen;

// Black 2's, not checked against Black
// A text window of a page, in tiles
struct PStaWindowSetup {
    u8 x;
    u8 y;
    u8 width;
    u8 height;
};

struct PStaOamSetup {
    GFLBitmap *bitmap;
    s16 x;
    s16 y;
    u32 palette;
    u32 paletteOffset;
    u8 priority;
    u8 bgPriority;
    u16 surface;
    u32 vramType;
};

// pokebw2's PStaOam_Create
// p_sta_oam.c
PStaOam *func_ov131_021dba90(HeapID heapId, ClActUnit *unit);
// pokebw2's PStatus_PrintToWindow
void func_ov131_021d618c(PStatusWork *wk, BmpWin *window, u32 msgId, u16 x, u16 y, u16 color);
// pokebw2's PStatus_PrintFormattedToWindow
void func_ov131_021d6214(PStatusWork *wk, BmpWin *window, WordSet *wordSet, u32 msgId, u16 x, u16 y, u16 color);
// pokebw2's PStaOam_CreateActor
PStaOamActor *func_ov131_021dbad0(PStaOam *oam, const struct PStaOamSetup *setup);
// pokebw2's PStaOam_SetVisible
void func_ov131_021dbca4(PStaOamActor *actor, BOOL visible);
// pokebw2's PStaOam_FreeActor
void func_ov131_021dbc3c(PStaOamActor *actor);
// pokebw2's PStaOam_SwapBitmaps
void func_ov131_021dbe98(PStaOamActor *a, PStaOamActor *b);
// pokebw2's PStaOam_Upload
void func_ov131_021dbcf4(PStaOamActor *actor);
// pokebw2's PStatus_Print
void func_ov131_021d614c(PStatusWork *wk, GFLBitmap *bitmap, u32 msgId, u16 x, u16 y, u16 color);
// pokebw2's PStatus_PrintFormattedRight
void func_ov131_021d6244(PStatusWork *wk, GFLBitmap *bitmap, WordSet *wordSet, u32 msgId, u16 x, u16 y,
                                 u16 color);
// pokebw2's PStatus_PrintFormatted
void func_ov131_021d61b8(PStatusWork *wk, GFLBitmap *bitmap, WordSet *wordSet, u32 msgId, u16 x, u16 y, u16 color);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's PStaOam_Free
void func_ov131_021dbac0(PStaOam *oam);

#endif // BWPORT_OV131_P_STATUS_LOCAL_H
