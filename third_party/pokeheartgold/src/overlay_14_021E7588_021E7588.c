#include "global.h"

#include "pokemon.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

BoxPokemon *ov14_021E60C0(void *, u32, s32);
s32 ov14_021E765C(void *);
s32 ov14_021E7588(void *arg0, s32 arg1);

s32 ov14_021E7588(void *arg0, s32 arg1) {
    UnkStruct_ov14_mon *temp_r0_2;
    BoxPokemon *temp_r0;
    s32 temp_r7;
    temp_r0 = ov14_021E60C0(arg0, ((UnkStruct_ov14_boxapp *)arg0)->unk1F, arg1);
    if (temp_r0 != 0L) {
        temp_r7 = AcquireBoxMonLock(temp_r0);
        temp_r0_2 = ov14_021E7358(temp_r0);
        if (temp_r0_2 != NULL) {
            if ((*((s32 *)(((u8 *)(*((void **)arg0))) + 8))) == 3) {
                ov14_021E74F0(arg0, temp_r0_2);
            } else {
                ov14_021E7470(arg0, temp_r0_2);
            }
            ov14_021E7468(temp_r0_2);
        } else {
            ov14_021E765C(arg0);
            ReleaseBoxMonLock(temp_r0, temp_r7);
            return 0;
        }
        ReleaseBoxMonLock(temp_r0, temp_r7);
    } else {
        ov14_021E765C(arg0);
        return 0;
    }
    return 1;
}
