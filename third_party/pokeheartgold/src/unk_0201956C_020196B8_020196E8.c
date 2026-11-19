#include "global.h"

#include "bg_window.h"
#include "unk_0201956C.h"

extern void (*const _020F6298[])(BgConfig *bgConfig, u8 bgId);

void sub_020196E8(UnkStruct_0201956C *a0, int a1, int a2, int a3) {
    u8 destX, destY, destWidth, destHeight, srcX, srcY;
    s16 tilesW, clipRight, clipBottom, clipLeft, clipTop;
    s32 right, bottom, tilesH;
    BGWinFrameEntry *e;

    e = &a0->entries[a1];
    e->x = a2;
    e->y = a3;
    clipRight = e->right;
    if (a2 < clipRight) {
        clipBottom = e->bottom;
        if (a3 < (s32)clipBottom) {
            tilesW = e->width;
            right = a2 + tilesW;
            clipLeft = e->left;
            if (right >= clipLeft) {
                tilesH = e->height;
                bottom = a3 + tilesH;
                clipTop = e->top;
                if (bottom >= (s32)clipTop) {
                    destX = (u8)a2;
                    destWidth = (u8)tilesW;
                    srcX = 0;
                    if (a2 < clipLeft) {
                        destX = (u8)clipLeft;
                        destWidth = destWidth - (clipLeft - a2);
                        srcX = (u8)(clipLeft - a2);
                    }
                    if (right >= clipRight) {
                        destWidth = destWidth - (right - clipRight);
                    }
                    destY = (u8)a3;
                    destHeight = (u8)tilesH;
                    srcY = 0;
                    if (a3 < (s32)clipTop) {
                        destY = (u8)clipTop;
                        destHeight = destHeight - (clipTop - a3);
                        srcY = (u8)(clipTop - a3);
                    }
                    if (bottom >= (s32)clipBottom) {
                        destHeight = destHeight - (bottom - clipBottom);
                    }
                    CopyToBgTilemapRect(a0->bgConfig, e->bg, destX, destY, destWidth, destHeight, e->screen, srcX, srcY, (u8)tilesW, (u8)tilesH);
                    _020F6298[a0->transferMode](a0->bgConfig, e->bg);
                    e->hidden = 0;
                }
            }
        }
    }
}

#if defined(SDK_PORT) || defined(NONMATCHING)
// sub_0201980C (the DS build keeps asm/unk_0201956C_020196B8_0201980C.s): hides frame a1 and clears the part of its
// rect inside the frame's clip box. Typed on BGWinFrameEntry (the lifted C read DS offsets of the 0x10-byte entry:
// 24 bytes on a 64-bit host, so it crashed opening the Pokedex).
void sub_0201980C(UnkStruct_0201956C *a0, int a1) {
    BGWinFrameEntry *e = &a0->entries[a1];
    s8 x, y, width, height;
    s32 right, bottom;

    e->hidden = 1;
    x = e->x;
    y = e->y;
    if (x >= e->right || y >= e->bottom) {
        return;
    }
    right = x + e->width;
    bottom = y + e->height;
    if (right < e->left || bottom < e->top) {
        return;
    }
    width = e->width;
    if (e->left > x) {
        width = width - (e->left - x);
        x = e->left;
    }
    if (right >= e->right) {
        width = width - (right - e->right);
    }
    height = e->height;
    if (y < e->top) {
        height = height - (e->top - y);
        y = e->top;
    }
    if (bottom >= e->bottom) {
        height = height - (bottom - e->bottom);
    }
    FillBgTilemapRect(a0->bgConfig, e->bg, 0, x, y, width, height, 0x10);
    _020F6298[a0->transferMode](a0->bgConfig, e->bg);
}
#endif
