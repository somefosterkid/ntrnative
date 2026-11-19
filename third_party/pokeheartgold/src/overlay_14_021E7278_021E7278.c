#include "global.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void *ov14_021E60C0(void *self, int index, int slot);
BOOL ov14_021E7278(UnkStruct_ov14_boxapp *self);

u32 GetBoxMonData(void *boxMon, int attr, void *ptr);

BOOL ov14_021E7278(UnkStruct_ov14_boxapp *self) {
    UnkStruct_ov14_boxscan *state = self->sys->unk_88DC;
    int pos = state->pos;
    int count;
    int j;
    void *mon;
    int idx;
    if (pos == 0x222) {
        return 0;
    }
    count = 0;
    do {
        if (((u32)pos) < 0x21c) {
            int box = pos / 0x1e;
            int slot = pos % 0x1e;
            if ((box == self->unk1F) && (slot == self->unk21)) {
                mon = 0L;
            } else {
                mon = ov14_021E60C0(self, box, slot);
            }
        } else if ((pos - 0x21c) == (self->unk21 - 0x1e)) {
            mon = 0L;
        } else {
            mon = ov14_021E60C0(self, 0xff, pos - 0x21c);
        }
        if ((mon != 0L) && (GetBoxMonData(mon, 0xac, 0L) != 0)) {
            for (j = 0; ((u32)j) < 4; j++) {
                idx = ov14_021E71C8((u16)GetBoxMonData(mon, j + 0x36, 0L));
                if (idx != (-1)) {
                    state->moveFlags = (u8)(state->moveFlags & (0xff ^ (1u << idx)));
                }
            }
        }
        state->pos += 1;
        pos = state->pos;
        if (pos == 0x222) {
            return 0;
        }
        count++;
    } while (((u32)count) < 0xf);
    return 1;
}
