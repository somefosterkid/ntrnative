#include "global.h"
#include "overlay_87_scratch_off_card_app.h"
struct ScratchOffCardApp;

s32 ov87_021E7590(void *, int, int);

void ov87_021E7550(struct ScratchOffCardApp *arg0);

void ov87_021E7550(ScratchOffCardApp *arg0) {
    s32 v1;

    v1 = 0;
    if ((s32) arg0->tpBuffer.bufferSize > 0) {
        do {
            ov87_021E7590(arg0, (u8) arg0->tpBuffer.buffer[v1].x, (u8) arg0->tpBuffer.buffer[v1].y);
            v1 += 1;
        } while (v1 < (s32) arg0->tpBuffer.bufferSize);
    }
}
