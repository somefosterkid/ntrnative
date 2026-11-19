#include "global.h"

#include "constants/heap.h"
#include "constants/sndseq.h"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "font_types_def.h"
#include "gf_3d_loader.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "msgdata.h"
#include "overlay_29.h"
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
#include "overlay_29_number_entry_data.h"

typedef enum Font Font;

struct Background;

struct BgConfig;

struct BgTemplate;

struct DpadMenuBox;

struct G2dRenderer;

struct GF_2DGfxRawResMan;

struct GF_2DGfxRawResObj;

struct GF_2DGfxResMan;

struct GridCallbacks;

struct GridInputHandler;

struct MsgData;

struct NARC;

struct NumberEntryRodata;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct String;

struct SysTask;

struct SysTaskQueue;

struct TextPrinterTemplate;

struct Window;

struct WindowTemplate;

struct _MAT;

struct _MAT_ENTRY;

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

void ov29_0225D714(NumberEntryData *data);

extern const WindowTemplate ov29_0225E178[3];

void ov29_0225D714(NumberEntryData *data) {
    MsgData *msgData;
    String *string;
    u16 i;
    u16 center;
    u16 x;

    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 196, HEAP_ID_8);
    FontID_Alloc((Font)(4), HEAP_ID_8);
    for (i = 0; i < 3; i++) {
        AddWindow(data->bgConfig, &data->windows[i], &ov29_0225E178[i]);
        FillWindowPixelBuffer(&data->windows[i], 0);
        string = NewString_ReadMsgData(msgData, 24 + i);
        center = GetWindowWidth(&data->windows[i]) * 8 / 2;
        x = (Font)(center - FontID_String_GetWidth((Font)(4), string, 0) / 2);
        AddTextPrinterParameterizedWithColor(&data->windows[i], (Font)(4), string, x, 0, 0xFF, ((((15) & 0xFF) << 16) | (((14) & 0xFF) << 8) | (((0) & 0xFF) << 0)), 0L);
        String_Delete(string);
        CopyWindowPixelsToVram_TextMode(&data->windows[i]);
        ScheduleWindowCopyToVram(&data->windows[i]);
    }
    FontID_Release((enum Font)(4));
    DestroyMsgData(msgData);
}
