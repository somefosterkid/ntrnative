// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/zukan_detail.h: the lines in use so far.
#ifndef BWPORT_APP_ZUKAN_DETAIL_H
#define BWPORT_APP_ZUKAN_DETAIL_H

#include "types.h"
#include "gfl/heap.h"
#include "gfl/clact.h"
#include "gfl/bg_sys.h"
#include "system/printsys.h"

typedef struct ZukanDetailProcSys ZukanDetailProcSys;
typedef struct ZukanDetailCommon ZukanDetailCommon;

// Black 2's, not checked against Black
typedef BOOL (*ZukanDetailProcFunc)(ZukanDetailProcSys *sys, int *seq, void *param, void *work,
                                    ZukanDetailCommon *common);

// Black 2's, not checked against Black
typedef void (*ZukanDetailProcCommandFunc)(ZukanDetailProcSys *sys, int *seq, void *param, void *work,
                                           ZukanDetailCommon *common, int command);

// Black 2's, not checked against Black
typedef void (*ZukanDetailProcDrawFunc)(ZukanDetailProcSys *sys, int *seq, void *param, void *work,
                                        ZukanDetailCommon *common);

typedef struct {
    ZukanDetailProcFunc init;
    ZukanDetailProcFunc main;
    ZukanDetailProcFunc exit;
    // Called every frame with the touch bar's command
    ZukanDetailProcCommandFunc command;
    ZukanDetailProcDrawFunc draw;
} ZukanDetailProcFuncs;

struct ZukanDetailProcSys {
    const ZukanDetailProcFuncs *funcs;
    int seq;
    int subSeq;
    void *param;
    void *work;
};

// pokebw2's ZukanDetailProcSys_Create
ZukanDetailProcSys *func_ov208_021bc240(const ZukanDetailProcFuncs *funcs, void *param, HeapID heapId);
// pokebw2's ZukanDetailProcSys_Main
// Returns TRUE once the page has exited
BOOL func_ov208_021bc274(ZukanDetailProcSys *sys, ZukanDetailCommon *common);
// pokebw2's ZukanDetailProcSys_Command
void func_ov208_021bc2f0(ZukanDetailProcSys *sys, ZukanDetailCommon *common, int command);
// pokebw2's ZukanDetailProcSys_Draw
void func_ov208_021bc310(ZukanDetailProcSys *sys, ZukanDetailCommon *common);
// pokebw2's ZukanDetailProcSys_AllocWork
void *func_ov208_021bc32c(ZukanDetailProcSys *sys, u32 size, HeapID heapId);
// pokebw2's ZukanDetailProcSys_FreeWork
void func_ov208_021bc34c(ZukanDetailProcSys *sys);

// Black 2's, without its MAP_PLACE and MAP_PLACE_TOUCH (the FORM_*_TOUCH ones are 28..32 in Black); twinned code confirms the values of ZUKAN_DETAIL_PAGE_INFO, ZUKAN_DETAIL_PAGE_MAP, ZUKAN_DETAIL_PAGE_VOICE
// and ZUKAN_DETAIL_PAGE_FORM in Black
// The pages
enum {
    ZUKAN_DETAIL_PAGE_NONE,
    ZUKAN_DETAIL_PAGE_INFO,
    ZUKAN_DETAIL_PAGE_MAP,
    ZUKAN_DETAIL_PAGE_VOICE,
    ZUKAN_DETAIL_PAGE_FORM,
};

typedef struct ZukanDetailTouchbar ZukanDetailTouchbar;
typedef struct ZkndTbar ZkndTbar;

// Black 2's, not checked against Black
// The icons that the bar draws itself. The others are the app's own
enum {
    ZKND_TBAR_ICON_CLOSE,
    ZKND_TBAR_ICON_RETURN,
    ZKND_TBAR_ICON_CUR_D,
    ZKND_TBAR_ICON_CUR_U,
    ZKND_TBAR_ICON_CUR_L,
    ZKND_TBAR_ICON_CUR_R,
    ZKND_TBAR_ICON_CHECK,
    ZKND_TBAR_ICON_CUSTOM,
};

// Black 2's, without its MAP_PLACE and MAP_PLACE_TOUCH (the FORM_*_TOUCH ones are 28..32 in Black); twinned code confirms the offsets of pos, activeAnim, inactiveAnim, pushedAnim and se in Black
typedef struct {
    int icon;
    ClActorPos pos;
    u16 width;
    // For a ZKND_TBAR_ICON_CUSTOM icon, its resources and animations, its key and its sound
    u16 chars;
    u16 palette;
    u16 cellAnims;
    u16 activeAnim;
    u16 inactiveAnim;
    u16 pushedAnim;
    u32 key;
    u32 se;
} ZkndTbarIcon;

// Black 2's, not checked against Black
typedef struct {
    const ZkndTbarIcon *icons;
    u32 iconCount;
    ClActUnit *unit;
    // The bar's BG, the palettes for the bar and its icons, and the OBJ VRAM mapping mode
    u32 bg;
    u32 bgPalette;
    u32 objPalette;
    u32 mapping;
    // Whether the app has loaded the bar's BG itself
    BOOL noBG;
} ZkndTbarParam;

// Black 2's, without its MAP_PLACE and MAP_PLACE_TOUCH (the FORM_*_TOUCH ones are 28..32 in Black); twinned code confirms the values of ZUKAN_DETAIL_CMD_INFO, ZUKAN_DETAIL_CMD_MAP, ZUKAN_DETAIL_CMD_VOICE,
// ZUKAN_DETAIL_CMD_FORM, ZUKAN_DETAIL_CMD_INFO_TOUCH, ZUKAN_DETAIL_CMD_MAP_TOUCH, ZUKAN_DETAIL_CMD_VOICE_TOUCH and
// ZUKAN_DETAIL_CMD_FORM_TOUCH in Black
// The commands of the bar's icons: the first once the icon's animation has played, the _TOUCH ones as it is touched
enum {
    ZUKAN_DETAIL_CMD_NONE,
    ZUKAN_DETAIL_CMD_CLOSE,
    ZUKAN_DETAIL_CMD_RETURN,
    ZUKAN_DETAIL_CMD_CUR_D,
    ZUKAN_DETAIL_CMD_CUR_U,
    ZUKAN_DETAIL_CMD_CHECK,
    ZUKAN_DETAIL_CMD_INFO,
    ZUKAN_DETAIL_CMD_MAP,
    ZUKAN_DETAIL_CMD_VOICE,
    ZUKAN_DETAIL_CMD_FORM,
    ZUKAN_DETAIL_CMD_CLOSE_TOUCH,
    ZUKAN_DETAIL_CMD_RETURN_TOUCH,
    ZUKAN_DETAIL_CMD_CUR_D_TOUCH,
    ZUKAN_DETAIL_CMD_CUR_U_TOUCH,
    ZUKAN_DETAIL_CMD_CHECK_TOUCH,
    ZUKAN_DETAIL_CMD_INFO_TOUCH,
    ZUKAN_DETAIL_CMD_MAP_TOUCH,
    ZUKAN_DETAIL_CMD_VOICE_TOUCH,
    ZUKAN_DETAIL_CMD_FORM_TOUCH,
    // The map's and the forms' bars
    ZUKAN_DETAIL_CMD_MAP_RETURN,
    ZUKAN_DETAIL_CMD_MAP_RETURN_TOUCH,
    ZUKAN_DETAIL_CMD_FORM_RETURN,
    ZUKAN_DETAIL_CMD_FORM_CUR_R,
    ZUKAN_DETAIL_CMD_FORM_CUR_L,
    ZUKAN_DETAIL_CMD_FORM_CUR_D,
    ZUKAN_DETAIL_CMD_FORM_CUR_U,
    ZUKAN_DETAIL_CMD_FORM_BUTTON,
    ZUKAN_DETAIL_CMD_FORM_RETURN_TOUCH,
    ZUKAN_DETAIL_CMD_FORM_CUR_R_TOUCH,
    ZUKAN_DETAIL_CMD_FORM_CUR_L_TOUCH,
    ZUKAN_DETAIL_CMD_FORM_CUR_D_TOUCH,
    ZUKAN_DETAIL_CMD_FORM_CUR_U_TOUCH,
    ZUKAN_DETAIL_CMD_FORM_BUTTON_TOUCH,
};

// Black 2's, not checked against Black
// The bar's icons: the pages' tabs, or the map's or the forms' buttons
enum {
    ZUKAN_DETAIL_TOUCHBAR_GENERAL,
    ZUKAN_DETAIL_TOUCHBAR_MAP,
    ZUKAN_DETAIL_TOUCHBAR_FORM,
};

// Black 2's, not checked against Black
// ZukanDetailTouchbar_GetState
enum {
    ZUKAN_DETAIL_TOUCHBAR_HIDDEN,
    ZUKAN_DETAIL_TOUCHBAR_APPEARING,
    ZUKAN_DETAIL_TOUCHBAR_SHOWN,
    ZUKAN_DETAIL_TOUCHBAR_DISAPPEARING,
};

// pokebw2's ZkndTbar_Unlock
void func_ov208_021bd360(ZkndTbar *tbar);
// pokebw2's ZkndTbar_SetPos
void func_ov208_021bd3cc(ZkndTbar *tbar, int icon, const ClActorPos *pos);
// pokebw2's ZkndTbar_Free
void func_ov208_021bd208(ZkndTbar *tbar);

typedef struct ZukanDetailHeadbar ZukanDetailHeadbar;

// The titles, messages 179 on of system message file 441
#define ZUKAN_DETAIL_HEADBAR_TITLE_COUNT 5

// pokebw2's ZukanDetailHeadbar_Create
ZukanDetailHeadbar *func_ov208_021be710(HeapID heapId, Font *font);
// pokebw2's ZukanDetailHeadbar_Free
void func_ov208_021be778(ZukanDetailHeadbar *headbar);
// pokebw2's ZukanDetailHeadbar_Update
void func_ov208_021be7a4(ZukanDetailHeadbar *headbar);
// pokebw2's ZukanDetailHeadbar_SetTitle
void func_ov208_021be82c(ZukanDetailHeadbar *headbar, int title);
// pokebw2's ZukanDetailHeadbar_Appear
// Slides the bar in or out
void func_ov208_021be844(ZukanDetailHeadbar *headbar);
// pokebw2's ZukanDetailHeadbar_Disappear
void func_ov208_021be854(ZukanDetailHeadbar *headbar);

typedef struct ZukanDetailGraphic ZukanDetailGraphic;

// Black 2's, not checked against Black
// A BG to create, as the graphics and the map page set them up
struct ZukanDetailBGSetup {
    u32 bg;
    BGSetup setup;
    u32 mode;
    u32 enabled;
};

// pokebw2's ZukanDetailGraphic_Create
ZukanDetailGraphic *func_ov208_021bce10(u32 layout, HeapID heapId, BOOL with3D);
// pokebw2's ZukanDetailGraphic_Free
void func_ov208_021bcec0(ZukanDetailGraphic *graphic);
// pokebw2's ZukanDetailGraphic_Begin3D
// Draw the 3D system's frame, if it has been created
void func_ov208_021bcf2c(ZukanDetailGraphic *graphic);
// pokebw2's ZukanDetailGraphic_Create3D
void func_ov208_021bcf58(ZukanDetailGraphic *graphic, HeapID heapId);
// pokebw2's ZukanDetailGraphic_Free3D
void func_ov208_021bcf6c(ZukanDetailGraphic *graphic);

typedef struct ZukanDetailBlend ZukanDetailBlend;
typedef struct ZukanDetailPalFade ZukanDetailPalFade;
typedef struct ZukanDetailBackground ZukanDetailBackground;

struct ZukanDetailCommon {
    GameData *gameData;
    ZukanDetailGraphic *graphic;
    Font *font;
    PrintQueue *printQueue;
    ZukanDetailTouchbar *touchbar;
    ZukanDetailHeadbar *headbar;
    // The Pokémon that the screen pages through, and where in the list it is
    const u16 *list;
    u16 count;
    u16 *index;
};

// A screen's blending, which fades layers in or out by 2 of 16 every frame
struct ZukanDetailBlend {
    u32 plane1;
    u32 plane2;
    int ev;
    int step;
    int wait;
};

// Fades the palettes of the screens to black and back
enum {
    ZUKAN_DETAIL_PALFADE_MAIN_BG = 1 << 0,
    ZUKAN_DETAIL_PALFADE_SUB_BG = 1 << 1,
    ZUKAN_DETAIL_PALFADE_MAIN_OBJ = 1 << 2,
    ZUKAN_DETAIL_PALFADE_SUB_OBJ = 1 << 3,
};

enum {
    ZUKAN_DETAIL_PALFADE_SHOWN,
    ZUKAN_DETAIL_PALFADE_HIDDEN,
    ZUKAN_DETAIL_PALFADE_FADING_IN,
    ZUKAN_DETAIL_PALFADE_FADING_OUT,
};

struct ZukanDetailPalFade {
    TCBManager *tcbMgr;
    void *tcbBuffer;
    void *palette;
    // The ZUKAN_DETAIL_PALFADE_* palettes that it fades
    u16 buffers;
    int state;
};

// A page's scrolling background
struct ZukanDetailBackground {
    u32 chars;
    u8 bg;
    u8 wait;
};

// pokebw2's ZukanDetailCommon_Create
ZukanDetailCommon *func_ov208_021bc35c(HeapID heapId, GameData *gameData, ZukanDetailGraphic *graphic, Font *font,
                                            PrintQueue *printQueue, ZukanDetailTouchbar *touchbar,
                                            ZukanDetailHeadbar *headbar, const u16 *list, u16 count, u16 *index);
// pokebw2's ZukanDetailCommon_GetSpecies
u16 func_ov208_021bc3c0(ZukanDetailCommon *common);
// pokebw2's ZukanDetailCommon_GoNext
void func_ov208_021bc3cc(ZukanDetailCommon *common);
// pokebw2's ZukanDetailCommon_GoPrev
void func_ov208_021bc40c(ZukanDetailCommon *common);
// pokebw2's ZukanDetailBlend_Create
ZukanDetailBlend *func_ov208_021bc448(HeapID heapId);
// pokebw2's ZukanDetailBlend_Update
void func_ov208_021bc46c(ZukanDetailBlend *main, ZukanDetailBlend *sub);
// pokebw2's ZukanDetailBlend_IsActive
BOOL func_ov208_021bc4c8(ZukanDetailBlend *blend);
// pokebw2's ZukanDetailBlend_StartIn
void func_ov208_021bc4d8(ZukanDetailBlend *blend);
// pokebw2's ZukanDetailBlend_StartOut
void func_ov208_021bc4e4(ZukanDetailBlend *blend);
// pokebw2's ZukanDetailBlend_SetIn
// screen is 0 for the main screen and 1 for the sub screen
void func_ov208_021bc4f4(u32 screen, ZukanDetailBlend *blend);
// pokebw2's ZukanDetailBlend_SetOut
void func_ov208_021bc508(u32 screen, ZukanDetailBlend *blend);
// pokebw2's ZukanDetailBlend_InitPlanes
void func_ov208_021bc518(ZukanDetailBlend *blend);
// pokebw2's ZukanDetailPalFade_CreateEx
ZukanDetailPalFade *func_ov208_021bc560(HeapID heapId, u16 buffers);
// pokebw2's ZukanDetailPalFade_Create
ZukanDetailPalFade *func_ov208_021bc610(HeapID heapId);
// pokebw2's ZukanDetailPalFade_Free
void func_ov208_021bc61c(ZukanDetailPalFade *fade);
// pokebw2's ZukanDetailPalFade_Update
void func_ov208_021bc678(ZukanDetailPalFade *fade);
// pokebw2's ZukanDetailPalFade_IsFading
BOOL func_ov208_021bc6c8(ZukanDetailPalFade *fade);
// pokebw2's ZukanDetailPalFade_StartIn
void func_ov208_021bc6d8(ZukanDetailPalFade *fade);
// pokebw2's ZukanDetailPalFade_StartOut
void func_ov208_021bc6f8(ZukanDetailPalFade *fade);
// pokebw2's ZukanDetailPalFade_SetHidden
void func_ov208_021bc718(ZukanDetailPalFade *fade);
// pokebw2's ZukanDetailPalFade_LoadPalette
void func_ov208_021bc738(ZukanDetailPalFade *fade, ArcTool *arc, u32 fileId, HeapID heapId, u32 buffer,
                                    u32 size, u16 offset, u16 srcOffset);
// pokebw2's ZukanDetailPalFade_ReadPalettes
void func_ov208_021bc758(ZukanDetailPalFade *fade);
// pokebw2's ZukanDetailBackground_Free
void func_ov208_021bc97c(ZukanDetailBackground *background);
// pokebw2's ZukanDetailBackground_Update
void func_ov208_021bc99c(ZukanDetailBackground *background);
// pokebw2's ZukanDetail_LoadBG
u32 func_ov208_021bc9bc(BOOL loaded, HeapID heapId, u8 bg, u32 palettes, u8 palette, u8 srcPalette, u32 arcId, u32 nclr,
                       u32 ncgr, u32 nscr, u32 chars);
// pokebw2's ZukanDetail_FreeBG
void func_ov208_021bca54(u32 bg, u32 chars);

// Black 2's, not checked against Black
// What touching an icon does: plays its pushed animation, or flips it on or off
enum {
    ZKND_TBAR_TYPE_PUSH,
    ZKND_TBAR_TYPE_FLIP,
};

// pokebw2's ZkndTbar_Push
// Acts as if the icon were touched
void func_ov208_021bd3f8(ZkndTbar *tbar, int icon);

// Black 2's, not checked against Black
// A window to create, as BmpWin_CreateDynamic takes it, in the pages' tables
struct ZukanDetailWindowData {
    u8 bg;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
    u8 palette;
    u8 fromEnd;
};

// Only the allocation's size is known for Black: the heap ID alone, like the Voice and Form pages
struct ZukanDetailInfoParam {
    HeapID heapId;
};

struct ZukanDetailFormParam {
    HeapID heapId;
};

// Black 2's, without its MAP_PLACE and MAP_PLACE_TOUCH (the FORM_*_TOUCH ones are 28..32 in Black); twinned code confirms the values of ZUKAN_DETAIL_FORM_RES_MAIN_CHARS, ZUKAN_DETAIL_FORM_RES_MAIN_PALETTE,
// ZUKAN_DETAIL_FORM_RES_MAIN_CELL_ANIMS, ZUKAN_DETAIL_FORM_RES_COLOR_CHARS, ZUKAN_DETAIL_FORM_RES_COLOR_PALETTE,
// ZUKAN_DETAIL_FORM_RES_COLOR_CELL_ANIMS, ZUKAN_DETAIL_FORM_RES_ARROW_CHARS, ZUKAN_DETAIL_FORM_RES_ARROW_PALETTE and
// ZUKAN_DETAIL_FORM_RES_ARROW_CELL_ANIMS in Black
// The resources of the forms page's actors and buttons
enum {
    ZUKAN_DETAIL_FORM_RES_MAIN_CHARS,
    ZUKAN_DETAIL_FORM_RES_MAIN_PALETTE,
    ZUKAN_DETAIL_FORM_RES_MAIN_CELL_ANIMS,
    ZUKAN_DETAIL_FORM_RES_COLOR_CHARS,
    ZUKAN_DETAIL_FORM_RES_COLOR_PALETTE,
    ZUKAN_DETAIL_FORM_RES_COLOR_CELL_ANIMS,
    ZUKAN_DETAIL_FORM_RES_ARROW_CHARS,
    ZUKAN_DETAIL_FORM_RES_ARROW_PALETTE,
    ZUKAN_DETAIL_FORM_RES_ARROW_CELL_ANIMS,
    ZUKAN_DETAIL_FORM_RES_COUNT,
};

struct ZukanDetailFormPos {
    f32 x;
    f32 y;
    f32 z;
};

// Black 2's, not checked against Black
struct ZukanDetailFormActorData {
    u8 x;
    u8 y;
    u8 sequence;
    u8 priority;
    u8 bgPriority;
    // ZUKAN_DETAIL_FORM_RES_*
    u8 chars;
    u8 palette;
    u8 cellAnims;
};

// The forms page's tables, in zukan_detail_form_data.c: the messages, the actors and the sprites' places
extern const u16 data_ov208_021c8794[6];
extern const ZukanDetailFormActorData data_ov208_021c87a0[4];
extern const ZukanDetailFormPos data_ov208_021c87c0[5];

struct ZukanDetailVoiceParam {
    HeapID heapId;
};

// Black 2's, not checked against Black
// What the screen returns to the list
enum {
    // Close the Pokédex, or return to its list (Black's 0 and 1, from the assembly of its detail screen)
    ZUKAN_DETAIL_RESULT_CLOSE,
    ZUKAN_DETAIL_RESULT_RETURN,
    // The player picked a place on the habitat map
    ZUKAN_DETAIL_RESULT_PLACE,
};

// Black 2's, not checked against Black
struct ZukanDetailParam {
    GameData *gameData;
    // The ZUKAN_DETAIL_PAGE_* to start on
    int page;
    // The Pokémon that the screen pages through, and where in the list it is
    const u16 *list;
    u16 count;
    u16 index;
    // A ZUKAN_DETAIL_RESULT_*
    int result;
    u32 mode;
    u16 unk18;
    // With ZUKAN_DETAIL_RESULT_PLACE
    u16 place;
};

// Black 2's, not checked against Black
// Black's allocation is 2 bytes: the heap ID alone (pokebw2 had a place here, which Black's page doesn't keep)
struct ZukanDetailMapParam {
    HeapID heapId;
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ZukanDetailProcSys_Free
void func_ov208_021bc26c(ZukanDetailProcSys *sys);
// pokebw2's ZkndTbar_SetActiveAll
void func_ov208_021bd358(ZkndTbar *tbar, BOOL active);
// pokebw2's ZkndTbar_Main
void func_ov208_021bd23c(ZkndTbar *tbar);
// pokebw2's ZkndTbar_GetTrigger
// The icon that was touched, once its animation has played, and as it is touched, or -1
int func_ov208_021bd2e0(ZkndTbar *tbar);
// pokebw2's ZkndTbar_GetTouch
int func_ov208_021bd308(ZkndTbar *tbar);
// pokebw2's ZkndTbar_SetVisibleAll
void func_ov208_021bd330(ZkndTbar *tbar, BOOL visible);
// pokebw2's ZkndTbar_SetVisible
void func_ov208_021bd378(ZkndTbar *tbar, int icon, BOOL visible);
// pokebw2's ZkndTbar_SetFlip
void func_ov208_021bd3a4(ZkndTbar *tbar, int icon, BOOL flip);
// pokebw2's ZkndTbar_GetFlip
BOOL func_ov208_021bd3b4(ZkndTbar *tbar, int icon);
// pokebw2's ZkndTbar_GetActiveAll
BOOL func_ov208_021bd35c(ZkndTbar *tbar);
// pokebw2's ZkndTbar_GetActor
ClActor *func_ov208_021bd3c0(ZkndTbar *tbar, int icon);
// pokebw2's ZkndTbar_Free
// pokebw2's ZukanDetailTouchbar_Create
ZukanDetailTouchbar *func_ov208_021bd8c0(HeapID heapId, BOOL showFormTab);
// pokebw2's ZukanDetailTouchbar_Free
void func_ov208_021bd954(ZukanDetailTouchbar *touchbar);
// pokebw2's ZukanDetailTouchbar_Update
void func_ov208_021bd974(ZukanDetailTouchbar *touchbar);
// pokebw2's ZukanDetailTouchbar_SetType
// page is a ZUKAN_DETAIL_PAGE_* less 1
void func_ov208_021bdaac(ZukanDetailTouchbar *touchbar, int type, int page, BOOL showArrows);
// pokebw2's ZukanDetailTouchbar_GetState
int func_ov208_021bdad0(ZukanDetailTouchbar *touchbar);
// pokebw2's ZukanDetailTouchbar_Appear
// Slides the bar in or out
void func_ov208_021bdad4(ZukanDetailTouchbar *touchbar, u32 speed);
// pokebw2's ZukanDetailTouchbar_Disappear
void func_ov208_021bdae8(ZukanDetailTouchbar *touchbar, u32 speed);
// pokebw2's ZukanDetailTouchbar_GetTrigger
int func_ov208_021bdb08(ZukanDetailTouchbar *touchbar);
// pokebw2's ZukanDetailTouchbar_GetTouch
int func_ov208_021bdb54(ZukanDetailTouchbar *touchbar);
// pokebw2's ZukanDetailTouchbar_Unlock
void func_ov208_021bdba0(ZukanDetailTouchbar *touchbar);
// pokebw2's ZukanDetailTouchbar_SetVisibleAll
void func_ov208_021bdbac(ZukanDetailTouchbar *touchbar, BOOL visible);
// pokebw2's ZukanDetailTouchbar_SetPage
void func_ov208_021bdbb8(ZukanDetailTouchbar *touchbar, int page);
// pokebw2's ZukanDetailTouchbar_SetFormArrowsVisible
void func_ov208_021bdbec(ZukanDetailTouchbar *touchbar, BOOL visible);
// pokebw2's ZukanDetailTouchbar_SetCheck
void func_ov208_021bdc14(ZukanDetailTouchbar *touchbar, BOOL check);
// pokebw2's ZukanDetailTouchbar_GetCheck
BOOL func_ov208_021bdc2c(ZukanDetailTouchbar *touchbar);
// pokebw2's ZukanDetailTouchbar_SetActive
void func_ov208_021bdc48(ZukanDetailTouchbar *touchbar, BOOL active);
// pokebw2's ZukanDetailTouchbar_GetIconPalette
u32 func_ov208_021bdc80(ZukanDetailTouchbar *touchbar);
// pokebw2's ZukanDetailTouchbar_SetBGPriority
void func_ov208_021bdca4(ZukanDetailTouchbar *touchbar, u8 priority);
// pokebw2's ZukanDetailHeadbar_GetState
// The same states as the touch bar's
int func_ov208_021be840(ZukanDetailHeadbar *headbar);
// pokebw2's ZukanDetailGraphic_Update
void func_ov208_021bcf18(ZukanDetailGraphic *graphic);
// pokebw2's ZukanDetailGraphic_End3D
void func_ov208_021bcf3c(ZukanDetailGraphic *graphic);
// pokebw2's ZukanDetailGraphic_GetClActUnit
ClActUnit *func_ov208_021bcf4c(ZukanDetailGraphic *graphic);
// pokebw2's ZukanDetailCommon_Free
void func_ov208_021bc39c(ZukanDetailCommon *common);
// pokebw2's ZukanDetailCommon_GetGameData
GameData *func_ov208_021bc3a4(ZukanDetailCommon *common);
// pokebw2's ZukanDetailCommon_GetGraphic
ZukanDetailGraphic *func_ov208_021bc3a8(ZukanDetailCommon *common);
// pokebw2's ZukanDetailCommon_GetFont
Font *func_ov208_021bc3ac(ZukanDetailCommon *common);
// pokebw2's ZukanDetailCommon_GetPrintQueue
PrintQueue *func_ov208_021bc3b0(ZukanDetailCommon *common);
// pokebw2's ZukanDetailCommon_GetTouchbar
ZukanDetailTouchbar *func_ov208_021bc3b4(ZukanDetailCommon *common);
// pokebw2's ZukanDetailCommon_GetHeadbar
ZukanDetailHeadbar *func_ov208_021bc3b8(ZukanDetailCommon *common);
// pokebw2's ZukanDetailCommon_GetCount
u16 func_ov208_021bc3bc(ZukanDetailCommon *common);
// pokebw2's ZukanDetailBlend_Free
void func_ov208_021bc464(ZukanDetailBlend *blend);
// pokebw2's ZukanDetailPalFade_VBlank
void func_ov208_021bc6bc(ZukanDetailPalFade *fade);
// pokebw2's ZukanDetailBackground_Create
ZukanDetailBackground *func_ov208_021bc844(HeapID heapId, u32 page, u8 bg, u8 bottomPalette, u8 topPalette);
// pokebw2's ZkndTbar_Create
ZkndTbar *func_ov208_021bd148(ZkndTbarParam *param, HeapID heapId);
// pokebw2's ZkndTbar_SetActive
void func_ov208_021bd368(ZkndTbar *tbar, int icon, BOOL active);
// pokebw2's ZkndTbar_GetVisible
BOOL func_ov208_021bd388(ZkndTbar *tbar, int icon);
// pokebw2's ZkndTbar_SetKey
void func_ov208_021bd394(ZkndTbar *tbar, int icon, u32 key);
// pokebw2's ZukanDetailForm_InitParam
void func_ov208_021c3b68(ZukanDetailFormParam *param, HeapID heapId);
// pokebw2's ZukanDetailInfo_InitParam
void func_ov208_021bea00(ZukanDetailInfoParam *param, HeapID heapId);
// pokebw2's ZUKAN_DETAIL_INFO_PROC_FUNCS
extern const ZukanDetailProcFuncs data_ov208_021c8508;
// pokebw2's ZukanDetailMap_InitParam
void func_ov208_021bf784(ZukanDetailMapParam *param, HeapID heapId);
// pokebw2's ZUKAN_DETAIL_MAP_PROC_FUNCS
extern const ZukanDetailProcFuncs data_ov208_021c8590;
// pokebw2's ZukanDetailVoice_InitParam
void func_ov208_021c255c(ZukanDetailVoiceParam *param, HeapID heapId);
// pokebw2's ZUKAN_DETAIL_VOICE_PROC_FUNCS
extern const ZukanDetailProcFuncs data_ov208_021c86f0;
// pokebw2's ZUKAN_DETAIL_FORM_PROC_FUNCS
extern const ZukanDetailProcFuncs data_ov208_021c881c;

#endif // BWPORT_APP_ZUKAN_DETAIL_H
