#ifndef POKEHEARTGOLD_OVERLAY_102_EASY_CHAT_CATEGORIES_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_102_EASY_CHAT_CATEGORIES_CONTEXT_H

// EasyChatCategoriesContext: merged by tools/hg/promote.py from 31 m2c local copies (1 variants, base
// src/overlay_102_021E77F8_021E7A58_021E7B18_021E7B18.c) and 4 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct EasyChatApp;
struct EasyChatWordList;
struct Sprite;
struct SysTask;
struct UnkStruct_ov20_021D2128_t;

typedef struct EasyChatCategoriesContext {
    struct UnkStruct_ov20_021D2128_t *unk_00;
    struct EasyChatApp *easyChatApp;
    struct EasyChatWordList *unk_08;
    struct BgConfig *bgConfig;
    u32 unk_10;
    u32 unk_14;
    int dummy;
    struct { int unk_00; int unk_04; int unk_08; int unk_0C; int unk_10; int unk_14; int unk_18; struct SysTask *unk_1C; } unk_1C;
    struct { struct BgConfig *unk_00; u32 unk_04; int unk_08; fx32 unk_0C; fx32 unk_10; fx32 unk_14; u16 unk_18; u16 unk_1A; struct SysTask *unk_1C; } unk_3C;
    struct Sprite *cursorSprite;
} EasyChatCategoriesContext;

// sizeof(EasyChatCategoriesContext) == 0x60

#endif // POKEHEARTGOLD_OVERLAY_102_EASY_CHAT_CATEGORIES_CONTEXT_H
