// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/dpc_main.h: the lines in use so far.
#ifndef BWPORT_APP_DPC_MAIN_H
#define BWPORT_APP_DPC_MAIN_H

#include "types.h"
#include "gfl/clact.h"
#include "nitro/rtc.h"
#include "save/hall_of_fame.h"
#include "system/printsys.h"
#include "struct_decls.h"

// The actors: two sets of a team's Pokémon, of which one is shown while the next page's is loaded, and the buttons
enum {
    DPCOBJ_POKE_A,
    DPCOBJ_POKE_B = DPCOBJ_POKE_A + HALL_OF_FAME_TEAM_SIZE,
    DPCOBJ_ARROW_LEFT = DPCOBJ_POKE_B + HALL_OF_FAME_TEAM_SIZE,
    DPCOBJ_ARROW_RIGHT,
    DPCOBJ_CLOSE,
    DPCOBJ_RETURN,
    DPCOBJ_MAX,
};

// The actors' resources: one per Pokémon of both sets, and the buttons'
#define DPCOBJ_RES_BUTTON (HALL_OF_FAME_TEAM_SIZE * 2)

#define DPCOBJ_RES_MAX (DPCOBJ_RES_BUTTON + 1)

// The windows: the page's title on the touch screen and the chosen Pokémon's data on the top screen
enum {
    DPCBMP_WIN_TITLE,
    DPCBMP_WIN_INFO,
    DPCBMP_WIN_MAX,
};

// A team as the screen shows it
typedef struct {
    HallOfFamePokemon pokemon[HALL_OF_FAME_TEAM_SIZE];
    RTCDate date;
    u16 count;
    // Which entry into the Hall of Fame the team was
    u16 number;
} DendouPCTeam;

// Black 2's; twinned code confirms the offsets of param, teams, angle, angleNext, vblankTask, palFade, fade, fadeNow,
// fadeNext, win, font, smallFont, msgData, wordSet, expandBuf, printQueue, unit, actors, chars, palettes, cellAnims,
// bmpOam, pageActor, pageBitmap, pokePos, pokeNext, page, pageMax, objSwap, speed, stepCount, animeObj, animeNext, seq,
// subSeq and wipeNext in Black
typedef struct {
    DendouPCParam *param;
    u32 unk4[2];
    DendouPCTeam teams[HALL_OF_FAME_TEAMS];
    // Where each Pokémon is on the ring, in degrees, and where the turn takes it
    s16 angle[HALL_OF_FAME_TEAM_SIZE];
    s16 angleNext[HALL_OF_FAME_TEAM_SIZE];
    TCB *vblankTask;
    u32 unkB2C;
    PaletteFade *palFade;
    // How dark each Pokémon is by its place on the ring, now, and after the turn
    u8 fade[HALL_OF_FAME_TEAM_SIZE];
    u8 fadeNow[HALL_OF_FAME_TEAM_SIZE];
    u8 fadeNext[HALL_OF_FAME_TEAM_SIZE];
    PrintWindow win[DPCBMP_WIN_MAX];
    Font *font;
    Font *smallFont;
    MsgData *msgData;
    WordSet *wordSet;
    StrBuf *expandBuf;
    PrintQueue *printQueue;
    ClActUnit *unit;
    ClActor *actors[DPCOBJ_MAX];
    u32 chars[DPCOBJ_RES_MAX];
    u32 palettes[DPCOBJ_RES_MAX];
    u32 cellAnims[DPCOBJ_RES_MAX];
    // The page number, a bitmap shown as OBJs
    BmpOamSys *bmpOam;
    BmpOamActor *pageActor;
    GFLBitmap *pageBitmap;
    // The Pokémon at the front of the ring, and the one chosen to come there
    s8 pokePos;
    s8 pokeNext;
    s8 page;
    u8 pageMax;
    // Which set of actors the next page loads
    u8 objSwap;
    // The turn of the ring: degrees per frame and the frames left
    s8 speed;
    u8 stepCount;
    // The button whose animation DPCSEQ_BUTTON_ANIME waits for, and the sequence after it
    u32 animeObj;
    u32 animeNext;
    u32 seq;
    u32 subSeq;
    // The sequence after the wipe
    u32 wipeNext;
} DendouPCWork;

// pokebw2's DpcMain_LoadTeams
// Loads the teams of the screen's mode from the save
void func_ov171_021dccc8(DendouPCWork *wk);
// pokebw2's DpcMain_InitBg
void func_ov171_021dc92c(void);
// pokebw2's DpcMain_InitPaletteFade
void func_ov171_021dcb70(DendouPCWork *wk);
// pokebw2's DpcMain_SetBlend
void func_ov171_021dcc0c(void);
// pokebw2's DpcMain_InitVBlank
void func_ov171_021dc8b8(DendouPCWork *wk);
// pokebw2's DpcMain_ExitVBlank
void func_ov171_021dc8d4(DendouPCWork *wk);
// pokebw2's DpcMain_ExitMsg
void func_ov171_021dcc88(DendouPCWork *wk);
// pokebw2's DpcMain_ExitPaletteFade
void func_ov171_021dcb8c(DendouPCWork *wk);
// pokebw2's DpcMain_ExitBg
void func_ov171_021dca40(void);
// pokebw2's DpcMain_FreeTeams
void func_ov171_021dce28(DendouPCWork *wk);
// pokebw2's DpcMain_BlendPokePalettes
// Darkens each Pokémon's palette by its fadeNow, and whether that is still being sent
void func_ov171_021dcba4(DendouPCWork *wk);
// pokebw2's DpcMain_IsPaletteFading
BOOL func_ov171_021dcbe8(DendouPCWork *wk);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's DpcMain_GetVramBanks
const BGSysVRAMConfig *func_ov171_021dc924(void);
// pokebw2's DpcMain_InitVram
void func_ov171_021dc910(void);
// pokebw2's DpcMain_LoadBgGraphic
void func_ov171_021dca7c(void);
// pokebw2's DpcMain_InitMsg
void func_ov171_021dcc24(DendouPCWork *wk);

#endif // BWPORT_APP_DPC_MAIN_H
