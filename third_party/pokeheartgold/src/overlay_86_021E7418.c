#include "global.h"

#include "heap.h"
#include "overlay_86_frontier_records_app.h"
#include "unk_02030A98.h"
#include "unk_0205BFF0.h"

void *ov86_021E73EC(s32, u32 *);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov86_021E7418(FrontierRecordsApp *arg0) {
    u32 out;
    unsigned int new_var;
    u32 v3;
    u32 *v2;
    s32 v1;
    FrontierRecordsApp *v0;
    s32 v6;
    u16 v4;
    u16 v7;
    FrontierRecordsList *v5;
    void *ptr;

    v0 = arg0;
    v2 = sub_020312C4(arg0->saveData, 0xBU, &out);
    if (out == 1) {
        v1 = 0;
        new_var = 0U;
        do {
            ptr = ov86_021E73EC(v1, &v3);
            v5 = &v0->lists[v1];
            v5->entries = Heap_Alloc(HEAP_ID_121, v3 * 2);
            v7 = 0;
            if (v3 > new_var) {
                do {
                    v6 = v7 * 2;
                    if (sub_020312E0(v0->saveData, v2, sub_0205C144(v0->challengeType), (u32)(*((u16 *)(ptr + v6)))) != 0) {
                        v5->entries[v5->count] = *((u16 *)(ptr + v6));
                        v5->count = v5->count + 1;
                    }
                    v7 += 1;
                } while (((u32)v7) < v3);
            }
            Heap_Free(ptr);
            v4 = v1 + 1;
            v1 = (s32)v4;
        } while (((u32)v4) < 0x1AU);
    }
    if (v2 != 0L) {
        Heap_Free(v2);
    }
}
#else
void ov86_021E7418(FrontierRecordsApp *arg0) {
    u32 out;
    unsigned int new_var;
    u32 v3;
    u32 *v2;
    s32 v1;
    void *v0;
    s32 v6;
    u16 v4;
    u16 v7;
    void *v5;
    void *ptr;

    v0 = arg0;
    v2 = sub_020312C4(*((SaveData **)&arg0->saveData), 0xBU, &out);
    if (out == 1) {
        v1 = 0;
        new_var = 0U;
        do {
            ptr = ov86_021E73EC(v1, &v3);
            v5 = v0 + (v1 * 8);
            *((void **)(((u8 *)v5) + 0x260)) = Heap_Alloc(HEAP_ID_121, v3 * 2);
            v7 = 0;
            if (v3 > new_var) {
                do {
                    v6 = v7 * 2;
                    if (sub_020312E0(*(SaveData **)((u8 *)v0 + 0x224), v2, sub_0205C144(*((u8 *)v0 + 6)), (u32)(*((u16 *)(ptr + v6)))) != 0) {
                        *((u16 *)((*((void **)(((u8 *)v5) + 0x260))) + ((*((s32 *)(((u8 *)v5) + 0x264))) * 2))) = *((u16 *)(ptr + v6));
                        *((s32 *)(((u8 *)v5) + 0x264)) = (s32)((*((s32 *)(((u8 *)v5) + 0x264))) + 1);
                    }
                    v7 += 1;
                } while (((u32)v7) < v3);
            }
            Heap_Free(ptr);
            v4 = v1 + 1;
            v1 = (s32)v4;
        } while (((u32)v4) < 0x1AU);
    }
    if (v2 != 0L) {
        Heap_Free(v2);
    }
}
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov86_021E74D0(FrontierRecordsApp *arg0) {
    u32 v0;
    void *ptr;
    FrontierRecordsList *v1;

    v1 = arg0->lists;
    v0 = 0;
    do {
        ptr = v1->entries;
        if (ptr != NULL) {
            Heap_Free(ptr);
        }
        v0 += 1;
        v1++;
    } while (v0 < 0x1AU);
}
#else
void ov86_021E74D0(FrontierRecordsApp *arg0) {
    u32 v0;
    void *ptr;
    void *v1;

    v1 = arg0;
    v0 = 0;
    do {
        ptr = *(void **)((u8 *)v1 + 0x260);
        if (ptr != NULL) {
            Heap_Free(ptr);
        }
        v0 += 1;
        v1 += 8;
    } while (v0 < 0x1AU);
}
#endif
