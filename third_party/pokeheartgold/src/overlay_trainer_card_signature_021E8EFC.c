#include "global.h"
#include "sprite_resources_header.h"

#include "bg_window.h"
#include "options.h"
#include "pm_string.h"
#include "unk_0200A090.h"
#include "math_util.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "signature_app.h"
#include "game_stats.h"
#include "msgdata.h"
#include "yes_no_prompt.h"

struct GameStats;

struct MsgData;

struct SpriteList;

struct SpriteResourceCollection;

struct MessageFormat;

struct YesNoPrompt;

enum SpriteResourceType {
    SPRITE_RESOURCE_CHAR = 0,
    SPRITE_RESOURCE_PLTT,
    SPRITE_RESOURCE_CELL,
    SPRITE_RESOURCE_ANIM,
    SPRITE_RESOURCE_MULTI_CELL,
    SPRITE_RESOURCE_MULTI_ANIM,
    SPRITE_RESOURCE_MAX,
};

enum SignatureAppSubstate {
    SIGNATURE_APP_SUBSTATE_0_UNUSED = 0,
    SIGNATURE_APP_SUBSTATE_MAIN,
    SIGNATURE_APP_SUBSTATE_DONE_PRESSED,
    SIGNATURE_APP_SUBSTATE_CONFIRM_DONE,
    SIGNATURE_APP_SUBSTATE_PROMPT_RESET,
    SIGNATURE_APP_SUBSTATE_OPEN_RESET_MENU,
    SIGNATURE_APP_SUBSTATE_CONFIRM_RESET,
};

int ov52_021E92E8(int printerID);

void ov52_021E8D3C(SignatureApp *signatureApp);

void ov52_021E8D64(BgConfig *bgConfig, YesNoPrompt *menu);

int ov52_021E8EFC(SignatureApp *signatureApp, int state);

void ov52_021E8F2C(Window *window, void *src, int srcX, int srcY, int srcWidth, int srcHeight, int destX, int destY, int destWidth, int destHeight);
void ov52_021E8FAC(Window *window, const u8 *src, int endX, int endY, int *startX, int *startY, int index, int prevBufferSize);
void ov52_021E9124(SignatureTouchDataBuffer *touchData, PreviousTouchDataBuffer *prevTouchData);
extern const u8 ov52_021E95A0[2][8][16];
extern void ov52_021E9158(Window *window, SignatureTouchDataBuffer touchData[], PreviousTouchDataBuffer *prevTouchData, BOOL allowWindowUpdate);

void ov52_021E9218(void);
void ov52_021E921C(u16 *arg0);
void ov52_021E925C(void);

int ov52_021E8EFC(SignatureApp *signatureApp, int state) {
    if (ov52_021E92E8(signatureApp->printerID)) {
        signatureApp->subState = SIGNATURE_APP_SUBSTATE_CONFIRM_RESET;
        ov52_021E8D64(signatureApp->bgConfig, signatureApp->yesNoMenu);
    }

    ov52_021E8D3C(signatureApp);
    return state;
}

void ov52_021E8F2C(Window *param0, void *param1, int param2, int param3, int param4, int param5, int param6, int param7, int param8, int param9);

void ov52_021E8F2C(Window *param0, void *param1, int param2, int param3, int param4, int param5, int param6, int param7, int param8, int param9) {
    if (param6 < 0) {
        int v0;

        v0 = param6 * -1;

        if (v0 > param8) {
            v0 = param8;
        }

        param6 = 0;
        param2 += v0;
        param4 -= v0;
        param8 -= v0;
    }

    if (param7 < 0) {
        int v1;

        v1 = param7 * -1;

        if (v1 > param9) {
            v1 = param9;
        }

        param7 = 0;
        param3 += v1;
        param5 -= v1;
        param9 -= v1;
    }

    BlitBitmapRectToWindow(param0, param1, param2, param3, param4, param5, param6, param7, param8, param9);
}

void ov52_021E8FAC(Window *window, const u8 *src, int endX, int endY, int *startX, int *startY, int index, int prevBufferSize) {
    int destX = *startX;
    int destY = *startY;
    int maxX = endX;
    int maxY = endY;

    if (index == 0 && prevBufferSize == 0) {
        *startX = endX;
        *startY = endY;
        return;
    }

    int xDistance = MATH_IAbs(maxX - destX);
    int yDistance = MATH_IAbs(maxY - destY);

    int tmp;

    if (xDistance > yDistance) {
        int deltaY;
        if (destX > maxX) {
            deltaY = destY > maxY ? 1 : -1;
            tmp = destX;
            destX = maxX;
            maxX = tmp;
            destY = maxY;
        } else {
            deltaY = destY < maxY ? 1 : -1;
        }

        ov52_021E8F2C(window, (void *)src, 0, 0, 4, 4, destX, destY, 4, 4);
        int distanceUntilYChange = xDistance >> 1;

        while (++destX <= maxX) {
            if ((distanceUntilYChange -= yDistance) < 0) {
                distanceUntilYChange += xDistance;
                destY += deltaY;
            }

            ov52_021E8F2C(window, (void *)src, 0, 0, 4, 4, destX, destY, 4, 4);
        }
    } else {
        int deltaX;
        if (destY > maxY) {
            deltaX = destX > maxX ? 1 : -1;
            tmp = destY;
            destY = maxY;
            maxY = tmp;
            destX = maxX;
        } else {
            deltaX = destX < maxX ? 1 : -1;
        }

        ov52_021E8F2C(window, (void *)src, 0, 0, 4, 4, destX, destY, 4, 4);
        int distanceUntilXChange = yDistance >> 1;

        while (++destY <= maxY) {
            if ((distanceUntilXChange -= xDistance) < 0) {
                distanceUntilXChange += yDistance;
                destX += deltaX;
            }

            ov52_021E8F2C(window, (void *)src, 0, 0, 4, 4, destX, destY, 4, 4);
        }
    }

    *startX = endX;
    *startY = endY;
}

void ov52_021E9124(SignatureTouchDataBuffer *touchData, PreviousTouchDataBuffer *prevTouchData) {
    for (int i = 0; i < 5; i++) {
        prevTouchData[i].bufferSize = touchData[i].bufferSize;

        if (touchData[i].bufferSize != 0) {
            prevTouchData[i].lastX = touchData[i].x[touchData[i].bufferSize - 1];
            prevTouchData[i].lastY = touchData[i].y[touchData[i].bufferSize - 1];
        }
    }
}

void ov52_021E9158(Window *window, SignatureTouchDataBuffer touchData[], PreviousTouchDataBuffer *prevTouchData, BOOL allowWindowUpdate) {
    int i;
    BOOL windowUpdate = 0;

    for (i = 0; i < 5; i++) {
        if (touchData[i].bufferSize != 0) {
            int prevX, prevY;
            if (prevTouchData[i].bufferSize) {
                prevX = prevTouchData[i].lastX - 4 * 8;
                prevY = prevTouchData[i].lastY - 9 * 8;
            }

            for (int j = 0; j < touchData[i].bufferSize; j++) {
                int x = touchData[i].x[j] - 4 * 8;
                int y = touchData[i].y[j] - 9 * 8;

                ov52_021E8FAC(window, ov52_021E95A0[0][touchData[i].brushTypeUnused], x, y, &prevX, &prevY, j, prevTouchData[i].bufferSize);

                windowUpdate = 1;
            }
        }
    }

    if (windowUpdate && allowWindowUpdate) {
        CopyWindowToVram(window);
    }

    ov52_021E9124(touchData, prevTouchData);

    for (i = 0; i < 5; i++) {
        touchData[i].bufferSize = 0;
    }
}

void ov52_021E9218(void) {
}

void ov52_021E921C(u16 *arg0) {
    s16 pSrc;
    s32 v0;

    *arg0 += 0x14;
    if ((u32)*arg0 > 0x168U) {
        *arg0 = 0;
    }
    v0 = 0xA * GF_SinDeg(*arg0);
    pSrc = ((((s32)(v0 + ((u32)(v0 >> 0xB) >> 0x14)) >> 0xC) + 0xF) << 5) | 0x1D;
    GX_LoadOBJPltt(&pSrc, 0x18U, 2U);
}

void ov52_021E925C(void) {
}
