#ifndef POKEHEARTGOLD_UNK_02013534_FONT_SYSTEM_H
#define POKEHEARTGOLD_UNK_02013534_FONT_SYSTEM_H

#include "bg_window.h"
#include "sprite.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

typedef struct {
    Sprite *unk_00;
    int unk_04;
    int unk_08;
} UnkStruct_02012CE0_sub1;

// sizeof == 0x14 on the DS (0x20 on a 64-bit host: the two pointers); allocate and clear with sizeof.
struct FontOAM {
    UnkStruct_02012CE0_sub1 *unk_00;
    int unk_04;
    const Sprite *unk_08;
    int x;
    int y;
};

// The font system FontSystem_NewInit builds: sizeof == 0x68 on the DS (0xD0 on a 64-bit host).
struct UnkStruct_02012744_t {
    void *unk_00[12];
    NNSG2dCellDataBank *unk_30[12];
    FontOAM *unk_60;
    int unk_64;
};

#endif // POKEHEARTGOLD_UNK_02013534_FONT_SYSTEM_H
