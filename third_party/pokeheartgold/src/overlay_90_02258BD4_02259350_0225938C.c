#include "global.h"
#include "save.h"
#include "bg_window.h"
#include "options.h"
#include "overlay_90_unk_struct_ov114_02260284.h"
#include "overlay_90_unk_struct_ov114_0225cef0.h"
#include "overlay_90_unk_struct_ov114_0225d338.h"

struct BgConfig;

struct Options;

struct SaveData;

struct String;

typedef struct String String;

String *String_New(u32 size, u32 heapID);

enum BgLayer {
    BG_LAYER_MAIN_0 = 0,
    BG_LAYER_MAIN_1,
    BG_LAYER_MAIN_2,
    BG_LAYER_MAIN_3,
    BG_LAYER_SUB_0,
    BG_LAYER_SUB_1,
    BG_LAYER_SUB_2,
    BG_LAYER_SUB_3,

    BG_LAYER_MAX,
};

void AddWindowParameterized(BgConfig *bgConfig, Window *window, u8 bgLayer, u8 tilemapLeft, u8 tilemapTop, u8 width, u8 height, u8 palette, u16 baseTile);

void FillWindowPixelBuffer(Window *window, u8 val);


#if defined(SDK_PORT) || defined(NONMATCHING)
void LoadUserFrameGfx2(BgConfig *bgConfig, GFBgLayer bgLayer, u16 tileOffset, u8 palOffset, u8 messageBoxFrame, u32 heapID);
#else
void LoadUserFrameGfx2(BgConfig *bgConfig, u8 bgLayer, u16 tileOffset, u8 palOffset, u8 messageBoxFrame, u32 heapID);
#endif

Options *Save_PlayerData_GetOptionsAddr(SaveData *saveData);

void TextFlags_BeginAutoScroll(BOOL lockTextWithAutoScroll);

void ov90_0225938C(UnkStruct_ov114_0225D338 *param0, UnkStruct_ov114_0225CEF0 *param1, SaveData *saveData, BOOL param3, u32 heapID);

void ov90_0225938C(UnkStruct_ov114_0225D338 *param0, UnkStruct_ov114_0225CEF0 *param1, SaveData *saveData, BOOL param3, u32 heapID) {
    u32 v0 = Options_GetFrame(Save_PlayerData_GetOptionsAddr(saveData));
    u32 v1;
    int v2;
    u8 v3;

    TextFlags_BeginAutoScroll(TRUE);
    LoadUserFrameGfx2(param1->unk_00, BG_LAYER_MAIN_2, 1, 13, v0, heapID);
    LoadUserFrameGfx2(param1->unk_00, BG_LAYER_SUB_0, 1, 13, v0, heapID);

    for (v2 = 0; v2 < 2; v2++) {
        if (v2 == 0) {
            v1 = 2;
            v3 = 19;
        } else {
            v1 = 4;
            v3 = 1;
        }

        AddWindowParameterized(param1->unk_00, &param0->unk_00[v2], v1, 2, v3, 27, 4, 12, 1 + (18 + 12));
        FillWindowPixelBuffer(&param0->unk_00[v2], 0);

        param0->unk_24[v2] = String_New(128, heapID);
    }

    param0->unk_34 = param3;
}
