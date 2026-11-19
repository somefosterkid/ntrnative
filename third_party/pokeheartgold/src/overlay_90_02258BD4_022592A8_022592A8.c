#include "global.h"
#include "overlay_90_unk_struct_ov114_0225_d084.h"
#include "constants/heap.h"
#include "pm_string.h"
#include "bg_window.h"
#include "msgdata.h"
#include "overlay_90_unk_struct_ov114_02260284.h"
#include "overlay_90_unk_struct_ov114_0225cef0.h"

struct BgConfig;

struct BgTemplate;

struct GraphicsModes;

struct Window;

typedef u32 TextColor;

void ov90_02259350(UnkStruct_ov114_0225D084 *param0, u32 param1, Window *param2, u8 param3, u8 param4, TextColor param5);

void ov90_02258FF0(UnkStruct_ov114_0225CEF0 *param0, const GraphicsModes *param1, const UnkStruct_ov114_02260284 *param2, u32 param3, enum HeapID heapID);

void ov90_022592A8(UnkStruct_ov114_0225D084 *param0, u32 param1, Window *param2, u8 param3, u8 param4);

void ov90_022592A8(UnkStruct_ov114_0225D084 *param0, u32 param1, Window *param2, u8 param3, u8 param4) {
    ov90_02259350(param0, param1, param2, param3, param4, ((TextColor)(((15 & 0xFF) << 16) | ((14 & 0xFF) << 8) | ((0 & 0xFF) << 0))));
}
