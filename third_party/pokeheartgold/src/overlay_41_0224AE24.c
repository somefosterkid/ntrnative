#include "global.h"

#include "error_handling.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_41_sprite_resource_collection.h"

u8 ov41_0224AE24(Window *arg0, enum NarcId arg1, s32 arg2, s32 arg3, u32 arg4, u32 arg5, u32 arg6, u32 arg7);

struct SpriteResourceCollection;
typedef struct {
    SpriteResource *unk_00[4];
    Sprite *unk_10[20];
    int unk_60;
    int unk_64;
} UnkStruct_ov22_0225A914;
void ov41_0224AFF8(SpriteResource **param0, SpriteResourceCollection **param1, int param2, NARC *param3, int param4, int param5, int param6, int param7, int param8, int param9);
void ov41_0224B0B8(SpriteResource **param0, SpriteResourceCollection **param1, SpriteResourcesHeader *param2, int param3);
void ov41_0224B084(SpriteResource **param0, SpriteResourceCollection **param1);
void ov41_0224AED8(UnkStruct_ov22_0225A914 *param0, SpriteList *param1, SpriteResourceCollection **param2, int param3, NARC *param4);
void ov41_0224AF8C(UnkStruct_ov22_0225A914 *param0, int param1);
void ov41_0224AFD4(UnkStruct_ov22_0225A914 *param0, SpriteResourceCollection **param1);

u8 ov41_0224AE24(Window *window, enum NarcId narc, s32 fileId, s32 strno, u32 x, u32 y, u32 color, u32 textSpeed) {
    MsgData *msgData;
    String *string;
    u8 v0;

    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, narc, fileId, HEAP_ID_13);
    if (msgData == NULL) {
        GF_AssertFail();
    }
    string = NewString_ReadMsgData(msgData, strno);
    v0 = AddTextPrinterParameterizedWithColor(window, FONT_MESSAGE, string, x, y, textSpeed, color, NULL);
    String_Delete(string);
    DestroyMsgData(msgData);
    return v0;
}

u8 ov41_0224AE78(Window *arg0, enum NarcId arg1, s32 arg2, s32 arg3, u32 arg4, u32 arg5, u32 arg6, u32 arg7, String **arg8);

u8 ov41_0224AE78(Window *window, enum NarcId narc, s32 fileId, s32 arg3, u32 x, u32 y, u32 color, u32 textSpeed, String **arg8) {
    s32 strno;
    MsgData *msgData;
    u8 v0;

    strno = arg3;
    if (*arg8 != NULL) {
        GF_AssertFail();
    }
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, narc, fileId, HEAP_ID_13);
    if (msgData == NULL) {
        GF_AssertFail();
    }
    *arg8 = NewString_ReadMsgData(msgData, strno);
    v0 = AddTextPrinterParameterizedWithColor(window, FONT_MESSAGE, *arg8, x, y, textSpeed, color, NULL);
    DestroyMsgData(msgData);
    return v0;
}

void ov41_0224AED8(UnkStruct_ov22_0225A914 *param0, SpriteList *param1, SpriteResourceCollection **param2, int param3, NARC *param4) {
    SpriteResourcesHeader v0;
    SimpleSpriteTemplate v1;
    int v2, v3;

    ov41_0224AFF8(param0->unk_00, param2, 14, param4, 103, 225, 102, 101, 2, 2000);
    ov41_0224B0B8(param0->unk_00, param2, &v0, 0);

    v1.spriteList = param1;
    v1.header = &v0;
    v1.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    v1.priority = 0;
    v1.heapID = HEAP_ID_14;

    for (v2 = 0; v2 < (20 / 10); v2++) {
        v1.position.y = 104 + (18 * v2);
        v1.position.y *= ((fx32)0x0000000000001000L);
        v1.position.y += (512 * ((fx32)0x0000000000001000L));

        for (v3 = 0; v3 < 10; v3++) {
            v1.position.x = 38 + (18 * v3);
            v1.position.x *= ((fx32)0x0000000000001000L);

            param0->unk_10[(v2 * 10) + v3] = Sprite_Create(&v1);
            Sprite_SetAnimCtrlSeq(param0->unk_10[(v2 * 10) + v3], 1);

            if (((v2 * 10) + v3) >= param3) {
                Sprite_SetDrawFlag(param0->unk_10[(v2 * 10) + v3], 0);
            }
        }
    }
}

void ov41_0224AF8C(UnkStruct_ov22_0225A914 *param0, int param1) {
    int v0;

    if (param0->unk_60 < param1) {
        for (v0 = param0->unk_60; v0 < param1; v0++) {
            Sprite_SetAnimCtrlSeq(param0->unk_10[v0], 0);
        }
    } else if (param0->unk_60 > param1) {
        for (v0 = param0->unk_60 - 1; v0 >= param1; v0--) {
            Sprite_SetAnimCtrlSeq(param0->unk_10[v0], 1);
        }
    }

    param0->unk_60 = param1;
}

void ov41_0224AFD4(UnkStruct_ov22_0225A914 *param0, SpriteResourceCollection **param1) {
    int v0;

    for (v0 = 0; v0 < 20; v0++) {
        Sprite_Delete(param0->unk_10[v0]);
    }

    ov41_0224B084(param0->unk_00, param1);
}
