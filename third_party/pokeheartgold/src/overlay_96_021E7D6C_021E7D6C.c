#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_save.h"

void ov96_021E7F48(PokeathlonCourseData *param_1);

typedef struct {
    u16 b0 : 1;
    u16 b1 : 1;
    u16 rest : 14;
} Flags16;

void ov96_021E7D6C(PokeathlonCourseData *param_1, u8 *param_2);

void ov96_021E7D6C(PokeathlonCourseData *param_1, u8 *param_2) {
    u8 uVar1;
    u32 uVar2;
    s32 iVar3;
    PokeathlonFieldData *fieldData = param_1->fieldData;

    uVar1 = ov96_021E5F24(param_1);
    if (param_1->args->mode == 1) {
        ov96_021E7F98(1, 9999999, (Pokeathlon_UnkSubStruct_B00 *)(param_2 + 0x38));
        if (((Flags16 *)(fieldData[4].data + 0x50))->b0) {
            ov96_021E7F98(1, 9999999, (Pokeathlon_UnkSubStruct_B00 *)(param_2 + 0x3c));
            ov96_021E7F48(param_1);
        } else if (((Flags16 *)(fieldData[4].data + 0x50))->b1) {
            ov96_021E7F98(1, 9999999, (Pokeathlon_UnkSubStruct_B00 *)(param_2 + 0x40));
        }
    } else {
        ov96_021E7F98(1, 9999999, (Pokeathlon_UnkSubStruct_B00 *)(param_2 + 4));
        if (((Flags16 *)(fieldData[4].data + 0x50))->b0) {
            ov96_021E7F98(1, 9999999, (Pokeathlon_UnkSubStruct_B00 *)(param_2 + 8));
            ov96_021E7F48(param_1);
        } else if (((Flags16 *)(fieldData[4].data + 0x50))->b1) {
            ov96_021E7F98(1, 9999999, (Pokeathlon_UnkSubStruct_B00 *)(param_2 + 0xc));
        }
    }
    uVar2 = 0;
    if (uVar2 < param_1->maxParticipants) {
        do {
            if (fieldData[4].data[uVar2 + 0x48] != 0) {
                ov96_021E7F98(1, 9999999, (Pokeathlon_UnkSubStruct_B00 *)(param_2 + 0x44 + ((u8)(param_1->field_3D8[uVar2])) * 4));
            } else {
                if (fieldData[4].data[uVar2 + 0x4c] != 0) {
                    ov96_021E7F98(1, 9999999, (Pokeathlon_UnkSubStruct_B00 *)(param_2 + 0x6c));
                }
            }
            uVar2 = uVar2 + 1 & 0xff;
        } while (uVar2 < param_1->maxParticipants);
    }
    uVar2 = 0;
    do {
        iVar3 = uVar2 * 0x20;
        ov96_021E7F98(*(s32 *)(fieldData[uVar1].data + iVar3), 9999999, (Pokeathlon_UnkSubStruct_B00 *)(param_2 + 0x18));
        ov96_021E7F98(*(s32 *)(fieldData[uVar1].data + iVar3 + 4), 9999999, (Pokeathlon_UnkSubStruct_B00 *)(param_2 + 0x1c));
        ov96_021E7F98(*(s32 *)(fieldData[uVar1].data + iVar3 + 8), 9999999, (Pokeathlon_UnkSubStruct_B00 *)(param_2 + 0x20));
        ov96_021E7F98(*(s32 *)(fieldData[uVar1].data + iVar3 + 0xc), 9999999, (Pokeathlon_UnkSubStruct_B00 *)(param_2 + 0x24));
        ov96_021E7F98(*(s32 *)(fieldData[uVar1].data + iVar3 + 0x10), 9999999, (Pokeathlon_UnkSubStruct_B00 *)(param_2 + 0x28));
        ov96_021E7F98(*(s32 *)(fieldData[uVar1].data + iVar3 + 0x14), 9999999, (Pokeathlon_UnkSubStruct_B00 *)(param_2 + 0x2c));
        ov96_021E7F98(*(s32 *)(fieldData[uVar1].data + iVar3 + 0x18), 9999999, (Pokeathlon_UnkSubStruct_B00 *)(param_2 + 0x30));
        ov96_021E7F98(*(s32 *)(fieldData[uVar1].data + iVar3 + 0x1c), 9999999, (Pokeathlon_UnkSubStruct_B00 *)(param_2 + 0x34));
        uVar2 = uVar2 + 1 & 0xff;
    } while (uVar2 < 3);
    ov96_021E7F98(*(s32 *)fieldData[4].data, 9999999, (Pokeathlon_UnkSubStruct_B00 *)(param_2 + 0x10));
    ov96_021E7F98(*(s32 *)(fieldData[4].data + 4), 9999999, (Pokeathlon_UnkSubStruct_B00 *)(param_2 + 0x14));
    return;
}
