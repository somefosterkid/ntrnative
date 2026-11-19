#include "global.h"

#include "unk_02095EAC.h"
#include "unk_02035900.h"
#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "render_window.h"
#include "text.h"
#include "unk_02034354.h"

BOOL sub_02037108(int cmd, const void *data, int size);

BOOL sub_0203783C(void);

BOOL sub_02037854(void);

void ov37_021E755C(UnkStruct_02095EAC *param0);

u32 ov37_021E75C4(void);
void ov37_021E75E0(void);
u32 ov37_021E75E8(void *arg0);
void ov37_021E762C(UnkStruct_02095EAC *arg0, s32 arg1, u32 arg2);

void ov37_021E755C(UnkStruct_02095EAC *param0) {
    if (sub_0203769C() == 0) {
        if (sub_0203783C()) {
            param0->unk_43DC.unk_09 = param0->unk_9458;
            param0->unk_9421[0] = param0->unk_43DC;
            sub_02037108(120, param0->unk_9421, 10 * 5);
        }
    } else {
        if (sub_02037854()) {
            sub_02037030(119, &param0->unk_43DC, 10);
        }
    }
}

u32 ov37_021E75C4(void) {
    u32 a0;
    u32 v0;

    v0 = 0;
    a0 = 0;
    do {
        if (sub_02034818(a0) != NULL) {
            v0 += 1;
        }
        a0 += 1;
    } while ((s32)a0 < 5);
    return v0;
}

void ov37_021E75E0(void) {
    ov37_021E75C4();
}

u32 ov37_021E75E8(void *arg0) {
    s32 v2;
    u32 a0;
    u32 v3;
    void *v0;
    void *v1;

    v1 = arg0;
    v3 = 0;
    a0 = 0;
    v0 = v1;
    do {
        *(PlayerProfile **)((u8 *)v0 + offsetof(UnkStruct_02095EAC, unk_398[0][1])) = (PlayerProfile *)*(PlayerProfile **)((u8 *)v0 + offsetof(UnkStruct_02095EAC, unk_398));
        *(PlayerProfile **)((u8 *)v0 + offsetof(UnkStruct_02095EAC, unk_398)) = sub_02034818(a0);
        a0 += 1;
        v0 += sizeof(((UnkStruct_02095EAC *)0)->unk_398[0]);
    } while ((s32)a0 < 5);
    v2 = 0;
    do {
        if (*(s32 *)((u8 *)v1 + offsetof(UnkStruct_02095EAC, unk_398[0][1])) != *(s32 *)((u8 *)v1 + offsetof(UnkStruct_02095EAC, unk_398))) {
            v3 = 1;
        }
        v2 += 1;
        v1 += sizeof(((UnkStruct_02095EAC *)0)->unk_398[0]);
    } while (v2 < 5);
    return v3;
}

void ov37_021E762C(UnkStruct_02095EAC *arg0, s32 strno, u32 arg2) {
    String *string;
    u32 textSpeed;

    textSpeed = arg2;
    string = String_New(0x50U, HEAP_ID_39);
    ReadMsgDataIntoString(arg0->unk_10, strno, string);
    StringExpandPlaceholders(arg0->unk_0C, arg0->unk_2C, string);
    String_Delete(string);
    FillWindowPixelBuffer(&arg0->unk_33C, 0xFU);
    DrawFrameAndWindow2(&arg0->unk_33C, 0, 1U, 0xAU);
    if (textSpeed == 0) {
        textSpeed = 0;
    }
    arg0->unk_30 = (s32)AddTextPrinterParameterized(&arg0->unk_33C, FONT_MESSAGE, arg0->unk_2C, 0U, 0U, textSpeed, NULL);
    if (textSpeed == 0) {
        arg0->unk_30 = 0xFF;
    }
}
