#include "global.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "pm_string.h"

struct SpriteResource;
struct SpriteResourceCollection;
struct SpriteResourcesHeader;
typedef struct SpriteResource SpriteResource;
typedef struct SpriteResourceCollection SpriteResourceCollection;
void DestroySingle2DGfxResObj(SpriteResourceCollection *spriteResources, SpriteResource *resource);
void SpriteTransfer_DeleteCharTransferTask(SpriteResource *resource);
void SpriteTransfer_DeletePlttTransferTask(SpriteResource *resource);
typedef struct SpriteResourcesHeader SpriteResourcesHeader;
int GF2DGfxResObj_GetResID(const SpriteResource *spriteRes);
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

void ov18_021F92DC(SpriteResource **param0, SpriteResourceCollection **param1);
void ov18_021F9310(SpriteResource **param0, SpriteResourceCollection **param1, SpriteResourcesHeader *param2, int param3);

void ov18_021F92DC(SpriteResource **param0, SpriteResourceCollection **param1) {
    SpriteTransfer_DeleteCharTransferTask(param0[0]);
    SpriteTransfer_DeletePlttTransferTask(param0[1]);
    DestroySingle2DGfxResObj(param1[0], param0[0]);
    DestroySingle2DGfxResObj(param1[1], param0[1]);
    DestroySingle2DGfxResObj(param1[2], param0[2]);
    DestroySingle2DGfxResObj(param1[3], param0[3]);
}

void ov18_021F9310(SpriteResource **param0, SpriteResourceCollection **param1, SpriteResourcesHeader *param2, int param3) {
    CreateSpriteResourcesHeader(param2, GF2DGfxResObj_GetResID(param0[0]), GF2DGfxResObj_GetResID(param0[1]), GF2DGfxResObj_GetResID(param0[2]), GF2DGfxResObj_GetResID(param0[3]), 0xffffffff, 0xffffffff, 0, param3, param1[0], param1[1], param1[2], param1[3], 0L, 0L);
}
