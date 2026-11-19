#include "global.h"

#include "bg_window.h"
#include "msgdata.h"
#include "options.h"
#include "player_data.h"
#include "pm_string.h"
#include "yes_no_prompt.h"
#include "overlay_48_unk_struct_ov69_0225ddc8.h"

void ov48_0225B0D4(MsgData **msgDataPtr, s32 strno, String *dest);

void ov48_02259EAC(void *arg0, BgConfig **arg1, void *arg2, SaveData *arg3, enum HeapID arg4);

void ov48_02259EAC(void *arg0, BgConfig **arg1, void *arg2, SaveData *saveData, enum HeapID heapID) {
    memset(arg0, 0, 0x2CU);
    *(s32 *)((u8 *)arg0 + 8) = (s32)Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(saveData));
    *(String **)((u8 *)arg0 + 0xC) = String_New(0x80U, heapID);
    AddWindowParameterized(*arg1, arg0 + 0x10, 1U, 2U, 1U, 0x1BU, 4U, 1U, 0x34U);
    FillWindowPixelBuffer(arg0 + 0x10, 0xFU);
    ov48_0225B0D4(arg2, 5, *(String **)((u8 *)arg0 + 0xC));
    *((YesNoPrompt **)&((UnkStruct_ov69_0225DDC8 *)arg0)->unk_20) = YesNoPrompt_Create(heapID);
}
