#include "global.h"

#include "overlay_96_unk_struct_ov96_021_f7934.h"
#include "unk_02005D10.h"

void ov96_021F8128(UnkStruct_ov96_021F8128 *, s32, s32);

void ov96_021F8160(UnkStruct_ov96_021F8094 *, s32, s32);

s32 ov96_021F8180(UnkStruct_ov96_021F8128 *, s32);

void ov96_021F81C0(UnkStruct_ov96_021F8094 *, u32);

void ov96_021F82E4(UnkStruct_ov96_021F8094 *, void *, s32);

void ov96_021F8334(UnkStruct_ov96_021F8094 *, void *, s32);

void ov96_021F8354(UnkStruct_ov96_021F8128 *);

BOOL ov96_021F8360(UnkStruct_ov96_021F8094 *);

void *ov96_021F8378(UnkStruct_ov96_021F8094 *, void *, s32, s32);

void ov96_021F83BC(UnkStruct_ov96_021F8094 *, UnkStruct_ov96_021F8128 *);

UnkStruct_ov96_021F8128 *ov96_021F83D0(UnkStruct_ov96_021F8094 *);

u16 ov96_021F8978(void *);

BOOL ov96_021F81CC(UnkStruct_ov96_021F8094 *data, void *arg1);

BOOL ov96_021F81CC(UnkStruct_ov96_021F8094 *data, void *arg1) {
    s32 i;
    UnkStruct_ov96_021F8128 *p;
    u8 *state;
    UnkStruct_ov96_021F8128 *obj;

    state = &data->state;
    switch (*state) {
    case 0:
        ov96_021F8128(&data->extra, -0x50, 0);
        if (ov96_021F8180(&data->extra, 0) != 0) {
            ov96_021F82E4(data, data->extra.sprite, 0);
            ov96_021F8354(&data->extra);
            if (ov96_021F8360(data) != 0) {
                ov96_021F8334(data, ov96_021F8378(data, arg1, 0x100, 0x28), 0);
            }
            ov96_021F81C0(data, 0xA);
        }
        break;
    case 1:
        ov96_021F8160(data, -0x50, 0);
        for (i = 0, p = data->slots; i < 4; i++, p++) {
            if (ov96_021F8180(p, 0) == 0) {
                break;
            }
            if (data->fanfarePlayed == 0 && ov96_021F8978(p->sprite) == 0) {
                PlayFanfare(0x4B9);
                data->fanfarePlayed = 1;
            }
        }
        if (i == 4) {
            ov96_021F81C0(data, 0xA);
        }
        break;
    case 2:
        ov96_021F8160(data, 0, 8);
        obj = ov96_021F83D0(data);
        if (ov96_021F8180(obj, 1) != 0) {
            ov96_021F83BC(data, obj);
            *state = 0;
        }
        break;
    case 3:
        if ((data->holdFrames)-- <= 0) {
            if (ov96_021F8360(data) != 0) {
                *state = 2;
            } else {
                return TRUE;
            }
        }
        break;
    }
    return FALSE;
}
