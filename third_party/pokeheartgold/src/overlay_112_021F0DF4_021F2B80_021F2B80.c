#include "global.h"
#include "overlay_112_sprite_app.h"

u8 ov112_021F0F60(Ov112Record *);

void ov112_021F15E4(Ov112SpriteApp *);

void ov112_021F1904(Ov112SpriteApp *, u32, u32);

void ov112_021F194C(Ov112SpriteApp *, u32);

void ov112_021F1D58(Ov112SpriteApp *, u32);

void ov112_021F257C(Ov112SpriteApp *);

void ov112_021F25FC(Ov112SpriteApp *);

void ov112_021F2688(Ov112SpriteApp *);

void ov112_021F26E8(Ov112SpriteApp *);

void ov112_021F2768(Ov112SpriteApp *);

void ov112_021F27B8(Ov112SpriteApp *);

void ov112_021F27E0(Ov112SpriteApp *);

void ov112_021F2810(Ov112SpriteApp *);

void ov112_021F2874(Ov112SpriteApp *);

void ov112_021F28DC(Ov112SpriteApp *);

void ov112_021F2920(Ov112SpriteApp *);

void ov112_021F2944(Ov112SpriteApp *);

void ov112_021F298C(Ov112SpriteApp *);

void ov112_021F2A78(Ov112SpriteApp *, u32);

void ov112_021F2B2C(Ov112SpriteApp *);

void ov112_021F2B80(Ov112SpriteApp *param_1);

void ov112_021F2B80(Ov112SpriteApp *param_1) {
    u8 bVar1;
    u32 uVar2;
    Ov112Record *iVar3;
    u32 uVar4;

    iVar3 = param_1->records[param_1->curIdx];
    ov112_021F15E4(param_1);
    uVar4 = param_1->time.hour;
    uVar2 = ov112_021F0F60(iVar3);
    ov112_021F1904(param_1, uVar4, uVar2);
    uVar2 = ov112_021F0F60(iVar3);
    ov112_021F1D58(param_1, uVar2);
    if (param_1->bits144.page != iVar3->page) {
        uVar2 = ov112_021F0F60(iVar3);
        ov112_021F194C(param_1, uVar2);
    }
    param_1->bits144.page = iVar3->page;
    bVar1 = iVar3->kind;
    if ((bVar1 >= 1) && (bVar1 <= 10)) {
        if ((u16)(iVar3->unk_8 + 0xfff9) <= 1) {
            ov112_021F257C(param_1);
            return;
        }
        ov112_021F2B2C(param_1);
        return;
    }
    if (bVar1 == 0xb) {
        ov112_021F25FC(param_1);
        return;
    }
    if (bVar1 == 0xc) {
        ov112_021F2688(param_1);
        return;
    }
    if (bVar1 == 0xd) {
        ov112_021F26E8(param_1);
        return;
    }
    if (bVar1 == 0xe) {
        ov112_021F2768(param_1);
        return;
    }
    if (bVar1 == 0xf) {
        ov112_021F27B8(param_1);
        return;
    }
    if (bVar1 == 0x10) {
        ov112_021F27E0(param_1);
        return;
    }
    if (bVar1 == 0x11) {
        ov112_021F2810(param_1);
        return;
    }
    if ((bVar1 >= 0x12) && (bVar1 <= 0x16)) {
        ov112_021F2874(param_1);
        return;
    }
    if (bVar1 == 0x17) {
        ov112_021F28DC(param_1);
        return;
    }
    if (bVar1 == 0x18) {
        ov112_021F2920(param_1);
        return;
    }
    if (bVar1 == 0x19) {
        ov112_021F2944(param_1);
        return;
    }
    if (bVar1 == 0x1a) {
        ov112_021F298C(param_1);
        return;
    }
    if (bVar1 == 0x1b) {
        ov112_021F2A78(param_1, uVar4);
    }
    return;
}
