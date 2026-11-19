#include "global.h"

#include "overlay_25.h"
#include "poke_overlay.h"
#include "script.h"
#include "script_manager.h"
#include "field_system.h"
#include "field_types_def.h"
#include "overlay_01_022053EC.h"

FS_EXTERN_OVERLAY(OVY_25);

s32 ScrCmd_808(ScriptContext *ctx);

s32 ScrCmd_808(ScriptContext *ctx) {
    u16 trainerNum;

    trainerNum = FieldSystem_VarGet(ctx->fieldSystem, ScriptReadHalfword(ctx));
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_25), OVY_LOAD_ASYNC);
    TrainerHouse_StartBattle(ctx->fieldSystem, (u32)trainerNum);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_25));
    return 1;
}

void ov01_02205424(FieldSystem *fieldSystem) {
    fieldSystem->followMon.unk15 = 1;
}
