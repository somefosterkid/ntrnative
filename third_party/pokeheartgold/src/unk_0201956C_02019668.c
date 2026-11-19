#include "global.h"

#include "unk_0201956C.h"

void sub_02019668(UnkStruct_0201956C *frames, int index, u16 *screen);

void sub_02019668(UnkStruct_0201956C *frames, int index, u16 *screen) {
    BGWinFrameEntry *frame = &frames->entries[index];

    MIi_CpuCopy16(screen, frame->screen, frame->width * frame->height * sizeof(u16));
}
