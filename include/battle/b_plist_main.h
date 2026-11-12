// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/b_plist_main.h, the lines in use so
// far.
#ifndef BWPORT_BATTLE_B_PLIST_MAIN_H
#define BWPORT_BATTLE_B_PLIST_MAIN_H

#include "types.h"
#include "gfl/heap.h"
#include "gfl/clact.h"
#include "struct_decls.h"
#include "system/cursor_move.h"
#include "system/printsys.h"
#include "gfl/overlay.h"

// The battle party list in overlay 198 (b_plist_main.c and the other b_plist_*.c files, the first named by the ROM's
// embedded string): the party shown during a battle to switch a Pokémon in, see its summary and moves, or pick a
// move to forget. The type and field names are pokebw2's; swan has none for this overlay.

typedef struct {
    GameData *gameData;
    PokeParty *party;
    PokeParty *unk8;
    Font *font;
    HeapID heapId;
    u32 unk14;
    BOOL unk18; // TRUE in a multi battle
    u8 unk1C;
    u8 unk1D[2];
    u8 unk1F;
    u8 partyIndex;
    u8 unk21;
    u8 unk22;
    u8 unk23;
    u16 item;
    u16 move;
    TCBManager *tcbManager;
    PaletteFade *paletteFade;
    u32 unk30;
    u32 unk34; // 1 forces the exit
    u8 unk38[6];
    u8 unk3E[2];
    u32 unk40; // 1 plays the sound effects
    u8 *usingKeys;
    u8 unk48[3];
    // The slot of the move to forget, or 4 for none
    u8 slot;
    u8 done;
} BPlistParam;

// A move of a BPlistPokemon
struct BPlistMove {
    u16 move;
    u8 pp;
    u8 maxPp;
    u8 type;
    u8 category;
    u8 accuracy; // 0 when the move always hits
    u8 power;
};

// One Pokémon as the list shows it
struct BPlistPokemon {
    PartyPkm *pkm; // 0x00  NULL for an empty slot
    u16 species;   // 0x04  0 for an empty slot
    u16 attack;    // 0x06
    u16 defense;   // 0x08
    u16 speed;     // 0x0a
    u16 spAttack;  // 0x0c
    u16 spDefense; // 0x0e
    u16 hp;        // 0x10
    u16 maxHp;     // 0x12
    u8 type1;      // 0x14
    u8 type2;      // 0x15
    u8 level : 7;  // 0x16
    u8 hideSex : 1;
    u8 sex : 3; // 0x17
    u8 status : 4;
    u8 isEgg : 1;
    u16 ability;         // 0x18
    u16 item;            // 0x1a
    u32 exp;             // 0x1c
    u32 levelExp;        // 0x20
    u32 nextLevelExp;    // 0x24
    u32 form;            // 0x28
    BPlistMove moves[4]; // 0x2c
}; // 0x4c

// The list's work, shared by the overlay's files. Its fields from 0x1e9c on are in pokebw2's header, still to be
// checked against Black.
struct BPlistWork {
    BPlistParam *param;       // 0x0000
    BPlistPokemon pokemon[6]; // 0x0004
    u8 partyOrder[6];         // 0x01cc  the party slot at each list position
    TCBExManager *tcbExMgr;   // 0x01d4
    PaletteFade *paletteFade; // 0x01d8
    // Screen pieces cut from the list's tilemaps, by button
    u16 plateScrn[2][4][16 * 6];   // 0x01dc  buttons 0-5: [source column][state]
    u16 moveButtonScrn[4][13 * 5]; // 0x07dc  buttons 8-11
    u16 button12Scrn[4][5 * 5];    // 0x09e4
    u16 button13Scrn[4][5 * 5];    // 0x0aac
    u16 button6Scrn[4][5 * 5];     // 0x0b74
    u16 button7Scrn[3][30 * 17];   // 0x0c3c
    u16 button14Scrn[4][16 * 6];   // 0x1830  buttons 14-26
    u16 button27Scrn[3][26 * 5];   // 0x1b30
    u16 button28Scrn[3][5 * 2];    // 0x1e3c  buttons 28-31
    u16 savedPalette[16];          // 0x1e78
    u8 animSeq;                    // 0x1e98  also the swap animation's
    u8 animCount;                  // 0x1e99
    u8 animButton;                 // 0x1e9a
    u8 : 4;                        // 0x1e9b
    u8 animMode : 3;
    u8 animActive : 1;
    Font *smallFont;              // 0x1e9c
    MsgData *msgData;             // 0x1ea0
    WordSet *wordSet;             // 0x1ea4
    StrBuf *strBuf;               // 0x1ea8
    PrintQueue *printQueue;       // 0x1eac
    PrintStream *printStream;     // 0x1eb0
    BOOL unk_1eb4;                // 0x1eb4
    ClActUnit *unk_1eb8;          // 0x1eb8  the list's actor unit, the actors' parent
    ClActor *actors[41];          // 0x1ebc  BPLIST_ACTOR_MAX of them
    PrintWindow msgWins[2]; // 0x1f60
    PrintWindow windows[64]; // 0x1f70
    BmpWin *dummyWin;             // 0x2170
    u8 windowCount;               // 0x2174
    u8 dummyWinState : 5;        // 0x2175  1 when the dummy window is to be made
    u8 pokeTypeIconBuf : 1;       // 0x2175:5  which pair of the type icons' actors and chars is next (pokebw2's name)
    u8 typeIconBuf : 1;           // 0x2175:6  which pair of the move type icons' actors is next (pokebw2's name)
    u8 categoryIconBuf : 1;       // 0x2175:7  the category icon's actor and chars, toggled each load (pokebw2's name)
    u8 unk_2176;                  // 0x2176  the list's first position
    u8 pageChangeSeq : 4;         // 0x2177  the page change's state
    u8 infoFrameState : 4;        // 1 while the info frame is to be drawn
    u32 unk_2178;                 // 0x2178  the state the task runs next
    u32 unk_217c;                 // 0x217c  the state a message hands to
    u8 unk_2180;                  // 0x2180  the page
    u8 unk_2181[3];               // 0x2181
    CursorMove *cursorMove;       // 0x2184  the key cursor, see b_plist_cursor.c
    BAppCursor *cursor;           // 0x2188
    BOOL cursorVisible;           // 0x218c
    u8 unk_2190[3];               // 0x2190
    u8 unk_2193;                  // 0x2193
    BGWinFrame *unk_2194;         // 0x2194
    u8 unk_2198[0x2498 - 0x2198];
    u8 unk_2498;                  // 0x2498  the position the message was opened for
    u8 unk_2499[0x249c - 0x2499];
    u16 swaps[2][2];              // 0x249c
    u32 unk_24a4[26];             // 0x24a4  the first of three id arrays, cleared to -1 by func_ov198_0220e2f8
    u32 unk_250c[6];              // 0x250c
    u32 unk_2524[6];              // 0x2524
    const u8 *flushList;          // 0x253c
    BOOL unk_2540;                // 0x2540  set once the list is initialized
};

// pokebw2's BPlistMain_CheckPos, BPlistMain_IsPartnerSlot, BPlistMain_GetPartySlot and BPlistMain_PopSwap
int func_ov198_02210b00(BPlistWork *work, int pos);
BOOL func_ov198_02210f04(BPlistWork *work, u32 idx);
u8 func_ov198_022110d4(BPlistWork *work, int pos); // the party slot at a list position
BOOL func_ov198_02211128(BPlistWork *work, u8 *pos1, u8 *pos2, BOOL clear);

// pokebw2's BPlistMain_GetSwitchError
int func_ov198_02210da8(BPlistWork *work);

// pokebw2's BPlistMain_Start
void func_ov198_0220f3e8(BPlistParam *param);

// Black 2's; twinned code confirms the values of BPLIST_ACTOR_ITEM, BPLIST_ACTOR_POKE, BPLIST_ACTOR_STATUS,
// BPLIST_ACTOR_UNK1F08, BPLIST_ACTOR_POKE_TYPE, BPLIST_ACTOR_MOVE_TYPE and BPLIST_ACTOR_CATEGORY in Black
// The first of each kind of actor in BPlistWork's actors, which overlays its arrays of them
enum {
    BPLIST_ACTOR_ITEM = 0,
    BPLIST_ACTOR_POKE = 7,
    BPLIST_ACTOR_STATUS = 13,
    BPLIST_ACTOR_UNK1F08 = 19,
    BPLIST_ACTOR_POKE_TYPE = 25,
    BPLIST_ACTOR_MOVE_TYPE = 29,
    BPLIST_ACTOR_CATEGORY = 39,
    BPLIST_ACTOR_MAX = 41,
};

// Black 2's, not checked against Black
#define OVERLAY_OV285 OVERLAY_ID(285)
#define OVERLAY_OV287 OVERLAY_ID(287)

#endif // BWPORT_BATTLE_B_PLIST_MAIN_H
