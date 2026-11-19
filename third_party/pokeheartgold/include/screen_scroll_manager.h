#ifndef POKEHEARTGOLD_SCREEN_SCROLL_MANAGER_H
#define POKEHEARTGOLD_SCREEN_SCROLL_MANAGER_H

// ScreenScrollManager: merged by tools/hg/promote.py from 3 m2c local copies (1 variants, base
// src/unk_02014A08_02014B08.c) and 2 accesses; fields at every accessed offset, pads elsewhere.

struct BufferManager;
struct SysTask;

typedef struct ScreenScrollManager {
    u32 buffer1[192];
    u32 buffer2[192];
    struct BufferManager *bufferManager;
    struct SysTask *scrollTask;
    BOOL unused;
    s16 sineTable[192];
    u8 start;
    u8 end;
    u8 pad_78E[0x2];
    void *offsetRegisterAddr;
    u32 initValue;
    s16 scrollPos;
    s16 scrollSpeed;
} ScreenScrollManager;

// sizeof(ScreenScrollManager) == 0x79c

#endif // POKEHEARTGOLD_SCREEN_SCROLL_MANAGER_H
