#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "heap.h"
#include "party.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "unk_020932E0.h"

void sub_02094528(UnkStruct_020932E0 *arg0) {
    UnkStruct_020932E0_Info sp28;
    void *sp24;
    NARC *sp20;
    s32 sp1C;

    s32 var_r4;
    u8 temp_r0;
    UnkStruct_020932E0_Info *var_r2;

    sp20 = NARC_New(NARC_poketool_icongra_poke_icon, arg0->unk_4);
    sp24 = Heap_AllocAtEnd(arg0->unk_4, 0x1000U);
    temp_r0 = (u8)Party_GetCount(arg0->unk_4640);
    var_r4 = 0;
    sp1C = (s32)temp_r0;
    if ((s32)temp_r0 > 0) {
        do {
            sub_02094F14(arg0, var_r4, &sp28);
            var_r2 = &arg0->unk_198[var_r4].unk_4;
            *var_r2 = sp28;
            sub_02094C08(&sp28, arg0->unk_8D4, var_r4, arg0->unk_198[var_r4].unk_0, sp24, sp20, 0x40);
            if (sp28.unk_8 != 0) {
                Sprite_SetOamMode(arg0->unk_198[var_r4].unk_0, GX_OAM_MODE_XLU);
            } else {
                Sprite_SetOamMode(arg0->unk_198[var_r4].unk_0, GX_OAM_MODE_NORMAL);
            }
            var_r4 += 1;
        } while (var_r4 < sp1C);
    }
    sp28.unk_0 = 0;
    sp28.unk_8 = 0U;
    sp28.unk_A = 0;
    sp28.unk_C = 0;
    sp28.unk_E = 0;
    if (var_r4 < 0x1E) {
        do {
            sub_02094C08(&sp28, arg0->unk_8D4, var_r4, NULL, sp24, sp20, 0x40);
            Sprite_SetDrawFlag(arg0->unk_198[var_r4].unk_0, 0);
            var_r4 += 1;
        } while (var_r4 < 0x1E);
    }
    Heap_Free(sp24);
    NARC_Delete(sp20);
    arg0->unk_4644 = sub_02094758;
    sub_02094C88(arg0, arg0->unk_F);
    sub_02094EB4(arg0);
    CopyRectToBgTilemapRect(arg0->unk_0, 2U, 0U, 0U, 0x14U, 0x14U, arg0->unk_46D0->rawData, 0U, 0U, 0x14U, 0x14U);
    ScheduleBgTilemapBufferTransfer(arg0->unk_0, 2U);
}
