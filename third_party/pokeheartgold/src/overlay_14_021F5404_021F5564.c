#include "global.h"

#include "overlay_14_unk_struct_ov14_boxsys.h"
struct UnkStruct_ov14_boxapp;

#include "bg_window.h"
#include "item.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F5564(struct UnkStruct_ov14_boxapp *arg0, u8 arg1);
s32 ov14_021F5564(struct UnkStruct_ov14_boxapp *arg0, u8 arg1);

u8 ov14_021F4F24(void *, String *, s32, s32, s32, s32, s32);

#if defined(SDK_PORT) || defined(NONMATCHING)

s32 ov14_021F5564(UnkStruct_ov14_boxapp *arg0, u8 arg1) {
    u8 itemId;
    s32 v4;
    UnkStruct_ov14_boxsys *v0;
    Window *v3;

    itemId = arg1;
    v0 = ((void *)arg0->sys);
    if (v0->unk_44E_0 == 0) {
        v4 = 0x14;
    } else {
        v4 = 0x16;
    }
    v3 = v0->windows;
    v0->unk_44E_0 ^= 1;
    FillWindowPixelBuffer(&v3[v4], 0xDU);
    FillWindowPixelBuffer(&v3[v4 + 1], 0xDU);
    if (itemId != 0) {
        GetItemNameIntoString(arg0->sys->unk_28, (u16)itemId, HEAP_ID_10);
        ov14_021F4F24(&v3[v4], arg0->sys->unk_28, 0, 0, 0, 0x10200, 0);
        CopyWindowPixelsToVram_TextMode(&v3[v4]);
        GetItemDescIntoString(arg0->sys->unk_28, (u16)itemId, HEAP_ID_10);
        ov14_021F4F24(&v3[v4 + 1], arg0->sys->unk_28, 0, 0, 0, 0x10200, 0);
        CopyWindowPixelsToVram_TextMode(&v3[v4 + 1]);
    }
    return v4;
}
#else

s32 ov14_021F5564(UnkStruct_ov14_boxapp *arg0, u8 arg1) {
    u8 itemId;
    s32 v2;
    s32 v4;
    u8 v1;
    UnkStruct_ov14_boxsys *v0;
    void *v3;

    itemId = arg1;
    v0 = ((void *)arg0->sys);
    if (((u32)(*((u8 *)v0 + 0x44E) << 0x1C) >> 0x1C) == 0) {
        v4 = 0x14;
    } else {
        v4 = 0x16;
    }
    v1 = *((s8 *)v0 + 0x44E);
    v3 = v0->windows;
    v2 = v4 * 0x10;
    *((u8 *)v0 + 0x44E) = (u8)((v1 & ~0xF) | (0xF & (u8)(1 ^ ((u32)(v1 << 0x1C) >> 0x1C))));
    FillWindowPixelBuffer(v3 + v2, 0xDU);
    FillWindowPixelBuffer(v3 + v2 + 0x10, 0xDU);
    if (itemId != 0) {
        GetItemNameIntoString(arg0->sys->unk_28, (u16)itemId, HEAP_ID_10);
        ov14_021F4F24(v3 + v2, arg0->sys->unk_28, 0, 0, 0, 0x10200, 0);
        CopyWindowPixelsToVram_TextMode(v3 + v2);
        GetItemDescIntoString(arg0->sys->unk_28, (u16)itemId, HEAP_ID_10);
        ov14_021F4F24(v3 + v2 + 0x10, arg0->sys->unk_28, 0, 0, 0, 0x10200, 0);
        CopyWindowPixelsToVram_TextMode(v3 + v2 + 0x10);
    }
    return v4;
}
#endif
