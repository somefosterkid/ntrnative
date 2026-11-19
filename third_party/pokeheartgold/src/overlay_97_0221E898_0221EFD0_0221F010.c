#include "global.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "bg_window.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "sprite_transfer.h"
#include "unk_0200A090.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "overlay_97_course_sub_screen.h"

void ov97_0221F010(Ov97CourseSubScreen *param_1);

void ov97_0221F010(Ov97CourseSubScreen *param_1) {
  if (param_1->sprites.spriteList != NULL) {
    SpriteList_RenderAndAnimateSprites(param_1->sprites.spriteList);
  }
  return;
}

void ov97_0221F0E0(Ov97CourseSubScreenSprites *arg0);

void ov97_0221F020(Ov97CourseSubScreen *arg0);

void ov97_0221F020(Ov97CourseSubScreen *arg0) {
    u8 var_r4;
    u8 var_r4_2;
    Ov97CourseSubScreenSprites *temp_r5;

    DestroyMsgData(arg0->msgData);
    MessageFormat_Delete(arg0->msgFormat);
    String_Delete(arg0->string);
    RemoveWindow(&arg0->windows[0]);
    RemoveWindow(&arg0->windows[1]);
    RemoveWindow(&arg0->windows[2]);
    RemoveWindow(&arg0->windows[3]);
    RemoveWindow(&arg0->windows[4]);
    RemoveWindow(&arg0->windows[5]);
    FreeBgTilemapBuffer(arg0->bgConfig, 4U);
    FreeBgTilemapBuffer(arg0->bgConfig, 5U);
    FreeBgTilemapBuffer(arg0->bgConfig, 6U);
    FreeBgTilemapBuffer(arg0->bgConfig, 7U);
    temp_r5 = &arg0->sprites;
    ov97_0221F0E0(temp_r5);
    var_r4 = 0;
    do {
        SpriteTransfer_DeleteCharTransferTask(temp_r5->resObj[var_r4][0]);
        SpriteTransfer_DeletePlttTransferTask(temp_r5->resObj[var_r4][1]);
        var_r4 += 1;
    } while ((u32) var_r4 < 3U);
    var_r4_2 = 0;
    do {
        Destroy2DGfxResObjMan(temp_r5->resMan[var_r4_2]);
        var_r4_2 += 1;
    } while ((u32) var_r4_2 < 6U);
    SpriteList_Delete(temp_r5->spriteList);
    Heap_Free(arg0);
}

void ov97_0221F0E0(Ov97CourseSubScreenSprites *arg0) {
    s32 i;
    s32 j;
    s32 k;

    i = 0;
    do {
        Sprite_Delete(arg0->unk_1B4[i]);
        i += 1;
    } while (i < 5);
    k = 0;
    do {
        Sprite_Delete(arg0->statRows[k].arrow);
        j = 0;
loop_4:
        Sprite_Delete(arg0->statRows[k].marks[j]);
        j += 1;
        if (j < 5) {
            goto loop_4;
        }
        k += 1;
    } while (k < 5);
    Sprite_Delete(arg0->monSprite);
    Heap_Free(arg0->charData);
}

void ov97_0221F1D8(BgConfig * param_1);

void ov97_0221F14C(BgConfig *param_1, enum HeapID param_2);

void ov97_0221F14C(BgConfig *param_1, enum HeapID param_2) {
  ov97_0221F1D8(param_1);
  GfGfxLoader_GXLoadPal(NARC_a_2_4_1,0,GF_PAL_LOCATION_SUB_BG,GF_PAL_SLOT_0_OFFSET,0,param_2);
  GfGfxLoader_LoadCharData(NARC_a_2_4_1,1,param_1,GF_BG_LYR_SUB_3,0,0,1,param_2);
  GfGfxLoader_LoadScrnData(NARC_a_2_4_1,2,param_1,GF_BG_LYR_SUB_3,0,0x600,1,param_2);
  BgCommitTilemapBufferToVram(param_1,7);
  GfGfxLoader_LoadScrnData(NARC_a_2_4_1,3,param_1,GF_BG_LYR_SUB_2,0,0x600,1,param_2);
  BgCommitTilemapBufferToVram(param_1,6);
  LoadFontPal0(GF_PAL_LOCATION_SUB_BG,GF_PAL_SLOT_15_OFFSET,param_2);
  return;
}
