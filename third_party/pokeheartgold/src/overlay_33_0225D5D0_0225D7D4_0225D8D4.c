#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "font_types_def.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "msgdata.h"
#include "overlay_33.h"
#include "pm_string.h"
#include "sys_task.h"
#include "systask_environment.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"

typedef enum Font Font;

struct Background;

struct BgConfig;

struct MsgData;

struct NARC;

struct Ov33Args;

struct Ov33Config;

struct Ov33Work;

struct String;

struct SysTask;

struct SysTaskQueue;

struct TextPrinterTemplate;

struct Window;

struct WindowTemplate;

struct _MAT;

struct _MAT_ENTRY;

typedef struct Ov33Args {
    const u32 *strIds;
    u8 count;
    u8 unk5;
    u16 unk6;
} Ov33Args;

typedef struct Ov33Config {
    const WindowTemplate *windowTemplates;
    const TouchscreenHitbox *hitboxes;
    u8 unk8;
    u8 unk9;
    u16 unkA;
} Ov33Config;

typedef struct Ov33Work {
    BgConfig *bgConfig;
    void *unk4;
    u32 unk8;
    Ov33Args *unkC;
    Ov33Args *unk10;
    SysTask *sysTask;
    Window windows[2];
    u8 buffer[0x180];
    u16 state;
    u16 count;
    u32 touchedHitbox;
} Ov33Work;

extern const Ov33Config ov33_0225DA50[2];

void ov33_0225D8D4(Ov33Work *work);

void ov33_0225D8D4(Ov33Work *work) {
    const Ov33Config *config;
    MsgData *msgData;
    u16 i;
    FontID_Alloc((Font)(4), HEAP_ID_8);
    msgData = NewMsgDataFromNarc((MsgDataLoadType)0, (NarcId)0x1B, 0xBF, HEAP_ID_8);
    config = &ov33_0225DA50[work->count - 1];
    for (i = 0; i < work->count; i++) {
        u16 center;
        String *string;
        u16 x;
        LoadRectToBgTilemapRect(work->bgConfig, 5, work->buffer, config->unk8, (u8)(config->unk9 + (config->unkA + 6) * i), 0x20, 6);
        string = NewString_ReadMsgData(msgData, work->unk10->strIds[i]);
        center = (u16)(GetWindowWidth(&work->windows[i]) * 8 / 2);
        x = (Font)(center - FontID_String_GetWidth((Font)(4), string, 0) / 2);
        AddTextPrinterParameterizedWithColor(&work->windows[i], (Font)(4), string, x, 0, 0xFF, 0x00020100, 0L);
        CopyWindowPixelsToVram_TextMode(&work->windows[i]);
        ScheduleWindowCopyToVram(&work->windows[i]);
        String_Delete(string);
    }
    DestroyMsgData(msgData);
    FontID_Release((enum Font)(4));
    ScheduleBgTilemapBufferTransfer(work->bgConfig, 5);
}
