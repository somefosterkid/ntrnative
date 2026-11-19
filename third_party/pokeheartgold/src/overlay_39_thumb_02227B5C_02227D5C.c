#include "global.h"
struct UnkStruct_ov62_022349A8;

#include "message_format.h"
#include "msgdata.h"
#include "overlay_39_thumb.h"
#include "pm_string.h"
#include "overlay_39_unk_struct_ov62_022349_a8.h"

static s32 ov39_02227DE4(void *arg0);

s32 ov39_02227D50(void *, String *);

void ov39_02227D5C(struct UnkStruct_ov62_022349A8 *arg0, s32 arg1, s32 arg2);

void ov39_02227D5C(UnkStruct_ov62_022349A8 *arg0, s32 arg1, s32 num) {
    String *string;
    s32 strno;

    strno = arg1;
    if (strno == -1) {
        strno = 0xB;
    }
    BufferIntegerAsString(*((MessageFormat * *)&arg0->unk_3F4), 0U, num, 5U, PRINTING_MODE_LEADING_ZEROS, 1);
    string = NewString_ReadMsgData(*((MsgData * *)&arg0->unk_3F0), strno);
    StringExpandPlaceholders(*((MessageFormat * *)&arg0->unk_3F4), *((String * *)&arg0->unk_3F8), string);
    String_Delete(string);
    ov39_02227D50(arg0, *((String * *)&arg0->unk_3F8));
}

s32 ov39_02227DB8(void *arg0);

s32 ov39_02227DB8(void *arg0) {
    if ((ov39_02227DE4(arg0) == 0) && (*((s32 *)&((UnkStruct_ov62_022349A8 *)arg0)->unk_3E4) == 0x59DC) && (*((s32 *)&((UnkStruct_ov62_022349A8 *)arg0)->unk_3E8) == 0x59DC)) {
        return 1;
    }
    return 0;
}

static s32 ov39_02227DE4(void *arg0) {
    return *((s32 *)&((UnkStruct_ov62_022349A8 *)arg0)->unk_3B8);
}
