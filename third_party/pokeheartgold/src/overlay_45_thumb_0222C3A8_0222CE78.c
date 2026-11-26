#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "player_data.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_45_wifi_plaza_system.h"
#include "overlay_45_wifi_plaza_api.h"
#include "overlay_91_unk_struct_ov66_02230dbc.h"

void ov45_0222CE78(UnkStruct_ov66_02230DBC *arg0, enum HeapID heapID);

void ov45_0222CE78(UnkStruct_ov66_02230DBC *arg0, enum HeapID heapID) {
    s32 v0;

    v0 = 0;
    do {
        arg0->unk_20[v0] = PlayerProfile_New(heapID);
        v0 += 1;
    } while (v0 < 4);
}

void ov45_0222CE94(UnkStruct_ov66_02230DBC *arg0);

void ov45_0222CE94(UnkStruct_ov66_02230DBC *arg0) {
    s32 v0;

    v0 = 0;
    do {
        Heap_Free(arg0->unk_20[v0]);
        arg0->unk_20[v0] = NULL;
        v0 += 1;
    } while (v0 < 4);
}

u32 ov45_0222AAC8(const WiFiPlazaProfile *profile);

void ov45_0222AB38(WiFiPlazaSystem *param0, UnkStruct_ov66_02232068 *param1);

void ov45_0222A844(const WiFiPlazaProfile *profile, PlayerProfile *param1, enum HeapID heapID);

void ov45_0222CEB0(UnkStruct_ov66_02230DBC *arg0, WiFiPlazaSystem *arg1, enum HeapID arg2);

void ov45_0222CEB0(UnkStruct_ov66_02230DBC *arg0, WiFiPlazaSystem *arg1, enum HeapID arg2) {
    enum HeapID v0;
    const WiFiPlazaProfile *v1;
    s32 v2;
    s32 v3;

    v0 = arg2;
    arg0->unk_00 = ov45_0222AAC8(ov45_0222A5C0(arg1));
    ov45_0222AB38(arg1, &arg0->unk_08);
    ov45_0222A498(arg1, &arg0->unk_04);
    arg0->unk_30 = arg1;
    v2 = 0;
    arg0->unk_1C = 0;
    v3 = 0;
    do {
        v1 = ov45_0222A578(arg1, arg0->unk_04.unk_00[v2]);
        if (v1 != NULL) {
            ov45_0222A844(v1, arg0->unk_20[v3], v0);
        }
        v2 += 1;
        v3 += 1;
    } while (v2 < 4);
}

void ov45_0222CF00(void *arg0, void **arg1, enum HeapID arg2);

void ov45_0222CF00(void *arg0, void **arg1, enum HeapID heapID) {
    void *v0;
    void *v1;

    v0 = Heap_Alloc(heapID, 0x20U);
    *arg1 = v0;
    memset(v0, 0, 0x20U);
    v1 = *arg1;
    *(s32 *)v1 = (s32) * (s32 *)((u8 *)arg0 + 0x40);
    *(s32 *)((u8 *)v1 + 0x1C) = (s32) * (s32 *)((u8 *)arg0 + 0x3C);
}

void ov45_0222CF24(void **arg0);

void ov45_0222CF24(void **arg0) {
    if (*arg0 == NULL) {
        GF_AssertFail();
    }
    Heap_Free(*arg0);
    *arg0 = NULL;
}

s32 ov45_0222AD2C(void *);

s32 ov45_0222AD3C(void *);

s32 ov45_0222CDC4(void *, void **, s32 *);

extern s32 ov45_02254B94;

void ov45_0222CF40(void *arg0, void **arg1);

void ov45_0222CF40(void *arg0, void **arg1) {
    void *v0;

    v0 = *arg1;
    *(s32 *)((u8 *)v0 + 4) = ov45_0222AD2C(*(void **)((u8 *)arg0 + 0x3C));
    *(s32 *)((u8 *)v0 + 8) = ov45_0222AD3C(*(void **)((u8 *)arg0 + 0x3C));
    ov45_0222CDC4(arg0, arg1, &ov45_02254B94);
}

s32 ov45_0222CE2C(void *, s32);

s32 ov45_0222CF68(void *arg0, void **arg1);

s32 ov45_0222CF68(void *arg0, void **arg1) {
    u32 v0;

    v0 = *(u32 *)((u8 *)(*arg1) + 0x18);
    switch (v0) {
    case 0:
        ov45_0222CE2C(arg0, 9);
        break;
    case 1:
        ov45_0222CE2C(arg0, 1);
        break;
    case 2:
        ov45_0222CE2C(arg0, 2);
        break;
    case 6:
        ov45_0222CE2C(arg0, 3);
        break;
    case 7:
        ov45_0222CE2C(arg0, 4);
        break;
    case 3:
        ov45_0222CE2C(arg0, 6);
        break;
    case 4:
        ov45_0222CE2C(arg0, 7);
        break;
    case 5:
        ov45_0222CE2C(arg0, 8);
        break;
    case 8:
        ov45_0222CE2C(arg0, 0xA);
        break;
    case 9:
        ov45_0222CE2C(arg0, 0xB);
        break;
    case 10:
        ov45_0222CE2C(arg0, 0xC);
        break;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}
