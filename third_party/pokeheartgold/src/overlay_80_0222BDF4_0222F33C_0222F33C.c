#include "global.h"

#include "constants/heap.h"

#include "frontier/frontier_map.h"
#include "frontier/overlay_80_0222BDF4.h"

#include "filesystem.h"
#include "filesystem_files_def.h"
#include "palette.h"
#include "party_menu.h"
#include "sprite_system.h"
#include "overlay_80_app_0222f33c.h"

void ov80_0222F33C(FrontierMap *map) {
    NARC *narc;

    narc = NARC_New(NARC_graphic_plist_gra, HEAP_ID_101);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(map->paletteData, PLTTBUF_MAIN_OBJ, ((Ov80App_0222F33C *)map)->unk_34, ((Ov80App_0222F33C *)map)->unk_38, narc, (s32)sub_0207CAA0(), 0, 1, 1, 0x7D1);
    SpriteSystem_LoadCellResObjFromOpenNarc(((Ov80App_0222F33C *)map)->unk_34, ((Ov80App_0222F33C *)map)->unk_38, narc, (s32)sub_0207CAA4(), 0, 0x7D1);
    SpriteSystem_LoadAnimResObjFromOpenNarc(((Ov80App_0222F33C *)map)->unk_34, ((Ov80App_0222F33C *)map)->unk_38, narc, (s32)sub_0207CAA8(), 0, 0x7D1);
    SpriteSystem_LoadCharResObjAtEndWithHardwareMappingType(((Ov80App_0222F33C *)map)->unk_34, ((Ov80App_0222F33C *)map)->unk_38, NARC_graphic_plist_gra, (s32)sub_0207CA9C(), 0, 1, 0x7D9);
    NARC_Delete(narc);
}
