#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "mail.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

void ov103_021EE210(MailBoxWork *work);

void ov103_021EE210(MailBoxWork *work) {
    NNSG2dCharacterData *sp14;
    u16 *new_var;
    u16 sp10;
    void *spC;
    u32 sp8;
    u32 sp4;
    Mail *temp_r7;
    s32 var_r6;
    u32 temp_r0;
    unsigned int var_r4;
    void *temp_r0_2;
    new_var = &sp10;
    var_r4 = 0;
    temp_r7 = work->view->mails[work->mailIndex];
    var_r6 = 0;
    do {
        *new_var = sub_0202B404(temp_r7, var_r4, 2U, sub_0202B4E4(temp_r7));
        if (((*new_var) == 0xFFFF) || (temp_r0 = ((u32)((*new_var) << 0x14)) >> 0x14, sp4 = temp_r0, temp_r0 == 7)) {
            ov103_021EE0CC(work->view, var_r4 + 4, 0);
        } else {
            sp8 = NNS_G2dGetImageLocation(Sprite_GetImageProxy(work->view->sprites[var_r6 + 4]->sprite), NNS_G2D_VRAM_TYPE_2DSUB);
            temp_r0_2 = GfGfxLoader_GetCharDataFromOpenNarc(work->view->iconNarc, (s32)sp4, 0, &sp14, HEAP_ID_157);
            spC = temp_r0_2;
            DC_FlushRange(sp14->pRawData, 0x200U);
            GXS_LoadOBJ(sp14->pRawData, sp8, 0x200U);
            Heap_Free(temp_r0_2);
            ManagedSprite_SetPaletteOverride(work->view->sprites[var_r6 + 4], (s32)(((u32)(sp10 << 0x10)) >> 0x1C));
            ov103_021EE0CC(work->view, var_r4 + 4, 1);
        }
        var_r4 += 1;
        var_r6 += 1;
    } while (((u32)var_r4) < 3U);
}
