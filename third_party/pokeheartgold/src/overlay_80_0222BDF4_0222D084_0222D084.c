#include "global.h"

#include "frontier/frontier_script_context.h"
#include "frontier/overlay_80_0222BDF4.h"

#include "message_format.h"
#include "pm_string.h"

String *ov80_0222D0D4(u32, u32);

u32 FrtCmd_126(FrontierScriptContext *param_1);

u32 FrtCmd_126(FrontierScriptContext *param_1) {
    u8 bVar1;
    u8 bVar2;
    u16 uVar3;
    u32 uVar4;
    String *string;
    u8 *pbVar5;

    pbVar5 = (unsigned char *)(param_1->scriptPtr);
    param_1->scriptPtr = pbVar5 + 1;
    bVar1 = *pbVar5;
    uVar4 = FrontierScript_ReadVar(param_1);
    uVar3 = FrontierScriptContext_ReadHalfWord(param_1);
    pbVar5 = (unsigned char *)(param_1->scriptPtr);
    param_1->scriptPtr = pbVar5 + 1;
    bVar2 = *pbVar5;
    string = (String *)ov80_0222D0D4(uVar4, 0xb);
    BufferString(param_1->frontierSystem->unk44, (u32)bVar1, string, (u32)uVar3, (u32)bVar2, 2);
    String_Delete(string);
    return 0;
}
