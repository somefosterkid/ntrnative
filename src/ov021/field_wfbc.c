// Black's code at ov021 0x021938ac-0x02193f34. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/field_wfbc.h"
#include "field/encounter.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_camera.h"
#include "field/field_map.h"
#include "gfl/arc_util.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "struct_decls.h"
#include "field/eventdata_system.h"
#include "gfl/arc.h"
#include "field/city_state.h"
#include "field/event_mapchange.h"
#include "field/field_wfbc_data_local.h"
#include "save/player_info.h"
#include "system/wordset.h"

const u32 data_ov021_021d3f58[2] = { 0x1e, 0x1f };
const u32 data_ov021_021d3f60[2] = { 0x20, 0x21 };

// An entry of the table in archive 0x9a's file data_ov021_021d3f60[city] (8 bytes), names by offset
typedef struct {
    u16 unk_0;
    u16 unk_2;
    u32 unk_4;
} FieldWFBCEventEntry;

FieldWFBC *func_ov021_021938ac(HeapID heapId) {
    FieldWFBC *wfbc;

    wfbc = GFL_HeapAllocate(heapId, sizeof(FieldWFBC), 1, "field_wfbc.c", 0x12a);
    func_ov021_02193cbc(wfbc);
    return wfbc;
}

void func_ov021_021938d4(FieldWFBC *wfbc) {
    func_ov021_02193c80(wfbc);
    if (wfbc->unk_140 != NULL) {
        func_ov010_0216d974(wfbc->unk_140, 1);
    }
    GFL_HeapFree(wfbc);
}

u32 func_ov021_021938f4(FieldWFBC *wfbc) {
    return func_ov021_02193d08(wfbc);
}

void func_ov021_021938fc(FieldWFBC *wfbc, CityState *city) {
    func_ov021_02193e54(wfbc, city);
}

void func_ov021_02193904(FieldWFBC *wfbc) {
    func_ov021_02193eb4(wfbc);
}

FieldWFBCPerson *func_ov021_0219390c(FieldWFBC *wfbc, u32 id) {
    if (id < 0x1e) {
        return func_ov021_02193f34(wfbc, id);
    }
    return NULL;
}

u8 func_ov021_0219391c(FieldWFBC *wfbc) {
    return wfbc->city;
}

u8 func_ov021_02193920(FieldWFBC *wfbc) {
    return wfbc->isOther;
}

u32 func_ov021_02193924(FieldWFBC *wfbc, void *terrain, FieldPropSystem *propSystem, FieldChunk *chunk, u32 propCount,
                        s32 x, s32 z, HeapID heapId) {
    Ov021Cell cell;
    int tileX;
    int tileZ;
    LandDataPatch *ptrs;

    cell = func_ov021_02194038(wfbc->unk_18, x, z);
    if (cell.entry.unk1 != 0xff) {
        tileX = (x * 8) & 0x1f;
        tileZ = (z * 8) & 0x1f;
        ptrs = func_ov021_021c939c(cell.unk2, (heapId & 0x7fff) | 0x8000);
        func_ov021_021c93e4(ptrs, terrain, cell.entry.unk0_0 << 3, cell.entry.unk0_4 << 3, tileX, tileZ, 8, 8);
        if (cell.entry.unk0_0 == 0 && cell.entry.unk0_4 == 0) {
            propCount = func_ov021_021c9450(ptrs, propSystem, chunk, propCount, tileX, tileZ);
        }
        func_ov021_021c93d0(ptrs);
    }
    return propCount;
}

void func_ov021_021939c0(FieldWFBC *wfbc, EventDataSystem *eventData, BOOL isOther, HeapID heapId) {
    u32 length;
    FieldWFBCEventEntry *table;
    u32 *list;
    u32 count;
    u32 n;
    u32 i;
    u32 j;
    FieldWFBCEventEntry *entry;
    u32 heap;

    if (isOther == FALSE) {
        length = GFL_ArcToolGetDataLength(wfbc->arc, data_ov021_021d3f60[wfbc->city]);
        heap = heapId & 0x7fff | 0x8000;
        table = GFL_ArcToolReadHeapNew(wfbc->arc, data_ov021_021d3f60[wfbc->city], heap);
        count = length >> 3;
        list = GFL_HeapAllocate(heap, count << 2, 1, "field_wfbc.c", 0x208);
        n = 0;
        for (i = 0; i < count; i++) {
            entry = &table[i];
            if (func_ov021_02194044(wfbc->unk_18, entry->unk_0, entry->unk_2) != 0) {
                list[n] = entry->unk_4;
                n = n + 1;
            } else {
                for (j = 0; j < n; j++) {
                    if (list[j] == table[i].unk_4) {
                        break;
                    }
                }
                if (j >= n) {
                    for (j = i + 1; j < count; j++) {
                        if (entry->unk_4 == table[j].unk_4) {
                            break;
                        }
                    }
                    if (j >= count) {
                        func_ov010_0216245c(eventData, entry->unk_4);
                    }
                }
            }
        }
        GFL_HeapFree(list);
        GFL_HeapFree(table);
    }
}

void func_ov021_02193abc(FieldWFBC *wfbc, FieldCamera *camera, HeapID heapId) {
    u16 *ids;

    ids = GFL_ArcToolReadHeapNewLZ(wfbc->arc, 0x24, 0, heapId & 0x7fff | 0x8000);
    FieldCamera_SetDefaultsIndex(camera, ids[func_ov021_021938f4(wfbc)]);
    GFL_HeapFree(ids);
}

void func_ov021_02193afc(FieldWFBC *wfbc, MMSys *mmSys) {
    wfbc->unk_140 = mmSys;
}

void func_ov021_02193b04(FieldWFBC *wfbc, CityState *city, BOOL isOther, HeapID heapId) {
    func_ov021_02193c08(wfbc, heapId);
    func_ov021_02193d30(wfbc, city, isOther);
}

void func_ov021_02193b20(FieldWFBC *wfbc, u32 id) {
    func_ov021_02193eb4(wfbc);
    func_ov021_02193f6c(func_ov021_02193efc(wfbc, id));
    func_ov021_02193ee8(wfbc);
}

void func_ov021_02193b40(FieldWFBC *wfbc, u32 id) {
    FieldWFBCPerson *person;

    func_ov021_02193eb4(wfbc);
    person = func_ov021_02193efc(wfbc, id);
    if (wfbc->isOther == FALSE) {
        func_ov020_02186748(&person->resident);
        func_ov021_02193ee8(wfbc);
    }
}

void func_ov021_02193b68(FieldWFBC *wfbc, WordSet *wordSet, u32 id, u32 bufId, HeapID heapId) {
    FieldWFBCPerson *person;
    PlayerInfo *info;
    HeapID heap;

    func_ov021_02193eb4(wfbc);
    person = func_ov021_02193f34(wfbc, id);
    heap = heapId & 0x7fff | 0x8000;
    info = func_0200846c(heap);
    func_020084a0(info);
    copyTrainerName(info, person->resident.name);
    setIDAsUInt(info, person->resident.unk14);
    copyVarForText(wordSet, bufId, info);
    GFL_HeapFree(info);
}

void func_ov021_02193bc4(FieldWFBC *wfbc, u32 id) {
    func_ov021_02193eb4(wfbc);
    func_ov020_02186764(&func_ov021_02193efc(wfbc, id)->resident);
    func_ov021_02193ee8(wfbc);
}

BOOL func_ov021_02193be4(FieldWFBC *wfbc, u32 id) {
    func_ov021_02193eb4(wfbc);
    return func_ov020_02186770(&func_ov021_02193f34(wfbc, id)->resident);
}

CityResidentData *func_ov021_02193c00(FieldWFBCPerson *person) {
    return &person->data;
}

CityResident *func_ov021_02193c04(FieldWFBCPerson *person) {
    return &person->resident;
}

void func_ov021_02193c08(FieldWFBC *wfbc, HeapID heapId) {
    s32 i;

    if (wfbc->dataHandle == NULL) {
        i = 0;
        wfbc->dataHandle = func_ov020_021867c4(0, heapId);
        wfbc->people = GFL_HeapAllocate(heapId, CITY_RESIDENT_MAX * sizeof(FieldWFBCPerson), 1, "field_wfbc.c", 0x39e);
        do {
            func_ov021_02193f6c(&wfbc->people[i]);
            i = i + 1;
        } while (i < CITY_RESIDENT_MAX);
        wfbc->arc = GFL_ArcSysCreateFileHandle(0x9a, heapId);
        wfbc->layout = GFL_HeapAllocate(heapId, 0x600, 1, "field_wfbc.c", 0x3ab);
        wfbc->unk_118 = func_ov020_02186830(wfbc->dataHandle, wfbc->city, heapId);
    }
}

void func_ov021_02193c80(FieldWFBC *wfbc) {
    if (wfbc->dataHandle != NULL) {
        func_ov020_0218683c(wfbc->unk_118);
        GFL_HeapFree(wfbc->layout);
        GFL_ArcToolFree(wfbc->arc);
        func_ov020_02186804(wfbc->dataHandle);
        GFL_HeapFree(wfbc->people);
        wfbc->dataHandle = NULL;
        wfbc->people = NULL;
        wfbc->arc = NULL;
        wfbc->layout = NULL;
        wfbc->unk_118 = NULL;
    }
}

void func_ov021_02193cbc(FieldWFBC *wfbc) {
    s32 i;

    i = 0;
    wfbc->city = 0;
    wfbc->isOther = FALSE;
    wfbc->state = NULL;
    if (wfbc->people != NULL) {
        do {
            func_ov021_02193f6c(&wfbc->people[i]);
            i = i + 1;
        } while (i < CITY_RESIDENT_MAX);
    }
    if (wfbc->layout != NULL) {
        MI_CpuFill8(wfbc->layout, 0, 0x600);
    }
    func_ov021_02193fe8(wfbc->unk_18);
    func_ov021_02194078(&wfbc->unk_11c);
}

s32 func_ov021_02193d08(FieldWFBC *wfbc) {
    s32 i;
    s32 j;
    FieldWFBCPerson *people;

    i = 0;
    people = wfbc->people;
    if (people != NULL) {
        j = 0;
        do {
            if (people[j].unk_00 != 0) {
                i = i + 1;
            }
            j = j + 1;
        } while (j < CITY_RESIDENT_MAX);
    }
    return i;
}

void func_ov021_02193d30(FieldWFBC *work, CityState *city, BOOL isOther) {
    CityResident *visitors;
    s32 i;
    u8 *pos;
    CityResidentData *data;

    func_ov020_0218625c(city);
    func_ov021_02193cbc(work);
    work->city = city->city;
    work->isOther = isOther;
    /* FAKEMATCH(cast): the original computes the id's i * 0x18 again instead of sharing the test's; the (u32)i casts
       stand in for whatever typed that index apart (an accessor's u32 parameter?). The permuter's `visitors` local
       then matched it. */
    for (i = 0; i < CITY_RESIDENT_MAX; i++) {
        if (isOther == FALSE) {
            if (func_ov010_02161ecc(&city->residents[i])) {
                func_ov020_02186818(work->dataHandle, city->residents[(u32)i].id);
                pos = func_ov020_02186844(work->unk_118, i);
                data = func_ov020_0218682c(work->dataHandle);
                func_ov021_02193f80(&work->people[i], &city->residents[i], pos, data);
            }
        } else {
            visitors = &city->visitors[i];
            if (func_ov010_02161ecc(visitors)) {
                func_ov020_02186818(work->dataHandle, city->visitors[(u32)i].id);
                pos = func_ov020_02186844(work->unk_118, i);
                func_ov020_0218682c(work->dataHandle);
                data = func_ov020_0218682c(work->dataHandle);
                func_ov021_02193f80(&work->people[i], &city->visitors[i], pos, data);
            }
        }
    }
    func_ov021_02194084(&work->unk_11c, city, work->dataHandle);
    if (isOther == FALSE) {
        work->state = city;
    } else {
        work->state = NULL;
    }
    GFL_ArcToolGetDataLength(work->arc, data_ov021_021d3f58[work->city]);
    GFL_ArcToolRead(work->arc, data_ov021_021d3f58[work->city], work->layout);
    func_ov021_02194128(&work->unk_11c, work->layout, work->unk_18);
}

void func_ov021_02193e54(FieldWFBC *wfbc, CityState *city) {
    s32 i;

    city->initialized = 1;
    city->city = (u16)wfbc->city;
    if (wfbc->isOther == FALSE) {
        func_ov020_0218609c(city);
        i = 0;
        do {
            func_ov021_02193fd4(&wfbc->people[i], city->residents + i);
            i = i + 1;
        } while (i < CITY_RESIDENT_MAX);
    } else {
        func_ov020_021860b8(city);
        i = 0;
        do {
            func_ov021_02193fd4(&wfbc->people[i], city->visitors + i);
            i = i + 1;
        } while (i < CITY_RESIDENT_MAX);
    }
}

void func_ov021_02193eb4(FieldWFBC *wfbc) {
    s32 i;

    if (wfbc->isOther == FALSE && wfbc->state != NULL) {
        i = 0;
        do {
            func_ov021_02193fbc(&wfbc->people[i], &wfbc->state->residents[i]);
            i = i + 1;
        } while (i < CITY_RESIDENT_MAX);
    }
}

void func_ov021_02193ee8(FieldWFBC *wfbc) {
    if (wfbc->isOther == FALSE) {
        if (wfbc->state != NULL) {
            func_ov021_021938fc(wfbc, wfbc->state);
        }
    }
}

FieldWFBCPerson *func_ov021_02193efc(FieldWFBC *wfbc, u32 id) {
    s32 i;

    if (wfbc->people != NULL) {
        i = 0;
        do {
            if (wfbc->people[i].unk_00 != 0) {
                if (func_ov020_021867b0(&wfbc->people[i].resident) == id) {
                    return &wfbc->people[i];
                }
            }
            i = i + 1;
        } while (i < CITY_RESIDENT_MAX);
    }
    return NULL;
}
