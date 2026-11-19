#include "global.h"
#include "unk_0201956C.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "map_object.h"

struct MapObject;

struct ObjectEventGfxRenderDetailsEntry;

typedef struct MapObject MapObject;

void ov14_021E7B04(MapObject *mapObj, int param1);

void ov14_021E7B98(MapObject *mapObj);

s32 ov14_021E7D8C(UnkStruct_0201956C *, s32, s32);
s32 ov14_021E81A8(UnkStruct_0201956C *, s32);
s32 ov14_021E8394(UnkStruct_0201956C *);
s32 ov14_021E85AC(UnkStruct_0201956C *);
s32 ov14_021E8600(UnkStruct_0201956C *);
s32 ov14_021F6244(UnkStruct_ov14_boxsys *);
s32 ov14_021F62CC(UnkStruct_ov14_boxsys *);
s32 ov14_021F62E4(UnkStruct_ov14_boxsys *);
s32 ov14_021F62FC(UnkStruct_ov14_boxsys *);
s32 ov14_021F6314(void *);
s32 ov14_021F63C8(UnkStruct_ov14_boxsys *);
s32 ov14_021F63F0(UnkStruct_ov14_boxsys *);
void ov14_021E7BA4(void *arg0);

void ov14_021E7B98(MapObject *mapObj) {
    ov14_021E7B04(mapObj, 0);
}

void ov14_021E7BA4(void *arg0) {
    void *v0;

    ((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0 = sub_0201956C(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_14, 2, 0x11, HEAP_ID_10);
    sub_020195F4(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0, 0xD, 1, 0x20, 7);
    sub_020195F4(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0, 1, 1, 0xB, 0x12);
    sub_020195F4(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0, 2, 0, 0xB, 0x12);
    sub_020195F4(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0, 0xC, 0, 0xB, 6);
    ov14_021F6244(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys));
    ov14_021F62CC(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys));
    ov14_021F62E4(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys));
    ov14_021F62FC(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys));
    ov14_021F6314(arg0);
    if (*(u32 *)((u8 *)(*(void **)arg0) + 8) == 0) {
        sub_020195F4(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0, 0, 0, 0x20, 7);
        ov14_021E7D8C(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0, 0, 0xC);
    } else {
        sub_020195F4(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0, 0, 1, 0x20, 6);
        ov14_021E7D8C(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0, 0, 0xB);
    }
    ov14_021E7D8C(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0, 0xD, 0xD);
    v0 = ((void *)((UnkStruct_ov14_boxapp *)arg0)->sys);
    if ((u32) * (u32 *)((u8 *)(*(void **)arg0) + 8) <= 1U) {
        ov14_021E7D8C(((UnkStruct_ov14_boxsys *)v0)->unk_2F0, 1, 9);
    } else {
        ov14_021E81A8(((UnkStruct_ov14_boxsys *)v0)->unk_2F0, 1);
        ov14_021F63F0(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys));
    }
    if (*(u32 *)((u8 *)(*(void **)arg0) + 8) == 3) {
        sub_020195F4(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0, 0x10, 6, 0x20, 9);
        sub_020196E8(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0, 0x10, 0, 0x18);
        ov14_021E7D8C(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0, 0x10, 0x55);
    } else {
        sub_020195F4(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0, 0x10, 6, 0xC, 0xA);
        sub_020196E8(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0, 0x10, 0x20, 0xC);
        ov14_021E7D8C(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0, 0x10, 0x56);
    }
    ov14_021E7D8C(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0, 2, 0xA);
    ov14_021F63C8(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys));
    ov14_021E8394(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0);
    ov14_021E85AC(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0);
    ov14_021E8600(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0);
}
