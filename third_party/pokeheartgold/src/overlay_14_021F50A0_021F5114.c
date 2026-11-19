#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "item.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "pm_string.h"
#include "pokedex_util.h"
#include "pokemon_storage_system.h"
#include "save.h"
#include "sprite_system.h"
#include "string_util.h"
#include "unk_02013534.h"
#include "unk_0201956C.h"
#include "overlay_14_unk_struct_ov14_mon.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"


struct BgConfig;

struct BoxPokemon;

struct ManagedSprite;

struct MessageFormat;

struct MessagePrinter;

struct MsgData;

struct SpriteManager;

struct String;

struct TextOBJ;

struct UnkStruct_02013534;

struct UnkStruct_0201956C;

struct UnkStruct_02021AC8;

struct UnkStruct_ov14_boxsys;

struct UnkStruct_ov14_mon;

struct UnkStruct_ov14_textobj;

void ov14_021F5114(UnkStruct_ov14_boxsys *sys, UnkStruct_ov14_mon *mon, int winIdx) {
    FillWindowPixelBuffer(&sys->windows[winIdx], 0);
    if (!mon->isEgg) {
        if (mon->hasGender == 1) {
            if (mon->gender == 0) {
                ov14_021F4F84(sys, sys->unk_20, winIdx, 0x52, 0, 0, 0, ((((7) & 0xFF) << 16) | (((8) & 0xFF) << 8) | (((0) & 0xFF) << 0)), 0);
            } else if (mon->gender == 1) {
                ov14_021F4F84(sys, sys->unk_20, winIdx, 0x53, 0, 0, 0, ((((3) & 0xFF) << 16) | (((4) & 0xFF) << 8) | (((0) & 0xFF) << 0)), 0);
            }
        }
    }
    ScheduleWindowCopyToVram(&sys->windows[winIdx]);
}
