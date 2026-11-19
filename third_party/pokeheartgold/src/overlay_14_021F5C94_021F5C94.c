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

void ov14_021F5C94(UnkStruct_ov14_boxapp *work, int winIdx, int msgId);

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

void ov14_021F5C94(UnkStruct_ov14_boxapp *work, int winIdx, int msgId) {
    Window *windows = work->sys->windows;
    Window *window = &windows[winIdx];
    u8 width;
    NNSG2dCharacterData *charData;
    void *data;
    u8 *pixels;
    String *str;
    u8 i;

    width = GetWindowWidth(window);
    data = GfGfxLoader_GetCharData(NARC_a_0_1_9, 0xE, TRUE, &charData, HEAP_ID_10);
    pixels = charData->pRawData;
    BlitBitmapRect(window, pixels + 0xAE0, 0, 0, 0x18, 8, 0, 0, 0x18, 8, 0xFF);
    BlitBitmapRect(window, pixels + 0xB60, 0, 0, 0x18, 8, 0, 8, 0x18, 8, 0xFF);
    BlitBitmapRect(window, pixels + 0xB20, 0, 0, 0x18, 8, 0, 0x10, 0x18, 8, 0xFF);
    BlitBitmapRect(window, pixels + 0xD60, 0, 0, 8, 8, (width - 1) * 8, 0, 8, 8, 0xFF);
    BlitBitmapRect(window, pixels + 0xD20, 0, 0, 8, 8, (width - 1) * 8, 8, 8, 8, 0xFF);
    BlitBitmapRect(window, pixels + 0xD40, 0, 0, 8, 8, (width - 1) * 8, 0x10, 8, 8, 0xFF);
    for (i = 1; i < width - 1; i++) {
        BlitBitmapRect(window, pixels + 0xB00, 0, 0, 8, 8, i * 8, 0, 8, 8, 0xFF);
        BlitBitmapRect(window, pixels + 0xB80, 0, 0, 8, 8, i * 8, 8, 8, 8, 0xFF);
        BlitBitmapRect(window, pixels + 0xB40, 0, 0, 8, 8, i * 8, 0x10, 8, 8, 0xFF);
    }
    Heap_Free(data);
    str = NewString_ReadMsgData(work->sys->unk_20, msgId);
    ov14_021F4F24(window, str, width * 8 / 2, 4, 4, MAKE_TEXT_COLOR(9, 10, 0), 2);
    String_Delete(str);
    CopyWindowPixelsToVram_TextMode(window);
}
