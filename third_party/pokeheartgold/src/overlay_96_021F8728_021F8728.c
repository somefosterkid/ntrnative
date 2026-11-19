#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02013534.h"

void ov96_021F8E94(u32 *param_1);

typedef struct {
    Sprite *sprites[4];
    u32 unk10[4];
    u32 unk20[4];
    u8 unk30[0x18];
} UnkStruct_ov96_021F8728_Sub;

typedef struct {
    u8 unk0[0x14];
    UnkStruct_ov96_021F8728_Sub sub[5];
} UnkStruct_ov96_021F8728;

void ov96_021F8728(void *param_1);

void ov96_021F8728(void *param_1) {
    UnkStruct_ov96_021F8728 *data = param_1;
    void *pvVar2;
    Window *window;
    s32 iVar4;

    for (iVar4 = 0; iVar4 < 5; iVar4++) {
        ov96_021F8E94(data->sub[iVar4].unk10);
        ov96_021F8E94(data->sub[iVar4].unk20);
        Sprite_Delete(data->sub[iVar4].sprites[0]);
        Sprite_Delete(data->sub[iVar4].sprites[1]);
        Sprite_Delete(data->sub[iVar4].sprites[2]);
        Sprite_Delete(data->sub[iVar4].sprites[3]);
    }
    iVar4 = 0;
    window = (Window *)((u8 *)param_1 + 0x2fc);
    do {
        sub_02013938((UnkStruct_02013910 *)window[1].bgConfig);
        RemoveWindow(window);
        iVar4 = iVar4 + 1;
        window = (Window *)&window[1].height;
    } while (iVar4 < 2);
    sub_020135AC(*(UnkStruct_02013534 **)((u8 *)param_1 + 0x10));
    SpriteTransfer_DeleteCharTransferTask(*(SpriteResource **)((u8 *)param_1 + 0x2c0));
    SpriteTransfer_DeletePlttTransferTask(*(SpriteResource **)((u8 *)param_1 + 0x2c4));
    iVar4 = 0;
    pvVar2 = param_1;
    do {
        Destroy2DGfxResObjMan(*(GF_2DGfxResMan **)((u8 *)pvVar2 + 0x2a8));
        iVar4 = iVar4 + 1;
        pvVar2 = (void *)((u8 *)pvVar2 + 4);
    } while (iVar4 < 6);
    SpriteList_Delete(*(SpriteList **)((u8 *)param_1 + 0x17c));
    Heap_Free(param_1);
    return;
}
