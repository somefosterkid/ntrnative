#include "global.h"
#include "constants/heap.h"
#include "font.h"
#include "save.h"
#include "filesystem.h"
#include "message_format.h"
#include "msgdata.h"

struct BgConfig;
struct MsgData;
struct NARC;
struct SaveData;
struct String;
struct MessageFormat;
struct SysTask;
struct SysTaskManager;
struct Window;
typedef struct SysTaskManager SysTaskManager;
struct SysTaskManager {
    u16 maxTasks;
    u16 stackPointer;
    SysTask sentinelTask;
    SysTask **taskStack;
    SysTask *tasks;
    BOOL locked;
    SysTask *currentTask;
    SysTask *nextTask;
};
typedef struct {
    SysTask *unk_00;
    BOOL unk_04;
    u16 unk_08[4][16];
    u16 unk_88[21][16];
    s16 unk_328;
    s8 unk_32A;
    u8 unk_32B;
} UnkStruct_ov61_0222C3B0;
void ClearWindowTilemapAndScheduleTransfer(Window *window);
void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);
void Heap_Free(void *ptr);
void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);
u8 TextPrinterCheckActive(u8 printerID);
void RemoveTextPrinter(u8 printerID);
SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);
static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *);
static inline void inline_ov61_0222C3B0_sub(SysTask *, void *);
static inline void inline_ov61_0222C3B0(UnkStruct_ov61_0222C3B0 *param0, NARC *param1, int param2, enum HeapID heapID)
{
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
static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0)
{
    int v0, v1, v2, v3, v4, v5;
    v1 = 0;

    for (v0 = 0; v0 < 4 - 1; v0++) {
        v2 = v0 + 1;
        v3 = 0;
        v5 = 0;
        do {
            ((v1 < (((16 << 8) / 0x300 + 2) * (4 - 1))) ? (void)0 : GF_AssertFail()) ;
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
        } while (1 );
    }
    DC_FlushRange(param0->unk_88, (((16 << 8) / 0x300 + 2) * (4 - 1)) * 16 * sizeof(u16));
}
static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1)
{
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
void ClearWindowPixelBuffer(Window *window, u8 skipTransfer);
void ClearFrameAndWindow2(Window *window, u8 skipTransfer);
typedef struct {
    MessageFormat *unk_00;
    MsgData *unk_04;
    Window unk_08;
    String *unk_18;
    String *unk_1C;
    u32 unk_20;
    void *unk_24;
    u32 unk_28;
    u32 unk_2C;
} UnkStruct_ov67_0225D210;
void ov46_02259474(UnkStruct_ov67_0225D210 *param0);
void ov67_0225D188(UnkStruct_ov67_0225D210 *param0, BgConfig *param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6, u32 param7, u32 param8, SaveData *saveData, enum HeapID heapID);
void *WaitingIcon_New(Window *window, u32 baseTile);
void sub_0200F450(void *taskData);
void String_Delete(String *string);
void RemoveWindow(Window *window);
void MessageFormat_Delete(MessageFormat *template);
void DestroyMsgData(MsgData *loader);
void FillWindowPixelBuffer(Window *window, u8 val);
typedef u32 TextColor;
u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontID, const String *string, u32 xOffset, u32 yOffset, u32 renderDelay, TextColor color, PrinterCallback_t callback);

void ov46_022593F8(UnkStruct_ov67_0225D210 *param0);
void ov46_02259450(UnkStruct_ov67_0225D210 *param0);
void ov46_02259474(UnkStruct_ov67_0225D210 *param0);
void ov46_02259494(UnkStruct_ov67_0225D210 *param0);
void ov46_022594E0(UnkStruct_ov67_0225D210 *param0, u32 param1);

void ov46_022593F8(UnkStruct_ov67_0225D210 *param0)
{
    if (TextPrinterCheckActive(param0->unk_2C)) {
        RemoveTextPrinter(param0->unk_2C);
    }

    if (param0->unk_20 == 0) {
        ClearWindowPixelBuffer(&param0->unk_08, 1);
        ClearWindowTilemapAndScheduleTransfer(&param0->unk_08);
    } else {
        if (param0->unk_24) {
            ov46_02259474(param0);
        }

        ClearFrameAndWindow2(&param0->unk_08, 1);
        ClearWindowTilemapAndScheduleTransfer(&param0->unk_08);
    }
}

void ov46_02259450(UnkStruct_ov67_0225D210 *param0)
{
    if (param0->unk_20 == 1) {
        ((param0->unk_24 == 0L) ? (void)0 : GF_AssertFail()) ;
        param0->unk_24 = WaitingIcon_New(&param0->unk_08, 1);
    }
}

void ov46_02259474(UnkStruct_ov67_0225D210 *param0)
{
    if (param0->unk_20 == 1) {
        ((param0->unk_24 != 0L) ? (void)0 : GF_AssertFail()) ;
        sub_0200F450(param0->unk_24);
        param0->unk_24 = 0L ;
    }
}

void ov46_02259494(UnkStruct_ov67_0225D210 *param0)
{
    if (TextPrinterCheckActive(param0->unk_2C)) {
        RemoveTextPrinter(param0->unk_2C);
    }

    if (param0->unk_24) {
        ov46_02259474(param0);
    }

    RemoveWindow(&param0->unk_08);
    String_Delete(param0->unk_1C);
    String_Delete(param0->unk_18);
    DestroyMsgData(param0->unk_04);
    MessageFormat_Delete(param0->unk_00);
}

void ov46_022594E0(UnkStruct_ov67_0225D210 *param0, u32 param1)
{
    FillWindowPixelBuffer(&param0->unk_08, 0);
    ReadMsgDataIntoString(param0->unk_04, param1, param0->unk_1C);
    StringExpandPlaceholders(param0->unk_00, param0->unk_18, param0->unk_1C);
    AddTextPrinterParameterizedWithColor(&param0->unk_08, FONT_MESSAGE, param0->unk_18, FontID_String_GetCenterAlignmentX(FONT_SYSTEM, param0->unk_18, 0, 0xB0), 0, 0 , ((TextColor)(((15 & 0xFF) << 16) | (( 14 & 0xFF) << 8) | (( 0 & 0xFF) << 0))) , 0);
}
