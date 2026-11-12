// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btlv_input.h: the lines in use so
// far.
#ifndef BWPORT_BATTLE_BTLV_INPUT_H
#define BWPORT_BATTLE_BTLV_INPUT_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/touchpanel.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
// A screen's buttons: their touch rectangles, a flag word per button (bit 0: a cancel, bit 1: a cancel for a later
// Pokémon on the command screen) and the palette rows a press flashes (-1: none, bit 16: the input stays unlocked)
typedef struct {
    const TouchRect *rects;
    const s32 *flags;
    const s32 *flashRows;
} BtlvInputButtonSet;

// Black 2's, not checked against Black
// A stop of the key cursor, 12 bytes
struct BtlvInputKeyStop {
    s8 corners[6]; // the rectangle each cursor actor sits on a corner of, -1 to hide it
    s8 up;         // the stop each key moves to, KEY_CURSOR_NONE for none; -n is the stop the cursor came from, or n
    s8 down;
    s8 left;
    s8 right;
    s8 a; // the result of A
    s8 b; // the result of B; -n presses stop n, only for a later Pokémon
};

// Black 2's, not checked against Black
#define KEY_CURSOR_NONE (-128)

// Black 2's, not checked against Black
// Screen 4, yes or no
typedef struct {
    StrBuf *strs[2];
    BOOL canCancel;
} BtlvInputYesNoParam;

// pokebw2's BtlvInput_Delete
void func_ov094_022021f4(BtlvInput *work);
// The finger demo of the tutorial: done when it says so (1)
u32 func_ov094_02203040(BtlvInput *work);

// pokebw2's BtlvInput_Update
void func_ov094_02202228(BtlvInput *work);
// pokebw2's BtlvInput_IsFading
BOOL func_ov094_022026fc(BtlvInput *work);

// Black 2's, not checked against Black
// Set on a stop: the stop the cursor came from, if it is in the list the caller passes
#define KEY_CURSOR_LAST 0x40

// Black 2's, not checked against Black
// A party slot of the command screen; only the icon animation byte is read
struct BtlvInputPokeEntry {
    u8 unk0;
    u8 iconAnim; // added to the actor's base sequence
    u16 unk2;
    u32 unk4;
    u32 unk8;
};

// Black 2's, not checked against Black
// Screen 1, the command screen
struct BtlvInputCommandParam {
    BtlvInputPokeEntry entries[2][6]; // 0x00
    BOOL laterPoke;                   // 0x90  not the first mon to choose this turn
    BOOL secondRow;                   // 0x94
    u32 species[3];                   // 0x98  the party icons of the mons on the field
    u32 forms[3];                     // 0xa4
    u32 sexes[3];                     // 0xb0
    s32 pokePos;                      // 0xbc  the battle position, halved in place from 2 on into a slot
    u32 scdState;      // 0xc0  the creator's scdState, which nothing reads
    u8 launcherPoints; // 0xc4  the Wonder Launcher's points
    u8 ballSlide;      // 0xc5  the ball count's slide, 0 for none
};

// Black 2's, not checked against Black
// Screen 2, the move screen
struct BtlvInputMoveParam {
    u16 moves[4];    // 0x00
    u8 pp[4];        // 0x08
    u8 maxPP[4];     // 0x0c
    u32 pos;         // 0x10  the battle position
    BOOL noMoveInfo; // 0x14  a move's information can't be shown with L held
};

// Black 2's, not checked against Black
// A panel of the target screen, 12 bytes
struct BtlvInputTargetEntry {
    u8 msgForm : 2; // the name's message, by sex mark
    u8 selectable : 1;
    u8 unk1;
    u16 unk2;
    s16 hp;    // 0: no mon in that position, or it has fainted
    s16 maxHp;
    PartyPkm *pkm;
};

// Black 2's, not checked against Black
// Screen 3, the target screen
struct BtlvInputTargetParam {
    BtlvInputTargetEntry mons[6];
    u8 attackerPos; // 0x48
    u8 range;       // 0x49  the move's range
};

// Black 2's, not checked against Black
// Screen 5, the rotation screen
struct BtlvInputRotationParam {
    BattleMon *mons[3];
    BOOL moveUsable[3][4];
};

// Black 2's, not checked against Black
// Screen 6, the recorder screen
struct BtlvInputRecorderParam {
    s32 unk0;
    s32 unk4; // printed left of the slash, in red when it differs from unk0
    s32 unk8; // printed right of it
    u32 mode; // 0 none, 1-3 a message (message 9-11) and BG 4 scrolled
};

// An actor slot; the second word is never touched
typedef struct {
    ClActor *actor;
    u32 unk4;
} BtlvInputActor;

// The lower screen's work, 0x36c bytes
struct BtlvInput {
    TCBManager *tcbMgr;                // 0x000
    void *tcbMgrBuf;                   // 0x004
    TCB *tasks[8];                     // 0x008
    void (*taskEndFuncs[8])(TCB *tcb); // 0x028
    ArcTool *arc;                      // 0x048  archive 11
    GameData *gameData;                // 0x04c
    u32 rule;                          // 0x050  2 = triple
    u32 unk54;                         // 0x054
    u32 screen;                        // 0x058  0..7
    MsgData *msgData;                  // 0x05c
    BtlvFingerCursor *fingerCursor;    // 0x060
    PaletteFade *paletteFade;          // 0x064
    // 0x068
    u32 busy : 1;          // bit 0: a screen change runs
    u32 subTaskCount : 3;  // bits 1-3: running sub tasks
    u32 laterPoke : 1;     // bit 4: not the first mon to choose this turn
    u32 cursor : 4;        // bits 5-8: current button
    u32 prevCursor : 4;    // bits 9-12: initialised to 0xf
    u32 cursorRefresh : 1; // bit 13
    u32 secondRow : 1;     // bit 14
    u32 fadeMode : 2;      // bits 15-16
    u32 unk68_17 : 2;      // bits 17-18
    u32 pokeSlot : 2;      // bits 19-20: index into mons[] and moveUsable[]
    u32 unk68_21 : 2;      // bits 21-22: never touched
    u32 fingerSeq : 4;     // bits 23-26: the finger cursor demonstration's step
    u32 fingerCount : 4;   // bits 27-30
    u32 inputLocked : 1;   // bit 31
    // 0x06c
    u32 canCancel : 1;       // bit 0
    u32 selectionLocked : 1; // bit 1
    u32 pos : 8;             // bits 2-9: the move's range, the column of ov169's target tables
    u32 unk6c_10 : 8;        // never touched
    u32 noCancel : 1;        // bit 18: no cancel sound
    u32 unk6c_19 : 13;
    s32 pressedButton;                    // 0x070  -1 none; | 0x8000 when L is held
    BtlvInputPokeEntry pokeEntries[2][6]; // 0x074
    u32 chars;                            // 0x104
    u32 palette;                          // 0x108
    u32 cellAnims;                        // 0x10c
    u32 chars2;                           // 0x110
    u32 cellAnims2;                       // 0x114
    ClActUnit *pokeUnit;                  // 0x118
    BtlvInputActor pokeActors[2][6];      // 0x11c
    ClActUnit *cursorUnit;                // 0x17c
    BtlvInputActor cursorActors[6];       // 0x180
    u32 iconChars[3];                     // 0x1b0
    u32 iconPalette;                      // 0x1bc
    u32 iconCellAnims;                    // 0x1c0
    ClActUnit *iconUnit;                  // 0x1c4
    BtlvInputActor iconActors[3];         // 0x1c8
    u32 moveChars[4];                     // 0x1e0
    u32 movePalette;                      // 0x1f0
    u32 moveCellAnims;                    // 0x1f4
    ClActUnit *moveUnit;                  // 0x1f8
    ClActor *moveActors[4];               // 0x1fc
    ClActUnit *unit20c;                   // 0x20c
    ClActor *actors210[2];                // 0x210
    ClActUnit *unit218;                   // 0x218
    ClActor *actor21c;                    // 0x21c
    BOOL noUsableMove;                    // 0x220
    Font *font;                           // 0x224
    BmpWin *msgWin;                       // 0x228
    GFLBitmap *msgBitmap;                 // 0x22c
    BmpWin *subWin;                       // 0x230
    GFLBitmap *subBitmap;                 // 0x234
    TCB *coreTask;                        // 0x238
    BOOL inBattle;                        // 0x23c
    HeapID heapId;                        // 0x240
    u8 lastCursor[3][8];                  // 0x242  [pokeIndex][screen]
    u8 buttonEnabled[8];                  // 0x25a
    u8 moveExists[4];                     // 0x262
    u16 lastMove[3];                      // 0x266  [pokeIndex]
    u16 moves[4];                         // 0x26c
    u8 *keyCursorFlag;                    // 0x274
    u32 pokePos;                          // 0x278
    s32 pokeIndex;                        // 0x27c
    BOOL noMoveInfo;                      // 0x280  the move screen's parameter
    u16 savedPalette[16];                 // 0x284
    BattleMon *mons[3];                   // 0x2a4
    BOOL moveUsable[3][4];                // 0x2b0
}; // 0x2e0

// The sound and map helpers after this file's range, in the gap before btlv_gauge.c; they take the work
u32 func_ov094_02206a7c(BtlvInput *work);
void func_ov094_02206af8(BtlvInput *work);
void func_ov094_02206b14(BtlvInput *work);
void func_ov094_02206b30(BtlvInput *work);

// pokebw2's BtlvInput_IsBusy
BOOL func_ov094_022033cc(BtlvInput *work);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BtlvInput_CreateSimple
BtlvInput *func_ov094_02202058(GameData *gameData, u32 rule, PaletteFade *paletteFade, Font *font, u8 *keyCursorFlag,
                                  HeapID heapId);
// pokebw2's BtlvInput_CheckInput
s32 func_ov094_02202dc4(BtlvInput *work, const BtlvInputButtonSet *set, const BtlvInputKeyStop *stops);
// pokebw2's BtlvInput_StartPaletteFade
void func_ov094_022025d8(BtlvInput *work, u32 mode);
// pokebw2's BtlvInput_StartFadeInWithGraphics
void func_ov094_0220266c(BtlvInput *work);
// pokebw2's BtlvInput_SetScreen
void func_ov094_0220270c(BtlvInput *work, u32 screen, void *param);
// pokebw2's BtlvInput_CreateForBattle
BtlvInput *func_ov094_0220207c(GameData *gameData, u32 rule, u32 unk54, PaletteFade *paletteFade, Font *font,
                               u8 *keyCursorFlag, u32 scdState, HeapID heapId);

#endif // BWPORT_BATTLE_BTLV_INPUT_H
