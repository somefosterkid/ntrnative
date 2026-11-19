#include "global.h"

#include "easy_chat.h"
#include "overlay_45_unk_struct_ov66_022309a4.h"

void ov45_0222CA10(WallpaperPasswordBank *arg0, s32 arg1, void *arg2);

void ov45_0222CA10(WallpaperPasswordBank *wallpaperPasswordBank, s32 arg1, void *arg2) {
    s32 v2;
    s32 v1;
    s32 v0;
    u32 v7;
    u8 v3;
    u8 v4;
    u8 v5;
    u8 v6;

    v7 = WallpaperPasswordBank_GetCount(wallpaperPasswordBank);
    v2 = arg1;
    v5 = *(u8 *)(&v2);
    v3 = *(u8 *)((u8 *)(&v2) + 3);
    v0 = (s32)v3;
    *(u16 *)arg2 = WallpaperPasswordBank_GetWordAtIndex(wallpaperPasswordBank, (s32)((u32)(v3 + v5) % v7));
    v4 = *(u8 *)((u8 *)(&v2) + 1);
    v1 = (s32)v4;
    *(u16 *)((u8 *)arg2 + 2) = WallpaperPasswordBank_GetWordAtIndex(wallpaperPasswordBank, (s32)((u32)(v5 + v4) % v7));
    v6 = *(u8 *)((u8 *)(&v2) + 2);
    *(u16 *)((u8 *)arg2 + 4) = WallpaperPasswordBank_GetWordAtIndex(wallpaperPasswordBank, (s32)((u32)(v4 + v6) % v7));
    *(u16 *)((u8 *)arg2 + 6) = WallpaperPasswordBank_GetWordAtIndex(wallpaperPasswordBank, (s32)((u32)(v6 + v3) % v7));
}

void ov45_0222CA7C(void *arg0, s32 arg1, s32 arg2);

void ov45_0222CA7C(void *arg0, s32 arg1, s32 arg2) {
    *(s32 *)arg0 = 0;
    *(s32 *)((u8 *)arg0 + 0xC) = 0;
    *(s32 *)((u8 *)arg0 + 0x10) = (s32)(0x1E * (arg2 - arg1));
}

s32 ov45_0222ECB8(void *);

void ov45_0222CA8C(void *arg0);

void ov45_0222CA8C(void *arg0) {
    *(s32 *)arg0 = 1;
    ((UnkStruct_ov66_022309A4 *)arg0)->unk_0C = 0;
    ov45_0222ECB8(arg0 + 4);
}
