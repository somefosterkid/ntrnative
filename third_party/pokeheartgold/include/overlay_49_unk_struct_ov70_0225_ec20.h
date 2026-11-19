#ifndef POKEHEARTGOLD_OVERLAY_49_UNK_STRUCT_OV70_0225_EC20_H
#define POKEHEARTGOLD_OVERLAY_49_UNK_STRUCT_OV70_0225_EC20_H

#include "global.h"

#include "bg_window.h"
#include "list_menu.h"

struct Sprite;
struct SpriteResource;

typedef struct UnkStruct_ov70_0225EC20 {
    ListMenuTemplate unk_00;
    Window unk_20;
    struct ListMenu *unk_30;
    ListMenuItem *unk_34;
    u16 unk_38;
    u16 unk_3A;
    BOOL unk_3C;
    struct SpriteResource *unk_40[4];
    struct Sprite *unk_50[2];
} UnkStruct_ov70_0225EC20;

// sizeof(UnkStruct_ov70_0225EC20) == 0x58

#endif // POKEHEARTGOLD_OVERLAY_49_UNK_STRUCT_OV70_0225_EC20_H
