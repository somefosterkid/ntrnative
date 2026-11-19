#include "global.h"
struct GTSApplicationState;

#include "sprite.h"
#include "system.h"
#include "unk_02005D10.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_02238F9C(Sprite *, int, int);

s32 ov70_0223C2EC(void *);

extern u8 ov70_022464FE[];

extern u8 ov70_02246500[];

extern u8 ov70_02246502[];

typedef struct {
    u8 pad[0xDCC];
    Sprite *sprite;
    u8 pad2[0x11C4 - 0xDD0];
    u8 *sub;
} UnkStruct_ov70_0223C304;

#define SUB(w) (((UnkStruct_ov70_0223C304 *)(w))->sub)

#define F(p, o) (*(s32 *)((p) + (o)))

void ov70_0223C304(struct GTSApplicationState *arg0);

void ov70_0223C304(GTSApplicationState *arg0) {
    u8 *p;
    s32 a;
    s32 b;
    if (0x40 & gSystem.newKeys) {
        p = SUB(arg0);
        if (F(p, 0x24) == 0) {
            if (F(p, 0x28) > 0) {
                F(p, 0x28)
                --;
                PlaySE(0x5DC);
            }
        } else if (F(p, 0x2C) > 0) {
            PlaySE(0x5DC);
            F(SUB(arg0), 0x2C)
            --;
        }
    } else if (0x80 & gSystem.newKeys) {
        p = SUB(arg0);
        if (F(p, 0x24) == 0) {
            if (F(p, 0x28) < 3) {
                F(p, 0x28)
                ++;
                PlaySE(0x5DC);
            }
        } else if (F(p, 0x2C) < 2) {
            PlaySE(0x5DC);
            F(SUB(arg0), 0x2C)
            ++;
        }
    } else if (0x10 & gSystem.newKeys) {
        if (F(SUB(arg0), 0x24) != 1) {
            PlaySE(0x5DC);
        }
        F(SUB(arg0), 0x24) = 1;
    } else if (gSystem.newKeys & 0x20) {
        if (F(SUB(arg0), 0x24) != 0) {
            PlaySE(0x5DC);
        }
        F(SUB(arg0), 0x24) = 0;
    }
    a = ov70_0223C2EC(arg0);
    b = ov70_0223C2EC(arg0);
    ov70_02238F9C(arg0->cursorSprite, *(u16 *)(ov70_022464FE + a * 6), *(u16 *)(ov70_02246500 + b * 6));
    Sprite_SetAnimCtrlSeq(arg0->cursorSprite, *(u16 *)(ov70_02246502 + ov70_0223C2EC(arg0) * 6));
}
