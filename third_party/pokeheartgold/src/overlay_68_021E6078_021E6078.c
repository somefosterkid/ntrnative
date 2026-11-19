#include "global.h"

#include "menu_input_state.h"
#include "yes_no_prompt.h"
#include "overlay_68_move_relearner.h"

s32 ov68_021E7BC8(s32);

s32 ov68_021E6078(MoveRelearner *arg0);

s32 ov68_021E6078(MoveRelearner *arg0) {
    YesNoPromptTemplate template;
    MI_CpuFill8(&template, 0U, sizeof(YesNoPromptTemplate));
    template.bgConfig = arg0->bgConfig;
    template.bgId = 4;
    template.tileStart = 1;
    template.plttSlot = 0xB;
    template.x = 0x1A;
    template.y = 0xA;
    template.ignoreTouchFlag = (u8)arg0->menuInputState;
    template.initialCursorPos = 0;
    YesNoPrompt_InitFromTemplate(arg0->yesNoPrompt, &template);
    ov68_021E7BC8(1);
    return 4;
}

#if defined(SDK_PORT) || defined(NONMATCHING)
typedef s32 (*MoveRelearnerYesNoHandler)(MoveRelearner *);

typedef struct MoveRelearnerYesNoHandlers {
    MoveRelearnerYesNoHandler yes;
    MoveRelearnerYesNoHandler no;
} MoveRelearnerYesNoHandlers;

s32 ov68_021E6D00(MoveRelearner *);
s32 ov68_021E6D20(MoveRelearner *);
s32 ov68_021E6D40(MoveRelearner *);
s32 ov68_021E6D4C(MoveRelearner *);
s32 ov68_021E6D58(MoveRelearner *);
s32 ov68_021E6D64(MoveRelearner *);
s32 ov68_021E6D80(MoveRelearner *);
s32 ov68_021E6D9C(MoveRelearner *);
s32 ov68_021E6DB8(MoveRelearner *);
s32 ov68_021E6DD0(MoveRelearner *);

// Indexed by MoveRelearner.question (ov68_021E7D3C on the DS: the yes handler is the word at +0, the no handler the word at +4).
static const MoveRelearnerYesNoHandlers sYesNoHandlers[] = {
    { ov68_021E6D00, ov68_021E6D20 },
    { ov68_021E6D40, ov68_021E6D4C },
    { ov68_021E6D58, ov68_021E6D64 },
    { ov68_021E6D80, ov68_021E6D9C },
    { ov68_021E6DB8, ov68_021E6DD0 },
};
#else
extern u32 ov68_021E7D3C;

extern u32 ov68_021E7D40;
#endif

u32 ov68_021E60D8(MoveRelearner *arg0);

u32 ov68_021E60D8(MoveRelearner *arg0) {
    YesNoResponse YVar1;
    u32 uVar2;
    MenuInputState MVar3;

    YVar1 = YesNoPrompt_HandleInput(arg0->yesNoPrompt);
    switch (YVar1) {
    case YESNORESPONSE_YES:
#if defined(SDK_PORT) || defined(NONMATCHING)
        uVar2 = sYesNoHandlers[arg0->question].yes(arg0);
#else
        uVar2 = (**(u32(**)())(&ov68_021E7D3C + (u32)arg0->question * 2))(arg0);
#endif
        ov68_021E7BC8(0);
        break;
    case YESNORESPONSE_NO:
#if defined(SDK_PORT) || defined(NONMATCHING)
        uVar2 = sYesNoHandlers[arg0->question].no(arg0);
#else
        uVar2 = (**(u32(**)())(&ov68_021E7D40 + (u32)arg0->question * 2))(arg0);
#endif
        ov68_021E7BC8(0);
        break;
    default:
        return 4;
    }
    MVar3 = YesNoPrompt_IsInTouchMode(arg0->yesNoPrompt);
    arg0->menuInputState = MVar3;
    YesNoPrompt_Reset(arg0->yesNoPrompt);
    return uVar2;
}
