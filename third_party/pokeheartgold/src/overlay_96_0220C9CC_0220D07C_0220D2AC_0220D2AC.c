#include "global.h"

typedef struct UnkStruct_ov96_0220D2AC {
    u32 unk0_0 : 9;
    u32 max : 9;
    u32 cur : 8;
    u32 unk0_26 : 6;
} UnkStruct_ov96_0220D2AC;

void ov96_0220D2AC(UnkStruct_ov96_0220D2AC *s, u32 frame, int sel);

void ov96_0220D2AC(UnkStruct_ov96_0220D2AC *s, u32 frame, int sel) {
    int i = 0;
    u32 m60 = frame % 60;
    u32 m30 = frame % 30;
    for (; i < 3; i++, s++) {
        u16 cur = s->cur;
        u16 max = s->max;
        if (frame != 0 && cur < max) {
            if (i == sel) {
                if (m60 == 0) {
                    s->cur += 8;
                }
            } else if (m30 == 0) {
                s->cur += 5;
            }
            if (s->cur > max) {
                s->cur = max;
            }
        }
    }
}
