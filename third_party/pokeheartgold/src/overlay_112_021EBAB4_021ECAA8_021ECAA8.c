#include "global.h"

#include "unk_02005D10.h"
#include "overlay_112_hitbox.h"

extern u8 gSystem[];

extern const int ov112_021FF0C4[];

s8 ov112_021ECA88(void *work);

void ov112_021EC440(void *work, int a1, int a2, int a3);

void ov112_021EC8A4(void *work, int a1, int a2);

void ov112_021EA51C(void *work, int a1);

void ov112_021ECDA0(void *work, int dir);

void ov112_021ECA18(void *work, int a1);

void ov112_021EA6A0(void *work, int a1, int a2);

int ov112_021ECAA8(u8 *work);

int ov112_021ECAA8(u8 *work) {
    int sel = ov112_021E9888(4);
    u32 keys;

    if (sel != -1) {
        if ((u32)sel <= 4) {
            (*(int *)((work) + 0x1EC50)) = sel;
            if (ov112_021ECA88(work) >= 0) {
                ov112_021EC440(work, sel, ov112_021FF0C4[sel], 0xB);
                PlaySE(0x5DD);
                ov112_021EC8A4(work, (*(int *)((work) + 0x1EC54)), (*(int *)((work) + 0x1EC50)));
                return 0x41;
            }
            ov112_021EC8A4(work, (*(int *)((work) + 0x1EC54)), (*(int *)((work) + 0x1EC50)));
            PlaySE(0x5DC);
            ov112_021EA51C(work, ov112_021FF0C4[(*(int *)((work) + 0x1EC50))]);
        } else if (sel == 6) {
            ov112_021ECDA0(work, -1);
            ov112_021ECA18(work, (*(int *)((work) + 0x1EC54)));
            ov112_021EA6A0(work, 3, 2);
        } else if (sel == 7) {
            ov112_021ECDA0(work, 1);
            ov112_021ECA18(work, (*(int *)((work) + 0x1EC54)));
            ov112_021EA6A0(work, 4, 3);
        } else {
            ov112_021EC440(work, sel, ov112_021FF0C4[sel], 0xB);
            PlaySE(0x5DD);
            return 0x41;
        }
        return 10;
    }

    keys = *(u32 *)(gSystem + 0x48);
    if (keys & 0x40) {
        if ((*(int *)((work) + 0x1EC50)) > 0) {
            (*(int *)((work) + 0x1EC50))--;
            PlaySE(0x5DC);
        }
        ov112_021EA51C(work, ov112_021FF0C4[(*(int *)((work) + 0x1EC50))]);
        ov112_021EC8A4(work, (*(int *)((work) + 0x1EC54)), (*(int *)((work) + 0x1EC50)));
    } else if (keys & 0x80) {
        if ((*(int *)((work) + 0x1EC50)) < 5) {
            (*(int *)((work) + 0x1EC50))++;
            PlaySE(0x5DC);
        }
        ov112_021EA51C(work, ov112_021FF0C4[(*(int *)((work) + 0x1EC50))]);
        ov112_021EC8A4(work, (*(int *)((work) + 0x1EC54)), (*(int *)((work) + 0x1EC50)));
    } else if (keys & 0x20) {
        ov112_021ECDA0(work, -1);
        ov112_021ECA18(work, (*(int *)((work) + 0x1EC54)));
        ov112_021EA6A0(work, 3, 2);
    } else if (keys & 0x10) {
        ov112_021ECDA0(work, 1);
        ov112_021ECA18(work, (*(int *)((work) + 0x1EC54)));
        ov112_021EA6A0(work, 4, 3);
    } else if (keys & 1) {
        int cur = (*(int *)((work) + 0x1EC50));
        if (cur > 4) {
            ov112_021EC440(work, cur, ov112_021FF0C4[cur], 0xB);
            PlaySE(0x5DD);
            return 0x41;
        }
        if (ov112_021ECA88(work) >= 0) {
            ov112_021EC440(work, (*(int *)((work) + 0x1EC50)), ov112_021FF0C4[(*(int *)((work) + 0x1EC50))], 0xB);
            PlaySE(0x5DD);
            return 0x41;
        }
        PlaySE(0x5DC);
    } else if (keys & 2) {
        ov112_021EC440(work, 5, 3, 0xB);
        PlaySE(0x5DC);
        return 0x41;
    }
    return 10;
}
