#include "global.h"
#include "system.h"
#include "overlay_59_apricorn_app.h"

void ov59_0223A7FC(Ov59ApricornApp *p, s32 param_2);


u32 ov59_0223A678(Ov59ApricornApp *p);

u32 ov59_0223A678(Ov59ApricornApp *p) {
    switch (p->unk_49) {
    case 0:
        if (gSystem.newKeys & 0x10) {
            ov59_0223A7FC(p, 0);
            return 1;
        }
        if ((gSystem.newKeys & 0x40) && p->unk_4A != 0) {
            p->unk_4C = 0;
            p->unk_4A--;
            return 1;
        }
        if ((gSystem.newKeys & 0x80) && p->unk_4A < 3) {
            p->unk_4C = 0;
            p->unk_4A++;
            return 1;
        }
        break;
    case 1:
        if (gSystem.newKeys & 0x20) {
            ov59_0223A7FC(p, 1);
            return 1;
        }
        if (gSystem.newKeys & 0x10) {
            p->unk_4C = p->unk_4A;
            p->unk_4A = 0;
            p->unk_49++;
            return 1;
        }
        if ((gSystem.newKeys & 0x40) && p->unk_4A != 0) {
            p->unk_4B = 0;
            p->unk_4A--;
            return 1;
        }
        if ((gSystem.newKeys & 0x80) && p->unk_4A < 2) {
            p->unk_4B = 0;
            p->unk_4A++;
            return 1;
        }
        break;
    case 2:
        if (gSystem.newKeys & 0x20) {
            p->unk_4A = p->unk_4C;
            p->unk_49--;
            return 1;
        }
        if (p->mode == 2) {
            return 0;
        }
        if ((gSystem.newKeys & 0x90) && p->mode != 0) {
            p->unk_49++;
            return 1;
        }
        break;
    case 3:
        if (gSystem.newKeys & 0x60) {
            p->unk_49--;
            return 1;
        }
    }
    return 0;
}
