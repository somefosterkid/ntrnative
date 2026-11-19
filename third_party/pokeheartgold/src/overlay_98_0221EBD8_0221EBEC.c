#include "global.h"

#include "constants/gx.h"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
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

struct BgConfig;

struct MessageFormat;

struct MsgData;

struct Ov98TextSys;

struct String;

struct Window;

struct WindowTemplate;

void ov98_0221EBEC(Ov98TextSys *textSys, u32 windowIdx, u32 msgId, BOOL center, u8 y, u32 fontId) {
    ov98_0221EC3C(textSys, windowIdx, msgId, center, 0, y, fontId, 0);
}

void ov98_0221EC08(Ov98TextSys *textSys, u32 windowIdx, u32 msgId, u8 x, u8 y) {
    ov98_0221EC3C(textSys, windowIdx, msgId, FALSE, x, y, 0, 0);
}
