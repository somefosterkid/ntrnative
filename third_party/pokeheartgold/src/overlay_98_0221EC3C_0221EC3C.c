#include "global.h"

#include "constants/gx.h"
#include "constants/heap.h"

#include "bg_window.h"
#include "error_handling.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "font_types_def.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_98.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "screen_fade.h"
#include "sprite.h"
#include "sprite_system.h"
#include "string_util.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "unk_02013FDC.h"
#include "overlay_98_ov98text_sys.h"

typedef enum Font Font;

struct Background;

struct BgConfig;

struct MessageFormat;

struct MessageFormatAttrs;

struct MessageFormatFields;

struct MsgData;

struct NARC;

struct Ov98TextSys;

struct String;

struct TextPrinterTemplate;

struct Window;

struct WindowTemplate;

struct _MAT;

struct _MAT_ENTRY;

void ov98_0221EC3C(Ov98TextSys *textSys, u32 windowIdx, u32 msgId, BOOL center, u8 x, u8 y, u32 fontId, u8 fill) {
    u8 xOffset = 0;
    String *string;

    ((windowIdx < textSys->windowCount) ? (void)0 : GF_AssertFail());
    string = ReadMsgData_ExpandPlaceholders(textSys->msgFmt, textSys->msgData, msgId, textSys->heapId);
    if (center) {
        xOffset = GetWindowWidth(&textSys->windows[windowIdx]) * 8;
        xOffset -= FontID_String_GetWidthMultiline((Font)(fontId), string, (Font)(0));
        xOffset /= 2;
    }
    FillWindowPixelBuffer(&textSys->windows[windowIdx], fill);
    AddTextPrinterParameterizedWithColor(&textSys->windows[windowIdx], (Font)(fontId), string, xOffset + x, y, 0xFF, ((((1) & 0xFF) << 16) | (((2) & 0xFF) << 8) | (((0) & 0xFF) << 0)), 0L);
    ScheduleWindowCopyToVram(&textSys->windows[windowIdx]);
    String_Delete(string);
}
