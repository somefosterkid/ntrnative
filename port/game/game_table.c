// A game's table for the host (port/include/port_game.h): compiled into each game; White's is w_bwgame (on the PC its
// names are renamed w_ anyway; on Apple the one global its relocatable object keeps: port/meson.build), Black 2's
// b2_bwgame and White 2's w2_bwgame (pokebw2's C, the maintainers' notes), Platinum's pl_bwgame (pret's C, docs/platinum.md),
// HeartGold's hg_bwgame (pret's pokeheartgold, docs/hgss.md).
#include "port_game.h"
#if !defined(PLATINUM) && !defined(HEARTGOLD)
#include "work_area.h"
#endif
#ifdef __APPLE__   // the host reads the table before the ROM fill: not in the game's __bwrom_* (macos_rom_section.h)
#pragma clang section rodata = "__DATA,__bwhro" relro = "__DATA,__bwhro"
#endif
// (Black 2 and White 2 carry their overlays' markers and DS layout on every platform: on the PC Black's and White's are the linker
// script's table, port/src/ovl.c)

void NitroMain(void);
extern unsigned char g_GFLSndHeap[];
void GFL_RandomUpdateMT(unsigned seed);
void PortGame_PinLC(void);   // port/extern*/src/system/gfl_use.c
#if defined(BWPORT_OVL_MACHO) || defined(BLACK2) || defined(WHITE2) || defined(PLATINUM) || defined(HEARTGOLD)
extern const unsigned bwovl_count;
extern const unsigned bwovl_ds[][4];
extern unsigned char *const bwovl_dptr[];
extern unsigned char *const bwovl_bptr[];
#endif

#if defined(PLATINUM)
#include "sound_system.h"   // pret's
void MTRNG_SetSeed(unsigned seed);   // pret's src/math_util.c
void LCRNG_SetSeed(unsigned seed);   // the same file: its 32-bit linear congruential generator (sLCRNGState)
// Its sound heap is SoundSystem's heapBuffer (pret's src/sound_system.c, not a global): the table's getter finds it, for
// acapture.c before Platinum runs (sSoundSystem is static storage). Its LCG is pinned as Black's is (port/src/bench_pin.c:
// BWPORT_BENCH_SEED, tools/route.py's --pin): the low half of Black's 0x0123456789ABCDEF, the state being 32 bits here (main.c's
// boot seeds both generators from the clock and the V-blank count, the same value). Battle speed's overlays by pret's names (platinum.us/main.lsf.in):
// battle 16, battle_sub_menus 13 (the battle's Bag and party screen); the encounter effects are in the field's overlay 5,
// so the battle's own stands for them
static unsigned char *PlatSndHeap(void) {
    return SoundSystem_Get()->heapBuffer;
}
static void PlatPinLC(void) {
    LCRNG_SetSeed(0x89ABCDEFu);
}
// BWPORT_PLATTRACE=FILE: one line per V-blank wait (the host's V-blank count, the game's counters, the LCG), to find the
// first frame two runs of the same keys part
#include <stdio.h>
#include <stdlib.h>
#include "system.h"
#include "screen_fade.h"
#include "sound.h"
#include "sound_playback.h"
#include "savedata.h"
#include "save_player.h"
#include "game_options.h"
u32 LCRNG_GetSeed(void);
// title_fork.c's leg forked at the title: its own save.bin read into the boot's SaveData (main.c's SaveData_Init, before the
// title), and the sound mode its options say (SoundSystem_Init took it from the boot's save)
static void PlatSaveReload(void) {
    SaveData_PortReload();
    Sound_SetPlaybackMode(SaveData_GetOptions(SaveData_Ptr())->soundMode);
}
static void PlatFrameHook(void) {
    static FILE *f;
    static int state;
    if (state == 0) {
        const char *e = getenv("BWPORT_PLATTRACE");
        state = e != NULL && *e && (f = fopen(e, "w")) != NULL ? 1 : -1;
    }
    if (state == 1) {
        fprintf(f, "%u %u %u %08x %08x pv%d bgm%d fade%d sfade%d\n", OS_GetVBlankCount(), (unsigned)gSystem.vblankCounter,
                (unsigned)gSystem.frameCounter, LCRNG_GetSeed(), (unsigned)gSystem.heldKeys, Sound_GetNumberOfPlayingSequencesForPlayer(0),
                Sound_GetNumberOfPlayingSequencesForPlayer(1), IsScreenFadeDone(), Sound_IsFadeActive());
        fflush(f); // a run ends by gdb's SIGINT: an unflushed buffer cut the file at a 4 KB boundary
    }
}
// Its entry: the SDK state its DS starts with where the exe's is Black's. GX's DMA channel (GXi_DmaId, the SDK's default 3 in
// Platinum, which never sets it) is Black's data's 0 here, and libntr's DMA 0 check stops at a host address (above the DS's
// 0x08000000) in Platinum's first GX_LoadOBJ.
// The music players (libntr's snd_interface.c SIM_Snd_MusicPlayerMask, BWPORT_MUSICPLAYERS, default 0x41: Black's BGM and
// BGM2) run on the real-time clock when the game is fast-forwarded or sped up (libntr-mu83.patch's music bus). Platinum's
// player 0 is PLAYER_PV, the cries (res/sound/pl_sound_data.json's playerInfo): with Black's mask a cry lasted its wall time
// at any speed and the title's wait for Giratina's cry (title_screen.c, Sound_IsPokemonCryPlaying) ended on a different
// frame each run. Its music is PLAYER_FIELD (1) and PLAYER_BGM (7); PLAYER_ME (2, the jingles the game waits for) stays on
// the frame clock
#if defined(_WIN32)
#define PLAT_SETENV(n, v) do { if (getenv(n) == NULL) _putenv_s(n, v); } while (0)
#else
#define PLAT_SETENV(n, v) setenv(n, v, 0)
#endif
extern u32 GXi_DmaId;
static void PlatMain(void) {
    GXi_DmaId = GX_DEFAULT_DMAID;
    PLAT_SETENV("BWPORT_MUSICPLAYERS", "82");
    NitroMain();
}
// The host reads the code before the ROM fill (PORT_GameByCode, the launcher), and "CPUE" is in Platinum's ARM9 too: the fill zeroed
// the literal in pl.o's .bwrom_pl (this build then ran Black). In .bwhro, the section the fill leaves as linked (tools/port_overlays.py)
// (Apple: the pragma above puts it in __DATA,__bwhro)
#ifdef __APPLE__
static const char plCode[] = "CPUE";
#else
__attribute__((section(".bwhro"))) static const char plCode[] = "CPUE";
#endif
const PortGame pl_bwgame = {plCode, PlatMain, NULL, SOUND_SYSTEM_HEAP_SIZE, MTRNG_SetSeed, &bwovl_count, bwovl_ds, bwovl_dptr, bwovl_bptr,
    .ovlBattle = 16, .ovlEncFirst = 16, .ovlEncLast = 16, .ovlBag = 13, .ovlParty = 13, .pinLC = PlatPinLC, .sndHeapFn = PlatSndHeap,
    .frameHook = PlatFrameHook, .saveReload = PlatSaveReload};
#elif defined(HEARTGOLD)
#include "sound.h"   // pret's: SND_HEAP_SIZE, GetSoundDataPointer
void SetMTRNGSeed(u32 seed);   // pret's src/math_util.c (u32: unsigned long on win64's libntr types, not unsigned)
// Its sound heap is SND_WORK's heap_buf (pret's src/sound.c keeps the struct to itself, sSoundWork): the same head, so the host's
// layout finds it (NNSSndArc's and the handle's size are the host's here)
struct HgSndWorkHead {
    NNSSndArc arc;
    NNSSndHeapHandle heap;
    unsigned char heap_buf[1];
};
static unsigned char *HgSndHeap(void) {
    return (unsigned char *)GetSoundDataPointer() + offsetof(struct HgSndWorkHead, heap_buf);
}
// Its LCG pinned as Platinum's (PlatPinLC: BWPORT_BENCH_SEED, port/src/bench_pin.c), pret's src/math_util.c
#include "math_util.h"
static void HgPinLC(void) {
    SetLCRNGSeed(0x89ABCDEFu);
}
// BWPORT_HGTRACE=FILE: one line per V-blank wait (the host's V-blank count, the game's counters, the LCG, the held keys), as
// Platinum's BWPORT_PLATTRACE: the first frame two runs of the same keys part
#include <stdio.h>
#include <stdlib.h>
#include "system.h"
static void HgFrameHook(void) {
    static FILE *f;
    static int state;
    if (state == 0) {
        const char *e = getenv("BWPORT_HGTRACE");
        state = e != NULL && *e && (f = fopen(e, "w")) != NULL ? 1 : -1;
    }
    if (state == 1) {
        fprintf(f, "%u %u %u %08x %08x\n", OS_GetVBlankCount(), (unsigned)gSystem.vblankCounter, (unsigned)gSystem.frameCounter,
                (unsigned)GetLCRNGSeed(), (unsigned)gSystem.heldKeys);
        fflush(f);   // a run ends by gdb's SIGINT
    }
}
// Its entry: GX's DMA channel as Platinum's (PlatMain). Battle speed's overlays by pret's names (main.lsf.in): the battle (12,
// overlay_12_battle_*) stands for the encounter effects; its Bag and party screen are not found yet, an overlay that is never
// loaded (11) stands in.
// The music players on the real-time clock (BWPORT_MUSICPLAYERS, see PlatMain): HeartGold's player order is Platinum's
// (pret's include/constants/sndseq.h: PLAYER_PV 0, the cries; PLAYER_FIELD 1; PLAYER_ME 2, the jingles the game waits for;
// PLAYER_BGM 7; PLAYER_OPED 8, the opening's music, stays on the frame clock as the intro's scenes wait on it): 0x82
#if defined(_WIN32)
#define HG_SETENV(n, v) do { if (getenv(n) == NULL) _putenv_s(n, v); } while (0)
#else
#define HG_SETENV(n, v) setenv(n, v, 0)
#endif
extern u32 GXi_DmaId;
// HeartGold and SoulSilver are one module: pret chooses the version with a define (GAME_VERSION), the PC build with the value
// gGameVersion (include/pm_version.h, port/subprojects/pokeheartgold-zversion.patch), set here before the game starts. The two
// ROMs have one file system (the same file ids), so nothing else differs
#include "config.h"
#include "pm_version.h"
static void HgMain(void) {
    gGameVersion = VERSION_HEARTGOLD;
    GXi_DmaId = GX_DEFAULT_DMAID;
    HG_SETENV("BWPORT_MUSICPLAYERS", "82");
    NitroMain();
}
static void SsMain(void) {
    gGameVersion = VERSION_SOULSILVER;
    GXi_DmaId = GX_DEFAULT_DMAID;
    HG_SETENV("BWPORT_MUSICPLAYERS", "82");
    NitroMain();
}
// title_fork.c's leg forked at the title: its own save.bin read into the boot's SaveData (main.c's SaveData_New, before the
// title; pokeheartgold-zsavereload.patch), and the mono flag its options say (InitSoundData took it from the boot's save)
#include "save.h"
#include "player_data.h"
#include "sound.h"
static void HgSaveReload(void) {
    SaveData_PortReload();
    GF_SndSetMonoFlag(Save_PlayerData_GetOptionsAddr(SaveData_Get())->soundMethod);
}
const PortGame hg_bwgame = {"IPKE", HgMain, NULL, SND_HEAP_SIZE, SetMTRNGSeed, &bwovl_count, bwovl_ds, bwovl_dptr, bwovl_bptr,
    .ovlBattle = 12, .ovlEncFirst = 12, .ovlEncLast = 12, .ovlBag = 11, .ovlParty = 11, .pinLC = HgPinLC, .sndHeapFn = HgSndHeap,
    .frameHook = HgFrameHook, .saveReload = HgSaveReload};
const PortGame ss_bwgame = {"IPGE", SsMain, NULL, SND_HEAP_SIZE, SetMTRNGSeed, &bwovl_count, bwovl_ds, bwovl_dptr, bwovl_bptr,
    .ovlBattle = 12, .ovlEncFirst = 12, .ovlEncLast = 12, .ovlBag = 11, .ovlParty = 11, .pinLC = HgPinLC, .sndHeapFn = HgSndHeap,
    .frameHook = HgFrameHook, .saveReload = HgSaveReload};
#else
#if defined(BLACK2)
const PortGame b2_bwgame = {"IREO",
#elif defined(WHITE2)
const PortGame w2_bwgame = {"IRDO",
#elif defined(WHITE)
const PortGame w_bwgame = {"IRAO",
#else
const PortGame bwgame = {"IRBO",
#endif
    NitroMain, g_GFLSndHeap,
#if defined(BLACK2) || defined(WHITE2)
    WORK_AREA_SIZE(0x9d000),   // pokebw2's src/system/snd_sys.c
#else
    WORK_AREA_SIZE(0x95ad0),   // src/system/snd_sys.c
#endif
    GFL_RandomUpdateMT,
#if defined(BWPORT_OVL_MACHO) || defined(BLACK2) || defined(WHITE2)
    &bwovl_count, bwovl_ds, bwovl_dptr, bwovl_bptr,
#endif
#if defined(BLACK2) || defined(WHITE2)
    .ovlBattle = 167, .ovlEncFirst = 148, .ovlEncLast = 154, .ovlBag = 286, .ovlParty = 287,   // Black 2's, White 2's
#else
    .ovlBattle = 93, .ovlEncFirst = 76, .ovlEncLast = 79, .ovlBag = 197, .ovlParty = 198,
#endif
    .pinLC = PortGame_PinLC,
};
#endif
