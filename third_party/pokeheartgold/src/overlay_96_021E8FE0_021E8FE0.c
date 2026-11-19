#include "global.h"

#include "overlay_96_sprite_res_man.h"
#include "unk_0200A090.h"

u8 ov96_021E9104(Ov96SpriteResEntry *entry);

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov96_021E8FE0(Ov96SpriteResLoadTask *task) {
    u8 idx;
    s32 flag;
    Ov96SpriteResEntry *ent;
    Ov96SpriteResMan *man;

    switch (task->state) {
    case 0:
        idx = task->baseIndex + task->pos;
        man = task->man;
        ov96_021E8C70(man, &man->entries[idx], &task->descs[idx], task->params.vramType, idx, task->params.unk_8);
        task->man->entries[idx].desc = task->descs[idx];
        task->man->entries[idx].unk_24 = ov96_021E9104(&task->man->entries[idx]);
        flag = 0;
        if (task->man->mode != 0 && task->descs[idx].isLarge != 0) {
            flag = 1;
        }
        task->man->entries[idx].unk_28 = flag;
        if (task->man->entries[idx].unk_28 != 0) {
            task->man->entries[idx].unk_24 += 8;
        }
        if (task->params.unk_8 == 0) {
            ent = &task->man->entries[idx];
            sub_0200A740(task->man->entries[idx].charRes);
            sub_0200A740(ent->plttRes);
        }
        task->pos++;
        if (task->pos >= task->count) {
            task->pos = 0;
            task->state = 1;
        }
        break;
    case 1:
        idx = task->baseIndex + task->pos;
        ov96_021E8EE4(task->man, task->params.unk_4, task->params.unk_8, task->params.screen, idx, idx);
        task->pos++;
        task->man->numLoaded++;
        if (task->pos >= task->count) {
            return 1;
        }
        break;
    }
    return 0;
}
#else
s32 ov96_021E8FE0(Ov96SpriteResLoadTask *task) {
    void *arg0 = task;
    u8 *p = arg0;
    u8 idx;
    u32 off;
    s32 flag;
    u8 *ent;
    u8 *ctx;
    s32 r6;

    switch (*(s32 *)(p + 8)) {
    case 0:
        idx = *(s32 *)(p + 4) + *(u16 *)(p + 2);
        off = idx * 0x2C;
        ctx = *(u8 **)(p + 0xC);
        r6 = idx * 0x10;
        ov96_021E8C70((Ov96SpriteResMan *)ctx, (Ov96SpriteResEntry *)(*(u8 **)(ctx + 0x154) + off), (Ov96SpriteResDesc *)(p + 0x54 + r6), *(s32 *)(p + 0x1C), idx, *(s32 *)(p + 0x18));
        *(Ov96SpriteResDesc *)(*(u8 **)(*(u8 **)(p + 0xC) + 0x154) + off + 0x14) = *(Ov96SpriteResDesc *)(p + idx * 0x10 + 0x54);
        *(s32 *)(*(u8 **)(*(u8 **)(p + 0xC) + 0x154) + off + 0x24) = ov96_021E9104((Ov96SpriteResEntry *)(*(u8 **)(*(u8 **)(p + 0xC) + 0x154) + off));
        flag = 0;
        if (*(s32 *)(*(u8 **)(p + 0xC) + 8) != 0 && *(u16 *)(p + idx * 0x10 + 0x58) != 0) {
            flag = 1;
        }
        *(s32 *)(*(u8 **)(*(u8 **)(p + 0xC) + 0x154) + 0x2C * idx + 0x28) = flag;
        if (*(s32 *)(*(u8 **)(*(u8 **)(p + 0xC) + 0x154) + 0x2C * idx + 0x28) != 0) {
            *(s32 *)(*(u8 **)(*(u8 **)(p + 0xC) + 0x154) + 0x24 + off) += 8;
        }
        if (*(s32 *)(p + 0x18) == 0) {
            ent = *(u8 **)(*(u8 **)(p + 0xC) + 0x154) + off;
            sub_0200A740(*(SpriteResource **)(*(u8 **)(*(u8 **)(p + 0xC) + 0x154) + off));
            sub_0200A740(*(SpriteResource **)(ent + 4));
        }
        *(u16 *)(p + 2) += 1;
        if (*(u16 *)(p + 2) >= *(u16 *)p) {
            *(u16 *)(p + 2) = 0;
            *(s32 *)(p + 8) = 1;
        }
        break;
    case 1:
        idx = *(s32 *)(p + 4) + *(u16 *)(p + 2);
        ov96_021E8EE4(*(void **)(p + 0xC), *(s32 *)(p + 0x14), *(s32 *)(p + 0x18), (NNS_G2D_VRAM_TYPE) * (s32 *)(p + 0x20), idx, idx);
        *(u16 *)(p + 2) += 1;
        *(u16 *)(*(u8 **)(p + 0xC) + 6) += 1;
        if (*(u16 *)(p + 2) >= *(u16 *)p) {
            return 1;
        }
        break;
    }
    return 0;
}
#endif
