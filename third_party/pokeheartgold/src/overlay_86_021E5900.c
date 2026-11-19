#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_86.h"
#include "overlay_86_frontier_records_app.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "pm_string.h"
#include "system.h"
#include "unk_02030A98.h"
struct FrontierRecordsApp;

void ov86_021E5CDC(void *);

s32 ov86_021E5E0C(struct FrontierRecordsApp *);

u8 ov86_021E5E98(u8);

s32 ov86_021E6E30(struct FrontierRecordsApp *);

s32 ov86_021E6E98(struct FrontierRecordsApp *);

s32 ov86_021E7DF8(BgConfig *, Window *, u8);

s32 ov86_021E7E68(BgConfig *, Window *);

BOOL ov86_021E5900(OverlayManager *man, int *state) {
    FrontierRecordsApp *app;
    UnkStruct_0203FAB4 *args;

    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= 0xFFFFE0FF;
    reg_GXS_DB_DISPCNT &= 0xFFFFE0FF;
    reg_GX_POWCNT &= 0xFFFF7FFF;
    Heap_Create(HEAP_ID_3, HEAP_ID_121, 0x30000U);
    app = OverlayManager_CreateAndGetData(man, sizeof(FrontierRecordsApp), HEAP_ID_121);
    memset(app, 0, sizeof(FrontierRecordsApp));
    app->man = man;
    app->bgConfig = BgConfig_Alloc(HEAP_ID_121);
    args = OverlayManager_GetArgs(man);
    app->saveData = args->unk0;
    app->frontierSave = Save_Frontier_GetStatic(app->saveData);
    app->challengeType = args->unk4;
    app->facility = args->unk5;
    app->species = args->unk6;
    app->options = Save_PlayerData_GetOptionsAddr(app->saveData);
    FontID_Alloc(FONT_UNK_4, HEAP_ID_121);
    ov86_021E5E0C(app);
    app->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x13, HEAP_ID_121);
    app->msgFmt = MessageFormat_New(HEAP_ID_121);
    app->string = String_New(0x320U, HEAP_ID_121);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_121);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, HEAP_ID_121);
    ov86_021E7DF8(app->bgConfig, app->windows, ov86_021E5E98(app->facility));
    ov86_021E7E68(app->bgConfig, &app->buttonWindow);
    ov86_021E6E30(app);
    ov86_021E6E98(app);
    Main_SetVBlankIntrCB(ov86_021E5CDC, app);
    *state = 0;
    return 1;
}
