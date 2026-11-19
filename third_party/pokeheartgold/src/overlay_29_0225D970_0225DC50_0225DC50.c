#include "global.h"

#include "constants/sndseq.h"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "msgdata.h"
#include "overlay_29.h"
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
#include "overlay_29_number_entry_data.h"

struct BgConfig;

struct BgTemplate;

struct DpadMenuBox;

struct G2dRenderer;

struct GF_2DGfxResMan;

struct GridCallbacks;

struct GridInputHandler;

struct NumberEntryRodata;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SysTask;

struct SysTaskQueue;

struct Window;

struct WindowTemplate;

typedef struct NumberEntryRodata {
    u8 xOffsets1[1];
    TouchscreenHitbox hitboxes1[2];
    TouchscreenHitbox hitboxes2[3];
    TouchscreenHitbox hitboxes3[4];
    GridCallbacks gridCallbacks;
    TouchscreenHitbox hitboxes4[5];
    TouchscreenHitbox hitboxes5[6];
    WindowTemplate windowTemplates[3];
    TouchscreenHitbox hitboxes6[7];
    const TouchscreenHitbox *hitboxes[7];
    BgTemplate bgTemplate4;
    const u8 *xOffsets[7];
    BgTemplate bgTemplate5;
    u8 spriteSpecs[11][4];
    TouchscreenHitbox padHitboxes[14];
    DpadMenuBox padDpadBoxes[13];
    u8 xOffsets2[4];
    u8 xOffsets3[4];
    u8 xOffsets4[4];
    u8 xOffsets5[8];
    u8 xOffsets6[8];
} NumberEntryRodata;

extern const NumberEntryRodata sRodata;

void ov29_0225DC50(NumberEntryData *data);

extern const u8 *const ov29_0225E1E4[7];

void ov29_0225DC50(NumberEntryData *data) {
    VecFx32 vec;

    vec.x = ov29_0225E1E4[data->numDigits][data->cursor] << 12;
    vec.y = ((fx32)(((292) > 0) ? ((292) * (1 << 12) + 0.5f) : ((292) * (1 << 12) - 0.5f)));
    Sprite_SetMatrix(data->sprites[1], &vec);
}

void ov29_0225DC84(NumberEntryData *data);

void ov29_0225DC84(NumberEntryData *data) {
    u32 i;
    const u8 *xOffsets;
    VecFx32 vec;

    xOffsets = ov29_0225E1E4[data->numDigits];
    vec.y = FX32_CONST(280);
    for (i = 0; i < data->numDigits; i++) {
        vec.x = xOffsets[i] << FX32_SHIFT;
        Sprite_SetMatrix(data->sprites[5 + i], &vec);
    }
}
