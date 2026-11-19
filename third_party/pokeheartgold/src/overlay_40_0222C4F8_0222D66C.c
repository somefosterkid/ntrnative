#include "global.h"
struct UnkStruct_ov62_022312B0;
struct UnkStruct_ov62_0223118C;

#include "bg_window.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "pm_string.h"
#include "text.h"
#include "unk_02013534.h"
#include "overlay_40_unk_struct_ov62_0223118c.h"
#include "overlay_40_unk_struct_ov62_022312b0.h"

void ov40_0222D66C(struct UnkStruct_ov62_022312B0 *arg0, struct UnkStruct_ov62_0223118C *arg1, s32 arg2);

void ov40_0222D66C(UnkStruct_ov62_022312B0 *arg0, UnkStruct_ov62_0223118C *arg1, s32 strno) {
    Window window;
    String *string;

    string = NewString_ReadMsgData(*((MsgData * *)&arg1->unk_34), strno);
    InitWindow(&window);
    AddTextWindowTopLeftCorner(arg1->unk_10, &window, 0x14U, 2U, 0U, 0U);
    AddTextPrinterParameterizedWithColor(&window, FONT_SYSTEM, string, 0U, 0U, 0xFFU, 0xE0D00U, NULL);
    TextOBJ_CopyFromBGWindow(*((TextOBJ * *)&arg0->unk_0C), *((UnkStruct_02013910 * *)&arg0->unk_10), &window, HEAP_ID_109);
    String_Delete(string);
    RemoveWindow(&window);
}

void ov40_0222D6D0(void *arg0);

void ov40_0222D6D0(void *arg0) {
    sub_02013938(((UnkStruct_02013910 *)((UnkStruct_ov62_022312B0 *)arg0)->unk_10));
    TextOBJ_Destroy(((TextOBJ *)((UnkStruct_ov62_022312B0 *)arg0)->unk_0C));
    sub_02021B5C(arg0 + 0x10);
}
