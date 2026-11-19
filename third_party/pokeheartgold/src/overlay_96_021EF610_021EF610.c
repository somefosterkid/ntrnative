#include "global.h"

#include "message_format.h"
#include "overlay_98.h"
#include "pm_string.h"
#include "string_util.h"
#include "overlay_96_unk_struct_ov96_021_ef610.h"
#include "overlay_98_ov98text_sys.h"

typedef struct ListMenuUI ListMenuUI;

typedef struct FieldMsgBG FieldMsgBG;

typedef struct ListMenuRequest ListMenuRequest;

typedef struct ListMenuOption ListMenuOption;

struct MessageFormat;

struct MessageFormatAttrs;

struct MessageFormatFields;

struct Ov98TextSys;

struct String;

struct UnkStruct_ov96_021EF610;

void ov96_021EE6A0(void *arg0);

u16 ov96_021EF5F4(u32 *param_1, s32 param_2);

void ov96_021EF610(UnkStruct_ov96_021EF610 *param0);

void ov96_021EF610(UnkStruct_ov96_021EF610 *param0) {
    u8 idx = param0->unk_20;
    u8 value = param0->unk_22 + 1;
    int record;
    MessageFormat *messageFormat;

    if (param0->unk_28 == 0) {
        ov98_0221EEEC(param0->textSys, (s32)(5), 5);
    }
    ov98_0221EBD8(param0->textSys, (s32)(0), (s32)(0xc5), (s32)(0));
    ov98_0221EBD8(param0->textSys, (s32)(2), (s32)(0xd0), (s32)(0));
    ov98_0221EC08(param0->textSys, 1, idx + 0xc6, 0, 0);
    ov98_0221EC08(param0->textSys, 3, idx * 3 + 0xd1, 0, 0);
    ov98_0221ECD0(param0->textSys, (s32)(5), (s32)(0xef), (s32)(value), 1, 0);
    if (param0->unk_24 == 0) {
        ov98_0221EBEC(param0->textSys, 4, 0xf0, 0, 0, 4);
    }
    ov96_021EE6A0(param0->unk_34);
    if (param0->unk_28 != 0) {
        record = ov96_021EF5F4((u32 *)(param0), param0->unk_20);
        if (record == 0xFFFF) {
            record = 0;
        }
        if (param0->unk_20 == 0) {
            messageFormat = (struct MessageFormat *)(ov98_0221EEFC(param0->textSys));
            BufferIntegerAsString(messageFormat, 0, record / 30, 3, (PrintingMode)(0), 1);
            BufferIntegerAsString(messageFormat, 1, (record % 30) * 10 / 30, 1, (PrintingMode)(0), 1);
        } else if (param0->unk_20 == 6) {
            ov98_0221EDA4(param0->textSys, (u32)record >> 10, 2, 0);
            ov98_0221EDA4(param0->textSys, (record % 1024) * 10 / 1024, 1, 1);
        } else {
            ov98_0221EDA4(param0->textSys, record, 3, 0);
        }
        ov98_0221EBD8(param0->textSys, (s32)(6), (s32)(0xb0), (s32)(0));
        ov98_0221ED3C(param0->textSys, 7, idx + 0xb1);
    }
}
