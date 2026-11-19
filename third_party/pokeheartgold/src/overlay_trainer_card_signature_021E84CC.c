#include "global.h"
#include "sprite_resources_header.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "options.h"
#include "pm_string.h"
#include "filesystem.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "game_stats.h"
#include "msgdata.h"
#include "yes_no_prompt.h"
#include "signature_app.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

struct BgConfig;
struct GameStats;
struct MsgData;
struct Options;
struct SpriteList;
struct SpriteResource;
struct SpriteResourceCollection;
struct String;
struct MessageFormat;
struct SysTask;
struct SysTaskQueue;
struct YesNoPrompt;
void Heap_Free(void *ptr);
void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);
enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};
enum SpriteResourceType {
    SPRITE_RESOURCE_CHAR = 0,
    SPRITE_RESOURCE_PLTT,
    SPRITE_RESOURCE_CELL,
    SPRITE_RESOURCE_ANIM,
    SPRITE_RESOURCE_MULTI_CELL,
    SPRITE_RESOURCE_MULTI_ANIM,
    SPRITE_RESOURCE_MAX,
};
YesNoPrompt *YesNoPrompt_Create(enum HeapID heapID);
void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);
SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);
static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *);
static inline void inline_ov61_0222C3B0_sub(SysTask *, void *);
static inline void inline_ov61_0222C3B0(UnkStruct_ov61_0222C3B0 *param0, NARC *param1, int param2, enum HeapID heapID) {
    NNSG2dPaletteData *v0;
    void *v1;

    MI_CpuClear8(param0, sizeof(UnkStruct_ov61_0222C3B0));

    v1 = GfGfxLoader_GetPlttDataFromOpenNarc(param1, param2, &v0, heapID);

    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_08, 4 * 0x20);
    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_88, 4 * 0x20);
    Heap_Free(v1);
    inline_ov61_0222C3B0_sub_1(param0);

    param0->unk_04 = 1;
    param0->unk_00 = SysTask_ExecuteOnVBlank(inline_ov61_0222C3B0_sub, param0, 20);
}
static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0) {
    int v0, v1, v2, v3, v4, v5;
    v1 = 0;

    for (v0 = 0; v0 < 4 - 1; v0++) {
        v2 = v0 + 1;
        v3 = 0;
        v5 = 0;
        do {
            ((v1 < (((16 << 8) / 0x300 + 2) * (4 - 1))) ? (void)0 : GF_AssertFail());
            for (v4 = 1; v4 < 1 + 15; v4++) {
                BlendPalette(&param0->unk_08[v0][v4], &param0->unk_88[v1][v4], 1, v3 >> 8, param0->unk_08[v2][v4]);
            }
            v1++;
            if (v5 == 1) {
                break;
            }
            v3 += 0x300;
            if (v3 >= (16 << 8)) {
                v3 = (16 << 8);
                v5 = 1;
            }
        } while (1);
    }
    DC_FlushRange(param0->unk_88, (((16 << 8) / 0x300 + 2) * (4 - 1)) * 16 * sizeof(u16));
}
static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1) {
    UnkStruct_ov61_0222C3B0 *v0 = param1;

    if (v0->unk_04 == 0) {
        return;
    }

    v0->unk_32B ^= 1;

    if (v0->unk_32B & 1) {
        return;
    }

    GX_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);
    GXS_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);

    if (v0->unk_32A == 0) {
        v0->unk_328++;
        if (v0->unk_328 >= (((16 << 8) / 0x300 + 2) * (4 - 1))) {
            v0->unk_328 = (((16 << 8) / 0x300 + 2) * (4 - 1)) - 2;
            v0->unk_32A ^= 1;
        }
    } else {
        v0->unk_328--;
        if (v0->unk_328 < 0) {
            v0->unk_328 = 1;
            v0->unk_32A ^= 1;
        }
    }
}
enum SignatureAppSubstate {
    SIGNATURE_APP_SUBSTATE_0_UNUSED = 0,
    SIGNATURE_APP_SUBSTATE_MAIN,
    SIGNATURE_APP_SUBSTATE_DONE_PRESSED,
    SIGNATURE_APP_SUBSTATE_CONFIRM_DONE,
    SIGNATURE_APP_SUBSTATE_PROMPT_RESET,
    SIGNATURE_APP_SUBSTATE_OPEN_RESET_MENU,
    SIGNATURE_APP_SUBSTATE_CONFIRM_RESET,
};
void String_Delete(String *string);
void YesNoPrompt_Destroy(YesNoPrompt *menu);
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
void FreeBgTilemapBuffer(BgConfig *bgConfig, u8 bgLayer);
void BG_ClearCharDataRange(u8 bgLayer, u32 size, u32 offset, enum HeapID heapID);
enum PaletteLoadLocation {
    PAL_LOAD_MAIN_BG = 0,
    PAL_LOAD_MAIN_OBJ,
    PAL_LOAD_MAIN_BGEXT,
    PAL_LOAD_MAIN_OBJEXT,
    PAL_LOAD_SUB_BG,
    PAL_LOAD_SUB_OBJ,
    PAL_LOAD_SUB_BGEXT,
    PAL_LOAD_SUB_OBJEXT,
};
u32 GfGfxLoader_LoadCharDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, BgConfig *bgConfig, u32 bgLayer, u32 offset, u32 size, BOOL compressed, u32 heapID);
void GfGfxLoader_LoadScrnDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, BgConfig *bgConfig, u32 bgLayer, u32 offset, u32 size, BOOL compressed, enum HeapID heapID);
void GfGfxLoader_GXLoadPalFromOpenNarc(NARC *narc, u32 narcMemberIdx, enum PaletteLoadLocation loadLocation, u32 offset, u32 size, u32 heapID);
void LoadFontPal1(enum PaletteLoadLocation palLocation, u32 palSlotOffset, enum HeapID heapID);
void LoadUserFrameGfx1(BgConfig *bgConfig, u8 bgLayer, u16 tileOffset, u8 palOffset, u8 standardWindowType, u32 heapID);
void LoadUserFrameGfx2(BgConfig *bgConfig, u8 bgLayer, u16 tileOffset, u8 palOffset, u8 messageBoxFrame, u32 heapID);

void ov52_021E84CC(SignatureApp *signatureApp);
void ov52_021E8568(SignatureApp *signatureApp);
void ov52_021E85A0(BgConfig *param0);
void ov52_021E85DC(SignatureApp *signatureApp, NARC *narc);

void ov52_021E84CC(SignatureApp *signatureApp) {
    for (int i = 0; i < 5; i++) {
        signatureApp->dummyStrings[i] = String_New(7 + 1, HEAP_ID_39);
        signatureApp->touchDataBuffers[i].bufferSize = 0;
        signatureApp->prevTouchData[i].bufferSize = 0;
    }

    signatureApp->buttonText = String_New(10 * 2, HEAP_ID_39);
    signatureApp->descriptionText = String_New(20 * 2, HEAP_ID_39);
    signatureApp->messageText = String_New(40 * 2, HEAP_ID_39);
    signatureApp->brushTypeUnused = 0;
    signatureApp->subState = SIGNATURE_APP_SUBSTATE_MAIN;

    ReadMsgDataIntoString(signatureApp->loader, 12, signatureApp->buttonText);
    ReadMsgDataIntoString(signatureApp->loader, 9, signatureApp->descriptionText);

    signatureApp->yesNoMenu = YesNoPrompt_Create(HEAP_ID_39);

    MI_CpuClearFast(&signatureApp->drawingState, sizeof(DrawingState));
}

void ov52_021E8568(SignatureApp *signatureApp) {
    YesNoPrompt_Destroy(signatureApp->yesNoMenu);

    for (int i = 0; i < 5; i++) {
        String_Delete(signatureApp->dummyStrings[i]);
    }

    String_Delete(signatureApp->messageText);
    String_Delete(signatureApp->descriptionText);
    String_Delete(signatureApp->buttonText);
}

void ov52_021E85A0(BgConfig *param0) {
    FreeBgTilemapBuffer(param0, BG_LAYER_SUB_1);
    FreeBgTilemapBuffer(param0, BG_LAYER_SUB_0);
    FreeBgTilemapBuffer(param0, BG_LAYER_MAIN_3);
    FreeBgTilemapBuffer(param0, BG_LAYER_MAIN_2);
    FreeBgTilemapBuffer(param0, BG_LAYER_MAIN_1);
    FreeBgTilemapBuffer(param0, BG_LAYER_MAIN_0);
    Heap_Free(param0);
}

void ov52_021E85DC(SignatureApp *signatureApp, NARC *narc) {
    BgConfig *bgConfig = signatureApp->bgConfig;

    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0, PAL_LOAD_MAIN_BG, 0, ((16) * sizeof(u16)) * 3, HEAP_ID_39);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 2, PAL_LOAD_SUB_BG, 0, ((16) * sizeof(u16)) * 2, HEAP_ID_39);
    LoadFontPal1(PAL_LOAD_MAIN_BG, ((13) * ((16) * sizeof(u16))), HEAP_ID_39);
    LoadFontPal1(PAL_LOAD_SUB_BG, ((13) * ((16) * sizeof(u16))), HEAP_ID_39);
    BG_ClearCharDataRange(BG_LAYER_MAIN_1, (8 * (8 / 2)), 0, HEAP_ID_39);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 3, bgConfig, BG_LAYER_MAIN_2, 0, 32 * 8 * (8 * (8 / 2)), 1, HEAP_ID_39);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 5, bgConfig, BG_LAYER_MAIN_2, 0, 32 * 24 * sizeof(u16), 1, HEAP_ID_39);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 4, bgConfig, BG_LAYER_SUB_1, 0, 32 * 8 * (8 * (8 / 2)), 1, HEAP_ID_39);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 6, bgConfig, BG_LAYER_SUB_1, 0, 32 * 24 * sizeof(u16), 1, HEAP_ID_39);
    LoadUserFrameGfx2(bgConfig, BG_LAYER_MAIN_0, 1, 10, Options_GetFrame(signatureApp->options), HEAP_ID_39);
    LoadUserFrameGfx1(bgConfig, BG_LAYER_MAIN_0, (1 + ((6 * 3) + ((2 * 2) * 3))), 11, 0, HEAP_ID_39);
}
