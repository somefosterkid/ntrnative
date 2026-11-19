#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "error_handling.h"
#include "filesystem.h"
#include "gf_3d_loader.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "overlay_98.h"
#include "overlay_manager.h"
#include "palette.h"
#include "pm_string.h"
#include "poke_overlay.h"
#include "screen_fade.h"
#include "sprite.h"
#include "sprite_system.h"
#include "system.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_98_ov98text_sys.h"
#include "pokeathlon/pokeathlon_save.h"
#include "msgdata.h"
#include "overlay_99.h"

struct Background;

struct BgConfig;

struct G2dRenderer;

struct GF_2DGfxRawResMan;

struct GF_2DGfxRawResObj;

struct GF_2DGfxResHeader;

struct GF_2DGfxResMan;

struct GF_2DGfxResObjList;

struct ManagedSprite;

struct MessageFormat;

struct MessageFormatAttrs;

struct MessageFormatFields;

struct MsgData;

struct NARC;

struct OverlayManager;

struct OverlayManagerTemplate;

struct PaletteBuffer;

struct PaletteData;

struct PokeathlonCourseRecord;

struct PokeathlonCourseRecordArgs;

struct PokeathlonCourseRecordSelection;

struct PokeathlonEventRecordEntry;

struct PokeathlonEventRecordTeam;

struct PokeathlonSave;


struct PokeathlonStatistics;

struct Ov98TextSys;

struct Pokeathlon_RecordsLink;

struct Pokeathlon_RecordsSolo;

struct SelectedPaletteData;

struct Sprite;

struct SpriteList;

struct SpriteManager;

struct SpriteResource;

struct SpriteResourceHeaderList;

struct SpriteResourcesHeader;

struct SpriteSystem;

struct String;

struct Window;

struct WindowTemplate;

struct _MAT;

struct _MAT_ENTRY;

typedef struct PokeathlonStatistics {
    u32 playTime;
    u32 coursesJoined;
    u32 courseFirstPlaces;
    u32 courseLastPlaces;
    u32 bonusesEarned;
    u32 monInstructions;
    u32 monFailures;
    u32 monJumps;
    u32 monPointsEarned;
    u32 monTackles;
    u32 monFalls;
    u32 monDashes;
    u32 monSwitches;
    u32 monSelfImpedes;
    u32 linkCoursesJoined;
    u32 linkCourseFirstPlaces;
    u32 linkCourseLastPlaces;
    u32 eventFirstPlaces[10];
    u32 eventLastPlaces;
    u32 fanCount;
} PokeathlonStatistics;

typedef struct PokeathlonEventRecordTeam {
    u16 mons[3];
} PokeathlonEventRecordTeam;

typedef struct PokeathlonEventRecordEntry {
    u16 score;
    PokeathlonEventRecordTeam team;
} PokeathlonEventRecordEntry;

typedef union PokeathlonSavePrefix {
    struct {
        u8 bytesDC[0xDC];
        u8 bytes1F0[0x1F0];
    } legacy;
    u16 nativeHalfwords[0x2CC / 2];
} PokeathlonSavePrefix;



typedef struct PokeathlonCourseRecordSelection {
    s8 cursor;
    BOOL scrollForward;
    int page;
    BOOL scrolling;
    BOOL pageChanged;
} PokeathlonCourseRecordSelection;

typedef struct PokeathlonCourseRecord {
    PokeathlonCourseRecordArgs *args;
    BgConfig *bgConfig;
    NARC *narc;
    enum HeapID heapId;
    Ov98TextSys *textContext;
    PokeathlonSpriteContext *spriteContext;
    ManagedSprite *sprites[26];
    PokeathlonCourseRecordSelection selection;
} PokeathlonCourseRecord;

FS_EXTERN_OVERLAY(OVY_98);

void ov99_021E5B54(PokeathlonCourseRecord *work, const PokeathlonCourseRecordArgs *args);

void ov99_021E5C88(PokeathlonCourseRecord *work);

void ov99_021E5D58(PokeathlonCourseRecord *work);

void ov99_021E5FE8(PokeathlonCourseRecord *work, u32 course);

void ov99_021E6018(PokeathlonCourseRecord *work);

void ov99_021E6050(PokeathlonCourseRecord *work, u32 course);

void ov99_021E6250(PokeathlonCourseRecord *work);

void ov99_021E6274(PokeathlonCourseRecord *work);

void ov99_021E6438(PokeathlonCourseRecord *work);

void ov99_021E64E0(PokeathlonCourseRecord *work, BOOL visible);

void ov99_021E6530(PokeathlonCourseRecord *work, int course);

extern const SpriteResourceCountsListUnion ov99_021E95A4;

extern const WindowTemplate ov99_021E96D8[17];

BOOL PokeathlonCourseRecord_Init(OverlayManager *man, int *state);

BOOL PokeathlonCourseRecord_Init(OverlayManager *man, int *state) {
    PokeathlonCourseRecord *work;
    PokeathlonCourseRecordArgs *args;
    SpriteResourceCountsListUnion counts;

    HandleLoadOverlay(FS_OVERLAY_ID(OVY_98), OVY_LOAD_ASYNC);
    args = OverlayManager_GetArgs(man);
    ((args != ((void *)0)) ? (void)0 : GF_AssertFail());
    Heap_Create(HEAP_ID_3, HEAP_ID_132, 0x30000);
    work = OverlayManager_CreateAndGetData(man, sizeof(PokeathlonCourseRecord), HEAP_ID_132);
    MI_CpuFill8(work, 0, sizeof(PokeathlonCourseRecord));
    work->heapId = HEAP_ID_132;
    work->selection.scrolling = 0;
    work->selection.pageChanged = 1;
    work->selection.scrollForward = 1;
    ov99_021E5B54(work, args);
    ov98_0221F090();
    ov99_021E5C88(work);
    ov99_021E5D58(work);
    work->textContext = ov98_0221EABC(work->heapId, work->bgConfig, 17, (struct WindowTemplate *)ov99_021E96D8, 314);
    counts = ov99_021E95A4;
    work->spriteContext = ov98_0221E5E0(work->heapId, &counts, 26);
    ov99_021E6274(work);
    ov99_021E6438(work);
    ov99_021E64E0(work, 0);
    ov99_021E5FE8(work, 0);
    ov99_021E6050(work, 0);
    ov99_021E6530(work, 0);
    ov99_021E6018(work);
    SetKeyRepeatTimers(2, 4);
    ResetVisibleHardwareWindows(PM_LCD_TOP);
    ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
    Main_SetVBlankIntrCB((GFIntrCB)ov99_021E6250, work);
    return 1;
}
