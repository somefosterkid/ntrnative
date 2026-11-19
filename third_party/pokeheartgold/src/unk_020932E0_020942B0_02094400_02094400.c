#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "heap.h"
#include "sprite.h"
#include "unk_020932E0.h"

void sub_02094400(UnkStruct_020932E0 *arg0) {
    void *buf;
    NARC *narc;
    s32 i;
    UnkStruct_020932E0_Info data;

    narc = NARC_New(NARC_poketool_icongra_poke_icon, arg0->unk_4);
    buf = Heap_AllocAtEnd(arg0->unk_4, 0x1000);

    for (i = 0; i < 30; i++) {
        Sprite_SetOamMode(arg0->unk_198[i].unk_0, GX_OAM_MODE_NORMAL);
        if (arg0->unk_4648(arg0->unk_463C, arg0->unk_F, i, &data) != 0) {
            arg0->unk_198[i].unk_4 = data;
            sub_02094C08(&data, arg0->unk_8D4, i, arg0->unk_198[i].unk_0, buf, narc, 0x40);
            if (data.unk_8 != 0) {
                Sprite_SetOamMode(arg0->unk_198[i].unk_0, GX_OAM_MODE_XLU);
            } else {
                Sprite_SetOamMode(arg0->unk_198[i].unk_0, GX_OAM_MODE_NORMAL);
            }
        } else {
            sub_02094C08(&data, arg0->unk_8D4, i, NULL, buf, narc, 0x40);
            Sprite_SetDrawFlag(arg0->unk_198[i].unk_0, FALSE);
        }
    }
    Heap_Free(buf);
    NARC_Delete(narc);
    arg0->unk_4644 = sub_02094758;
    sub_02094C88(arg0, arg0->unk_F);
    sub_02094EB4(arg0);
    CopyRectToBgTilemapRect(arg0->unk_0, 2, 0, 0, 0x14, 0x14, arg0->unk_46D4->rawData, 0, 0, 0x20, 0x18);
    ScheduleBgTilemapBufferTransfer(arg0->unk_0, 2);
}
