// Black's code at ov021 0x02193f34-0x02194284. No name is known: the module and address.

#include "types.h"
#include "field/city_state.h"
#include "gfl/std.h"
#include "nitro/math.h"
#include "field/field_wfbc.h"
#include "field/field_wfbc_data_local.h"

// The record of the resident of an id, or NULL
FieldWFBCPerson *func_ov021_02193f34(FieldWFBC *wfbc, u32 id) {
    u8 result;
    s32 i;

    if (wfbc->people != NULL) {
        i = 0;
        do {
            if (wfbc->people[i].unk_00 != 0 && (result = func_ov020_021867b0(&wfbc->people[i].resident), id == result)) {
                return &wfbc->people[i];
            }
            i = i + 1;
        } while (i < 10);
    }
    return NULL;
}

void func_ov021_02193f6c(FieldWFBCPerson *person) {
    MI_CpuFill8(person, 0, sizeof(FieldWFBCPerson));
    person->unk_00 = 0;
}

void func_ov021_02193f80(FieldWFBCPerson *person, CityResident *resident, u8 *pos, CityResidentData *data) {
    func_ov010_02161ecc(resident);
    person->unk_00 = 1;
    MI_CpuCopy8(resident, &person->resident, sizeof(CityResident));
    MI_CpuCopy8(data, &person->data, sizeof(CityResidentData));
    person->unk_4c = (u16)*pos;
    person->unk_4e = (u16)pos[1];
}

void func_ov021_02193fbc(FieldWFBCPerson *person, CityResident *resident) {
    if (person->unk_00 == 1) {
        MI_CpuCopy8(resident, &person->resident, sizeof(CityResident));
    }
}

void func_ov021_02193fd4(FieldWFBCPerson *person, CityResident *resident) {
    if (person->unk_00 == 1) {
        MI_CpuCopy8(&person->resident, resident, sizeof(CityResident));
    }
}

void func_ov021_02193fe8(Ov021Cell (*grid)[8]) {
    s32 i;
    s32 j;
    Ov021Cell *cell;

    i = 0;
    do {
        j = 0;
        do {
            cell = &grid[i][j];
            cell->entry.unk1 = 0xff;
            j = j + 1;
            cell->unk2 = 0xffff;
        } while (j < 8);
        i = i + 1;
    } while (i < 8);
}

void func_ov021_02194014(Ov021Cell (*param_1)[8], u32 param_2, u32 param_3, Ov021Entry param_4, u16 param_5) {
    param_1[param_3][param_2].entry = param_4;
    param_1[param_3][param_2].unk2 = param_5;
}

Ov021Cell func_ov021_02194038(Ov021Cell (*grid)[8], u32 x, u32 z) {
    return grid[z][x];
}

BOOL func_ov021_02194044(Ov021Cell (*grid)[8], u32 param_2, u32 param_3) {
    s32 i;
    s32 j;
    Ov021Cell *cell;

    i = 0;
    do {
        j = 0;
        do {
            cell = &grid[i][j];
            if (param_2 == cell->entry.unk1 && param_3 == cell->unk2) {
                return TRUE;
            }
            j = j + 1;
        } while (j < 8);
        i = i + 1;
    } while (i < 8);
    return FALSE;
}

void func_ov021_02194078(Ov021Random *random) {
    MI_CpuFill8(random, 0, sizeof(Ov021Random));
}

void func_ov021_02194084(Ov021Random *param_1, CityState *param_2, CityResidentDataHandle *param_3) {
    s32 i;
    s32 count;
    u32 dataSum;
    u32 idSum;
    CityResidentData *data;

    dataSum = 0;
    idSum = 0;
    for (i = 0; i < CITY_RESIDENT_MAX; i++) {
        if (func_ov010_02161ecc(&param_2->residents[i])) {
            func_ov020_02186818(param_3, param_2->residents[i].id);
            data = func_ov020_0218682c(param_3);
            dataSum += data->unk2E;
            idSum += param_2->residents[i].id;
        }
    }
    count = func_ov020_02186278(param_2, 0);
    param_1->unk1C = count;
    MATH_InitRand32(&param_1->rand, param_2->unk4);
    if (count == 0) {
        param_1->unk18 = 1;
    } else {
        param_1->unk18 = dataSum + 2;
    }
    param_1->unk1A = idSum;
    param_1->unk20 = param_2->unk4;
}

static inline u8 func_ov021_02194128_parity(u8 seed, u32 x, u32 y) {
    u8 pos = x + y;
    u8 quotient = (u8)(seed + pos) / ((x + 3) + (y + 1));

    return quotient % 2;
}

void func_ov021_02194128(Ov021Random *param_1, Ov021Layout *param_2, Ov021Cell (*param_3)[8]) {
    s32 y;
    s32 x;
    Ov021Entry entry;
    u16 value;

    MATH_InitRand32(&param_1->rand, param_1->unk20);
    for (y = 0; y < 8; y++) {
        for (x = 0; x < 8; x++) {
            entry = param_2->unk0[y][x];
            if (entry.unk1 != 0xff) {
                if (entry.unk0_0 != 0 || entry.unk0_4 != 0) {
                    value = param_3[y - entry.unk0_4][x - entry.unk0_0].unk2;
                    func_ov021_02194014(param_3, x, y, entry, value);
                } else {
                    u32 dx = x - entry.unk0_0;
                    u32 dy = y - entry.unk0_4;
                    u8 parity = func_ov021_02194128_parity(param_1->unk1A, dx, dy);
                    s32 index = param_1->unk1C * 8 / 10;

                    index += MATH_Rand32(&param_1->rand, param_1->unk18);
                    if (index >= 8) {
                        index = 7;
                    }
                    index += parity * 8;
                    value = param_2->unk80[entry.unk1][index];
                    func_ov021_02194014(param_3, x, y, entry, value);
                }
            }
        }
    }
}


