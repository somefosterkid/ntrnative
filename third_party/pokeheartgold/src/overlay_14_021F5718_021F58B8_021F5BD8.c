#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "msgdata.h"
#include "pm_string.h"
#include "pokemon_storage_system.h"
#include "text.h"
#include "unk_0201956C.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

struct Background;

struct BgConfig;

struct BoxPokemon;

struct MsgData;

struct NARC;

struct PC_Box;

struct PokemonStorageSystem;

struct String;

struct UnkStruct_0201956C;

struct UnkStruct_ov14_args;

struct UnkStruct_ov14_boxapp;

struct UnkStruct_ov14_boxsys;

struct Window;

struct _MAT;

struct _MAT_ENTRY;

typedef struct UnkStruct_ov14_args {
    u8 unk0[8];
    int unk8;
} UnkStruct_ov14_args;

void ov14_021F4F24(Window *window, String *str, int x, int y, int fontId, u32 color, int align);

void ov14_021F5BD8(UnkStruct_ov14_boxapp *work, int winIdx, int msgId, int flag, u8 align) {
    Window *windows = work->sys->windows;
    u8 width;
    u32 color;
    String *str;

    GetWindowBgId(&windows[winIdx]);
    GetWindowX(&windows[winIdx]);
    GetWindowY(&windows[winIdx]);
    width = GetWindowWidth(&windows[winIdx]);
    GetWindowHeight(&windows[winIdx]);
    if (flag == 1) {
        FillWindowPixelBuffer(&windows[winIdx], 11);
        color = MAKE_TEXT_COLOR(14, 15, 0);
    } else {
        FillWindowPixelBuffer(&windows[winIdx], 4);
        color = MAKE_TEXT_COLOR(14, 15, 0);
    }
    str = NewString_ReadMsgData(work->sys->unk_20, msgId);
    if (align == 2 || align == 3) {
        ov14_021F4F24(&windows[winIdx], str, width * 8 / 2, 0, 4, color, align);
    } else {
        ov14_021F4F24(&windows[winIdx], str, 0, 0, 4, color, align);
    }
    String_Delete(str);
    CopyWindowPixelsToVram_TextMode(&windows[winIdx]);
}
