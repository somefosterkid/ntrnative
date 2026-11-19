#ifndef POKEHEARTGOLD_OVERLAY_14_IRC_BATTLE_MATCH_H
#define POKEHEARTGOLD_OVERLAY_14_IRC_BATTLE_MATCH_H

// IrcBattleMatch: merged by tools/hg/promote.py from 1 m2c local copies (1 variants, base
// src/overlay_14_021F2BE8_021F2BE8.c) and 3 accesses; fields at every accessed offset, pads elsewhere.

struct AppTaskMenu;
struct AppTaskMenuRes;
struct AppTaskMenuWin;
struct BmpWin;
struct ButtonMan;
struct ClActUnit;
struct ClActor;
struct Font;
struct IrcBattleMatchParam;
struct MsgData;
struct PrintQueue;
struct StrBuf;
struct TCB;
struct WordSet;

typedef struct IrcBattleMatch {
    u16 palettes[16][18];
    u16 paletteFrame;
    u16 paletteHeapId;
    struct IrcBattleMatchParam *param;
    void *state;
    s32 mode;
    s32 modes[4];
    u8 macs[6][4];
    u16 heapId;
    u8 pad_27A[0x2];
    struct BmpWin *messageWindow;
    struct BmpWin *countWindow;
    u32 unk284;
    struct MsgData *msgData;
    struct WordSet *wordSet;
    struct Font *font;
    struct StrBuf *strbuf;
    struct StrBuf *strbufFormatted;
    u32 unk29C[5];
    struct TCB *vblankTcb;
    struct ClActUnit *actUnit;
    u32 actorPalette;
    u32 actorChars;
    u32 actorCells;
    u32 unk2C4;
    u8 pad_2C8[0x2c];
    u32 unk_2F4;
    u32 unk_2F8;
    struct ClActor *actors[8];
    u32 acceptedPlayers;
    BOOL enoughPlayers;
    u32 unk2F0[3];
    u32 bgChars;
    u32 unk300;
    u32 windowFrameChars;
    u32 menuBgChars;
    struct TCB *unk30C;
    u32 unk310;
    u8 unk314[2408];
    BOOL linked;
    BOOL messagePending;
    BOOL bannerActive;
    s32 bannerFrames;
    BOOL linkChanged;
    u32 friendResult;
    BOOL linkChangePending;
    s32 linkChangeFrames;
    s32 scrollY;
    BOOL connectDone;
    struct ButtonMan *buttonMan;
    u32 unkCA8;
    s32 counter;
    u32 currentMsgId;
    void *paletteBackup;
    struct PrintQueue *printQueue;
    BOOL backOnly;
    struct AppTaskMenu *menu;
    struct AppTaskMenuRes *menuRes;
    struct { struct StrBuf *str; u16 color; u8 unk_8_pad[0x2]; u32 type; } menuItems[2];
    struct AppTaskMenuWin *taskWin0;
    struct AppTaskMenuWin *taskWin1;
    s32 shownCount;
    BOOL palettePulse;
} IrcBattleMatch;

// sizeof(IrcBattleMatch) == 0xd24

#endif // POKEHEARTGOLD_OVERLAY_14_IRC_BATTLE_MATCH_H
