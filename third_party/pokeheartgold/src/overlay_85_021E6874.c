#include "global.h"
#include "field_system_def.h"
#include "simple3dmodel.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "gf_gfx_planes.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "sprite_system.h"
#include "gf_3d_vramman.h"
#include "unk_struct_0209_bdf8.h"
#include "overlay_85_ov109_021D5140.h"
#include "simple3danimation.h"
#include "overlay_85_ov109_021D0F70.h"
#include "player_data.h"
#include "game_stats.h"
#include "msgdata.h"
#include "unk_struct_0209_c194.h"

struct GameStats;

struct JournalEntry;

struct MsgData;

struct MessageFormat;

struct PlayerProfile;

struct UnkStruct_0205C22C_t;

struct UnkStruct_ov109_021D0F70_t;

struct UnkStruct_ov109_021D5140_t;

typedef struct UnkStruct_0205C22C_t UnkStruct_0205C22C;

typedef struct JournalEntry JournalEntry;

void ov85_021E6874(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E692C(UnkStruct_ov109_021D0F70 *param0);
void ov85_021E6980(UnkStruct_ov109_021D0F70 *param0);

void ov85_021E6874(UnkStruct_ov109_021D0F70 *param0) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, 1);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, 1);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, 1);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, 1);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, 1);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, 0);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, 0);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, 1);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, 1);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, 1);

    G2_SetBG0Priority(1);
    G2_SetBG1Priority(0);
    G2_SetBG2Priority(2);
    G2_SetBG3Priority(3);
    G2S_SetBG0Priority(0);
    G2S_SetBG1Priority(1);
    G2S_SetBG2Priority(2);
    G2S_SetBG3Priority(3);

    {
        G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_BD, 11, 10);
    }
}

void ov85_021E692C(UnkStruct_ov109_021D0F70 *param0) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, 0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, 0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, 0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, 0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, 0);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, 0);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, 0);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, 0);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, 0);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, 0);
}

void ov85_021E6980(UnkStruct_ov109_021D0F70 *param0) {
    int v0;

    NNS_G3dInit();
    G3X_InitMtxStack();
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(1);
    G3X_AlphaTest(0, 0);
    G3X_AlphaBlend(1);
    G3X_EdgeMarking(1);

    for (v0 = 0; v0 < 8; v0++) {
        param0->unk_DA0[v0] = ((GXRgb)(((4) << (0)) | ((4) << (5)) | ((4) << (10))));
    }

    G3X_SetEdgeColorTable(param0->unk_DA0);
    G3X_SetClearColor(((GXRgb)(((31) << (0)) | ((31) << (5)) | ((31) << (10)))), 0, 0x7fff, 63, 0);
    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
    G3_ViewPort(0, 0, 255, 191);

    GF_3DVramMan_InitFrameTexVramManager(1, 1);
    GF_3DVramMan_InitFramePlttVramManager(0x4000, 1);
}
