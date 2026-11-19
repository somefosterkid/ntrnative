#include "global.h"

#include "overlay_75.h"
#include "unk_02037C94.h"
#include "yes_no_prompt.h"
#include "overlay_75_unk_struct_ov98_02247704.h"

s32 ov75_02246BCC(void *, s32, s32);

s32 ov75_02247854(void *, s32, s32);

s32 ov75_0224785C(void *);

s32 ov75_022494CC(void *, void *, s32, s32, u16);

s32 ov75_02248684(void *arg0) {
    enum YesNoResponse v0;

    v0 = YesNoPrompt_HandleInput(((YesNoPrompt *)((UnkStruct_ov98_02247704 *)arg0)->unk_88));
    if (v0 == YESNORESPONSE_YES) {
        YesNoPrompt_Destroy(((YesNoPrompt *)((UnkStruct_ov98_02247704 *)arg0)->unk_88));
        ov75_022494CC(arg0, *((void **)&((UnkStruct_ov98_02247704 *)arg0)->unk_2C), 1, 1, 0xF0F);
        ov75_02247854(arg0, 0x22, 0xC);
        ov75_0224785C(arg0);
    } else if (v0 == YESNORESPONSE_NO) {
        YesNoPrompt_Destroy(((YesNoPrompt *)((UnkStruct_ov98_02247704 *)arg0)->unk_88));
        sub_0203957C();
        ov75_02246BCC(*(void **)arg0, 6, 0);
        *((s32 *)&((UnkStruct_ov98_02247704 *)arg0)->unk_08) = 0x1D;
    }
    return 0;
}
