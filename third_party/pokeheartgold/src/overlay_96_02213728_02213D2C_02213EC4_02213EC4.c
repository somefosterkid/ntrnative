#include "global.h"

#include "error_handling.h"

#include "overlay_96_app_02210C98.h"

u32 ov96_02214394(u32 param_1, u32 param_2);

u32 ov96_022143DC(Ov96App_02210C98_Mover *param_1);

u32 ov96_02213EC4(Ov96App_02210C98_Mover *p);

u32 ov96_02213EC4(Ov96App_02210C98_Mover *p) {
    u32 r;
    s32 v;
    if (p->unk_39 == 3) {
        v = p->unk_8.y;
        if (v == 0x58000) {
            r = p->unk_43 + 0xc0;
        } else if (v == 0x68000) {
            r = p->unk_43 + 200;
        } else if (v == 0x78000) {
            r = p->unk_43 + 0xd0;
        } else {
            GF_AssertFail();
            r = p->unk_8.y / 4096;
        }
    } else if (p->unk_39 == 1) {
        if (p->unk_3B == 0xc) {
            r = p->unk_8.y / 4096;
        } else {
            u32 a = ov96_022143DC(p);
            u32 b = p->unk_3B / 3;
            r = ov96_02214394(a, (u8)b);
        }
    } else {
        r = p->unk_8.y / 4096;
    }
    return r & 0xff;
}
