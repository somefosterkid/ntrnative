#include "global.h"

#include "error_handling.h"
#include "overlay_96_ov96rival.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov96_0220D014(Ov96Rival *arg0, s32 arg1);

void ov96_0220D014(Ov96Rival *arg0, s32 arg1) {
    u32 v0;
    u32 v1;
    if ((((u32)(arg0->unk_1C << 0x16)) >> 0x1F) != 0) {
        GF_AssertFail();
    }
    if (arg1 != 0) {
        v0 = arg0->unk_14;
        arg0->unk_14 = (s32)((v0 & 0xFF00FFFF) | (((u32)(((((u32)(arg0->unk_14 << 8)) >> 0x18) + 0x1E) << 0x18)) >> 8));
    }
    v1 = arg0->unk_14;
    if (((u32)(((u32)(v1 << 8)) >> 0x18)) > 0x64U) {
        arg0->unk_14 = (s32)(0x640000 | (v1 & 0xFF00FFFF));
    }
    if (((u32)(((u32)(arg0->unk_14 << 8)) >> 0x18)) >= 0x64U) {
        arg0->unk_1C = (s32)(0x200 | (arg0->unk_1C));
        arg0->unk_18 = (s32)(0xFFC007FF & (arg0->unk_18));
    }
}
#else
void ov96_0220D014(void *arg0, s32 arg1);

void ov96_0220D014(void *arg0, s32 arg1) {
    s32 v0;
    s32 v1;
    if ((((u32)((*((s32 *)(((u8 *)arg0) + 0x1C))) << 0x16)) >> 0x1F) != 0) {
        GF_AssertFail();
    }
    if (arg1 != 0) {
        v0 = *((s32 *)(((u8 *)arg0) + 0x14));
        *((s32 *)(((u8 *)arg0) + 0x14)) = (s32)((v0 & 0xFF00FFFF) | (((u32)(((((u32)((*((s32 *)(arg0 + 0x14))) << 8)) >> 0x18) + 0x1E) << 0x18)) >> 8));
    }
    v1 = *((s32 *)(((u8 *)arg0) + 0x14));
    if (((u32)(((u32)(v1 << 8)) >> 0x18)) > 0x64U) {
        *((s32 *)(((u8 *)arg0) + 0x14)) = (s32)(0x640000 | (v1 & 0xFF00FFFF));
    }
    if (((u32)(((u32)((*((s32 *)(((u8 *)arg0) + 0x14))) << 8)) >> 0x18)) >= 0x64U) {
        *((s32 *)(((u8 *)arg0) + 0x1C)) = (s32)(0x200 | (*((s32 *)(((u8 *)arg0) + 0x1C))));
        *((s32 *)(((u8 *)arg0) + 0x18)) = (s32)(0xFFC007FF & (*((s32 *)(((u8 *)arg0) + 0x18))));
    }
}
#endif
