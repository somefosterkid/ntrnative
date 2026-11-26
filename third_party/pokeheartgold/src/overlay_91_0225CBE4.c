#include "global.h"

#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_00_thumb.h"
#include "save.h"
#include "system.h"
#include "unk_02035900.h"
#include "vram_transfer_manager.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_90_unk_struct_ov114_0225c9a8.h"
#include "overlay_90_unk_struct_ov114_0225c76c.h"
#include "overlay_91_unk_struct_ov66_02230dbc.h"
#include "overlay_91_unk_struct_ov115_02260440.h"

// ov91-93 view of the Wi-Fi Plaza launch record: 0x34 here (embedded by value), the header copy is 0x3c

s32 ov90_022588A4(void *, s32);

void *ov90_022588CC(void *, s32);

void ov91_0225CBE4(void *arg0, enum HeapID arg1);

struct PlayerProfile;
struct UnkStruct_ov114_0225D678_t;
struct UnkStruct_ov114_0225E854_t;
struct UnkStruct_ov115_0226095C_t;
struct UnkStruct_ov115_02260BE4_t;
struct UnkStruct_ov115_02260D78_t;
struct UnkStruct_ov66_0222DFF8_t;
typedef struct UnkStruct_ov115_0226095C_t UnkStruct_ov115_0226095C;
typedef struct UnkStruct_ov114_0225D678_t UnkStruct_ov114_0225D678;
typedef struct UnkStruct_ov114_0225E854_t UnkStruct_ov114_0225E854;
typedef struct UnkStruct_ov115_02260BE4_t UnkStruct_ov115_02260BE4;
typedef struct {
    s32 unk_00;
    s32 unk_04;
    s32 unk_08;
    s32 unk_0C;
    s32 unk_10;
    s32 unk_14;
} UnkStruct_ov115_02262DC4;
typedef struct {
    u32 unk_00;
    UnkStruct_ov115_02262DC4 *unk_04;
    const PlayerProfile *unk_08[4];
    u32 unk_18[4];
} UnkStruct_ov115_02262F50;
typedef struct UnkStruct_ov115_02260D78_t UnkStruct_ov115_02260D78;
struct UnkStruct_ov115_0226095C_t {
    UnkStruct_ov114_0225D678 *unk_00;
    UnkStruct_ov114_0225E854 *unk_04;
    UnkStruct_ov114_0225C76C unk_08;
    UnkStruct_ov114_0225C9A8 unk_18;
    UnkStruct_ov115_02260BE4 *unk_30;
    UnkStruct_ov115_02260D78 *unk_34;
    BOOL unk_38;
    BOOL unk_3C;
    BOOL unk_40;
    u16 unk_44;
    u16 unk_46;
    UnkStruct_ov115_02262F50 unk_48;
    u32 unk_70;
    u32 unk_74;
    u32 unk_78;
    u32 unk_7C;
    BOOL unk_80;
    u8 unk_84[4];
};
void ov90_02258938(UnkStruct_ov114_0225D678 *param0);
void ov90_02258A04(UnkStruct_ov114_0225E854 *param0);
void ov91_0225CCEC(UnkStruct_ov115_02260BE4 *param0);
void ov91_0225CE80(UnkStruct_ov115_02260D78 *param0);
void ov91_0225CC38(UnkStruct_ov115_0226095C *param0);
void ov91_0225CC4C(UnkStruct_ov115_0226095C *param0, UnkStruct_ov115_02260440 *param1);
BOOL ov91_0225CCA8(UnkStruct_ov115_0226095C *param0, int param1, const void *param2, int param3);

void ov91_0225CBE4(void *arg0, enum HeapID heapID) {
    u32 sizeOut;
    s32 v1;
    void *v0;
    void *v2;

    *(void **)((u8 *)arg0 + 0x4C) = GfGfxLoader_LoadFromNarc_GetSizeOut(NARC_a_2_1_0, 0, 0, heapID, 0, &sizeOut);
    *(u32 *)((u8 *)arg0 + 0x48) = (u32)(sizeOut / 24U);
    v1 = 0;
    if ((s32) * (u8 *)((u8 *)arg0 + 0x10) > 0) {
        v2 = arg0;
        v0 = arg0 + 8;
        do {
            *(void **)((u8 *)v2 + 0x50) = ov90_022588CC(v0, v1);
            *(s32 *)((u8 *)v2 + 0x60) = ov90_022588A4(v0, v1);
            v1 += 1;
            v2 += 4;
        } while (v1 < (s32) * (u8 *)((u8 *)arg0 + 0x10));
    }
}

void ov91_0225CC38(UnkStruct_ov115_0226095C *param0) {
    Heap_Free(param0->unk_48.unk_04);
    param0->unk_48.unk_04 = 0L;
    param0->unk_48.unk_00 = 0;
}

void ov91_0225CC4C(UnkStruct_ov115_0226095C *param0, UnkStruct_ov115_02260440 *param1) {
    Main_SetVBlankIntrCB(0L, 0L);
    HBlankInterruptDisable();

    if (param0->unk_00 != 0L) {
        ov90_02258938(param0->unk_00);
        param0->unk_00 = 0L;
    }

    if (param0->unk_34) {
        if (param0->unk_44 == 0) {
            ov91_0225CCEC(param0->unk_30);
        }

        ov91_0225CE80(param0->unk_34);
        GF_DestroyVramTransferManager();
    }

    if (param1->unk_38) {
        ov00_021E6A4C();
    }

    if (param0->unk_04 != 0L) {
        ov90_02258A04(param0->unk_04);
        param0->unk_04 = 0L;
    }
}

BOOL ov91_0225CCA8(UnkStruct_ov115_0226095C *param0, int param1, const void *param2, int param3) {
    if (param0->unk_80 == 0) {
        return 0;
    }

    return sub_02037030(param1, param2, param3);
}
