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
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_mon.h"
#include "overlay_14_box_funcs.h"


struct BgConfig;

struct BoxPokemon;

struct ManagedSprite;

struct MessageFormat;

struct MessagePrinter;

struct MsgData;

struct NARC;

struct PC_Box;

struct PokemonStorageSystem;

struct SpriteManager;

struct String;

struct TextOBJ;

struct UnkStruct_02013534;

struct UnkStruct_0201956C;

struct UnkStruct_02021AC8;

struct UnkStruct_ov14_args;

struct UnkStruct_ov14_boxapp;

struct UnkStruct_ov14_boxsys;

struct UnkStruct_ov14_mon;

struct UnkStruct_ov14_textobj;

struct Window;

struct _MAT;

typedef struct UnkStruct_ov14_args UnkStruct_ov14_args;

int ov14_021F5404(UnkStruct_ov14_boxapp *work, UnkStruct_ov14_mon *mon) {
    NNSG2dCharacterData *charData;
    void *data;
    u8 *pixels;
    MsgData *msgData;
    u16 i;
    u16 j;
    int winBase;
    UnkStruct_ov14_boxsys *sys = work->sys;

    if (sys->unk_44E_0 == 0) {
        winBase = 9;
    } else {
        winBase = 13;
    }
    sys->unk_44E_0 ^= 1;
    data = GfGfxLoader_GetCharData(NARC_a_0_1_9, 6, 1, &charData, HEAP_ID_10);
    pixels = charData->pRawData;
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 750, HEAP_ID_10);
    if (!mon->isEgg) {
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 0xB; j++) {
                BlitBitmapRect(&work->sys->windows[winBase + i], pixels + 0x160, 0, 0, 8, 8, j * 8, 0, 8, 8, 0xFF);
                BlitBitmapRect(&work->sys->windows[winBase + i], pixels + 0x20, 0, 0, 8, 8, j * 8, 8, 8, 8, 0xFF);
            }
            ov14_021F4F84(work->sys, msgData, winBase + i, mon->msgIds[i], 0, 0, 0, ((((1) & 0xFF) << 16) | (((2) & 0xFF) << 8) | (((0) & 0xFF) << 0)), 0);
            CopyWindowPixelsToVram_TextMode(&work->sys->windows[winBase + i]);
        }
        work->sys->unk_44E_4 = 1;
    } else {
        work->sys->unk_44E_4 = 0;
    }
    DestroyMsgData(msgData);
    Heap_Free(data);
    return winBase;
}
