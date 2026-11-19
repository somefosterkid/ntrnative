#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "filesystem_files_def.h"
#include "heap.h"
#include "overlay_96_ov96_icon_work.h"
#include "pokemon_icon_idx.h"
#include "sprite.h"

void ov96_021FC450(Ov96IconWork *work, Ov96IconSpec *specs) {
    SpriteTemplate template;
    NNSG2dCharacterData *charData;
    NARC *narc;
    void *buf;
    s32 i;
    u16 species;
    u8 form;
    u8 plt;
    u32 offset;
    s32 x;

    narc = NARC_New(NARC_poketool_icongra_poke_icon, work->heapID);
    buf = Heap_AllocAtEnd(work->heapID, 0x1000U);
    ov96_021FC5E0(&template, work, &work->resHeader, NNS_G2D_VRAM_TYPE_2DSUB);
    offset = 0x200;
    x = 0x10;
    for (i = 0; i < 12; i++, x += 0x20) {
        work->icons[i].monIcon = Sprite_CreateAffine(&template);
        Sprite_SetAnimActiveFlag(work->icons[i].monIcon, TRUE);
        Sprite_SetAnimCtrlSeq(work->icons[i].monIcon, i);
        work->icons[i].teamMark = Sprite_CreateAffine(&template);
        Sprite_SetAnimCtrlSeq(work->icons[i].teamMark, i / 3 + 12);
        work->icons[i].rank = Sprite_CreateAffine(&template);
        if (i < 8) {
            Sprite_SetAnimCtrlSeq(work->icons[i].rank, i + 16);
        }
        species = specs[i].species;
        form = specs[i].form;
        if (species == 0) {
            Sprite_SetDrawFlag(work->icons[i].monIcon, FALSE);
            Sprite_SetDrawFlag(work->icons[i].teamMark, FALSE);
            Sprite_SetDrawFlag(work->icons[i].rank, FALSE);
        }
        Sprite_SetDrawPriority(work->icons[i].monIcon, 2U);
        Sprite_SetDrawPriority(work->icons[i].teamMark, 3U);
        Sprite_SetDrawPriority(work->icons[i].rank, 1U);
        ov96_021FC248(work, i, x);
        Sprite_SetDrawFlag(work->icons[i].rank, TRUE);
        NARC_ReadWholeMember(narc, GetMonIconNaixEx(species, 0, form), buf);
        NNS_G2dGetUnpackedBGCharacterData(buf, &charData);
        plt = GetMonIconPaletteEx(species, form, 0) + 3;
        DC_FlushRange(charData->pRawData, 0x200);
        GXS_LoadOBJ(charData->pRawData, offset, 0x200);
        Sprite_SetPaletteOverride(work->icons[i].monIcon, plt);
        offset += 0x200;
    }
    Heap_Free(buf);
    NARC_Delete(narc);
}
