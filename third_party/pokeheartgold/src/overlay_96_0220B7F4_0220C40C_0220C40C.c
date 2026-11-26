#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "sprite_system.h"
#include "overlay_96_unk_struct_ov96_0220_be28.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov96_0220C004(void *, void *);
#else
s32 ov96_0220C004(void *, void *);
#endif

s32 ov96_0220C0DC(void *, void *);

u32 ov96_0220C3E0(void *);

typedef struct {
    u32 state : 2;
    u32 unk_2 : 12;
    u32 lock : 1;
    u32 unk_15 : 8;
    u32 cur : 2;
    u32 idx : 2;
    u32 unk_27 : 5;
} UnkStruct_ov96_0220C40C_Bits38;

typedef struct {
    u32 unk_0 : 24;
    u32 val : 2;
    u32 unk_26 : 6;
} UnkStruct_ov96_0220C40C_Entry;

void ov96_0220C40C(void *arg0);

void ov96_0220C40C(void *arg0) {
    UnkStruct_ov96_0220C40C_Entry *data;

    u32 idx;
    data = (UnkStruct_ov96_0220C40C_Entry *)ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(*((PokeathlonCourseData **)arg0)) + 0xF0);
    idx = ((UnkStruct_ov96_0220C40C_Bits38 *)(((u8 *)arg0) + 0x38))->idx;
    if (((UnkStruct_ov96_0220C40C_Bits38 *)(((u8 *)arg0) + 0x38))->cur == data[idx].val) {
        ov96_0220C004(arg0, &data[idx]);
    } else {
        ManagedSprite_SetDrawFlag(((UnkStruct_ov96_0220BE28 *)arg0)->spr24, 0);
        ManagedSprite_SetDrawFlag(((UnkStruct_ov96_0220BE28 *)arg0)->spr28, 0);
    }
    ov96_0220C0DC(arg0, &data[idx]);
    if (((UnkStruct_ov96_0220C40C_Bits38 *)(((u8 *)arg0) + 0x38))->lock == 0 && ((UnkStruct_ov96_0220C40C_Bits38 *)(((u8 *)arg0) + 0x38))->cur != data[idx].val) {
        if (data[idx].val >= 3) {
            GF_AssertFail();
        }
        ((UnkStruct_ov96_0220C40C_Bits38 *)(((u8 *)arg0) + 0x38))->state = data[idx].val;
        ov96_0220C3E0(arg0);
    }
}
