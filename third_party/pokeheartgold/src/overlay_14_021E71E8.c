#include "global.h"

#include "heap.h"
#include "pokemon.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

BoxPokemon *ov14_021E60C0(void *, u32, u32);
void ov14_021E71E8(void *arg0);
void ov14_021E7264(u8 *param_1);

void ov14_021E71E8(void *arg0) {
    BoxPokemon *boxMon;
    s32 v1;
    u32 v3;
    UnkStruct_ov14_boxsys *v0;
    UnkStruct_ov14_boxscan *v2;

    ((UnkStruct_ov14_boxapp *)arg0)->sys->unk_88DC = Heap_AllocAtEnd(HEAP_ID_10, sizeof(UnkStruct_ov14_boxscan));
    v0 = ((UnkStruct_ov14_boxapp *)arg0)->sys;
    v2 = v0->unk_88DC;
    v2->sprite = v0->sprites[v0->unk_4094[((UnkStruct_ov14_boxapp *)arg0)->unk21]];
    v2->pos = 0;
    v2->moveFlags = 0U;
    boxMon = ov14_021E60C0(arg0, ((UnkStruct_ov14_boxapp *)arg0)->unk1F, ((UnkStruct_ov14_boxapp *)arg0)->unk21);
    v3 = 0;
loop_1:
    v1 = ov14_021E71C8((u16)GetBoxMonData(boxMon, v3 + 0x36, NULL));
    if (v1 != -1) {
        v2->moveFlags = (u8)(v2->moveFlags | (1 << v1));
        return;
    }
    v3 += 1;
    if (v3 >= 4U) {
        return;
    }
    goto loop_1;
}

void ov14_021E7264(u8 *param_1) {
    Heap_Free(((UnkStruct_ov14_boxapp *)param_1)->sys->unk_88DC);
    return;
}
