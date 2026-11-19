#include "global.h"

#include "frontier/frontier_script_context.h"

#include "overlay_80_02235390.h"
#include "sound.h"

s32 Sound_SetFieldBGM(u16);

s32 FrtCmd_090(FrontierScriptContext *ctx) {
    u32 seqno;

    seqno = FrontierScriptContext_ReadHalfWord(ctx);
    Sound_SetFieldBGM(seqno);
    PlayBGM(seqno);
    return 0;
}
