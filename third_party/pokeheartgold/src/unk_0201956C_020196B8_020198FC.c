#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "unk_0201956C_types.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void sub_020198FC(UnkStruct_0201956C *frames, int a1, int a2, int a3, int a4);
#else
void sub_020198FC(UnkStruct_0201956C *frames, int a1, int a2, int a3, u8 a4);
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
void sub_020198FC(UnkStruct_0201956C *frames, int a1, int a2, int a3, int a4) {
#else
void sub_020198FC(UnkStruct_0201956C *frames, int a1, int a2, int a3, u8 a4) {
#endif
    BGWinFrameEntry *entry = &frames->entries[a1];
    entry->moveX = a2;
    entry->moveY = a3;
    entry->moveCount = a4;
    entry->moving = 1;
    frames->moving = 1;
}
