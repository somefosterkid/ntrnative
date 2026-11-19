#include "global.h"
#include "bg_window.h"
#include "filesystem_files_def.h"
#include "overlay_28.h"
#include "pm_string.h"
#include "sprite_resources_header.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"

void ov28_0225D9BC(SpriteResource **resObjs, GF_2DGfxResMan **resMans, SpriteResourcesHeader *header, int priority) {
    CreateSpriteResourcesHeader(header, GF2DGfxResObj_GetResID(resObjs[0]), GF2DGfxResObj_GetResID(resObjs[1]), GF2DGfxResObj_GetResID(resObjs[2]), GF2DGfxResObj_GetResID(resObjs[3]), 0xffffffff, 0xffffffff, 0, priority, resMans[0], resMans[1], resMans[2], resMans[3], NULL, NULL);
}
