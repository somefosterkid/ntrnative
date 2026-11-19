#include "global.h"

#include "constants/sndseq.h"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_32.h"
#include "player_data.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "systask_environment.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"
#include "unk_0202C034.h"
#include "dex_mon_measures.h"

struct BgConfig;

struct BgTemplate;

struct DpadMenuBox;

struct GridCallbacks;

struct NamePickerRodata;

struct PokedexData;

struct WindowTemplate;

PokedexData *PokedexData_Create(enum HeapID heapID);

void PokedexData_Delete(PokedexData *pokedexData);

void PokedexData_LoadAll(PokedexData *pokedexData, int playerGender, enum HeapID heapID);

void PokedexData_UnloadAll(PokedexData *pokedexData);

u32 PokedexData_GetHeight(PokedexData *pokedexData, int species);

u32 PokedexData_GetWeight(PokedexData *pokedexData, int species);

typedef struct NamePickerRodata {
    TouchscreenHitbox pageHitboxes[3];
    GridCallbacks gridCallbacks;
    u8 spriteSpecs[4][4];
    BgTemplate bgTemplate6;
    BgTemplate bgTemplate4;
    BgTemplate bgTemplate5;
    TouchscreenHitbox gridHitboxes[10];
    DpadMenuBox gridDpadBoxes[9];
    WindowTemplate windowTemplates[18];
} NamePickerRodata;

extern const NamePickerRodata _0225E15C;

void ov32_0225D634(BgConfig *bgConfig) {
    GXS_SetGraphicsMode(GX_BGMODE_0);
    {
        BgTemplate template = _0225E15C.bgTemplate4;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &template, GF_BG_TYPE_TEXT);
        BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, HEAP_ID_8);
        BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    }
    {
        BgTemplate template = _0225E15C.bgTemplate5;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &template, GF_BG_TYPE_TEXT);
    }
    {
        BgTemplate template = _0225E15C.bgTemplate6;
        InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &template, GF_BG_TYPE_TEXT);
    }
}
