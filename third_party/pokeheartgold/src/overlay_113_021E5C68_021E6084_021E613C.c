#include "global.h"
#include "overlay_113.h"

#include "msgdata.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"

s32 ov113_021E6238(UnownReportData *, UnownReportScreen *, u16);

u8 ov113_021E6274(Window *, String *, s32, s32, u16);

s32 ov113_021E629C(Window *, String *, s32, s32);

typedef struct Ov113MsgRange {
    u16 msgId : 6;
    u16 count : 2;
    u16 minLevel : 8;
} Ov113MsgRange;

extern const Ov113MsgRange ov113_021E6BD0[][2];

void ov113_021E613C(UnownReportData *arg0, s32 arg1);

void ov113_021E613C(UnownReportData *arg0, s32 arg1) {
    const Ov113MsgRange *p;
    s32 i;

    if (arg1 == 0) {
        ov113_021E6238(arg0, &arg0->screens[5], 6);
        ov113_021E6238(arg0, &arg0->screens[2], 2);
        for (i = 0; i < 0xE; i++) {
            ManagedSprite_SetDrawFlag(arg0->monIcons[i], 0);
        }
    }
    p = ov113_021E6BD0[arg1];
    if (p[0].minLevel <= arg0->reportLevel) {
        for (i = 0; i < p[0].count; i++) {
            ReadMsgDataIntoString(arg0->msgData, p[0].msgId + i, arg0->strBuf2);
            if (p[0].msgId + i == 0xF) {
                ov113_021E6274(&arg0->windows[1], arg0->strBuf2, 3, 4, 0);
            } else {
                ov113_021E629C(&arg0->windows[1], arg0->strBuf2, 4, 0x18);
            }
        }
    }
    if (p[1].minLevel <= arg0->reportLevel) {
        for (i = 0; i < p[1].count; i++) {
            ReadMsgDataIntoString(arg0->msgData, p[1].msgId + i, arg0->strBuf2);
            ov113_021E629C(&arg0->windows[2], arg0->strBuf2, 4, 0x18);
        }
    }
}
