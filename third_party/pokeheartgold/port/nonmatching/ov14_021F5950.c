/* PORTOK hk39 */
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
typedef struct UnkStruct_ov14_boxsys {
    u8 unk0[0x14];
    BgConfig *bgConfig;
    u8 unk18[8];
    MsgData *msgData;
    u8 unk24[0xC];
    Window windows[0x2C];
    UnkStruct_0201956C *unk2F0;
    u8 unk2F4[0x44D - 0x2F4];
    u8 unk44D;
} UnkStruct_ov14_boxsys;
typedef struct UnkStruct_ov14_boxapp {
    UnkStruct_ov14_args *args;
    PCStorage *storage;
    u8 unk8[0x2C];
    UnkStruct_ov14_boxsys *sys;
} UnkStruct_ov14_boxapp;
void ov14_021F4F24(Window *window, String *str, int x, int y, int fontId, u32 color, int align);

void ov14_021F5950(UnkStruct_ov14_boxapp *work, int winIdx, int msgId, int flag);
void ov14_021F5950(UnkStruct_ov14_boxapp *work, int winIdx, int msgId, int flag) {
    Window *windows = work->sys->windows;
    Window *window = &windows[winIdx];
    int width;
    NNSG2dCharacterData *charData[3];
    int lastRow;
    int height;
    u8 *pixels;
    int off;
    u32 color;
    String *str;
    u8 i;
    void *data;
    int rowY;
    int lastCol;
    int colX;

    GetWindowBgId(window);
    GetWindowX(window);
    GetWindowY(window);
    width = GetWindowWidth(window);
    height = GetWindowHeight(window);
    if (flag == 1) {
        FillWindowPixelBuffer(window, 11);
        color = MAKE_TEXT_COLOR(14, 15, 0);
        off = 0x180;
    } else {
        FillWindowPixelBuffer(window, 4);
        color = MAKE_TEXT_COLOR(14, 15, 0);
        off = 0;
    }
    data = GfGfxLoader_GetCharData(NARC_a_0_1_9, 0x40, TRUE, charData, HEAP_ID_10);
    pixels = charData[0]->pRawData;
    BlitBitmapRect(window, pixels + off, 0, 0, 8, 8, 0, 0, 8, 8, 0xFF);
    lastCol = width - 1;
    colX = lastCol * 8;
    BlitBitmapRect(window, pixels + (off + 0x40), 0, 0, 8, 8, (width - 1) * 8, 0, 8, 8, 0xFF);
    lastRow = height - 1;
    rowY = lastRow * 8;
    BlitBitmapRect(window, pixels + (off + 0x120), 0, 0, 8, 8, 0, rowY, 8, 8, 0xFF);
    BlitBitmapRect(window, pixels + (off + 0x160), 0, 0, 8, 8, (width - 1) * 8, (height - 1) * 8, 8, 8, 0xFF);
    for (i = 1; i < lastRow; i++) {
        BlitBitmapRect(window, pixels + (off + 0x60), 0, 0, 8, 8, 0, i * 8, 8, 8, 0xFF);
        BlitBitmapRect(window, pixels + (off + 0xA0), 0, 0, 8, 8, colX, i * 8, 8, 8, 0xFF);
    }
    for (i = 1; i < lastCol; i++) {
        BlitBitmapRect(window, pixels + (off + 0x20), 0, 0, 8, 8, i * 8, 0, 8, 8, 0xFF);
        BlitBitmapRect(window, pixels + (off + 0x140), 0, 0, 8, 8, i * 8, rowY, 8, 8, 0xFF);
    }
    Heap_Free(data);
    str = NewString_ReadMsgData(work->sys->msgData, msgId);
    ov14_021F4F24(window, str, width * 8 / 2, 4, 4, color, 2);
    String_Delete(str);
    CopyWindowPixelsToVram_TextMode(window);
}
