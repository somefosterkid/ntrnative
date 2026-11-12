// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov086/dowsing.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/arc.h"
#include "constants/flags.h"
#include "constants/sound.h"
#include "constants/zones.h"
#include "field/dowsing.h"
#include "field/field.h"
#include "field/field_map.h"
#include "field/ov021_021cfa58.h"
#include "field/hidden_item.h"
#include "field/player_state.h"
#include "field/subscreen.h"
#include "field/zone.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/net.h"
#include "gfl/sound.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/ioreg.h"
#include "save/event_work.h"
#include "save/save_control.h"
#include "system/app_menu_common.h"
#include "system/bmp_winframe.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/infowin.h"
#include "system/palanm.h"
#include "gfl/arc.h"

// The Dowsing MCHN's touch screen, named after the ROM's "dowsing.c". Every frame it looks for the closest hidden item
// in the area around the player, picks a signal from the map of the archive (how near it is), and shows it with the
// sound that goes with it. The names are ours

// The event flag that says the Hidden Grotto's entrance is waiting to be found
#define EVENT_FLAG_HIDDEN_GROTTO_FOUND EVENT_FLAG_0x097f

// The zone of no zone, for a position outside the map
#define ZONE_NONE 0xffff

// What the search found: nothing or a hidden item
#define HIT_NONE 0
#define HIT_HIDDEN_ITEM 1

// The area that is searched around the player, in tiles: the signal map is 32 wide, the player at its (15, 11)
#define MAP_WIDTH 32
#define MAP_PLAYER_X 15
#define MAP_PLAYER_Z 11
#define SEARCH_LEFT 15
#define SEARCH_RIGHT 17
#define SEARCH_TOP 11
#define SEARCH_BOTTOM 13

// The signals, which are the values of the signal map. 8 is the closest, 9 is none
#define SIGNAL_CLOSEST 8
#define SIGNAL_NONE 9

// The sub screen's BGs and the touch bar's
#define BG_SIGNAL 7
#define BG_INFOWIN 4

// The icons
#define ICON_COUNT 4
#define ICON_RING 0
#define ICON_DOT 1
#define ICON_DIRECTION 2
#define ICON_CLOSEST 3

// The fade commands
#define FADE_CMD_NONE 0
#define FADE_CMD_IN 1
#define FADE_CMD_OUT 2

// The states of the exit button
#define BUTTON_WAIT 0
#define BUTTON_PRESSED 1
#define BUTTON_RELEASED 2
#define BUTTON_DONE 3

// Where the exit button is on the touch screen
#define BUTTON_X 0xe8
#define BUTTON_Y 0xa8
#define BUTTON_WIDTH 0x18
#define BUTTON_HEIGHT 0x18

// The hidden items are found in order, each time asking whether the next is still there to find
typedef struct {
    EventWork *eventWork;
    u16 index;
    u16 unk_6;
    HighLinkSave *unk_8;
    u16 unk_c;
} DowsingSearch;

// A hidden item that the search found
typedef struct {
    u32 type;
    const HiddenItem *item;
    // What func_ov021_021cfb68 found, for the second kind of search
    const void *unk8;
} DowsingHit;

typedef struct {
    u32 chars;
    u32 cellAnims;
    ClActor *actor;
} DowsingIcon;

// What a signal shows: the animation of each of the first three icons, where the direction icon is, and the pan of the
// sound, where 8 is the center
typedef struct {
    u32 anims0 : 3;
    u32 anims1 : 3;
    u32 anims2 : 3;
    u32 x : 8;
    u32 y : 8;
    u32 pan : 7;
} DowsingSignal;

struct DowsingWork {
    u16 heapId;
    u16 subscreenHeapId;
    FieldSubscreen *subscreen;
    Field *field;
    ClActUnit *unit;
    // The characters of the signal BG
    u32 signalChars;
    // The characters of the touch bar's BG
    u32 barChars;
    u32 buttonPalette;
    u32 buttonChars;
    u32 buttonCellAnims;
    ClActor *button;
    u32 buttonState;
    u32 iconPalette;
    DowsingIcon icons[ICON_COUNT];
    // The signal of each tile around the player, MAP_WIDTH wide
    u8 *signalMap;
    // The signal before this frame's and this frame's
    s32 prevSignal;
    s32 signal;
    // The signal the icons show
    s32 shownSignal;
    // The hit that this frame's signal comes from and the one of the frame before
    DowsingHit prevHit;
    DowsingHit hit;
    // Where the player was when the signal was last found
    s32 lastX;
    s32 lastY;
    s32 lastZ;
    u32 unk8C;
    s32 soundPlayer;
    BOOL soundOn;
    u16 soundTimer;
    BOOL restartIcon;
    TCB *vblankTCB;
    u8 frame;
    BOOL active;
    TCBManager *tcbManager;
    void *tcbMemory;
    PaletteFade *paletteFade;
    u32 fadeCmd;
    u32 fadeState;
    // Whether the search starts again, and whether it is the first since the screen opened
    BOOL searchRestart;
    BOOL searchFirst;
    DowsingSearch *search;
};

typedef struct {
    u32 bg;
    BGSetup setup;
    u32 mode;
    u32 enabled;
} DowsingBGSetup;

typedef struct {
    u8 file;
    u8 cellFile;
    u8 animFile;
} DowsingIconRes;

static DowsingSearch *func_ov033_021efec0(HeapID heapId, EventWork *eventWork, HighLinkSave *unk);
static void func_ov033_021efee4(DowsingSearch *search);
BOOL func_ov033_021efeec(DowsingSearch *search, const DowsingHit *hit);
void func_ov033_021eff40(DowsingSearch *search);
static BOOL func_ov033_021eff48(DowsingSearch *search, DowsingHit *hit, int mode);
void func_ov033_021f0674(TCB *tcb, void *data);
static void func_ov033_021f06ac(DowsingWork *work);
static void func_ov033_021f0738(DowsingWork *work);
static void func_ov033_021f0760(DowsingWork *work);
static void func_ov033_021f07dc(DowsingWork *work);
static void func_ov033_021f07f4(DowsingWork *work);
static void func_ov033_021f08ec(DowsingWork *work);
static void func_ov033_021f0924(DowsingWork *work);
static void func_ov033_021f0a90(DowsingWork *work);
void func_ov033_021f0ac4(DowsingWork *work);
static void func_ov033_021f0b74(DowsingWork *work);
static void func_ov033_021f0c0c(DowsingWork *work);
static void func_ov033_021f0c44(DowsingWork *work);

// The Hidden Grotto's entrance, as if it were a hidden item of the grotto's own zone
static const HiddenItem sHiddenGrottoItem = { 0, 0, 0, ZONE_HIDDEN_GROTTO, 15, 12 };

static const DowsingIconRes data_ov033_021f0dbc[ICON_COUNT] = {
    { 3, 4, 8 },
    { 3, 4, 8 },
    { 11, 5, 9 },
    { 12, 6, 10 },
};

static const DowsingSignal data_ov033_021f0dc8[10] = {
    { 0, 0, 0, 128, 88, 8 },
    { 1, 1, 1, 128, 104, 8 },
    { 3, 3, 3, 128, 64, 0 },
    { 2, 2, 2, 128, 64, 16 },
    { 5, 5, 5, 128, 88, 4 },
    { 7, 7, 7, 128, 104, 4 },
    { 4, 4, 4, 128, 88, 12 },
    { 6, 6, 6, 128, 104, 12 },
    { 4, 5, 0, 128, 152, 0 },
    { 5, 4, 0, 128, 88, 0 },
};

static const DowsingBGSetup data_ov033_021f0df0[] = {
    { BG_INFOWIN,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x7000, GX_BG_CHARBASE_0x04000, 0x2000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
    { BG_SIGNAL,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x6000, GX_BG_CHARBASE_0x00000, 0x4000,
        GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
};

// A position from a hit's unk8: the three coordinates and one more value (named by offset)
typedef struct {
    s16 unk_0;
    s16 unk_2;
    s16 unk_4;
    u16 unk_6;
} DowsingHitPos;

// Black's overlay 10 function that gives the hidden items table and its count
const HiddenItem *func_ov010_0216476c(u16 *count);

void func_02024abc(void);
int RangeCheckChunkCoordinateWorld(MapMatrix *matrix, s32 x, s32 z);
u32 GetZoneIDAtMatrixXZWorld(MapMatrix *matrix, s32 x, s32 z);

// pokebw2's DowsingSearch_Create
static DowsingSearch *func_ov033_021efec0(HeapID heapId, EventWork *eventWork, HighLinkSave *unk) {
    DowsingSearch *search = GFL_HeapAllocate(heapId, sizeof(DowsingSearch), TRUE, "dowsing.c", 141);
    search->eventWork = eventWork;
    search->index = 0;
    search->unk_8 = unk;
    search->unk_c = 0;
    return search;
}

// pokebw2's DowsingSearch_Free
static void func_ov033_021efee4(DowsingSearch *search) {
    GFL_HeapFree(search);
}

BOOL func_ov033_021efeec(DowsingSearch *search, const DowsingHit *hit) {
    if (hit->type == HIT_HIDDEN_ITEM) {
        return !EventWork_FlagGet(search->eventWork, hit->item->index + HIDDEN_ITEM_FLAG_START);
    }
    if (hit->type == 2) {
        const DowsingHitPos *pos = hit->unk8;
        return func_ov021_021cfac0(search->unk_8, pos->unk_6, pos->unk_0, pos->unk_2, pos->unk_4) != 0xff;
    }
    return FALSE;
}


// pokebw2's DowsingSearch_Reset
void func_ov033_021eff40(DowsingSearch *search) {
    search->index = 0;
    search->unk_c = 0;
}

static BOOL func_ov033_021eff48(DowsingSearch *search, DowsingHit *hit, int mode) {
    u16 count;
    s32 zoneId;
    BOOL found = FALSE;
    const HiddenItem *items;
    DowsingHit candidate;
    const void *unk8;

    items = func_ov010_0216476c(&count);
    zoneId = search->unk_c;
    if (mode == 0 || mode == 1) {
        while (search->index < count) {
            candidate.type = HIT_HIDDEN_ITEM;
            candidate.item = &items[search->index];
            if (func_ov033_021efeec(search, &candidate)) {
                found = TRUE;
                hit->type = candidate.type;
                hit->item = candidate.item;
            }
            search->index++;
            if (found) {
                return TRUE;
            }
        }
    }
    if (mode == 0 || mode == 2) {
        while ((unk8 = func_ov021_021cfb68(search->unk_8, &zoneId), search->unk_c = zoneId, unk8 != NULL)) {
            candidate.unk8 = unk8;
            candidate.type = 2;
            if (func_ov033_021efeec(search, &candidate)) {
                found = TRUE;
                hit->type = candidate.type;
                hit->unk8 = candidate.unk8;
            }
            if (found) {
                return TRUE;
            }
        }
    }
    hit->type = HIT_NONE;
    return FALSE;
}


// pokebw2's Dowsing_Create
DowsingWork *func_ov033_021f000c(HeapID heapId, HeapID subscreenHeapId, FieldSubscreen *subscreen, Field *field) {
    GameData *gameData;
    EventWork *eventWork;
    u32 size;
    DowsingWork *work = GFL_HeapAllocate(heapId, sizeof(DowsingWork), TRUE, "dowsing.c", 626);
    work->heapId = heapId;
    work->subscreenHeapId = subscreenHeapId;
    work->subscreen = subscreen;
    work->field = field;
    func_ov033_021f06ac(work);
    func_ov033_021f0760(work);
    func_ov033_021f07f4(work);
    func_ov033_021f0924(work);
    work->signalMap = GFL_ArcSysReadHeapNewLZGetLen(ARCID_DOWSING, 13, FALSE, work->heapId, &size);
    work->vblankTCB = GFL_VBlankTCBAdd(func_ov033_021f0674, work, 1);
    work->frame = 0;
    work->active = TRUE;
    func_ov033_021f0b74(work);
    work->prevSignal = 10;
    work->signal = SIGNAL_NONE;
    work->shownSignal = 10;
    work->prevHit.type = HIT_NONE;
    work->hit.type = HIT_NONE;
    work->lastX = 0;
    work->lastY = 0;
    work->lastZ = 0;
    work->unk8C = 0;
    work->soundOn = FALSE;
    work->restartIcon = FALSE;
    func_ov033_021f05a4(work, TRUE);
    work->searchRestart = TRUE;
    work->searchFirst = TRUE;
    gameData = GSYS_GetGameData(Field_GetGameSystem(work->field));
    eventWork = GameData_GetEventWork(gameData);
    work->search = func_ov033_021efec0(work->heapId, eventWork, getHighLinkBlockAddress(GameData_GetSaveControl(gameData)));
    G2S_SetBlendAlpha(0x10, 0x18, 1, 7);
    func_0203d34c(FALSE, work->heapId);
    return work;
}

// pokebw2's Dowsing_Free
void func_ov033_021f0124(DowsingWork *work) {
    *(vu16 *)REG_DB_BLDCNT_ADDR = 0;
    func_ov033_021efee4(work->search);
    func_ov033_021f0c0c(work);
    GFL_TCBRemove(work->vblankTCB);
    GFL_HeapFree(work->signalMap);
    func_ov033_021f0a90(work);
    func_ov033_021f08ec(work);
    func_ov033_021f07dc(work);
    func_ov033_021f0738(work);
    GFL_HeapFree(work);
}

void func_ov033_021f0174(DowsingWork *work, BOOL active) {
    MapMatrix *matrix;
    GameSystem *gsys;
    VecFx32 *pos;
    s32 playerX;
    s32 playerY;
    s32 playerZ;
    u32 zoneId;
    BOOL zoneIsMatrix0;
    BOOL search;
    u16 count;
    u32 type;
    u16 i;
    s32 minX;
    s32 maxX;
    s32 minZ;
    s32 maxZ;
    u32 bestDistance;
    u16 bestIndex;
    // The closest hit so far, and the hit being looked at
    DowsingHit hits[2];
    s32 x;
    s32 z;

    work->active = active;
    if (active) {
        work->fadeCmd = FADE_CMD_IN;
    } else {
        work->fadeCmd = FADE_CMD_OUT;
    }
    func_02024abc();
    if (!active) {
        return;
    }

    work->prevSignal = work->signal;
    work->prevHit.type = work->hit.type;
    if (work->hit.type == HIT_HIDDEN_ITEM) {
        work->prevHit.item = work->hit.item;
    } else if (work->hit.type == 2) {
        work->prevHit.unk8 = work->hit.unk8;
    }

    gsys = Field_GetGameSystem(work->field);
    pos = PlayerState_GetWPos(GSYS_GetPlayerState(gsys));
    playerX = (pos->x >> 4) / 4096;
    playerY = (pos->y >> 4) / 4096;
    playerZ = (pos->z >> 4) / 4096;

    matrix = GetMapMatrixSystem(GSYS_GetGameData(gsys));
    zoneId = ZONE_NONE;
    zoneIsMatrix0 = FALSE;
    if (RangeCheckChunkCoordinateWorld(matrix, pos->x, pos->z) == 1) {
        zoneId = GetZoneIDAtMatrixXZWorld(matrix, pos->x, pos->z);
        zoneIsMatrix0 = GetIsZoneMatrix0(zoneId);
    }

    // Both arms set search, and the code still tests it (movs r1, #1; cmp r1, #0): a flag whose other value the
    // source had lost by Black (pokebw2's twin has neither the flag nor the test)
    if (work->unk8C == FALSE) {
        work->unk8C = TRUE;
        search = TRUE;
    } else {
        search = TRUE;
    }

    if (search) {
        minX = playerX - SEARCH_LEFT;
        maxX = playerX + SEARCH_RIGHT;
        minZ = playerZ - SEARCH_TOP;
        maxZ = playerZ + SEARCH_BOTTOM;
        bestDistance = -1;
        hits[0].type = HIT_NONE;

        if (work->searchRestart) {
            func_ov033_021eff40(work->search);
            work->searchRestart = FALSE;
        }
        if (work->searchFirst) {
            count = 0xffff;
            work->searchFirst = FALSE;
        } else {
            count = 11;
        }

        for (i = 0; i < count; i++) {
            if (i == 0) {
                if (work->prevHit.type != HIT_NONE) {
                    if (func_ov033_021efeec(work->search, &work->prevHit)) {
                        hits[1].type = work->prevHit.type;
                        if (hits[1].type == HIT_HIDDEN_ITEM) {
                            hits[1].item = work->prevHit.item;
                        } else if (hits[1].type == 2) {
                            hits[1].unk8 = work->prevHit.unk8;
                        }
                    } else {
                        work->searchRestart = TRUE;
                        work->searchFirst = TRUE;
                        break;
                    }
                }
            } else {
                work->searchRestart = !func_ov033_021eff48(work->search, &hits[1], 0);
                if (work->searchRestart) {
                    break;
                }
            }
            if (zoneId != ZONE_NONE) {
                if (hits[1].type == HIT_HIDDEN_ITEM) {
                    const HiddenItem *item = hits[1].item;
                    if (zoneId != item->unk4) {
                        if (!item->unk2 || !zoneIsMatrix0 || GetZoneIsEntralinkAny(item->unk4)) {
                            continue;
                        }
                    }
                    x = item->unk6;
                    z = item->unk8;
                } else if (hits[1].type == 2) {
                    const DowsingHitPos *pos2 = hits[1].unk8;
                    if (!GetIsZoneMatrix0(pos2->unk_6) || !zoneIsMatrix0) {
                        continue;
                    }
                    x = pos2->unk_0;
                    z = pos2->unk_4;
                }
                if (minX <= x && x < maxX && minZ <= z && z < maxZ) {
                    s32 dz = z - playerZ;
                    s32 dx = x - playerX;
                    u16 index = (dx + SEARCH_LEFT) + (dz + SEARCH_TOP) * MAP_WIDTH;
                    if (work->signalMap[index] != SIGNAL_NONE) {
                        u32 distance = dx * dx + dz * dz;
                        if (distance < bestDistance) {
                            bestIndex = index;
                            bestDistance = distance;
                            hits[0].type = hits[1].type;
                            if (hits[1].type == HIT_HIDDEN_ITEM) {
                                hits[0].item = hits[1].item;
                            } else if (hits[1].type == 2) {
                                hits[0].unk8 = hits[1].unk8;
                            }
                        }
                    }
                }
            }
        }

        type = hits[0].type;
        work->hit.type = type;
        if (type != HIT_NONE) {
            work->signal = work->signalMap[bestIndex];
            if (type == HIT_HIDDEN_ITEM) {
                work->hit.item = hits[0].item;
            } else if (type == 2) {
                work->hit.unk8 = hits[0].unk8;
            }
        } else {
            work->signal = SIGNAL_NONE;
        }
    }

    {
        BOOL quiet = TRUE;
        if (work->signal == SIGNAL_CLOSEST) {
            BOOL start = FALSE;
            if (!work->soundOn) {
                start = TRUE;
                work->soundOn = TRUE;
            } else if (work->soundTimer == 6) {
                if (!GFL_SndPlayerIsActive(work->soundPlayer)) {
                    work->soundTimer--;
                }
            } else if (work->soundTimer == 0) {
                start = TRUE;
            } else {
                work->soundTimer--;
            }
            if (start) {
                work->soundPlayer = GFL_SndSeqGetPlayerIndex(SEQ_SE_FLD_120);
                GFL_SEPlayKeepVol(SEQ_SE_FLD_120, work->soundPlayer);
                GFL_SndPlayerSetParams(work->soundPlayer, -1, 6 << 6, -1);
                work->soundTimer = 6;
                work->restartIcon = TRUE;
            }
            quiet = FALSE;
        } else {
            work->soundOn = FALSE;
        }

        if (quiet && work->signal != SIGNAL_CLOSEST && work->hit.type != HIT_NONE) {
            BOOL changed = FALSE;
            s32 oldX;
            s32 oldZ;
            s32 newX;
            s32 newZ;
            if (work->prevHit.type == HIT_HIDDEN_ITEM) {
                oldX = work->prevHit.item->unk6;
                oldZ = work->prevHit.item->unk8;
            } else if (work->prevHit.type == 2) {
                oldX = ((const DowsingHitPos *)work->prevHit.unk8)->unk_0;
                oldZ = ((const DowsingHitPos *)work->prevHit.unk8)->unk_4;
            } else {
                changed = TRUE;
                goto play;
            }
            if (work->hit.type == HIT_HIDDEN_ITEM) {
                newX = work->hit.item->unk6;
                newZ = work->hit.item->unk8;
            } else if (work->hit.type == 2) {
                newX = ((const DowsingHitPos *)work->hit.unk8)->unk_0;
                newZ = ((const DowsingHitPos *)work->hit.unk8)->unk_4;
            }
            if ((oldX != newX || oldZ != newZ) && work->prevSignal != work->signal) {
                changed = TRUE;
            }
        play:
            if (changed) {
                s32 player = GFL_SndSeqGetPlayerIndex(SEQ_SE_FLD_120);
                s32 pan = (data_ov033_021f0dc8[work->signal].pan - 8) * 16;
                if (pan < -127) {
                    pan = -127;
                } else if (pan > 127) {
                    pan = 127;
                }
                GFL_SEPlayKeepVol(SEQ_SE_FLD_120, player);
                GFL_SndPlayerSetParams(player, -1, -1, pan);
                quiet = FALSE;
            }
        }

        if (quiet && work->prevSignal != work->signal) {
            GFL_SndSEPlay(SEQ_SE_FLD_122);
        }
    }

    work->lastX = playerX;
    work->lastY = playerY;
    work->lastZ = playerZ;
    func_ov033_021f0ac4(work);
}


// pokebw2's Dowsing_UpdateAltFrame
void func_ov033_021f05a4(DowsingWork *work, BOOL active) {
    func_ov033_021f0c44(work);
    if (active) {
        if (work->shownSignal != work->signal) {
            ClActorPos pos;
            func_0204b3dc(work->icons[ICON_DIRECTION].actor, work->signal != SIGNAL_CLOSEST && work->signal != SIGNAL_NONE);
            func_0204b3dc(work->icons[ICON_CLOSEST].actor, work->signal == SIGNAL_CLOSEST);
            pos.x = data_ov033_021f0dc8[work->signal].x;
            pos.y = data_ov033_021f0dc8[work->signal].y;
            func_0204b404(work->icons[ICON_DIRECTION].actor, &pos, 1);
            func_0204b878(work->icons[ICON_RING].actor, data_ov033_021f0dc8[work->signal].anims0);
            func_0204b878(work->icons[ICON_DOT].actor, data_ov033_021f0dc8[work->signal].anims1);
            func_0204b878(work->icons[ICON_DIRECTION].actor, data_ov033_021f0dc8[work->signal].anims2);
            work->shownSignal = work->signal;
        }
        if (work->signal == SIGNAL_CLOSEST && work->restartIcon) {
            func_0204b878(work->icons[ICON_CLOSEST].actor, 0);
            func_0204b92c(work->icons[ICON_CLOSEST].actor, TRUE);
            func_0204b8c4(work->icons[ICON_CLOSEST].actor, 0);
            work->restartIcon = FALSE;
        }
    }
}

// pokebw2's Dowsing_VBlank
void func_ov033_021f0674(TCB *tcb, void *data) {
    DowsingWork *work = data;
    func_02021d50(work->paletteFade);
    if (work->active) {
        if (work->frame >= 4) {
            work->frame = 0;
        }
        work->frame++;
    }
}

// pokebw2's Dowsing_InitBG
static void func_ov033_021f06ac(DowsingWork *work) {
    int i;

    for (i = 4; i <= 7; i++) {
        GFL_BGSysSetBGEnabled(i, FALSE);
    }
    for (i = 0; i < NELEMS(data_ov033_021f0df0); i++) {
        const DowsingBGSetup *bgSetup = &data_ov033_021f0df0[i];
        u32 bg = bgSetup->bg;
        GFL_BGSysCreateBG(bg, &bgSetup->setup, bgSetup->mode);
        GFL_BGSysClearBG(bg);
        GFL_BGSysSetBGEnabled(bg, bgSetup->enabled);
        GFL_BGSysMoveBG(bg, BG_MOVE_SET_X, 0);
        GFL_BGSysMoveBG(bg, BG_MOVE_SET_Y, 0);
    }
    work->unit = func_0204b100(8, 0, work->heapId);
    func_0204b270(work->unit);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's Dowsing_ExitBG
static void func_ov033_021f0738(DowsingWork *work) {
    int i;

    func_0204b1cc(work->unit);
    for (i = 0; i < NELEMS(data_ov033_021f0df0); i++) {
        GFL_BGSysReleaseBG(data_ov033_021f0df0[i].bg);
    }
}

// pokebw2's Dowsing_LoadSignalBG
static void func_ov033_021f0760(DowsingWork *work) {
    u32 chars;
    HeapID heapId = work->subscreenHeapId;
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_DOWSING, heapId);

    GFL_G2DIOLoadArcNCLR(arc, 0, PALTYPE_SUB_BG, 0, 0, 0x40, heapId);
    chars = GFL_BGSysLoadArcNCGRDynamic(arc, 2, BG_SIGNAL, 0, FALSE, heapId);
    GFL_G2DIOLoadNSCRSync(arc, 7, BG_SIGNAL, 0, CHAR_POS(chars), 0, FALSE, heapId);
    GFL_ArcToolFree(arc);
    GFL_BGSysSetScrPaletteNo(BG_SIGNAL, 0, 0, 32, 24, 1);
    GFL_BGSysQueueScrLoad(BG_SIGNAL);
    work->signalChars = chars;
}

// pokebw2's Dowsing_FreeSignalBG
static void func_ov033_021f07dc(DowsingWork *work) {
    GFL_BGSysFreeCharMemory(BG_SIGNAL, CHAR_POS(work->signalChars), CHAR_SIZE(work->signalChars));
}

// pokebw2's Dowsing_InitIcons
static void func_ov033_021f07f4(DowsingWork *work) {
    u8 i;
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_DOWSING, work->subscreenHeapId);

    work->iconPalette = func_0204ac18(arc, 1, 1, 0, 0, 1, work->heapId);
    for (i = 0; i < ICON_COUNT; i++) {
        ClActorSetup setup = { 0, 0, 0, 0, 0 };
        if (i == ICON_DOT) {
            work->icons[i].chars = work->icons[ICON_RING].chars;
            work->icons[i].cellAnims = work->icons[ICON_RING].cellAnims;
        } else {
            const DowsingIconRes *res = &data_ov033_021f0dbc[i];
            work->icons[i].chars = func_0204a6c8(arc, res->file, FALSE, 1, work->heapId);
            work->icons[i].cellAnims = func_0204af28(arc, res->cellFile, res->animFile, work->heapId);
        }
        setup.priority = i;
        work->icons[i].actor = func_0204b294(work->unit, work->icons[i].chars, work->iconPalette,
                                             work->icons[i].cellAnims, &setup, 1, work->heapId);
        func_0204b92c(work->icons[i].actor, TRUE);
        func_0204b688(work->icons[i].actor, 1);
    }
    GFL_ArcToolFree(arc);

    {
        ClActorPos pos;
        pos.x = 0x50;
        pos.y = 0x60;
        func_0204b404(work->icons[ICON_RING].actor, &pos, 1);
        pos.x = 0xb0;
        pos.y = 0x60;
        func_0204b404(work->icons[ICON_DOT].actor, &pos, 1);
        pos.x = 0x80;
        pos.y = 0x60;
        func_0204b404(work->icons[ICON_CLOSEST].actor, &pos, 1);
    }
}

// pokebw2's Dowsing_FreeIcons
static void func_ov033_021f08ec(DowsingWork *work) {
    u8 i;

    for (i = 0; i < ICON_COUNT; i++) {
        func_0204b3b4(work->icons[i].actor);
        if (i != ICON_DOT) {
            func_0204afd8(work->icons[i].cellAnims);
            func_0204a8d4(work->icons[i].chars);
        }
    }
    func_0204ada4(work->iconPalette);
}

// pokebw2's Dowsing_InitButton
static void func_ov033_021f0924(DowsingWork *work) {
    u16 i;
    NNSG2dScreenData *screen;
    void *screenBuffer;
    u16 chars;
    u16 *tiles;
    u16 heapId = work->subscreenHeapId;
    ArcTool *arc;

    func_02024a20(BG_INFOWIN, 3, GSYS_GetGameData(Field_GetGameSystem(work->field)), work->heapId);
    ClActorSetup setup = { 0, 0, 0, 0, 0 };
    arc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, func_02027624(), PALTYPE_SUB_BG, 0x40, 0x20, heapId);
    work->barChars = GFL_BGSysLoadArcNCGRDynamic(arc, func_02027628(), BG_INFOWIN, 0, FALSE, heapId);
    screenBuffer = GFL_G2DIOReadNSCRArc(arc, func_0202762c(), FALSE, &screen, heapId);
    chars = work->barChars;
    tiles = (u16 *)screen->rawData;
    for (i = 0; i < 0x300; i++) {
        tiles[i] &= 0xfff;
        tiles[i] = (chars + tiles[i]) | (2 << 12);
    }
    GFL_BGSysLoadScrArea(BG_INFOWIN, 0, 0x15, 0x20, 3, screen->rawData, 0, 0x15, 0x20, 0x18);
    GFL_BGSysQueueScrLoad(BG_INFOWIN);
    GFL_HeapFree(screenBuffer);

    work->buttonPalette = func_0204ac18(arc, func_02027614(), 1, 0x20, 0, 3, work->heapId);
    work->buttonChars = func_0204a6c8(arc, func_02027618(), FALSE, 1, work->heapId);
    work->buttonCellAnims = func_0204af28(arc, func_0202761c(0), func_02027620(0), work->heapId);
    setup.x = BUTTON_X;
    setup.y = BUTTON_Y;
    work->button = func_0204b294(work->unit, work->buttonChars, work->buttonPalette, work->buttonCellAnims, &setup, 1,
                                 work->heapId);
    func_0204b92c(work->button, TRUE);
    func_0204b954(work->button, 1 << 13);
    func_0204b878(work->button, 0);
    GFL_ArcToolFree(arc);
    work->buttonState = BUTTON_WAIT;
}

// pokebw2's Dowsing_ExitButton
static void func_ov033_021f0a90(DowsingWork *work) {
    func_0204b3b4(work->button);
    func_0204afd8(work->buttonCellAnims);
    func_0204a8d4(work->buttonChars);
    func_0204ada4(work->buttonPalette);
    GFL_BGSysFreeCharMemory(BG_INFOWIN, CHAR_POS(work->barChars), CHAR_SIZE(work->barChars));
    func_02024dcc();
}

// pokebw2's Dowsing_UpdateButton
void func_ov033_021f0ac4(DowsingWork *work) {
    u32 x;
    u32 y;
    BOOL pressed;

    switch (work->buttonState) {
    case BUTTON_WAIT:
        pressed = FALSE;
        if (func_02035d20(&x, &y) && x >= BUTTON_X && x < 256 && y >= BUTTON_Y && y < BUTTON_Y + BUTTON_HEIGHT) {
            pressed = TRUE;
            func_020355d8(TRUE);
        }
        if (!pressed && (GCTX_HIDGetPressedKeys() & PAD_BUTTON_X)) {
            func_020355d8(FALSE);
            pressed = TRUE;
        }
        if (pressed) {
            work->buttonState = BUTTON_PRESSED;
            func_0204b878(work->button, 8);
            GFL_SndSEPlay(SEQ_SE_CLOSE1);
            return;
        }
        break;
    case BUTTON_PRESSED:
        if (!func_0204b98c(work->button)) {
            work->buttonState = BUTTON_RELEASED;
            func_0204b878(work->button, 0);
        }
        break;
    case BUTTON_RELEASED:
        FieldSubscreen_ReqChange(work->subscreen, 0);
        work->buttonState = BUTTON_DONE;
        break;
    case BUTTON_DONE:
        break;
    }
}

// pokebw2's Dowsing_InitFade
static void func_ov033_021f0b74(DowsingWork *work) {
    work->tcbMemory = GFL_HeapAllocate(work->heapId, GFL_TCBMgrCalcAllocSize(2), TRUE, "dowsing.c", 1759);
    work->tcbManager = GFL_TCBMgrCreate(2, work->tcbMemory);
    work->paletteFade = func_02021518(work->heapId);
    func_02021ee4(work->paletteFade, TRUE);
    func_0202155c(work->paletteFade, 1, 0x60, work->heapId);
    func_0202155c(work->paletteFade, 3, 0x80, work->heapId);
    func_020216d4(work->paletteFade, 1, 0, 0x60);
    func_020216d4(work->paletteFade, 3, 0, 0x80);
    work->fadeCmd = FADE_CMD_NONE;
    work->fadeState = 0;
}

// pokebw2's Dowsing_FreeFade
static void func_ov033_021f0c0c(DowsingWork *work) {
    func_020215a0(work->paletteFade, 1);
    func_020215a0(work->paletteFade, 3);
    func_02021540(work->paletteFade);
    func_02030da4(work->tcbManager);
    GFL_HeapFree(work->tcbMemory);
}

// pokebw2's Dowsing_UpdateFade
static void func_ov033_021f0c44(DowsingWork *work) {
    u8 i;
    u16 mask;
    u16 mask2;

    GFL_TCBMgrUpdate(work->tcbManager);
    if (work->fadeCmd == FADE_CMD_NONE || func_02021ed8(work->paletteFade) != 0) {
        return;
    }

    switch (work->fadeCmd) {
    case FADE_CMD_IN:
        if (work->fadeState != 0) {
            mask = 0;
            for (i = 0; i < 3; i++) {
                mask |= 1 << i;
            }
            func_0202173c(work->paletteFade, 2, mask, 0, 0, 0, 0, work->tcbManager);
            mask2 = 0;
            for (i = 0; i < 4; i++) {
                mask2 |= 1 << i;
            }
            func_0202173c(work->paletteFade, 8, mask2, 0, 0, 0, 0, work->tcbManager);
            work->fadeState = 0;
            for (i = 0; i < ICON_COUNT; i++) {
                func_0204b95c(work->icons[i].actor);
            }
        }
        break;
    case FADE_CMD_OUT:
        if (work->fadeState != 1) {
            mask = 0;
            for (i = 0; i < 3; i++) {
                mask |= 1 << i;
            }
            func_0202173c(work->paletteFade, 2, mask, 0, 8, 8, 0, work->tcbManager);
            mask2 = 0;
            for (i = 0; i < 4; i++) {
                mask2 |= 1 << i;
            }
            func_0202173c(work->paletteFade, 8, mask2, 0, 8, 8, 0, work->tcbManager);
            work->fadeState = 1;
            for (i = 0; i < ICON_COUNT; i++) {
                func_0204b974(work->icons[i].actor);
            }
        }
        break;
    }
    work->fadeCmd = FADE_CMD_NONE;
}
