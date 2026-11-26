#include "global.h"
struct UnkStruct_0208C06C;
#include "constants/heap.h"
#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_40_unk_struct_0208_c06_c.h"
#include "overlay_40_unk_struct_ov62_02241204.h"

void ov40_0222E09C(UnkStruct_ov62_022323B8 *param0, UnkStruct_0208C06C *param1);

void ov40_0222E510(UnkStruct_ov62_022323B8 *param0, UnkStruct_0208C06C *param1);

void ov40_0222E690(UnkStruct_ov62_022323B8 *param0, UnkStruct_0208C06C *param1);

void ov40_0222E79C(UnkStruct_ov62_022323B8 *arg0, UnkStruct_0208C06C *arg1);

MessageFormat *ov40_0222DAB0(s32);
u8 sub_02031620(void *);
u8 sub_0203162C(struct UnkStruct_02030A80_t *);
void ov40_0222E7F0(u8 *arg0, struct UnkStruct_0208C06C *arg1);

void ov40_0222E79C(UnkStruct_ov62_022323B8 *arg0, UnkStruct_0208C06C *arg1) {
    ov40_0222E09C(arg0, arg1);
    ov40_0222E690(arg0, arg1);
    ov40_0222E510(arg0, arg1);
}

void ov40_0222E4A4(UnkStruct_ov62_022323B8 *param0, UnkStruct_0208C06C *param1);

void ov40_0222E4D4(void *arg0, UnkStruct_0208C06C *arg1);

void ov40_0222E624(void *arg0, UnkStruct_0208C06C *arg1);

void ov40_0222E7DC(void *arg0, s32 arg1);

void ov40_0222E7B8(UnkStruct_ov62_022323B8 *arg0, UnkStruct_0208C06C *arg1);

void ov40_0222E7B8(UnkStruct_ov62_022323B8 *arg0, UnkStruct_0208C06C *arg1) {
    ov40_0222E4A4(arg0, arg1);
    ov40_0222E7DC(arg0, 0);
    ov40_0222E624(arg0, arg1);
    ov40_0222E4D4(arg0, arg1);
}

s32 ov40_0222E4C4(void *arg0, s32 arg1);

s32 ov40_0222E618(void *, s32);

void ov40_0222E7DC(void *arg0, s32 arg1) {
    ov40_0222E4C4(arg0, arg1);
    ov40_0222E618(arg0, arg1);
}

void ov40_0222E7F0(u8 *arg0, UnkStruct_0208C06C *arg1) {
    String *string;
    u32 countryId;
    MessageFormat *messageFormat;
    String *src;
    Window *window;
    u32 v0;
    u32 cityId;

    v0 = sub_02031620(*((void **)&arg1->unk_88C[0]));
    countryId = v0;
    cityId = sub_0203162C(*((void **)&arg1->unk_88C[0]));
    window = (struct Window *)(arg0 + 0x18);
    if ((v0 == 0) && (cityId == 0)) {
        string = NewString_ReadMsgData(arg1->unk_14.unk_34, 0x7D);
    } else {
        messageFormat = ov40_0222DAB0(0x6D);
        if (cityId != 0) {
            string = String_New(0xFFU, HEAP_ID_109);
            src = NewString_ReadMsgData(arg1->unk_14.unk_34, 0x17);
            BufferCityName(messageFormat, 0U, countryId, cityId);
            StringExpandPlaceholders(messageFormat, string, src);
        } else {
            string = String_New(0xFFU, HEAP_ID_109);
            src = NewString_ReadMsgData(arg1->unk_14.unk_34, 0x16);
            BufferCountryName(messageFormat, 0U, countryId);
            StringExpandPlaceholders(messageFormat, string, src);
        }
        String_Delete(src);
        MessageFormat_Delete(messageFormat);
    }
    AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, string, 0U, 0x10U, 0xFFU, 0xF0D00U, NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(string);
}
