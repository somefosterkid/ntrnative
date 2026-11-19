#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_96_ov96hurdle_hud.h"
#include "pokemon_icon_idx.h"

void ov96_021EB52C(void *param_1, s32 param_2, s32 param_3);

void ov96_021EB588(void *param_1, VecFx32 *param_2);

void ov96_0220831C(Ov96HurdleHud *hud, s32 param_2, s32 param_3);

void ov96_022080F4(Ov96HurdleHud *hud, u16 *param_2);

void ov96_022080F4(Ov96HurdleHud *hud, u16 *param_2) {
    NARC *narc;
    s32 i;
    s32 j;
    u16 species;
    u8 form;
    VecFx32 pos;

    narc = NARC_New(NARC_poketool_icongra_poke_icon, hud->heapId);
    GfGfxLoader_GXLoadPal(NARC_poketool_icongra_poke_icon, sub_02074490(), GF_PAL_LOCATION_SUB_OBJ, GF_PAL_SLOT_6_OFFSET, 0x60, hud->heapId);
    for (i = 0; i < 12; i++) {
        hud->iconFile[i] = Heap_AllocAtEnd(hud->heapId, 0x1000);
        form = param_2[2 * i + 1];
        species = param_2[2 * i];
        NARC_ReadWholeMember(narc, GetMonIconNaixEx(species, FALSE, form), hud->iconFile[i]);
        NNS_G2dGetUnpackedBGCharacterData(hud->iconFile[i], &hud->iconChar[i]);
        hud->iconPal[i] = GetMonIconPaletteEx(species, form, FALSE) + 6;
    }
    NARC_Delete(narc);
    for (i = 0; i < 4; i++) {
        ov96_0220831C(hud, (u8)i, 0);
        ov96_021EB52C(hud->players[i].unk_0C, 1, 1);
        ov96_021EB52C(hud->players[i].unk_14, 1, 1);
        pos.z = 0;
        pos.x = (0x70 + i * 0x28) << 12;
        pos.y = 0x350000;
        ov96_021EB588(hud->players[i].unk_0C, &pos);
        ov96_021EB588(hud->players[i].unk_14, &pos);
        ov96_021EB588(hud->players[i].unk_10, &pos);
        for (j = 0; j < 2; j++) {
            ov96_021EB588(hud->unk_17C[i][j], &pos);
        }
        pos.y -= 0x20000;
        ov96_021EB588(hud->badges[i].unk_0, &pos);
        ov96_021EB588(hud->badges[i].unk_4, &pos);
        ov96_021EB588(hud->badges[i].unk_8, &pos);
    }
}
