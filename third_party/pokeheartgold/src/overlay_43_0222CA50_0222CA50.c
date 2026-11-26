#include "global.h"
#include "sprite_resources_header.h"

#include "bg_window.h"
#include "overlay_43_unk_struct_ov64_0222_e21_c.h"
#include "overlay_43_unk_struct_ov64_0222_e060.h"
#include "overlay_43_unk_struct_ov64_0222_f038.h"
#include "unk_0202C034.h"
#include "options.h"
#include "filesystem.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "font_types_def.h"
#include "list_menu.h"
#include "overlay_43_unk_struct_ov64_02230904.h"
#include "overlay_43_palpad.h"
#include "overlay_43_palpad_fns.h"
#include "font.h"
#include "overlay_manager.h"
#include "naming_screen.h"

struct ListMenuCursor;

struct ListMenu2D;

struct NARC;

struct NamingScreenArgs;

struct SaveData;

struct String;

struct SysTask;

struct SysTaskManager;

struct UnkStruct_ov63_0222AE60_t;

struct UnkStruct_ov63_0222BB38_t;

struct WiFiList;

struct Window;

typedef u16 charcode_t;

void String_Delete(String *string);

void CopyU16ArrayToString(String *dst, const charcode_t *src);

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

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

typedef struct UnkStruct_ov63_0222AE60_t UnkStruct_ov63_0222AE60;

typedef struct UnkStruct_ov63_0222BB38_t UnkStruct_ov63_0222BB38;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
} UnkStruct_ov64_022302EC;

UnkStruct_ov63_0222BB38 *ov42_022272BC(UnkStruct_ov63_0222AE60 *param0, const UnkStruct_ov64_022302EC *param1, u32 param2, u32 heapID);

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

typedef u32 TextColor;

u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontID, const String *string, u32 xOffset, u32 yOffset, u32 renderDelay, TextColor color, PrinterCallback_t callback);

typedef struct ListMenuCursor ListMenuCursor;

typedef struct ListMenu2D ListMenu2D;

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

typedef struct {
    SysTask *unk_00;
    BOOL unk_04;
    u16 unk_08[4][16];
    u16 unk_88[21][16];
    s16 unk_328;
    s8 unk_32A;
    u8 unk_32B;
} UnkStruct_ov61_0222C3B0;

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

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

typedef struct {
    s16 unk_00;
    s16 unk_02;
    u8 unk_04;
    u8 unk_05;
} UnkStruct_ov64_0223221C;

typedef struct {
    void *unk_00[2];
    NNSG2dScreenData *unk_08[2];
} UnkStruct_ov64_022300E4;

typedef struct {
    u32 unk_00;
    u8 unk_04[32];
} UnkStruct_ov64_022301B4;

typedef struct {
    UnkStruct_ov64_0223221C unk_00;
    Window unk_08;
    UnkStruct_ov63_0222BB38 *unk_18[8];
} UnkStruct_ov64_02230074;

typedef struct {
    UnkStruct_ov64_0223221C unk_00;
    u8 unk_6[4];
} UnkStruct_ov64_02232324;

typedef struct {
    Window unk_00;
    Window unk_10;
    void *unk_20;
    NNSG2dScreenData *unk_24;
} UnkStruct_ov64_02230620;

typedef struct {
    UnkStruct_ov64_0223221C unk_00;
    u32 unk_08;
    Window *unk_0C[7];
    u32 unk_28[7];
    u8 unk_44[8];
} UnkStruct_ov64_02230F98;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_ov64_0223222E;

void ov43_0222CB34(UnkStruct_ov64_02230074 *param0, PalPadGfx *param1, const UnkStruct_ov64_022300E4 *param2, u32 param3);

extern const UnkStruct_ov64_02232324 ov43_0222EFA0[9];

void ov64_0222DF48(PalPadWork *param0, enum HeapID heapID);

void ov64_0222E040(PalPadShared *param0, void *param1, enum HeapID heapID);

void ov64_0222E1A4(PalPadGfx *param0, const PalPadShared *param1, enum HeapID heapID);

void ov64_0222E3D8(PalPadGfx *param0, enum HeapID heapID);

void ov64_0222E5D8(PalPadGfx *param0, enum HeapID heapID);

void ov64_0222E620(PalPadGfx *param0, const PalPadShared *param1, enum HeapID heapID);

void ov43_0222AB20(PalPadGfx *param0, SaveData *saveData, u32 param2, enum HeapID heapID);

void ov43_0222AB5C(PalPadGfx *param0, SaveData *saveData, u32 param2, enum HeapID heapID);

void ov64_0222EA68(PalPadMenu *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

int ov64_0222EA70(PalPadMenu *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

void ov43_0222B4BC(PalPadMenu *param0, PalPadGfx *param1, u32 param2, enum HeapID heapID);

int ov64_0222F0C4(PalPadRoster *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

void ov43_0222BB20(PalPadRoster *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

BOOL ov43_0222C024(PalPadRoster *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

void ov64_0222FA70(PalPadRoster *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

BOOL ov64_0222FBA4(PalPadRoster *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

void ov64_0222FC1C(PalPadRoster *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);


void ov43_0222C550(PalPadRoster *param0, PalPadGfx *param1, u32 param2, enum HeapID heapID);

void ov43_0222C918(UnkStruct_ov64_02230074 *param0, PalPadShared *param1, PalPadGfx *param2, UnkStruct_ov63_0222AE60 *param3, const UnkStruct_ov64_022300E4 *param4, const UnkStruct_ov64_022301B4 *param5, u32 param6, enum HeapID heapID);

void ov64_02230444(PalPadNaming *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

int ov43_0222CBB4(PalPadNaming *param0, PalPadWork *param1, PalPadShared *param2, PalPadGfx *param3, enum HeapID heapID);

NamingScreenArgs *ov64_022305DC(PalPadNaming *param0, PalPadShared *param1, enum HeapID heapID);

void ov64_02230620(UnkStruct_ov64_02230620 *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

void ov64_02230680(UnkStruct_ov64_02230620 *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

void ov64_022308DC(PalPadDetails *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

int ov64_02230904(PalPadDetails *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

void ov43_0222D584(PalPadDetails *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

void ov64_02230BD8(PalPadDetails *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

void ov43_0222DE14(UnkStruct_ov64_02230F98 *param0, PalPadGfx *param1, u32 param2, u32 param3, const UnkStruct_ov64_0223222E *param4, enum HeapID heapID);

void ov64_02231164(UnkStruct_ov64_02230F98 *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

void ov64_02231528(UnkStruct_ov64_02230F98 *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

void ov43_0222E220(UnkStruct_ov64_02230F98 *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

void ov43_0222E5B4(UnkStruct_ov64_02230F98 *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

void ov43_0222E78C(UnkStruct_ov64_02230F98 *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

void ov43_0222E904(UnkStruct_ov64_02230F98 *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

void ov64_02231E94(UnkStruct_ov64_02230F98 *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

BOOL ov64_0223217C(PalPadShared *param0, enum HeapID heapID);

void ov43_0222CA50(UnkStruct_ov64_02230074 *param0, UnkStruct_ov63_0222AE60 *param1, PalPadGfx *param2, const UnkStruct_ov64_022300E4 *param3, u32 param4, WiFiList *param5, u32 param6, enum HeapID heapID);

void ov43_0222CA50(UnkStruct_ov64_02230074 *param0, UnkStruct_ov63_0222AE60 *param1, PalPadGfx *param2, const UnkStruct_ov64_022300E4 *param3, u32 param4, WiFiList *param5, u32 param6, enum HeapID heapID) {
    int v0;
    int v1;
    UnkStruct_ov64_022302EC v2;
    String *v3;
    int v4, v5;
    extern const u32 ov43_0222ED94[];

    v1 = sub_0202C090(param5, param6, 7);
    v0 = sub_0202C090(param5, param6, 8);

    if (v0 == 2) {
        ov43_0222CB34(param0, param2, param3, param4);
    } else {
        v2.unk_00 = ((param0->unk_00.unk_00 + ov43_0222EFA0[param4].unk_00.unk_00) * 8) + 8;
        v2.unk_02 = ((param0->unk_00.unk_02 + ov43_0222EFA0[param4].unk_00.unk_02) * 8) + (6 + 256);
        v2.unk_04 = 8;
        v2.unk_06 = 1;
        param0->unk_18[param4] = ov42_022272BC(param1, &v2, v1, heapID);
    }

    v4 = ((ov43_0222EFA0[param4].unk_00.unk_00) * 8) + 0;
    v5 = ((ov43_0222EFA0[param4].unk_00.unk_02) * 8) + -24;
    v3 = String_New(128, heapID);

    CopyU16ArrayToString(v3, sub_0202C254(param5, param6));
    AddTextPrinterParameterizedWithColor(&param0->unk_08, (enum Font)4, v3, v4, v5, 0xFF, ov43_0222ED94[v0], 0L);
    String_Delete(v3);
}

extern const u8 Unk_ov64_02232258[5];

void ov43_0222CB34(UnkStruct_ov64_02230074 *param0, PalPadGfx *param1, const UnkStruct_ov64_022300E4 *param2, u32 param3) {
    CopyToBgTilemapRect(param1->bgConfig, 2, ((param0->unk_00.unk_00 < 0) ? (-param0->unk_00.unk_00) : (param0->unk_00.unk_00)) + ov43_0222EFA0[param3].unk_00.unk_00, ((param0->unk_00.unk_02 < 0) ? (-param0->unk_00.unk_02) : (param0->unk_00.unk_02)) + ov43_0222EFA0[param3].unk_00.unk_02, 3, 4, param2->unk_08[1]->rawData, 0, 0, param2->unk_08[1]->screenWidth / 8, param2->unk_08[1]->screenHeight / 8);
    ScheduleBgTilemapBufferTransfer(param1->bgConfig, 2);
}
