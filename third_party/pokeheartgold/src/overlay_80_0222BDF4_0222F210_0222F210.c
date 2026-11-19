#include "global.h"

#include "frontier/frontier_map.h"
#include "frontier/overlay_80_0222BDF4.h"

#include "filesystem_files_def.h"
#include "palette.h"
#include "pokemon_icon_idx.h"
#include "sprite_system.h"
#include "overlay_80_app_0222f210.h"

void ov80_0222F210(FrontierMap *map) {
    SpriteSystem_LoadPaletteBuffer(map->paletteData, PLTTBUF_MAIN_OBJ, ((Ov80App_0222F210 *)map)->unk_34, ((Ov80App_0222F210 *)map)->unk_38, NARC_poketool_icongra_poke_icon, (s32)sub_02074490(), 0, 3, 1, 0x7D0);
    SpriteSystem_LoadCellResObj(((Ov80App_0222F210 *)map)->unk_34, ((Ov80App_0222F210 *)map)->unk_38, NARC_poketool_icongra_poke_icon, (s32)sub_02074498(), 0, 0x7D0);
    SpriteSystem_LoadAnimResObj(((Ov80App_0222F210 *)map)->unk_34, ((Ov80App_0222F210 *)map)->unk_38, NARC_poketool_icongra_poke_icon, (s32)sub_020744A4(), 0, 0x7D0);
}
