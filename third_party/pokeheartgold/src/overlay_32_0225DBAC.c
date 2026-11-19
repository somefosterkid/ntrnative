#include "global.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "pm_string.h"
#include "unk_0200A090.h"
#include "sprite_resources_header.h"
#include "overlay_32.h"
#include "unk_02009D48.h"

void ov32_0225DBAC(SpriteResource **resObjs, GF_2DGfxResMan **resMans, SpriteResourcesHeader *header, int vramTransfer) {
    CreateSpriteResourcesHeader(header, GF2DGfxResObj_GetResID(resObjs[0]), GF2DGfxResObj_GetResID(resObjs[1]), GF2DGfxResObj_GetResID(resObjs[2]), GF2DGfxResObj_GetResID(resObjs[3]), 0xffffffff, 0xffffffff, 0, vramTransfer, resMans[0], resMans[1], resMans[2], resMans[3], NULL, NULL);
}
