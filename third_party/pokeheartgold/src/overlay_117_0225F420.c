#include "global.h"

#include "bg_window.h"
#include "field_system.h"
#include "field_types_def.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_01_encounter_effect.h"
#include "overlay_117.h"

void ov117_0225F420(EncounterEffect *encEffect, u8 bgId, s32 screenFileId) {
    NNSG2dScreenData *spC;
    void *temp_r6;

    temp_r6 = GfGfxLoader_GetScrnDataFromOpenNarc(encEffect->narc, screenFileId, 0, &spC, HEAP_ID_FIELD1);
    LoadRectToBgTilemapRect(encEffect->fieldSystem->bgConfig, bgId, spC->rawData, 0U, 0U, spC->screenWidth / 8, spC->screenHeight / 8);
    Heap_Free(temp_r6);
    BgCommitTilemapBufferToVram(encEffect->fieldSystem->bgConfig, bgId);
}
