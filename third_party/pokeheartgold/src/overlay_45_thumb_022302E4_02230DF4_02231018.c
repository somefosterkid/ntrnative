#include "global.h"

#include "constants/heap.h"
#include "overlay_45_wifi_plaza_system.h"
#include "filesystem.h"

struct NARC;

struct PlaceRecord;

struct UnkStruct_ov69_0225C980_t;

typedef struct PlaceRecord {
    u16 type;
    s16 x;
    s16 y;
} PlaceRecord;

typedef struct UnkStruct_ov69_0225C980_t {
    s16 unk_00;
    s16 unk_02;
} UnkStruct_ov69_0225C980;

u32 GeonetLocations_GetIndexFromCountry(u32 country);

u32 GeonetLocations_GetNarcMemberIndex(u32 index);

void *GfGfxLoader_LoadFromOpenNarc_GetSizeOut(NARC *narc, u32 narcMemberIdx, BOOL compressed, enum HeapID heapID, BOOL allocAtEnd, u32 *fileSize);

void Heap_Free(void *ptr);

UnkStruct_ov66_02231428 ov66_02234EA8(u16 param0, u8 param1, UnkStruct_ov66_02231428 param2, enum HeapID heapID);

void ov45_02231018(NARC *param0, u32 param1, u16 param2, u8 param3, fx32 *param4);

void ov45_02231018(NARC *param0, u32 param1, u16 param2, u8 param3, fx32 *param4) {
    BOOL v0 = 0;

    {
        void *v1;
        PlaceRecord *v2;
        u32 v3;
        int v4;

        v1 = GfGfxLoader_LoadFromOpenNarc_GetSizeOut((struct NARC *)(param0), 18, 0, (enum HeapID)(param1), 0, &v3);
        v2 = (PlaceRecord *)v1;
        v4 = v3 / 6;

        ((param2 < v4) ? (void)0 : GF_AssertFail());

        if (v2[param2].type != 2) {
            *param4 = v2[param2].y;
            v0 = 1;
        }

        Heap_Free(v1);
    }

    if (v0 == 0) {
        void *v5;
        UnkStruct_ov69_0225C980 *v6;
        u32 v7, v8;
        int v9, v10;

        v9 = GeonetLocations_GetIndexFromCountry(param2);
        v8 = GeonetLocations_GetNarcMemberIndex(v9);
        v5 = GfGfxLoader_LoadFromOpenNarc_GetSizeOut((struct NARC *)(param0), v8, 0, (enum HeapID)(param1), 0, &v7);
        v6 = (UnkStruct_ov69_0225C980 *)v5;
        v10 = v7 / 4;

        if (param3 < v10) {
            *param4 = v6[param3].unk_02;
        } else {
            ((0) ? (void)0 : GF_AssertFail());
            *param4 = v6[0].unk_02;
        }

        Heap_Free(v5);
    }
}
