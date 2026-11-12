// Black's code at ov208 0x021bf784-0x021c239c. No name is known: the module and address.

#include "types.h"
#include "app/townmap/townmap_data.h"
#include "app/zukan_detail.h"
#include "field/zone.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/str.h"
#include "nitro/fs.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_vec.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nnsys/g2d.h"
#include "pml/poke_party.h"
#include "struct_decls.h"
#include "system/printsys.h"
#include "field/player_state.h"
#include "system/game_data.h"
#include "field/townmap_util.h"
#include "app/ov208_021c239c.h"
#include "gfl/tcb.h"
#include "gfl/overlay.h"
#include "text/system/btl_server_flow_title.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "system/bmp_winframe.h"
#include "constants/text_banks.h"
#include "gfl/calctool.h"
#include "save/pokedex.h"
#include "system/wordset.h"
#include "gfl/key.h"
#include "gfl/sound.h"
#include "gfl/ui.h"
#include "gfl/touchpanel.h"
#include "nitro/math.h"

// The Pokédex's shortcut for the Y button, which the touch bar's check box registers
#define SHORTCUT_POKEDEX_MAP 18

struct ArcTool {
    FSFile file;
    u32 fimgOffset;
    u16 fatbOffset;
    u16 fileCount;
};
struct ClActRenderer {
    NNSG2dRendererInstance renderer;
    NNSG2dRenderSurface *surfaces;
    u32 surfaceCount;
};
struct ClActUnit {
    ClActor *actors;
    ClActor *actorList;
    ClActRenderer *renderer;
    u16 count;
    union {
        struct {
            u8 rendererType : 4;
            u8 unk4 : 4;
        };
        struct {
            u8 : 4;
            u8 active : 1;
            u8 unk5 : 1;
            u8 unk6 : 1;
        };
    };
    u8 priority;
    ClActUnit *next;
    ClActUnit *prev;
};
typedef struct {
    u32 type;
    NNSG2dCellDataBank *cells;
    NNSG2dAnimBankData *anims;
    union {
        struct {
            NNSG2dCellAnimation cellAnim;
            u32 transferHandle;
        };
        struct {
            NNSG2dMultiCellAnimation mcAnim;
            NNSG2dMultiCellDataBank *mcBank;
            NNSG2dAnimBankData *mcAnims;
            void *mcWork;
        };
    };
} ClActAnim;
struct ClActor {
    ClActor *next;
    ClActor *prev;
    ClActUnit *unit;
    ClActorPos pos;
    ClActorPos affineCenter;
    ClActorScale scale;
    NNSG2dImageProxy imgProxy;
    NNSG2dImagePaletteProxy pltProxy;
    fx32 unk54;
    u16 rotation;
    u16 sequence;
    u16 charIdx;
    u16 plttIdx;
    u32 priority : 8;
    u32 unk60_8 : 4;
    u32 palette : 4;
    u32 bgPriority : 2;
    u32 affineMode : 2;
    u32 flip : 2;
    u32 objMode : 2;
    u32 unk60_24 : 1;
    u32 unk60_25 : 1;
    u32 visible : 1;
    u32 playMode : 4;
    u32 unk60_31 : 1;
    u32 unk64_0 : 1;
    ClActAnim anim;
};
struct BmpWin {
    u16 magic : 15;
    u16 colorMode : 1;
    u8 bg;
    u8 x;
    u8 y;
    u8 sizeX;
    u8 sizeY;
    u8 width;
    u8 height;
    u8 palette;
    u16 charPos;
    GFLBitmap *bitmap;
};
struct StrBuf {
    u16 size;
    u16 length;
    u32 magic;
    u16 chars[1];
};
typedef struct {
    u16 langCount;
    u16 lineCount;
    u32 maxBlockSize;
    u32 unk8;
    u32 langOffsets[0];
} MsgFileHeader;
typedef struct {
    u32 offset;
    u16 length;
    u16 unk6;
} MsgEntry;
struct MsgData {
    MsgFileHeader *header;
    void *block;
    MsgEntry entry;
    u32 fileOffset;
    ArcTool *arc;
    HeapID heapId;
    u8 lang;
    u8 preloaded;
};

const ClActorPos data_ov208_021c8554 = { 128, 96 };

BOOL func_ov208_021bf788(ZukanDetailProcSys *sys, int *seq, void *param_, void *work, ZukanDetailCommon *common);
BOOL func_ov208_021bf9b0(ZukanDetailProcSys *sys, int *seq, void *param_, void *work, ZukanDetailCommon *common);
BOOL func_ov208_021bf8e0(ZukanDetailProcSys *sys, int *seq, void *param_, void *work, ZukanDetailCommon *common);
void func_ov208_021bfe08(ZukanDetailProcSys *sys, int *seq, void *param_, void *work, ZukanDetailCommon *common, int command);

// pokebw2's ZUKAN_DETAIL_MAP_PROC_FUNCS
const ZukanDetailProcFuncs data_ov208_021c8590 = {
    func_ov208_021bf788,
    func_ov208_021bf9b0,
    func_ov208_021bf8e0,
    func_ov208_021bfe08,
    NULL,
};

const ZukanDetailBGSetup data_ov208_021c85c0[2] = {
#include "asset/ov208_021bf784_data_ov208_021c85c0.inc"
};

const ZukanDetailBGSetup data_ov208_021c8618[2] = {
#include "asset/ov208_021bf784_data_ov208_021c8618.inc"
};

extern const u16 data_ov208_021c8670[62];

void func_ov208_021c0740(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common, u8 window);
u8 func_ov208_021c0c14(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c0ccc(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c1fc4(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c2238(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c1920(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common, int state);
void func_ov208_021c0a88(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c2304(ZukanDetailWork *wk);
void func_ov208_021c231c(ZukanDetailWork *wk);
void func_ov208_021c232c(ZukanDetailWork *wk);
void func_ov208_021c1f44(ZukanHabitat *habitat);
ZukanHabitat *func_ov208_021c1f28(u16 species, HeapID heapId, ArcTool *arc);
void func_ov208_021c1f50(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c1d30(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c0c00(u32 param, ClActor *wk);
void func_ov208_021c0b58(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c0950(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c0898(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c06a0(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c0524(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c03c4(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c06ec(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c0980(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c1fe0(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c2090(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c20c0(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c21d0(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c2228(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c2234(void);
void func_ov208_021c0048(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021bffec(TCB *tcb, void *data);
void func_ov208_021c06c8(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c0090(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c0134(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c0450(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c0544(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c0788(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c08e8(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c0b10(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c1ad4(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c0c50(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c0c8c(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common, BOOL visible);
void func_ov208_021c1d0c(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c1d24(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c10b8(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c0130(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c0968(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c0a8c(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c1810(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c0714(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);
void func_ov208_021c1e10(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common, BOOL next, BOOL touch);
void func_ov208_021c1450(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common);

void func_ov208_021bf784(ZukanDetailMapParam *param, HeapID heapId) {
    param->heapId = heapId;
}

BOOL func_ov208_021bf788(ZukanDetailProcSys *sys, int *seq, void *param_, void *work, ZukanDetailCommon *common) {
    ZukanDetailMapParam *param = param_;
    GameData *gameData = func_ov208_021bc3a4(common);
    ZukanDetailWork *wk;
    u8 i;

    GFL_OvlLoad(OVERLAY_ID(72));
    wk = func_ov208_021bc32c(sys, sizeof(ZukanDetailWork), param->heapId);
    MI_CpuFill8(wk, 0, sizeof(ZukanDetailWork));
    wk->unit = func_ov208_021bcf4c(func_ov208_021bc3a8(common));
    wk->font = func_ov208_021bc3ac(common);
    for (i = 0; i < 2; i++) {
        wk->icons[i] = NULL;
    }
    wk->icon = 0;
    wk->bySeason = FALSE;
    wk->found = TRUE;
    wk->habitat = NULL;
    wk->state = MAP_STATE_MAP;
    wk->panelSlide = SLIDE_NONE;
    wk->iconSlide = SLIDE_NONE;
    wk->season = GameData_GetSeason(gameData);
    wk->place = NO_PLACE;
    wk->step = 0;
    wk->active = TRUE;
    wk->townmap = func_ov072_021bbb24(param->heapId);
    func_ov208_021c0048(param, wk, common);
    wk->habitatArc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_AREA, param->heapId);
    wk->vblankTcb = GFL_VBlankTCBAdd(func_ov208_021bffec, wk, 1);
    wk->vblankRequest = VBLANK_NONE;
    func_ov208_021c06c8(param, wk, common);
    wk->blendMain = func_ov208_021bc448(param->heapId);
    wk->blendSub = func_ov208_021bc448(param->heapId);
    func_ov208_021bc518(wk->blendMain);
    func_ov208_021bc518(wk->blendSub);
    func_ov208_021bc508(0, wk->blendMain);
    func_ov208_021bc508(1, wk->blendSub);
    wk->palFade = func_ov208_021bc560(param->heapId, ZUKAN_DETAIL_PALFADE_SUB_BG | ZUKAN_DETAIL_PALFADE_SUB_OBJ);
    wk->exit = MAP_EXIT_NONE;
    wk->inputEnabled = TRUE;
    wk->arrowPushed = FALSE;
    wk->arrowLoaded = TRUE;
    wk->arrowDone = TRUE;
    return TRUE;
}


static inline void ZukanDetailMap_ReleaseIcon(ZukanDetailWork *wk, int icon) {
    if (wk->icons[icon] != NULL) {
        func_ov208_021c0c00(wk->iconChars[icon], wk->icons[icon]);
    }
    wk->icons[icon] = NULL;
}

BOOL func_ov208_021bf8e0(ZukanDetailProcSys *sys, int *seq, void *param_, void *work, ZukanDetailCommon *common) {
    ZukanDetailMapParam *param = param_;
    ZukanDetailWork *wk = work;
    u8 i;

    func_ov208_021c1f44(wk->habitat);
    for (i = 0; i < 2; i++) {
        ZukanDetailMap_ReleaseIcon(wk, i);
    }
    func_ov208_021c0b58(param, wk, common);
    func_ov208_021c0950(param, wk, common);
    func_ov208_021c0898(param, wk, common);
    func_ov208_021bc97c(wk->background);
    func_ov208_021c06a0(param, wk, common);
    func_ov208_021c0524(param, wk, common);
    func_ov208_021c03c4(param, wk, common);
    func_ov208_021bc61c(wk->palFade);
    func_ov208_021bc464(wk->blendSub);
    func_ov208_021bc464(wk->blendMain);
    func_ov208_021c06ec(param, wk, common);
    GFL_TCBRemove(wk->vblankTcb);
    GFL_ArcToolFree(wk->habitatArc);
    func_ov072_021bbb34(wk->townmap);
    func_ov208_021bc34c(sys);
    GFL_OvlUnload(OVERLAY_ID(72));
    return TRUE;
}

BOOL func_ov208_021bf9b0(ZukanDetailProcSys *sys, int *seq, void *param_, void *work, ZukanDetailCommon *common) {
    ZukanDetailMapParam *param = param_;
    ZukanDetailWork *wk = work;
    ZukanDetailTouchbar *touchbar = func_ov208_021bc3b4(common);
    ZukanDetailHeadbar *headbar = func_ov208_021bc3b8(common);

    switch (*seq) {
    case MAP_SEQ_INIT: {
        u32 i;
        u8 bg;

        *seq = MAP_SEQ_FADE_IN_SUB;
        GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_5, GX_BG0_AS_2D);
        for (i = 0; i < 2; i++) {
            GFL_BGSysReleaseBG(data_ov208_021c85c0[i].bg);
            GFL_BGSysCreateBG(data_ov208_021c85c0[i].bg, &data_ov208_021c85c0[i].setup, data_ov208_021c85c0[i].mode);
            GFL_BGSysClearBG(data_ov208_021c85c0[i].bg);
            GFL_BGSysSetBGEnabled(data_ov208_021c85c0[i].bg, data_ov208_021c85c0[i].enabled);
        }
        for (bg = 0; bg <= 7; bg++) {
            if (bg != 1 && bg != 5) {
                GFL_BGSysMoveBG(bg, BG_MOVE_SET_X, 0);
                GFL_BGSysMoveBG(bg, BG_MOVE_SET_Y, 0);
                GFL_BGSysClearBG(bg);
            }
        }
        GFL_BGSysSetBGPriority(2, 2);
        GFL_BGSysSetBGPriority(3, 3);
        GFL_BGSysSetBGPriority(0, 1);
        GFL_BGSysSetBGPriority(6, 1);
        GFL_BGSysSetBGPriority(7, 2);
        GFL_BGSysSetBGPriority(4, 3);
        func_ov208_021c0090(param, wk, common);
        func_ov208_021c0134(param, wk, common);
        func_ov208_021c0450(param, wk, common);
        func_ov208_021c0544(param, wk, common);
        wk->background = func_ov208_021bc844(param->heapId, 1, 4, 9, 10);
        func_ov208_021c0788(param, wk, common);
        func_ov208_021c08e8(param, wk, common);
        func_ov208_021c0b10(param, wk, common);
        func_ov208_021c1ad4(param, wk, common);
        func_0204b688(wk->icons[wk->icon], 1);
        func_ov208_021c0c50(param, wk, common);
        if (wk->found == TRUE) {
            func_ov208_021c0c8c(param, wk, common, TRUE);
        }
        func_ov208_021bc758(wk->palFade);
        func_ov208_021bc718(wk->palFade);
        break;
    }
    case MAP_SEQ_FADE_IN_SUB:
        *seq = MAP_SEQ_FADE_IN;
        func_ov208_021bc4f4(1, wk->blendSub);
        func_ov208_021c24f0(param, wk, common);
        break;
    case MAP_SEQ_FADE_IN:
        *seq = MAP_SEQ_WAIT_FADE_IN;
        func_ov208_021bc4d8(wk->blendMain);
        func_ov208_021bc6d8(wk->palFade);
        if (func_ov208_021bdad0(touchbar) != ZUKAN_DETAIL_TOUCHBAR_SHOWN) {
            func_ov208_021bdaac(touchbar, ZUKAN_DETAIL_TOUCHBAR_GENERAL, ZUKAN_DETAIL_PAGE_MAP - 1,
                                func_ov208_021bc3bc(common) > 1 ? TRUE : FALSE);
            func_ov208_021bdad4(touchbar, 0);
        } else {
            func_ov208_021bdbb8(touchbar, ZUKAN_DETAIL_PAGE_MAP - 1);
        }
        func_ov208_021bdc48(touchbar, FALSE);
        wk->active = FALSE;
        func_ov208_021bdc14(touchbar, GameData_IsShortcutRegistered(func_ov208_021bc3a4(common), SHORTCUT_POKEDEX_MAP));
        if (func_ov208_021be840(headbar) != ZUKAN_DETAIL_TOUCHBAR_SHOWN) {
            func_ov208_021be82c(headbar, 1);
            func_ov208_021be844(headbar);
        }
        break;
    case MAP_SEQ_WAIT_FADE_IN:
        if (!func_ov208_021bc4c8(wk->blendMain) && !func_ov208_021bc6c8(wk->palFade) &&
            func_ov208_021bdad0(touchbar) == ZUKAN_DETAIL_TOUCHBAR_SHOWN &&
            func_ov208_021be840(headbar) == ZUKAN_DETAIL_TOUCHBAR_SHOWN) {
            func_ov208_021bdba0(touchbar);
            func_ov208_021c2458(param, wk, common);
            func_ov208_021c1d0c(param, wk, common);
            *seq = MAP_SEQ_WAIT_STEP;
        }
        break;
    case MAP_SEQ_WAIT_STEP:
        if (wk->step == 0) {
            func_ov208_021bdc48(touchbar, TRUE);
            wk->active = TRUE;
            *seq = MAP_SEQ_MAIN;
        }
        break;
    case MAP_SEQ_MAIN:
        if (wk->exit != MAP_EXIT_NONE) {
            func_ov208_021c1d24(param, wk, common);
            *seq = MAP_SEQ_WAIT_EXIT;
        } else {
            if (wk->found == TRUE) {
                func_ov208_021c23bc(wk);
            }
            func_ov208_021c10b8(param, wk, common);
        }
        break;
    case MAP_SEQ_WAIT_EXIT:
        if (wk->step == 0) {
            func_ov208_021c24a4(param, wk, common);
            *seq = MAP_SEQ_WAIT_FADE_OUT;
            func_ov208_021bc4e4(wk->blendMain);
            func_ov208_021bc6f8(wk->palFade);
            func_ov208_021be854(headbar);
            if (wk->exit == MAP_EXIT_SCREEN) {
                func_ov208_021bdae8(touchbar, 0);
            }
        }
        break;
    case MAP_SEQ_WAIT_FADE_OUT: {
        BOOL done = FALSE;

        if (!func_ov208_021bc4c8(wk->blendMain) && !func_ov208_021bc6c8(wk->palFade) &&
            func_ov208_021be840(headbar) == ZUKAN_DETAIL_TOUCHBAR_HIDDEN) {
            if (wk->exit == MAP_EXIT_SCREEN) {
                if (func_ov208_021bdad0(touchbar) == ZUKAN_DETAIL_TOUCHBAR_HIDDEN) {
                    done = TRUE;
                }
            } else {
                done = TRUE;
            }
        }
        if (done) {
            *seq = MAP_SEQ_FADE_OUT_SUB;
        }
        break;
    }
    case MAP_SEQ_FADE_OUT_SUB:
        *seq = MAP_SEQ_EXIT;
        func_ov208_021c252c(param, wk, common);
        func_ov208_021bc508(1, wk->blendSub);
        break;
    case MAP_SEQ_EXIT: {
        u32 i;

        func_ov208_021c0130(param, wk, common);
        GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);
        for (i = 0; i < 2; i++) {
            GFL_BGSysReleaseBG(data_ov208_021c8618[i].bg);
            GFL_BGSysCreateBG(data_ov208_021c8618[i].bg, &data_ov208_021c8618[i].setup, data_ov208_021c8618[i].mode);
            GFL_BGSysClearBG(data_ov208_021c8618[i].bg);
            GFL_BGSysSetBGEnabled(data_ov208_021c8618[i].bg, data_ov208_021c8618[i].enabled);
        }
        return TRUE;
    }
    }

    if (*seq >= MAP_SEQ_FADE_IN) {
        func_ov208_021bc99c(wk->background);
        func_ov208_021c0968(param, wk, common);
        func_ov208_021c0a8c(param, wk, common);
    }
    func_ov208_021c1810(param, wk, common);
    func_ov208_021bc46c(wk->blendMain, wk->blendSub);
    func_ov208_021bc678(wk->palFade);
    func_ov208_021c0714(param, wk, common);
    return FALSE;
}

void func_ov208_021bfe08(ZukanDetailProcSys *sys, int *seq, void *param_, void *work, ZukanDetailCommon *common, int command) {
    ZukanDetailMapParam *param = param_;
    ZukanDetailWork *wk = work;

    if (wk != NULL) {
        ZukanDetailTouchbar *touchbar = func_ov208_021bc3b4(common);
        BOOL unlock = FALSE;

        // Input waits while the bar's icons play their animations
        switch (command) {
        case ZUKAN_DETAIL_CMD_CLOSE_TOUCH:
        case ZUKAN_DETAIL_CMD_RETURN_TOUCH:
        case ZUKAN_DETAIL_CMD_CUR_D_TOUCH:
        case ZUKAN_DETAIL_CMD_CUR_U_TOUCH:
        case ZUKAN_DETAIL_CMD_CHECK_TOUCH:
        case ZUKAN_DETAIL_CMD_INFO_TOUCH:
        case ZUKAN_DETAIL_CMD_VOICE_TOUCH:
        case ZUKAN_DETAIL_CMD_FORM_TOUCH:
        case ZUKAN_DETAIL_CMD_MAP_RETURN_TOUCH:
            wk->inputEnabled = FALSE;
            break;
        case ZUKAN_DETAIL_CMD_MAP_TOUCH:
            break;
        }
        switch (command) {
        case ZUKAN_DETAIL_CMD_CLOSE:
        case ZUKAN_DETAIL_CMD_RETURN:
        case ZUKAN_DETAIL_CMD_CUR_D:
        case ZUKAN_DETAIL_CMD_CUR_U:
        case ZUKAN_DETAIL_CMD_CHECK:
        case ZUKAN_DETAIL_CMD_INFO:
        case ZUKAN_DETAIL_CMD_VOICE:
        case ZUKAN_DETAIL_CMD_FORM:
        case ZUKAN_DETAIL_CMD_MAP_RETURN:
            unlock = TRUE;
            wk->inputEnabled = TRUE;
            break;
        }

        switch (command) {
        case ZUKAN_DETAIL_CMD_CUR_D_TOUCH: {
            u16 species = func_ov208_021bc3c0(common);

            func_ov208_021bc3cc(common);
            if (species != func_ov208_021bc3c0(common)) {
                func_ov208_021c1ad4(param, wk, common);
                func_ov208_021c1d0c(param, wk, common);
                wk->arrowPushed = TRUE;
                wk->arrowLoaded = FALSE;
                wk->arrowDone = FALSE;
            } else {
                wk->arrowPushed = TRUE;
                wk->arrowLoaded = TRUE;
                wk->arrowDone = FALSE;
            }
            break;
        }
        case ZUKAN_DETAIL_CMD_CUR_U_TOUCH: {
            u16 species = func_ov208_021bc3c0(common);

            func_ov208_021bc40c(common);
            if (species != func_ov208_021bc3c0(common)) {
                func_ov208_021c1ad4(param, wk, common);
                func_ov208_021c1d0c(param, wk, common);
                wk->arrowPushed = TRUE;
                wk->arrowLoaded = FALSE;
                wk->arrowDone = FALSE;
            } else {
                wk->arrowPushed = TRUE;
                wk->arrowLoaded = TRUE;
                wk->arrowDone = FALSE;
            }
            break;
        }
        }

        switch (command) {
        case ZUKAN_DETAIL_CMD_NONE:
            break;
        case ZUKAN_DETAIL_CMD_CLOSE:
        case ZUKAN_DETAIL_CMD_RETURN:
            wk->exit = MAP_EXIT_SCREEN;
            break;
        case ZUKAN_DETAIL_CMD_INFO:
        case ZUKAN_DETAIL_CMD_VOICE:
        case ZUKAN_DETAIL_CMD_FORM:
            wk->exit = MAP_EXIT_PAGE;
            break;
        case ZUKAN_DETAIL_CMD_CUR_D:
            if (wk->arrowPushed) {
                wk->arrowDone = TRUE;
                if (wk->arrowLoaded) {
                    wk->arrowPushed = FALSE;
                    func_ov208_021bdba0(touchbar);
                }
            }
            break;
        case ZUKAN_DETAIL_CMD_CUR_U:
            if (wk->arrowPushed) {
                wk->arrowDone = TRUE;
                if (wk->arrowLoaded) {
                    wk->arrowPushed = FALSE;
                    func_ov208_021bdba0(touchbar);
                }
            }
            break;
        case ZUKAN_DETAIL_CMD_CHECK:
            GameData_SetKeyItemRegistration(func_ov208_021bc3a4(common), SHORTCUT_POKEDEX_MAP,
                                            func_ov208_021bdc2c(touchbar));
            func_ov208_021bdba0(touchbar);
            break;
        case ZUKAN_DETAIL_CMD_MAP_RETURN:
            func_ov208_021c1920(param, wk, common, MAP_STATE_CLOSE_START);
            break;
        default:
            if (unlock) {
                func_ov208_021bdba0(touchbar);
            }
            break;
        }
    }
}




void func_ov208_021bffec(TCB *tcb, void *data) {
    ZukanDetailWork *wk = data;

    switch (wk->vblankRequest) {
    case 1:
        func_ov208_021c239c(wk);
        func_ov208_021c2304(wk);
        break;
    case 2:
        func_ov208_021c231c(wk);
        func_ov208_021c232c(wk);
        break;
    case 3:
        func_ov208_021c239c(wk);
        func_ov208_021c231c(wk);
        func_ov208_021bc4f4(0, wk->blendMain);
        break;
    }
    wk->vblankRequest = 0;
    func_ov208_021bc6bc(wk->palFade);
}


void func_ov208_021c0048(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    u8 i;
    u8 j;

    for (i = 0; i < TOWNMAP_PLACE_COUNT; i++) {
        u16 place = func_ov072_021bbb3c(wk->townmap, i, TOWNMAP_PARAM_ZONE);

        for (j = 0; j < TOWNMAP_PLACE_COUNT; j++) {
            if (place == data_ov208_021c8670[j]) {
                wk->placeOrder[i] = j;
                break;
            }
        }
    }
}
void func_ov208_021c0090(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_TOWNMAP_GRA, param->heapId);

    GFL_G2DIOLoadArcNCLRDefault(arc, 0, PALTYPE_MAIN_BG_EX, 0x6000, 0x2000, param->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 0, PALTYPE_MAIN_BG_EX, 0x4000, 0x2000, param->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 5, 2, 0, 0x4000, FALSE, param->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 7, 3, 0, 0xc000, FALSE, param->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 17, 2, 0, 0x800, FALSE, param->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 16, 3, 0, 0x800, FALSE, param->heapId);
    GFL_ArcToolFree(arc);
}

void func_ov208_021c0130(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
}

void func_ov208_021c0134(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    ClActSurfaceSetup surface = { 0, 0, 256, 192, 0, 0 };
    ArcTool *arc;
    u8 i;

    wk->renderer = func_0204b034(&surface, 1, param->heapId);
    wk->unit2 = func_0204b100(16, 0, param->heapId);
    func_0204b258(wk->unit2, wk->renderer);

    arc = GFL_ArcSysCreateFileHandle(ARCID_TOWNMAP_GRA, param->heapId);
    wk->mapPalette = func_0204ac18(arc, 2, CLACT_VRAM_MAIN, 0, 0, 5, param->heapId);
    wk->mapChars = func_0204a6c8(arc, 8, FALSE, CLACT_VRAM_MAIN, param->heapId);
    wk->mapCellAnims = func_0204af28(arc, 12, 19, param->heapId);
    GFL_ArcToolFree(arc);

    arc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_GRA, param->heapId);
    wk->seasonPalette = func_0204ac18(arc, 6, CLACT_VRAM_MAIN, 0xa0, 0, 2, param->heapId);
    wk->seasonChars = func_0204a6c8(arc, 16, FALSE, CLACT_VRAM_MAIN, param->heapId);
    wk->seasonCellAnims = func_0204af28(arc, 29, 46, param->heapId);
    GFL_ArcToolFree(arc);

    arc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_GRA, param->heapId);
    wk->areaPalette = func_0204ac18(arc, 5, CLACT_VRAM_MAIN, 0xe0, 0, 1, param->heapId);
    wk->areaChars = func_0204a6c8(arc, 15, FALSE, CLACT_VRAM_MAIN, param->heapId);
    wk->areaCellAnims = func_0204af28(arc, 28, 45, param->heapId);
    GFL_ArcToolFree(arc);

    {
        u8 anims[ACTOR_COUNT] = { 1, 4, 2, 0, 4, 5, 6 };
        u8 bgPriorities[ACTOR_COUNT] = { 2, 2, 2, 2, 1, 1, 1 };
        u8 priorities[ACTOR_COUNT] = { 2, 1, 0, 3, 0, 1, 2 };
        ClActorSetup setup;
        ClActorPos pos;

        MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
        for (i = ACTOR_CURSOR; i < ACTOR_COUNT; i++) {
            wk->actors[i] = func_0204b294(wk->unit2, wk->mapChars, wk->mapPalette, wk->mapCellAnims, &setup,
                                          CLACT_SURFACE_MAIN, param->heapId);
        }
        for (i = 0; i < ACTOR_CURSOR; i++) {
            wk->actors[i] = func_0204b294(wk->unit2, wk->seasonChars, wk->seasonPalette, wk->seasonCellAnims, &setup,
                                          CLACT_SURFACE_MAIN, param->heapId);
        }
        for (i = 0; i < ACTOR_COUNT; i++) {
            func_0204b878(wk->actors[i], anims[i]);
            func_0204b92c(wk->actors[i], TRUE);
            func_0204b84c(wk->actors[i], bgPriorities[i]);
            func_0204b800(wk->actors[i], priorities[i]);
            func_0204b3dc(wk->actors[i], FALSE);
            func_0204b688(wk->actors[i], 1);
        }
        pos.x = 128;
        pos.y = 96;
        for (i = 0; i < ACTOR_CURSOR; i++) {
            func_0204b404(wk->actors[i], &pos, CLACT_SURFACE_MAIN);
        }
    }
    wk->arrow = ACTOR_NONE;
}


const u16 data_ov208_021c8670[62] = {
#include "asset/ov208_021bf784_data_ov208_021c8670.inc"
};

void func_ov208_021c03c4(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    u8 i;

    for (i = 0; i < ACTOR_COUNT; i++) {
        func_0204b3b4(wk->actors[i]);
    }
    func_0204afd8(wk->areaCellAnims);
    func_0204a8d4(wk->areaChars);
    func_0204ada4(wk->areaPalette);
    func_0204afd8(wk->seasonCellAnims);
    func_0204a8d4(wk->seasonChars);
    func_0204ada4(wk->seasonPalette);
    func_0204afd8(wk->mapCellAnims);
    func_0204a8d4(wk->mapChars);
    func_0204ada4(wk->mapPalette);
    func_0204b1cc(wk->unit2);
    func_0204b084(wk->renderer);
}

void func_ov208_021c0450(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    ClActorSetup setup;
    u8 i;

    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    for (i = 0; i < TOWNMAP_PLACE_COUNT; i++) {
        setup.x = func_ov072_021bbb3c(wk->townmap, i, TOWNMAP_PARAM_AREA_X);
        setup.y = func_ov072_021bbb3c(wk->townmap, i, TOWNMAP_PARAM_AREA_Y);
        wk->areas[i] = func_0204b294(wk->unit, wk->areaChars, wk->areaPalette, wk->areaCellAnims, &setup, 0, param->heapId);
        func_0204b878(wk->areas[i], func_ov072_021bbb3c(wk->townmap, i, TOWNMAP_PARAM_AREA_ANIM));
        func_0204b92c(wk->areas[i], TRUE);
        func_0204b84c(wk->areas[i], 2);
        func_0204b800(wk->areas[i], 4);
        func_0204b3dc(wk->areas[i], FALSE);
        func_0204b688(wk->areas[i], 1);
    }
}


void func_ov208_021c0524(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    u8 i;

    for (i = 0; i < TOWNMAP_PLACE_COUNT; i++) {
        func_0204b3b4(wk->areas[i]);
    }
}

static inline BOOL ZukanDetailMap_IsPlaceHidden(GameData *gameData, u16 flag) {
    if (func_ov010_02166cd0(gameData, flag) == FALSE) {
        return TRUE;
    }
    return FALSE;
}

void func_ov208_021c0544(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    GameData *gameData = func_ov208_021bc3a4(common);
    ClActorSetup setup;
    u8 i;

    for (i = 0; i < TOWNMAP_PLACE_COUNT; i++) {
        u16 flag = func_ov072_021bbb3c(wk->townmap, i, TOWNMAP_PARAM_FLAG);

        if (flag != TOWNMAP_NO_FLAG) {
            if (ZukanDetailMap_IsPlaceHidden(gameData, flag)) {
                wk->places[i].state = PLACE_HIDDEN;
                wk->places[i].actor = NULL;
            } else {
                u16 type = func_ov072_021bbb3c(wk->townmap, i, TOWNMAP_PARAM_TYPE);

                setup.x = func_ov072_021bbb3c(wk->townmap, i, TOWNMAP_PARAM_X);
                setup.y = func_ov072_021bbb3c(wk->townmap, i, TOWNMAP_PARAM_Y);
                setup.sequence = type == TOWNMAP_PLACE_TYPE_TOWN ? 14 : 16;
                setup.priority = 5;
                setup.bgPriority = 2;
                wk->places[i].state = PLACE_SHOWN;
                wk->places[i].actor = func_0204b294(wk->unit2, wk->mapChars, wk->mapPalette, wk->mapCellAnims, &setup,
                                                    CLACT_SURFACE_MAIN, param->heapId);
                func_0204b92c(wk->places[i].actor, TRUE);
                func_0204b3dc(wk->places[i].actor, TRUE);
                func_0204b688(wk->places[i].actor, 1);
            }
        } else {
            wk->places[i].state = PLACE_NONE;
            wk->places[i].actor = NULL;
        }
    }
}


void func_ov208_021c06a0(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    u8 i;

    for (i = 0; i < TOWNMAP_PLACE_COUNT; i++) {
        if (wk->places[i].state == PLACE_SHOWN) {
            func_0204b3b4(wk->places[i].actor);
        }
    }
}

void func_ov208_021c06c8(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    u8 i;

    for (i = 0; i < WINDOW_COUNT; i++) {
        wk->printQueues[i] = func_0201c440(param->heapId);
        wk->transferPending[i] = FALSE;
    }
}

void func_ov208_021c06ec(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    u8 i;

    for (i = 0; i < WINDOW_COUNT; i++) {
        wk->transferPending[i] = FALSE;
        func_0201c6ec(wk->printQueues[i]);
        func_0201c4c0(wk->printQueues[i]);
    }
}

void func_ov208_021c0714(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    u8 i;

    for (i = 0; i < WINDOW_COUNT; i++) {
        func_0201c4e4(wk->printQueues[i]);
        func_ov208_021c0740(param, wk, common, i);
    }
}

void func_ov208_021c0740(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common, u8 window) {
    if (wk->transferPending[window] && !func_0201c6c4(wk->printQueues[window], BmpWin_GetBitmap(wk->windows[window]))) {
        BmpWin *bmpWin = wk->windows[window];

        BmpWin_FlushChar(bmpWin);
        BmpWin_FlushMap(bmpWin);
        GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(bmpWin));
        wk->transferPending[window] = FALSE;
    }
}

void func_ov208_021c0788(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    ZukanDetailWindowData bgWindows[2] = {
        { 6, 0, 0, 1, 1, 8, 0 },
        { 0, 0, 0, 1, 1, 0, 0 },
    };
    ZukanDetailWindowData windows[WINDOW_COUNT] = {
        { 6, 12, 21, 16, 3, 8, 0 },
        { 6, 9, 25, 16, 2, 8, 0 },
        { 0, 2, 0, 15, 2, 0, 0 },
        { 0, 8, 10, 16, 2, 0, 0 },
    };
    u8 i;

    GFL_G2DIOLoadNCLR(ARCID_FONT, 5, PALTYPE_SUB_BG, 0, 8 * 0x20, 0x20, param->heapId);
    GFL_G2DIOLoadNCLR(ARCID_FONT, 5, PALTYPE_MAIN_BG, 0, 0, 0x20, param->heapId);
    for (i = 0; i < 2; i++) {
        wk->bgWindows[i] = BmpWin_CreateDynamic(bgWindows[i].bg, bgWindows[i].x, bgWindows[i].y, bgWindows[i].width,
                                                bgWindows[i].height, bgWindows[i].palette, bgWindows[i].fromEnd);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->bgWindows[i]), 0);
        BmpWin_FlushChar(wk->bgWindows[i]);
    }
    for (i = 0; i < WINDOW_COUNT; i++) {
        wk->windows[i] = BmpWin_CreateDynamic(windows[i].bg, windows[i].x, windows[i].y, windows[i].width,
                                              windows[i].height, windows[i].palette, windows[i].fromEnd);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[i]), 0);
        BmpWin_FlushChar(wk->windows[i]);
    }
    wk->msgData[0] = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_BTL_SERVER_FLOW_TITLE, param->heapId);
    wk->msgData[1] = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_PLACE_NAMES, param->heapId);
}


void func_ov208_021c0898(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    u8 i;

    for (i = 0; i < 2; i++) {
        GFL_MsgDataFree(wk->msgData[i]);
    }
    for (i = 0; i < 4; i++) {
        wk->transferPending[i] = 0;
        func_0201c6ec(wk->printQueues[i]);
        BmpWin_Free(wk->windows[i]);
    }
    for (i = 0; i < 2; i++) {
        BmpWin_Free(wk->bgWindows[i]);
    }
}


void func_ov208_021c08e8(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_GRA, param->heapId);

    GFL_G2DIOLoadArcNCLR(arc, 2, PALTYPE_SUB_BG, 0, 0, 8 * 0x20, param->heapId);
    wk->panelChars = GFL_BGSysLoadArcNCGRDynamic(arc, 12, 7, 0, FALSE, param->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 38, 7, 0, CHAR_POS(wk->panelChars), 0, FALSE, param->heapId);
    GFL_ArcToolFree(arc);
}

void func_ov208_021c0950(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    GFL_BGSysFreeCharMemory(7, (u16)wk->panelChars, (u16)(wk->panelChars >> 16));
}

void func_ov208_021c0968(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    func_ov208_021c0980(param, wk, common);
    func_ov208_021c0a88(param, wk, common);
}

void func_ov208_021c0980(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    switch (wk->panelSlide) {
    case SLIDE_NONE:
        break;
    case SLIDE_IN:
        if (GFL_BGSysGetBGOffsetY(7) >= 64) {
            GFL_BGSysMoveBGReq(7, BG_MOVE_SET_Y, 64);
            wk->panelSlide = SLIDE_DONE;
        } else {
            GFL_BGSysMoveBGReq(7, BG_MOVE_DOWN, 8);
            GFL_BGSysMoveBGReq(6, BG_MOVE_DOWN, 8);
        }
        break;
    case SLIDE_DONE:
        break;
    case SLIDE_OUT:
        if (GFL_BGSysGetBGOffsetY(7) <= 0) {
            GFL_BGSysMoveBGReq(7, BG_MOVE_SET_Y, 0);
            wk->panelSlide = SLIDE_NONE;
        } else {
            GFL_BGSysMoveBGReq(7, BG_MOVE_UP, 8);
            GFL_BGSysMoveBGReq(6, BG_MOVE_UP, 8);
        }
        break;
    }

    if (wk->icons[wk->icon] != NULL) {
        ClActorPos pos;

        switch (wk->iconSlide) {
        case SLIDE_NONE:
            break;
        case SLIDE_IN:
            func_0204b454(wk->icons[wk->icon], &pos, CLACT_SURFACE_SUB);
            if (pos.y <= 112) {
                pos.y = 112;
                wk->iconSlide = SLIDE_DONE;
            } else {
                pos.y -= 8;
            }
            func_0204b404(wk->icons[wk->icon], &pos, CLACT_SURFACE_SUB);
            break;
        case SLIDE_DONE:
            break;
        case SLIDE_OUT:
            func_0204b454(wk->icons[wk->icon], &pos, CLACT_SURFACE_SUB);
            if (pos.y >= 176) {
                pos.y = 176;
                wk->iconSlide = SLIDE_NONE;
            } else {
                pos.y += 8;
            }
            func_0204b404(wk->icons[wk->icon], &pos, CLACT_SURFACE_SUB);
            break;
        }
    }
}




void func_ov208_021c0a88(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
}

void func_ov208_021c0a8c(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    ZukanDetailTouchbar *touchbar = func_ov208_021bc3b4(common);
    u16 anim;
    u8 other;
    u16 otherAnim;

    if (wk->arrow == 2) {
        anim = 2;
        other = 1;
        otherAnim = 4;
    } else if (wk->arrow == 1) {
        anim = 4;
        other = 2;
        otherAnim = 2;
    }
    if (wk->arrow != 7 && !func_0204b98c(wk->actors[wk->arrow])) {
        func_0204b878(wk->actors[wk->arrow], anim);
        func_0204b92c(wk->actors[other], TRUE);
        func_0204b878(wk->actors[other], otherAnim);
        wk->arrow = 7;
        if (wk->state == 0 || wk->state == 5) {
            func_ov208_021bdc48(touchbar, TRUE);
        }
    }
}


void func_ov208_021c0b10(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_POKEICON, param->heapId);

    wk->iconPalette = func_0204acec(arc, func_0201bc3c(), 1, 0, param->heapId);
    wk->iconCellAnims = func_0204af28(arc, func_0201bc78(), getOBJTileMapping_SubEng(), param->heapId);
    GFL_ArcToolFree(arc);
}

void func_ov208_021c0b58(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    func_0204afd8(wk->iconCellAnims);
    func_0204ada4(wk->iconPalette);
}

ClActor *func_ov208_021c0b6c(u32 *chars, u32 palette, u32 cellAnims, ClActUnit *unit, HeapID heapId, u32 species, u32 form, u32 sex, BOOL egg, u8 x, u8 y) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_POKEICON, heapId);
    ClActorSetup setup;
    ClActor *actor;

    setup.x = x;
    setup.y = y;
    setup.sequence = 1;
    setup.priority = 1;
    setup.bgPriority = 0;
    *chars = func_0204a6c8(arc, PokeParty_GetIconIndex(species, form, sex, egg), FALSE, CLACT_VRAM_SUB, heapId);
    actor = func_0204b294(unit, *chars, palette, cellAnims, &setup, CLACT_SURFACE_SUB, heapId);
    func_0204b92c(actor, FALSE);
    func_0204b6f4(actor, func_0201bb5c(species, form, sex, egg), 0);
    func_0204b688(actor, 0);
    GFL_ArcToolFree(arc);
    return actor;
}

void func_ov208_021c0c00(u32 chars, ClActor *actor) {
    func_0204b3b4(actor);
    func_0204a8d4(chars);
}

u8 func_ov208_021c0c14(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    GameData *gameData = func_ov208_021bc3a4(common);
    u16 zone = PlayerState_GetZoneID(GameData_GetPlayerState(gameData));
    u8 place = NO_PLACE;
    u16 found = func_ov072_021bbb48(wk->townmap, func_ov010_02166c7c(gameData, zone));

    if (found != TOWNMAP_PLACE_NONE) {
        place = found;
    }
    return place;
}

void func_ov208_021c0c50(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    u8 place = func_ov208_021c0c14(param, wk, common);

    if (place != NO_PLACE) {
        ClActorPos pos;

        pos.x = func_ov072_021bbb3c(wk->townmap, place, TOWNMAP_PARAM_X);
        pos.y = func_ov072_021bbb3c(wk->townmap, place, TOWNMAP_PARAM_Y);
        func_0204b404(wk->actors[ACTOR_PLAYER], &pos, CLACT_SURFACE_MAIN);
    }
}



void func_ov208_021c0c8c(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common, BOOL visible) {
    if (visible) {
        if (func_ov208_021c0c14(param, wk, common) != NO_PLACE) {
            func_0204b878(wk->actors[ACTOR_PLAYER], 6);
            func_0204b3dc(wk->actors[ACTOR_PLAYER], TRUE);
        }
    } else {
        func_0204b3dc(wk->actors[ACTOR_PLAYER], FALSE);
    }
}

BOOL func_ov208_021c0cc0(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    return func_0204b3f4(wk->actors[ACTOR_PLAYER]);
}

void func_ov208_021c0ccc(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    ClActorPos pos = data_ov208_021c8554;

    if (wk->place == NO_PLACE) {
        wk->place = func_ov208_021c0c14(param, wk, common);
    }
    if (wk->place != NO_PLACE) {
        pos.x = func_ov072_021bbb3c(wk->townmap, wk->place, TOWNMAP_PARAM_X);
        pos.y = func_ov072_021bbb3c(wk->townmap, wk->place, TOWNMAP_PARAM_Y);
    }
    func_0204b404(wk->actors[ACTOR_CURSOR], &pos, CLACT_SURFACE_MAIN);
    func_0204b3dc(wk->actors[ACTOR_CURSOR], TRUE);
}


void func_ov208_021c0d3c(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    wk->place = func_ov208_021c0c14(param, wk, common);
    func_ov208_021c0ccc(param, wk, common);
}

u8 func_ov208_021c0d5c(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common, u8 x, u8 y) {
    CalcSphere sphere;
    VecFx32 center;
    CalcCapsule capsule;
    VecFx32 start;
    VecFx32 end;
    CalcHitResult result;
    fx32 minDist;
    u8 place = NO_PLACE;
    u8 i;

    center.x = FX32_CONST(x);
    center.y = FX32_CONST(y);
    center.z = 0;
    func_02051f1c(&sphere, &center, 0);
    for (i = 0; i < TOWNMAP_PLACE_COUNT; i++) {
        if (wk->places[i].state != PLACE_HIDDEN) {
            start.x = FX32_CONST(func_ov072_021bbb3c(wk->townmap, i, TOWNMAP_PARAM_HIT_START_X));
            start.y = FX32_CONST(func_ov072_021bbb3c(wk->townmap, i, TOWNMAP_PARAM_HIT_START_Y));
            start.z = 0;
            end.x = FX32_CONST(func_ov072_021bbb3c(wk->townmap, i, TOWNMAP_PARAM_HIT_END_X));
            end.y = FX32_CONST(func_ov072_021bbb3c(wk->townmap, i, TOWNMAP_PARAM_HIT_END_Y));
            end.z = 0;
            func_0205207c(&capsule, &start, &end, FX32_CONST(func_ov072_021bbb3c(wk->townmap, i, TOWNMAP_PARAM_HIT_RADIUS)));
            if (func_02051f34(&capsule, &sphere, &result)) {
                if (place == NO_PLACE) {
                    minDist = result.dist;
                    place = i;
                } else if (minDist > result.dist) {
                    minDist = result.dist;
                    place = i;
                }
            }
        }
    }
    return place;
}


BOOL func_ov208_021c100c(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common, u8 place, u8 x, u8 y, u32 *distSq) {
    u16 placeX;
    u16 placeY;
    u32 dist;

    if (wk->places[place].state == 1) {
        return FALSE;
    }
    placeX = func_ov072_021bbb3c(wk->townmap, place, TOWNMAP_PARAM_CURSOR_X);
    placeY = func_ov072_021bbb3c(wk->townmap, place, TOWNMAP_PARAM_CURSOR_Y);
    dist = (placeX - x) * (placeX - x) + (placeY - y) * (placeY - y);
    if (dist < 0x90) {
        if (distSq != NULL) {
            *distSq = dist;
        }
        return TRUE;
    }
    return FALSE;
}

u8 func_ov208_021c1068(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common, u8 x, u8 y) {
    u8 best = NO_PLACE;
    u8 i;
    u32 dist;
    u32 minDist;

    for (i = 0; i < TOWNMAP_PLACE_COUNT; i++) {
        if (func_ov208_021c100c(param, wk, common, i, x, y, &dist)) {
            if (best == NO_PLACE) {
                best = i;
                minDist = dist;
            } else if (minDist > dist) {
                best = i;
                minDist = dist;
            }
        }
    }
    return best;
}

void func_ov208_021c10b8(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    ZukanDetailTouchbar *touchbar = func_ov208_021bc3b4(common);

    if (!wk->inputEnabled || wk->arrow != ACTOR_NONE) {
        return;
    }

    switch (wk->state) {
    case MAP_STATE_MAP: {
        BOOL handled = FALSE;
        BOOL open = FALSE;

        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
            open = TRUE;
            handled = TRUE;
        } else if (wk->bySeason) {
            if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_R) {
                wk->season++;
                if (wk->season >= SEASON_COUNT) {
                    wk->season = 0;
                }
                func_ov208_021c1e10(param, wk, common, TRUE, FALSE);
                GFL_SndSEPlay(SEQ_SE_SELECT3);
                handled = TRUE;
            } else if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_L) {
                if (wk->season == 0) {
                    wk->season = SEASON_COUNT - 1;
                } else {
                    wk->season--;
                }
                func_ov208_021c1e10(param, wk, common, FALSE, FALSE);
                GFL_SndSEPlay(SEQ_SE_SELECT3);
                handled = TRUE;
            }
        }
        if (handled) {
            func_020355d8(FALSE);
            if (open) {
                wk->touched = FALSE;
            }
        }
        if (!handled) {
            u32 x;
            u32 y;

            if (func_02035d20(&x, &y)) {
                if (!wk->bySeason) {
                    if (y < 168) {
                        open = TRUE;
                        handled = TRUE;
                    }
                } else if (x >= 136 && x < 160 && y < 16) {
                    wk->season++;
                    if (wk->season >= SEASON_COUNT) {
                        wk->season = 0;
                    }
                    handled = TRUE;
                    func_ov208_021c1e10(param, wk, common, TRUE, TRUE);
                    GFL_SndSEPlay(SEQ_SE_SELECT3);
                } else if (x < 24 && y < 16) {
                    if (wk->season == 0) {
                        wk->season = SEASON_COUNT - 1;
                    } else {
                        wk->season--;
                    }
                    handled = TRUE;
                    func_ov208_021c1e10(param, wk, common, FALSE, TRUE);
                    GFL_SndSEPlay(SEQ_SE_SELECT3);
                } else if ((x >= 160 || y >= 24) && y < 168) {
                    open = TRUE;
                    handled = TRUE;
                }
            }
            if (handled) {
                func_020355d8(TRUE);
                if (open) {
                    wk->touched = TRUE;
                    wk->touchX = x;
                    wk->touchY = y;
                }
            }
        }
        if (open && (wk->bySeason || wk->found)) {
            func_ov208_021c1920(param, wk, common, MAP_STATE_OPEN_START);
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov208_021bdc48(touchbar, FALSE);
        }
        break;
    }
    case MAP_STATE_PLACE: {
        BOOL handled = FALSE;
        BOOL move = TRUE;

        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
            wk->inputEnabled = FALSE;
            handled = TRUE;
            move = FALSE;
        } else if (wk->bySeason) {
            if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_R) {
                wk->season++;
                if (wk->season >= SEASON_COUNT) {
                    wk->season = 0;
                }
                move = FALSE;
                func_ov208_021c1e10(param, wk, common, TRUE, FALSE);
                GFL_SndSEPlay(SEQ_SE_SELECT3);
                handled = TRUE;
            } else if (GCTX_HIDGetTypedKeys() & PAD_BUTTON_L) {
                if (wk->season == 0) {
                    wk->season = SEASON_COUNT - 1;
                } else {
                    wk->season--;
                }
                move = FALSE;
                func_ov208_021c1e10(param, wk, common, FALSE, FALSE);
                GFL_SndSEPlay(SEQ_SE_SELECT3);
                handled = TRUE;
            }
        }
        if (handled) {
            func_020355d8(FALSE);
        }
        if (!handled) {
            u32 x;
            u32 y;

            if (func_02035d20(&x, &y)) {
                if (wk->bySeason == FALSE) {
                    if (y >= 168) {
                        move = FALSE;
                    }
                } else if (wk->bySeason == TRUE) {
                    if (x >= 136 && x < 160 && y < 16) {
                        wk->season++;
                        if (wk->season >= SEASON_COUNT) {
                            wk->season = 0;
                        }
                        handled = TRUE;
                        func_ov208_021c1e10(param, wk, common, TRUE, TRUE);
                        GFL_SndSEPlay(SEQ_SE_SELECT3);
                        move = FALSE;
                    } else if (x < 24 && y < 16) {
                        if (wk->season == 0) {
                            wk->season = SEASON_COUNT - 1;
                        } else {
                            wk->season--;
                        }
                        handled = TRUE;
                        func_ov208_021c1e10(param, wk, common, FALSE, TRUE);
                        move = FALSE;
                        GFL_SndSEPlay(SEQ_SE_SELECT3);
                    } else if (x < 160 && y < 24) {
                        move = FALSE;
                    } else if (y >= 168) {
                        move = FALSE;
                    }
                }
            }
            if (handled) {
                func_020355d8(TRUE);
            }
        }
        if (move) {
            func_ov208_021c1450(param, wk, common);
        }
        break;
    }
    }
}

void func_ov208_021c1450(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    u32 x;
    u32 y;

    if (!wk->found) {
        return;
    }
    if (!func_020355c4()) {
        if (func_02035d20(&x, &y)) {
            func_020355d8(TRUE);
            func_0204b3dc(wk->actors[ACTOR_CURSOR], FALSE);
        }
    } else if (GCTX_HIDGetPressedKeys()) {
        func_020355d8(FALSE);
        func_ov208_021c0ccc(param, wk, common);
        func_ov208_021c1d30(param, wk, common);
        func_ov208_021c2238(param, wk, common);
        return;
    }

    if (!func_020355c4()) {
        u8 prevPlace = wk->place;
        BOOL moved = FALSE;
        BOOL nearFound = FALSE;
        BOOL snap = FALSE;
        VecFx32 dir = { 0, 0, 0 };
        VecFx32 toPlace = { 0, 0, 0 };
        VecFx32 placeVec;
        VecFx32 cursorVec;
        ClActorPos cursorPos;
        ClActorPos placePos;
        u8 nearPlace;
        u32 held;

        func_0204b454(wk->actors[ACTOR_CURSOR], &cursorPos, CLACT_SURFACE_MAIN);
        held = GCTX_HIDGetHeldKeys();
        if ((held & PAD_KEY_RIGHT) || (held & PAD_KEY_LEFT) || (held & PAD_KEY_UP) || (held & PAD_KEY_DOWN)) {
            moved = TRUE;
            if (held & PAD_KEY_RIGHT) {
                dir.x += FX32_ONE;
            }
            if (held & PAD_KEY_LEFT) {
                dir.x -= FX32_ONE;
            }
            if (held & PAD_KEY_UP) {
                dir.y -= FX32_ONE;
            }
            if (held & PAD_KEY_DOWN) {
                dir.y += FX32_ONE;
            }
        }
        nearPlace = func_ov208_021c1068(param, wk, common, cursorPos.x, cursorPos.y);
        if (nearPlace != NO_PLACE) {
            nearFound = TRUE;
            placePos.x = func_ov072_021bbb3c(wk->townmap, nearPlace, TOWNMAP_PARAM_CURSOR_X);
            placePos.y = func_ov072_021bbb3c(wk->townmap, nearPlace, TOWNMAP_PARAM_CURSOR_Y);
            placeVec.x = FX32_CONST(placePos.x);
            placeVec.y = FX32_CONST(placePos.y);
            placeVec.z = 0;
            cursorVec.x = FX32_CONST(cursorPos.x);
            cursorVec.y = FX32_CONST(cursorPos.y);
            cursorVec.z = 0;
            VEC_Subtract(&placeVec, &cursorVec, &toPlace);
            if (VEC_Mag(&toPlace) < FX32_CONST(1.732f)) {
                snap = TRUE;
            }
            if (VEC_Mag(&toPlace) > FX32_ONE) {
                VEC_Normalize(&toPlace, &toPlace);
                vecfx_mul(&toPlace, FX32_ONE, &toPlace);
            }
        }
        if (!moved && !nearFound) {
            return;
        }
        if (!moved && nearFound && snap) {
            cursorPos = placePos;
        } else {
            fx32 dx;
            fx32 dy;

            VEC_Normalize(&dir, &dir);
            dx = toPlace.x + dir.x * 3;
            dy = toPlace.y + dir.y * 3;
            if (dx > 0 && dx < FX32_ONE) {
                dx = FX32_ONE;
            } else if (dx > -FX32_ONE && dx < 0) {
                dx = -FX32_ONE;
            }
            if (dy > 0 && dy < FX32_ONE) {
                dy = FX32_ONE;
            } else if (dy > -FX32_ONE && dy < 0) {
                dy = -FX32_ONE;
            }
            cursorPos.x += (s16)(dx >> FX32_SHIFT);
            cursorPos.y += (s16)(dy >> FX32_SHIFT);
        }
        cursorPos.x = MATH_CLAMP(cursorPos.x, 0, 248);
        cursorPos.y = MATH_CLAMP(cursorPos.y, 8, 168);
        func_0204b404(wk->actors[ACTOR_CURSOR], &cursorPos, CLACT_SURFACE_MAIN);
        wk->place = func_ov208_021c0d5c(param, wk, common, cursorPos.x, cursorPos.y);
        if (wk->place != prevPlace) {
            func_ov208_021c1d30(param, wk, common);
            func_ov208_021c2238(param, wk, common);
            if (wk->place != NO_PLACE) {
                GFL_SndSEPlay(SEQ_SE_SELECT1);
            }
        }
    } else if (func_02035d20(&x, &y)) {
        wk->place = func_ov208_021c0d5c(param, wk, common, x, y);
        func_ov208_021c1d30(param, wk, common);
        func_ov208_021c2238(param, wk, common);
        if (wk->place != NO_PLACE) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
        }
    }
}



void func_ov208_021c1810(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    switch (wk->state) {
    case MAP_STATE_MAP:
        break;
    case MAP_STATE_OPEN_START:
        func_ov208_021c1920(param, wk, common, MAP_STATE_OPEN);
        break;
    case MAP_STATE_OPEN:
        func_ov208_021c1920(param, wk, common, MAP_STATE_PLACE);
        break;
    case MAP_STATE_CHANGE:
        switch (wk->step) {
        case 1:
            func_ov208_021c1fe0(param, wk, common);
            wk->step = 2;
            break;
        case 2:
            func_ov208_021c2090(param, wk, common);
            wk->step = 3;
            break;
        case 3:
            func_ov208_021c20c0(param, wk, common);
            wk->step = 0;
            func_ov208_021c1920(param, wk, common, MAP_STATE_MAP);
            break;
        }
        break;
    case MAP_STATE_EXIT:
        switch (wk->step) {
        case 1:
            func_ov208_021c21d0(param, wk, common);
            wk->step = 2;
            break;
        case 2:
            func_ov208_021c2228(param, wk, common);
            wk->step = 3;
            break;
        case 3:
            func_ov208_021c2234();
            wk->step = 0;
            break;
        }
        break;
    case MAP_STATE_PLACE:
        break;
    case MAP_STATE_CLOSE_START:
        func_ov208_021c1920(param, wk, common, MAP_STATE_CLOSE);
        break;
    case MAP_STATE_CLOSE:
        func_ov208_021c1920(param, wk, common, MAP_STATE_MAP);
        break;
    case MAP_STATE_PLACE_CHANGE:
        switch (wk->step) {
        case 1:
            func_ov208_021c1fe0(param, wk, common);
            wk->step = 2;
            break;
        case 2:
            func_ov208_021c2090(param, wk, common);
            wk->step = 3;
            break;
        case 3:
            func_ov208_021c20c0(param, wk, common);
            wk->step = 0;
            func_ov208_021c1920(param, wk, common, MAP_STATE_PLACE);
            break;
        }
        break;
    }
}

void func_ov208_021c1920(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common, int state) {
    ZukanDetailTouchbar *touchbar = func_ov208_021bc3b4(common);

    switch (wk->state) {
    case MAP_STATE_MAP:
        if (state == MAP_STATE_OPEN_START) {
        } else if (state == MAP_STATE_CHANGE) {
            wk->step = 1;
        } else if (state == MAP_STATE_EXIT) {
            wk->step = 1;
        }
        break;
    case MAP_STATE_OPEN_START:
        if (state == MAP_STATE_OPEN) {
            func_ov208_021bdbac(touchbar, FALSE);
        }
        break;
    case MAP_STATE_OPEN:
        if (state == MAP_STATE_PLACE) {
            wk->panelSlide = SLIDE_IN;
            wk->iconSlide = SLIDE_IN;
            if (wk->found == TRUE) {
                if (!wk->touched) {
                    func_ov208_021c0d3c(param, wk, common);
                } else {
                    wk->place = func_ov208_021c0d5c(param, wk, common, wk->touchX, wk->touchY);
                    if (wk->place != NO_PLACE) {
                        func_0204b3dc(wk->actors[ACTOR_PLACE], TRUE);
                    }
                }
                func_ov208_021c1d30(param, wk, common);
                func_ov208_021c2238(param, wk, common);
            }
            func_ov208_021bdaac(touchbar, ZUKAN_DETAIL_TOUCHBAR_MAP, ZUKAN_DETAIL_PAGE_MAP - 1,
                                func_ov208_021bc3bc(common) > 1 ? TRUE : FALSE);
        }
        break;
    case MAP_STATE_CHANGE:
        if (state == MAP_STATE_MAP) {
            if (wk->arrowPushed) {
                wk->arrowLoaded = TRUE;
                if (wk->arrowDone) {
                    wk->arrowPushed = FALSE;
                    func_ov208_021bdba0(touchbar);
                }
            }
            if (wk->arrow == ACTOR_NONE && wk->active) {
                func_ov208_021bdc48(touchbar, TRUE);
            }
        }
        break;
    case MAP_STATE_EXIT:
        break;
    case MAP_STATE_PLACE:
        if (state == MAP_STATE_CLOSE_START) {
        } else if (state == MAP_STATE_PLACE_CHANGE) {
            wk->step = 1;
        }
        break;
    case MAP_STATE_CLOSE_START:
        if (state == MAP_STATE_CLOSE) {
            func_ov208_021bdbac(touchbar, FALSE);
        }
        break;
    case MAP_STATE_CLOSE:
        if (state == MAP_STATE_MAP) {
            BOOL showArrows;

            wk->panelSlide = SLIDE_OUT;
            wk->iconSlide = SLIDE_OUT;
            func_0204b3dc(wk->actors[ACTOR_CURSOR], FALSE);
            func_0204b3dc(wk->actors[ACTOR_PLACE], FALSE);
            showArrows = FALSE;
            if (func_ov208_021bc3bc(common) > 1) {
                showArrows = TRUE;
            }
            func_ov208_021bdaac(touchbar, ZUKAN_DETAIL_TOUCHBAR_GENERAL, ZUKAN_DETAIL_PAGE_MAP - 1, showArrows);
            func_ov208_021bdc14(touchbar, GameData_IsShortcutRegistered(func_ov208_021bc3a4(common), SHORTCUT_POKEDEX_MAP));
        }
        break;
    case MAP_STATE_PLACE_CHANGE:
        if (state == MAP_STATE_PLACE) {
            func_ov208_021bdba0(touchbar);
            if (wk->arrow == ACTOR_NONE && wk->active) {
                func_ov208_021bdc48(touchbar, TRUE);
            }
        }
        break;
    }
    wk->state = state;
}

void func_ov208_021c1ad4(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    BOOL wasBySeason = wk->bySeason;
    u16 species = func_ov208_021bc3c0(common);
    WordSet *wordSet;
    StrBuf *format;
    StrBuf *name;
    u32 sex;
    u32 rare;
    u32 form;

    func_ov208_021c1f44(wk->habitat);
    wk->habitat = func_ov208_021c1f28(species, param->heapId, wk->habitatArc);
    wk->bySeason = wk->habitat->allYear == FALSE ? TRUE : FALSE;
    if (!wk->bySeason) {
        wk->found = wk->habitat->seasons[0].none == FALSE ? TRUE : FALSE;
    } else {
        wk->found = wk->habitat->seasons[wk->season].none == FALSE ? TRUE : FALSE;
    }

    if (wk->icons[wk->icon] != NULL) {
        func_0204b3dc(wk->icons[wk->icon], FALSE);
    }
    wk->icon = (wk->icon + 1) % 2;
    if (wk->icons[wk->icon] != NULL) {
        if (wk->icons[wk->icon] != NULL) {
            func_ov208_021c0c00(wk->iconChars[wk->icon], wk->icons[wk->icon]);
        }
        wk->icons[wk->icon] = NULL;
    }
    func_0200cc40(GameData_GetPokedex(func_ov208_021bc3a4(common)), species, &sex, &rare, &form, param->heapId);
    wk->icons[wk->icon] = func_ov208_021c0b6c(&wk->iconChars[wk->icon], wk->iconPalette, wk->iconCellAnims,
                                              wk->unit, param->heapId, species, form, sex, FALSE, 56, 176);

    wk->transferPending[WINDOW_NAME] = FALSE;
    func_0201c6ec(wk->printQueues[WINDOW_NAME]);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[WINDOW_NAME]), 0);
    wordSet = GFL_WordSetSystemCreateDefault(param->heapId);
    format = GFL_MsgDataLoadStrbufNew(wk->msgData[0], BtlServerFlowTitle_Text_189);
    name = GFL_StrBufCreate(64, param->heapId);
    WordSet_LoadSpeciesName(wordSet, 0, species);
    GFL_WordSetFormatStrbuf(wordSet, name, format);
    GFL_StrBufFree(format);
    GFL_WordSetSystemFree(wordSet);
    func_0201c724(wk->printQueues[WINDOW_NAME], BmpWin_GetBitmap(wk->windows[WINDOW_NAME]), 0, 5, name, wk->font,
                  PRINT_COLOR(15, 2, 0));
    GFL_StrBufFree(name);
    wk->transferPending[WINDOW_NAME] = TRUE;
    func_ov208_021c0740(param, wk, common, WINDOW_NAME);

    if (!wk->bySeason) {
        if (wasBySeason == TRUE) {
            wk->transferPending[WINDOW_SEASON] = FALSE;
            func_0201c6ec(wk->printQueues[WINDOW_SEASON]);
            GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[WINDOW_SEASON]), 0);
            BmpWin_FlushChar(wk->windows[WINDOW_SEASON]);
            func_0204b3dc(wk->actors[ACTOR_SEASON], FALSE);
            func_0204b3dc(wk->actors[ACTOR_SEASON_NEXT], FALSE);
            func_0204b3dc(wk->actors[ACTOR_SEASON_PREV], FALSE);
        }
    } else if (wasBySeason == FALSE) {
        func_ov208_021c1f50(param, wk, common);
        func_0204b3dc(wk->actors[ACTOR_SEASON], TRUE);
        func_0204b878(wk->actors[ACTOR_SEASON_NEXT], 2);
        func_0204b3dc(wk->actors[ACTOR_SEASON_NEXT], TRUE);
        func_0204b878(wk->actors[ACTOR_SEASON_PREV], 4);
        func_0204b3dc(wk->actors[ACTOR_SEASON_PREV], TRUE);
    }
}






void func_ov208_021c1d0c(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    func_ov208_021c1fc4(param, wk, common);
    func_ov208_021c2238(param, wk, common);
}

void func_ov208_021c1d24(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    func_ov208_021c1920(param, wk, common, MAP_STATE_EXIT);
}

void func_ov208_021c1d30(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    wk->transferPending[WINDOW_PLACE] = FALSE;
    func_0201c6ec(wk->printQueues[WINDOW_PLACE]);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[WINDOW_PLACE]), 0);
    if (wk->place != NO_PLACE) {
        StrBuf *strbuf = GFL_MsgDataLoadStrbufNew(
            wk->msgData[1], ZoneData_GetPlaceNameID(func_ov072_021bbb3c(wk->townmap, wk->place, TOWNMAP_PARAM_ZONE)));
        ClActorPos pos;

        func_0201c724(wk->printQueues[WINDOW_PLACE], BmpWin_GetBitmap(wk->windows[WINDOW_PLACE]), 0, 1, strbuf,
                      wk->font, PRINT_COLOR(15, 2, 0));
        GFL_StrBufFree(strbuf);
        wk->transferPending[WINDOW_PLACE] = TRUE;
        func_ov208_021c0740(param, wk, common, WINDOW_PLACE);
        pos.x = func_ov072_021bbb3c(wk->townmap, wk->place, TOWNMAP_PARAM_X);
        pos.y = func_ov072_021bbb3c(wk->townmap, wk->place, TOWNMAP_PARAM_Y);
        func_0204b878(wk->actors[ACTOR_PLACE], 5);
        func_0204b404(wk->actors[ACTOR_PLACE], &pos, CLACT_SURFACE_MAIN);
        func_0204b3dc(wk->actors[ACTOR_PLACE], TRUE);
    } else {
        BmpWin_FlushChar(wk->windows[WINDOW_PLACE]);
        func_0204b3dc(wk->actors[ACTOR_PLACE], FALSE);
    }
}

void func_ov208_021c1e10(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common, BOOL next, BOOL touch) {
    ZukanDetailTouchbar *touchbar = func_ov208_021bc3b4(common);
    u16 anim;
    u8 other;

    if (!wk->bySeason) {
        wk->found = wk->habitat->seasons[0].none == FALSE ? TRUE : FALSE;
    } else {
        wk->found = wk->habitat->seasons[wk->season].none == FALSE ? TRUE : FALSE;
    }
    wk->transferPending[WINDOW_SEASON] = FALSE;
    func_0201c6ec(wk->printQueues[WINDOW_SEASON]);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[WINDOW_SEASON]), 0);
    func_ov208_021c1f50(param, wk, common);
    if (next) {
        wk->arrow = ACTOR_SEASON_NEXT;
        anim = 3;
        other = ACTOR_SEASON_PREV;
    } else {
        wk->arrow = ACTOR_SEASON_PREV;
        anim = 5;
        other = ACTOR_SEASON_NEXT;
    }
    func_0204b878(wk->actors[wk->arrow], anim);
    func_0204b92c(wk->actors[other], FALSE);
    func_0204b8c4(wk->actors[other], 0);
    func_ov208_021bdc48(touchbar, FALSE);
    if (touch == TRUE) {
        func_0204b3dc(wk->actors[ACTOR_CURSOR], FALSE);
    } else if (wk->state == MAP_STATE_PLACE && wk->found == TRUE) {
        func_ov208_021c0ccc(param, wk, common);
        func_ov208_021c1d30(param, wk, common);
    }
    func_ov208_021c1fc4(param, wk, common);
    func_ov208_021c2238(param, wk, common);
}


// The palettes of the habitat kinds' icons on the panel, without and with the kind, and the icons' screen areas
u8 data_ov208_021c8e14[3][2] = {
    { 5, 2 },
    { 6, 0 },
    { 7, 4 },
};

u8 data_ov208_021c8e1a[3][4] = {
#include "asset/ov208_021bf784_data_ov208_021c8e1a.inc"
};

ZukanHabitat *func_ov208_021c1f28(u16 species, HeapID heapId, ArcTool *arc) {
    u32 size;

    return GFL_ArcToolReadHeapNewLZGetLen(arc, species - 1, FALSE, heapId, &size);
}

void func_ov208_021c1f44(ZukanHabitat *habitat) {
    if (habitat != NULL) {
        GFL_HeapFree(habitat);
    }
}

void func_ov208_021c1f50(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    StrBuf *strbuf = GFL_MsgDataLoadStrbufNew(wk->msgData[0], BtlServerFlowTitle_Text_190 + wk->season);
    u16 width = GFL_FontGetBlockWidth(strbuf, wk->font, 0);
    int windowWidth = GFL_BitmapGetWidth(BmpWin_GetBitmap(wk->windows[WINDOW_SEASON]));
    u16 x = (windowWidth - width) / 2;

    func_0201c724(wk->printQueues[WINDOW_SEASON], BmpWin_GetBitmap(wk->windows[WINDOW_SEASON]), x, 1, strbuf,
                  wk->font, PRINT_COLOR(15, 2, 0));
    GFL_StrBufFree(strbuf);
    wk->transferPending[WINDOW_SEASON] = TRUE;
    func_ov208_021c0740(param, wk, common, WINDOW_SEASON);
}


void func_ov208_021c1fc4(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    if (wk->state == MAP_STATE_MAP) {
        func_ov208_021c1920(param, wk, common, MAP_STATE_CHANGE);
        return;
    }
    if (wk->state == MAP_STATE_PLACE) {
        func_ov208_021c1920(param, wk, common, MAP_STATE_PLACE_CHANGE);
    }
}

void func_ov208_021c1fe0(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    u8 i;

    if (func_0204b3f4(wk->actors[ACTOR_UNKNOWN])) {
        if (wk->found == TRUE) {
            wk->transferPending[WINDOW_UNKNOWN] = FALSE;
            func_0201c6ec(wk->printQueues[WINDOW_UNKNOWN]);
            GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[WINDOW_UNKNOWN]), 0);
            BmpWin_FlushChar(wk->windows[WINDOW_UNKNOWN]);
            func_0204b3dc(wk->actors[ACTOR_UNKNOWN], FALSE);
        }
    } else {
        for (i = 0; i < TOWNMAP_PLACE_COUNT; i++) {
            func_0204b3dc(wk->areas[i], FALSE);
        }
        if (wk->found == FALSE) {
            func_ov208_021c0c8c(param, wk, common, FALSE);
            wk->place = NO_PLACE;
            func_0204b3dc(wk->actors[ACTOR_CURSOR], FALSE);
            func_0204b3dc(wk->actors[ACTOR_PLACE], FALSE);
            func_ov208_021c1d30(param, wk, common);
            func_ov208_021c2238(param, wk, common);
        }
    }
}


void func_ov208_021c2090(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    if (!func_0204b3f4(wk->actors[ACTOR_UNKNOWN])) {
        if (wk->found == FALSE) {
            wk->vblankRequest = VBLANK_DIM;
        } else {
            wk->vblankRequest = VBLANK_HIGHLIGHT;
        }
    }
}

void func_ov208_021c20c0(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    BOOL unknownShown = func_0204b3f4(wk->actors[ACTOR_UNKNOWN]);

    if (wk->found == FALSE) {
        if (!unknownShown) {
            StrBuf *strbuf = GFL_MsgDataLoadStrbufNew(wk->msgData[0], BtlServerFlowTitle_Text_128);
            u16 width = GFL_FontGetBlockWidth(strbuf, wk->font, 0);
            int windowWidth = GFL_BitmapGetWidth(BmpWin_GetBitmap(wk->windows[WINDOW_UNKNOWN]));
            u16 x = (windowWidth - width) / 2;

            func_0201c724(wk->printQueues[WINDOW_UNKNOWN], BmpWin_GetBitmap(wk->windows[WINDOW_UNKNOWN]), x, 1, strbuf,
                          wk->font, PRINT_COLOR(15, 2, 0));
            GFL_StrBufFree(strbuf);
            wk->transferPending[WINDOW_UNKNOWN] = TRUE;
            func_ov208_021c0740(param, wk, common, WINDOW_UNKNOWN);
            func_0204b3dc(wk->actors[ACTOR_UNKNOWN], TRUE);
        }
    } else {
        u8 season = wk->bySeason == FALSE ? 0 : wk->season;
        u8 i;

        for (i = 0; i < TOWNMAP_PLACE_COUNT; i++) {
            if (wk->habitat->seasons[season].places[wk->placeOrder[i]] != 0 && wk->places[i].state != PLACE_HIDDEN) {
                func_0204b3dc(wk->areas[i], TRUE);
            }
        }
        if (!func_ov208_021c0cc0(param, wk, common)) {
            func_ov208_021c0c8c(param, wk, common, TRUE);
        }
    }
}


void func_ov208_021c21d0(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    u8 i;

    if (func_0204b3f4(wk->actors[ACTOR_UNKNOWN])) {
        wk->transferPending[WINDOW_UNKNOWN] = FALSE;
        func_0201c6ec(wk->printQueues[WINDOW_UNKNOWN]);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->windows[WINDOW_UNKNOWN]), 0);
        BmpWin_FlushChar(wk->windows[WINDOW_UNKNOWN]);
        func_0204b3dc(wk->actors[ACTOR_UNKNOWN], FALSE);
    } else {
        for (i = 0; i < TOWNMAP_PLACE_COUNT; i++) {
            func_0204b3dc(wk->areas[i], FALSE);
        }
    }
}

void func_ov208_021c2228(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    wk->vblankRequest = VBLANK_RESET;
}

void func_ov208_021c2234(void) {
}

void func_ov208_021c2238(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    u8 flags[3] = { FALSE, FALSE, FALSE };
    u8 i;

    if (wk->found == TRUE && wk->place != NO_PLACE) {
        u8 season = wk->bySeason ? wk->season : 0;
        u8 order = wk->placeOrder[wk->place];

        if (wk->habitat->seasons[season].places[order] & HABITAT_LAND) {
            flags[0] = TRUE;
        }
        if (wk->habitat->seasons[season].places[order] & HABITAT_WATER) {
            flags[1] = TRUE;
        }
        if (wk->habitat->seasons[season].places[order] & HABITAT_FISHING) {
            flags[2] = TRUE;
        }
    }
    for (i = 0; i < 3; i++) {
        GFL_BGSysSetScrPaletteNo(7, data_ov208_021c8e1a[i][0], data_ov208_021c8e1a[i][1], data_ov208_021c8e1a[i][2],
            data_ov208_021c8e1a[i][3], data_ov208_021c8e14[i][flags[i]]);
    }
    GFL_BGSysQueueScrLoad(7);
}


void func_ov208_021c2304(ZukanDetailWork *param) {
    G2_SetBlendAlpha(0, 0x3f, 0xc, 4);
}

void func_ov208_021c231c(ZukanDetailWork *wk) {
    func_ov208_021bc518(wk->blendMain);
}

void func_ov208_021c232c(ZukanDetailWork *wk) {
    GX_SetVisibleWnd(GX_WNDMASK_W0 | GX_WNDMASK_W1);
    G2_SetWnd0Position(0, 0, 160, 168);
    G2_SetWnd1Position(160, 0, 0, 168);
    G2_SetWnd0InsidePlane(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3 | GX_PLANEMASK_OBJ, TRUE);
    G2_SetWnd1InsidePlane(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3 | GX_PLANEMASK_OBJ, TRUE);
    G2_SetWndOutsidePlane(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3 | GX_PLANEMASK_OBJ, FALSE);
    func_ov208_021c2430(wk);
}

