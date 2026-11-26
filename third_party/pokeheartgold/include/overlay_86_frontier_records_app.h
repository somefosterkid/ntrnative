#ifndef POKEHEARTGOLD_OVERLAY_86_FRONTIER_RECORDS_APP_H
#define POKEHEARTGOLD_OVERLAY_86_FRONTIER_RECORDS_APP_H

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_planes.h"

struct FrontierSave;
struct GridInputHandler;
struct ManagedSprite;
struct MessageFormat;
struct MsgData;
struct Options;
struct OverlayManager;
struct PaletteData;
struct SaveData;
struct SpriteManager;
struct SpriteSystem;
struct String;
struct UnkStruct_020163E0;

enum BattleFacility {
    FACILITY_TOWER = 1,
    FACILITY_FACTORY,
    FACILITY_FACTORY_OPEN,
    FACILITY_CASTLE,
    FACILITY_HALL,
    FACILITY_ARCADE,
    MAX_FACILITIES,
};

typedef struct FrontierRecordsPaletteFlash {
    u8 isBg : 1;
    u8 index : 7;
    u8 palA : 4;
    u8 palB : 4;
    u8 state;
    u8 timer;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
} FrontierRecordsPaletteFlash;

typedef struct FrontierRecordsList {
    u16 *entries;
    s32 count;
} FrontierRecordsList;

typedef struct FrontierRecordsApp {
    struct OverlayManager *man;
    u8 state;
    u8 nextState;
    u8 challengeType;
    u8 facility;
    u16 species;
    u8 pad_0A[2];
    struct BgConfig *bgConfig;
    Window windows[32];
    struct MsgData *msgData;
    struct MessageFormat *msgFmt;
    struct String *string;
    struct PaletteData *plttData;
    struct Options *options;
    struct SaveData *saveData;
    struct FrontierSave *frontierSave;
    struct SpriteSystem *spriteSystem;
    struct SpriteManager *spriteManager;
    struct ManagedSprite *sprites[2];
    Window buttonWindow;
    FrontierRecordsPaletteFlash flash;
    struct GridInputHandler *inputHandler;
    u16 category;
    u16 cursor;
    s8 page;
    s8 lastPage;
    u16 windowSet;
    FrontierRecordsList lists[26];
    struct UnkStruct_020163E0 *unk_330;
    u16 tilemapRects[3][16];
} FrontierRecordsApp; // size = 0x394

void ov86_021E5CDC(void *data);
void ov86_021E5D10(void);
void ov86_021E5D30(BgConfig *bgConfig);
void ov86_021E5E0C(FrontierRecordsApp *arg0);
void ov86_021E5E54(BgConfig *bgConfig);
void ov86_021E5E90(FrontierRecordsApp *arg0, s32 *arg1, s32 arg2);
void ov86_021E5ECC(FrontierRecordsApp *arg0, enum GFBgLayer layer);
void ov86_021E5F58(FrontierRecordsApp *arg0, enum GFBgLayer layer);
void ov86_021E5FBC(FrontierRecordsApp *arg0, u32 idx, s32 num);
void ov86_021E5FD8(Window *window, struct String *string, u32 arg2, u32 y, enum Font fontId, u32 color, s32 arg6);
void ov86_021E6024(FrontierRecordsApp *arg0, s32 arg1, s32 strno, s32 arg3, s32 arg4, enum Font font, s32 arg6, s32 arg7);
void ov86_021E6064(FrontierRecordsApp *arg0, s32 arg1, s32 strno, s32 arg3, s32 arg4, enum Font font, s32 arg6, s32 arg7);
void ov86_021E60B8(FrontierRecordsApp *app);
void ov86_021E6114(FrontierRecordsApp *param_1);
void ov86_021E62F4(FrontierRecordsApp *param_1);
void ov86_021E64E0(FrontierRecordsApp *param_1);
void ov86_021E668C(FrontierRecordsApp *arg0, s32 strno);
void ov86_021E66D0(FrontierRecordsApp *param_1);
void ov86_021E690C(FrontierRecordsApp *param_1);
void ov86_021E6A88(FrontierRecordsApp *param_1);
void ov86_021E6BA8(FrontierRecordsApp *param_1);
void ov86_021E6E30(FrontierRecordsApp *param_1);
void ov86_021E6E98(FrontierRecordsApp *arg0);
void ov86_021E6FF4(FrontierRecordsApp *arg0);
void ov86_021E703C(FrontierRecordsApp *arg0);
void ov86_021E705C(FrontierRecordsApp *param_1, s32 param_2, s32 param_3);
void ov86_021E707C(FrontierRecordsApp *arg0, s32 arg1, s16 x, s16 y);
void ov86_021E71C0(FrontierRecordsApp *arg0);
void ov86_021E720C(FrontierRecordsApp *arg0);
void ov86_021E723C(FrontierRecordsApp *arg0);
void ov86_021E7258(FrontierRecordsApp *arg0, s32 arg1);
void ov86_021E7344(FrontierRecordsApp *arg0);
void ov86_021E73CC(FrontierRecordsApp *arg0);
void ov86_021E73E8(void);
void ov86_021E7418(FrontierRecordsApp *arg0);
void ov86_021E74D0(FrontierRecordsApp *arg0);
void ov86_021E74F0(FrontierRecordsApp *param_1);
void ov86_021E752C(FrontierRecordsApp *param_1);
void ov86_021E757C(FrontierRecordsApp *arg0);
void ov86_021E7598(FrontierRecordsApp *arg0);
void ov86_021E75EC(FrontierRecordsApp *arg0, s32 target);
void ov86_021E7630(FrontierRecordsApp *param_1);
void ov86_021E7678(void *data, int newTarget, int prevTarget);
void ov86_021E7680(void *data, int newTarget, int prevTarget);
void ov86_021E7688(FrontierRecordsApp *p, u32 idx);
void ov86_021E77BC(FrontierRecordsApp *arg0);
void ov86_021E781C(FrontierRecordsApp *arg0, s32 target);
void ov86_021E7860(FrontierRecordsApp *param_1);
void ov86_021E78A8(void *data, int newTarget, int prevTarget);
void ov86_021E78B0(void *data, int newTarget, int prevTarget);
void ov86_021E78B8(FrontierRecordsApp *app, s32 arg1);
void ov86_021E792C(FrontierRecordsApp *app, s8 x, s8 y);
void ov86_021E7B68(FrontierRecordsApp *arg0);
void ov86_021E7C70(FrontierRecordsApp *param_1);
void ov86_021E7CA0(FrontierRecordsApp *arg0);
void ov86_021E7CF8(FrontierRecordsApp *arg0);
void ov86_021E7D8C(FrontierRecordsApp *arg0);
void ov86_021E7DA8(FrontierRecordsApp *param_1);
void ov86_021E7DD4(FrontierRecordsApp *param_1);
void ov86_021E7DF8(BgConfig *bgConfigs, Window *windows, u8 isOpponentApp);
void ov86_021E7E40(Window *windows, u8 isOpponentApp);
void ov86_021E7E68(BgConfig *bgConfig, Window *window);
void ov86_021E7E90(Window *window);

#endif // POKEHEARTGOLD_OVERLAY_86_FRONTIER_RECORDS_APP_H
