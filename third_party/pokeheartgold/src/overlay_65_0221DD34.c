#include "global.h"

#include "error_handling.h"
#include "sprite.h"
#include "unk_02005D10.h"
#include "unk_02035900.h"

extern const int ov65_0221FF4C[13][2];

extern const int ov65_0221FD68[2][3];

void ov65_0221DD34(int slot, Sprite *sprite, int cursorIdx);

struct TradeRoomMonDisplayData;
typedef struct TradeRoomMonDisplayData TradeRoomMonDisplayData;
int ov65_0221DCFC(int currentSlot, int direction, TradeRoomMonDisplayData *displayData);
int ov65_0221DDC0(u32 *directionFlag, int *slotIdx, Sprite *sprite, TradeRoomMonDisplayData *displayData, int side);
void ov65_0221DE10(int unused, int cmd, int value);

void ov65_0221DD34(int slot, Sprite *sprite, int cursorIdx) {
    VecFx32 pos;

    ((slot < (6 * 2) + 1) ? (void)0 : GF_AssertFail());

    pos.x = ((fx32)0x0000000000001000L) * ov65_0221FF4C[slot][0];
    pos.y = ((fx32)0x0000000000001000L) * ov65_0221FF4C[slot][1];

    if (slot == (6 * 2)) {
        Sprite_SetMatrix(sprite, &pos);
        Sprite_SetAnimCtrlSeq(sprite, ov65_0221FD68[cursorIdx][2]);
    } else {
        Sprite_SetMatrix(sprite, &pos);

        if (slot < 6) {
            Sprite_SetAnimCtrlSeq(sprite, ov65_0221FD68[cursorIdx][0]);
        } else {
            Sprite_SetAnimCtrlSeq(sprite, ov65_0221FD68[cursorIdx][1]);
        }
    }
}

int ov65_0221DDC0(u32 *directionFlag, int *slotIdx, Sprite *sprite, TradeRoomMonDisplayData *displayData, int side) {
    int direction = *directionFlag - 1;
    int targetSlot = 0;
    int didMove = 0;

    if (side == 0) {
        if (*directionFlag) {
            targetSlot = ov65_0221DCFC(*slotIdx, direction, displayData);
            ov65_0221DD34(targetSlot, sprite, side);

            if (*slotIdx != targetSlot) {
                PlaySE(1500);
                *slotIdx = targetSlot;
                didMove = 1;
            }
        }

        *directionFlag = 0;
    } else {
        ov65_0221DD34(*slotIdx, sprite, side);
    }

    return didMove;
}

void ov65_0221DE10(int unused, int cmd, int value) {
    u8 byteValue = value;
    sub_02037030(cmd, &byteValue, 1);
}
