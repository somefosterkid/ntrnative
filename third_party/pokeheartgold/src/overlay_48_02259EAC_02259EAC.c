#include "global.h"

#include "bg_window.h"
#include "msgdata.h"
#include "options.h"
#include "player_data.h"
#include "pm_string.h"
#include "yes_no_prompt.h"
#include "overlay_48_unk_struct_ov69_0225ddc8.h"

void ov48_0225B0D4(MsgData **msgDataPtr, s32 strno, String *dest);

void ov48_02259EAC(UnkStruct_ov69_0225DDC8 *arg0, BgConfig **arg1, void *arg2, SaveData *arg3, enum HeapID arg4);

void ov48_02259EAC(UnkStruct_ov69_0225DDC8 *arg0, BgConfig **arg1, void *arg2, SaveData *saveData, enum HeapID heapID) {
#if defined(SDK_PORT) || defined(NONMATCHING)
    memset(arg0, 0, sizeof(UnkStruct_ov69_0225DDC8));
#else
    // the DS clears 0x2C bytes: 8 past the end of this 0x24-byte struct
    memset(arg0, 0, 0x2CU);
#endif
    arg0->unk_08 = Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(saveData));
    arg0->unk_0C = String_New(0x80U, heapID);
    AddWindowParameterized(*arg1, &arg0->unk_10, 1U, 2U, 1U, 0x1BU, 4U, 1U, 0x34U);
    FillWindowPixelBuffer(&arg0->unk_10, 0xFU);
    ov48_0225B0D4(arg2, 5, arg0->unk_0C);
    arg0->unk_20 = YesNoPrompt_Create(heapID);
}
