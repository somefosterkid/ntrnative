#include "global.h"

#include "pokemon.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E64D0(BoxPokemon *arg0);
s32 ov14_021F2ED0(void *, u32, u32, s32);

s32 ov14_021E6070(void *, u32, s32, void *);
BoxPokemon *ov14_021E60C0(void *, u32, u32);
s32 ov14_021E7588(void *, u32);
void ov14_021E6548(void *arg0, u32 arg1, u32 arg2);

s32 ov14_021E64D0(BoxPokemon *boxMon) {
    u16 v0;
    u16 v1;
    u16 v2;

    v0 = (u16)GetBoxMonData(boxMon, 5, NULL);
    if (v0 == 0x1ED) {
        v1 = (u16)GetBoxMonData(boxMon, 0x70, NULL);
        BoxMon_UpdateArceusForm(boxMon);
        if (v1 != (u16)GetBoxMonData(boxMon, 0x70, NULL)) {
            return 1;
        }
    } else {
        if (v0 == 0x1E7) {
            v2 = (u16)GetBoxMonData(boxMon, 0x70, NULL);
            BoxMon_UpdateGiratinaForm(boxMon);
            if (v2 != (u16)GetBoxMonData(boxMon, 0x70, NULL)) {
                return 1;
            }
        }
    }
    return 0;
}

void ov14_021E6548(void *arg0, u32 arg1, u32 arg2) {
    u32 v0;

    v0 = arg1;
    if (((v0 >= 0x1EU) || (arg2 >= 0x1EU)) && ((v0 < 0x1EU) || (arg2 < 0x1EU))) {
        if (v0 >= 0x1EU) {
            v0 = arg2;
        }
        if ((ov14_021E6070(arg0, v0, 5, 0) == 0x1EC) && (ov14_021E6070(arg0, v0, 0x70, 0) != 0)) {
            BoxMon_UpdateShayminForm(ov14_021E60C0(arg0, ((UnkStruct_ov14_boxapp *)arg0)->unk1F, v0), 0);
            ov14_021F2ED0(arg0, ((UnkStruct_ov14_boxapp *)arg0)->unk1F, v0, ((UnkStruct_ov14_boxapp *)arg0)->sys->unk_4094[v0]);
            if (((UnkStruct_ov14_boxapp *)arg0)->unk21 == v0) {
                ov14_021E7588(arg0, v0);
            }
        }
    }
}
