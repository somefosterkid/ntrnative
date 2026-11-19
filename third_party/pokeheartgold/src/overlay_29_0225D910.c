#include "global.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "pm_string.h"
#include "unk_0200A090.h"
#include "overlay_29_number_entry_data.h"
#include "sprite_resources_header.h"
#include "overlay_41_sprite_resource_collection.h"

struct SpriteResourceCollection;
struct SpriteResourcesHeader;
void CreateSpriteResourcesHeader(SpriteResourcesHeader *resourceHeader,
    int charResourceID,
    int plttResourceID,
    int cellResourceID,
    int animResourceID,
    int mcellResourceID,
    int manimResourceID,
    BOOL vramTransfer,
    int priority,
    SpriteResourceCollection *charResources,
    SpriteResourceCollection *plttResources,
    SpriteResourceCollection *cellResources,
    SpriteResourceCollection *animResources,
    SpriteResourceCollection *mcellResources,
    SpriteResourceCollection *manimResources);

void ov29_0225D910(SpriteResource **param0, SpriteResourceCollection **param1, SpriteResourcesHeader *param2, int param3);

void ov29_0225D910(SpriteResource **param0, SpriteResourceCollection **param1, SpriteResourcesHeader *param2, int param3) {
    CreateSpriteResourcesHeader(param2, GF2DGfxResObj_GetResID(param0[0]), GF2DGfxResObj_GetResID(param0[1]), GF2DGfxResObj_GetResID(param0[2]), GF2DGfxResObj_GetResID(param0[3]), 0xffffffff, 0xffffffff, 0, param3, param1[0], param1[1], param1[2], param1[3], 0L, 0L);
}
