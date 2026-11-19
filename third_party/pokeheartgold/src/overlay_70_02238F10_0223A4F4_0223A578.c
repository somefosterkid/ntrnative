#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "item.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "string_util.h"

void ov70_02245084(Window *, String *, u32, u32, u32, u32);

extern void *ov70_02245910;

extern u32 ov70_0224649C;

void ov70_0223A578(MsgData *param_1, MsgData *param_2, MessageFormat *param_3, Window *param_4, BoxPokemon *param_5, s16 *param_6);

void ov70_0223A578(MsgData *param_1, MsgData *param_2, MessageFormat *param_3, Window *param_4, BoxPokemon *param_5, s16 *param_6) {
    String *string_04;
    String *string_01;
    String *string_02;
    String *string_03;
    String *string_00;
    String *string_05;
    String *string;
    void *new_var;
    String *dest;
    s8 sVar3;
    s16 sVar2;
    u32 uVar4;
    short cVar1;
    s32 iVar5;
    s32 iVar6;
    Window *window;
    unsigned short new_var2;
    string = String_New(0x16, HEAP_ID_61);
    dest = String_New(0x12, HEAP_ID_61);
    GetBoxMonData(param_5, 0x77, string);
    sVar2 = *param_6;
    sVar3 = *((s8 *)(((u8 *)param_6) + 2));
    new_var = param_6;
    new_var2 = *((s8 *)(new_var + 3));
    uVar4 = GetBoxMonData(param_5, 6, (void *)0x0);
    cVar1 = new_var2;
    string_00 = NewString_ReadMsgData(param_1, 0x49);
    iVar5 = sVar3 * 4;
    string_01 = NewString_ReadMsgData(param_1, *((s32 *)(((u8 *)(&ov70_02245910)) + iVar5)));
    string_02 = NewString_ReadMsgData(param_1, 0x6a);
    BufferIntegerAsString(param_3, 3, (s32)cVar1, 3, PRINTING_MODE_LEFT_ALIGN, 1);
    string_03 = ReadMsgData_ExpandPlaceholders(param_3, param_1, 0x6b, HEAP_ID_61);
    string_04 = NewString_ReadMsgData(param_2, (s32)sVar2);
    GetItemNameIntoString(dest, uVar4, HEAP_ID_61);
    string_05 = NewString_ReadMsgData(param_1, 0x3b);
    iVar6 = 0;
    window = param_4;
    do {
        FillWindowPixelBuffer(window, 0);
        iVar6 = iVar6 + 1;
        window = window + 1;
    } while (iVar6 < 6);
    ov70_02245084(param_4, string, 0, 0, 0, 0x10200);
    if (sVar3 != 3) {
        ov70_02245084(param_4, string_01, 0x40, 0, 0, *((u32 *)(((u8 *)(&ov70_0224649C)) + iVar5)));
    }
    ov70_02245084(param_4 + 1, string_04, 0, 0, 0, 0x10200);
    ov70_02245084(param_4 + 2, string_02, 0, 0, 0, 0x10200);
    ov70_02245084(param_4 + 3, string_03, 0, 0, 0, 0x10200);
    ov70_02245084(param_4 + 4, string_00, 0, 0, 0, 0xf0200);
    ov70_02245084(param_4 + 5, dest, 0, 0, 0, 0x10200);
    ov70_02245084(param_4 + 6, string_05, 0, 0, 0, 0xf0200);
    String_Delete(string_00);
    String_Delete(dest);
    String_Delete(string_02);
    String_Delete(string_03);
    String_Delete(string_01);
    String_Delete(string);
    String_Delete(string_04);
    String_Delete(string_05);
    return;
}
