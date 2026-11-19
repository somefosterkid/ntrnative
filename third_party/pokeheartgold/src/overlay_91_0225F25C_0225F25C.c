#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "pm_string.h"
#include "sprite.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_91_unk_struct_ov115_02261_c18.h"
#include "overlay_91_unk_struct_ov115_02261_adc.h"
#include "struct_defs/resdat_narc_structs.h"
#include "msgdata.h"
#include "overlay_41_sprite_resource_collection.h"
#include "field/overlay_01_021FB878.h"
#include "unk_02013534.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "touch_pad_data_buffer.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

typedef struct CharTransferAllocation CharTransferAllocation;

typedef struct UnkStruct_ov115_02260D78_t UnkStruct_ov115_02260D78;

struct MessageBankEntry {
    u32 offset;
    u32 length;
};

struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
};

struct G3DPipelineBuffers {
    enum HeapID heapID;
    void *paletteBuffer;
    void *textureBuffer;
};

struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
};

struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
};

struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
};

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 padding_02[2];
} UnkStruct_ov115_022612BC;

typedef struct {
    s32 unk_00;
    s32 unk_04;
    s32 unk_08;
    s32 unk_0C;
    s32 unk_10;
    s32 unk_14;
} UnkStruct_ov115_02262DC4;

typedef struct {
    u32 unk_00;
    UnkStruct_ov115_02262DC4 *unk_04;
    const PlayerProfile *unk_08[4];
    u32 unk_18[4];
} UnkStruct_ov115_02262F50;

typedef struct {
    u32 unk_00[4];
} UnkStruct_ov115_02265AD4;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    VecFx32 unk_04;
    fx32 unk_10;
    VecFx32 unk_14;
    s32 unk_20;
    BOOL unk_24;
} UnkStruct_ov115_02262134;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u16 unk_02;
    UnkStruct_ov115_02262134 unk_04;
    VecFx32 unk_2C;
    s16 unk_38;
    u16 unk_3A;
    BOOL unk_3C;
    fx32 unk_40;
} UnkStruct_ov115_022622F8;

typedef struct {
    u32 unk_00;
    NNSG2dSVec2 unk_04[8];
    u16 unk_24;
    u16 unk_26;
    u32 unk_28;
} UnkStruct_ov115_02262CA4;

typedef struct {
    UnkStruct_ov115_02262DC4 unk_00;
    UnkStruct_ov115_02262DC4 unk_18;
    UnkStruct_ov115_02262DC4 unk_30;
    s32 unk_48;
} UnkStruct_ov115_02262E64;

typedef struct {
    SpriteResource *unk_00[4];
    SpriteResourcesHeader unk_10;
    Sprite *unk_34;
    u8 unk_38;
    u8 unk_39;
    u8 unk_3A;
    u8 unk_3B;
    s32 unk_3C;
} UnkStruct_ov115_02263130;

typedef struct {
    u32 unk_00[4];
    u32 unk_10;
    u32 unk_14;
} UnkStruct_ov115_022632C8;

typedef struct {
    u16 unk_00;
    u8 unk_02;
    s8 unk_03;
} UnkStruct_ov115_022658E0;

struct UnkStruct_ov115_02260D78_t {
    u32 unk_00;
    BOOL unk_04;
    s32 unk_08;
    s32 unk_0C;
    u32 unk_10;
    u32 unk_14;
    s32 unk_18;
    s32 unk_1C;
    UnkStruct_ov115_02262CA4 unk_20;
    UnkStruct_ov115_022622F8 unk_4C[96];
    UnkStruct_ov115_02261C18 unk_19CC;
    UnkStruct_ov115_02261ADC unk_1AB4;
    UnkStruct_ov115_02265AD4 unk_8680;
    TouchPadDataBuffer unk_8690;
    UnkStruct_ov115_02262E64 unk_86D4;
    const UnkStruct_ov115_02262F50 *unk_8720;
    u16 unk_8724;
    u16 unk_8726;
    UnkStruct_ov115_022612BC unk_8728;
    BOOL unk_872C;
    UnkStruct_ov115_02263130 unk_8730;
    UnkStruct_ov115_022632C8 unk_8770;
    UnkStruct_ov115_022658E0 unk_8788;
};

void ov91_0225D500(UnkStruct_ov115_02260D78 *param0, u16 param1, u16 param2, u16 param3, u16 param4, u32 param5);

void ov91_0225E62C(u8 *arg0);

void ov91_02260CB4(void *arg0);

void ov91_02260D00(void *arg0);

void ov91_0225E40C(UnkStruct_ov115_02260D78 *);

s32 ov91_0225E6D0(UnkStruct_ov115_02260D78 *, UnkStruct_ov115_02260D78 *);

void ov91_02260CE0(void *, u32, s32);

extern const s16 ov91_02261C84[];

BOOL ov91_0225F25C(UnkStruct_ov115_02263130 *param0, UnkStruct_ov115_02260D78 *param1);

BOOL ov91_0225F25C(UnkStruct_ov115_02263130 *param0, UnkStruct_ov115_02260D78 *param1) {
    u16 uVar1;
    s32 iVar3;
    u32 uVar2;

    switch (param0->unk_3A) {
    case 0:
        Sprite_SetAnimationFrame(param0->unk_34, 0);
        ov91_02260CB4(&param1->unk_1AB4.unk_6A34);
        ov91_02260CE0(&param1->unk_1AB4.unk_6A34, 0x80, 0xb1);
        param0->unk_3B = 0;
        param0->unk_3A = param0->unk_3A + 1;
        break;
    case 1:
        Sprite_UpdateAnim(param0->unk_34, FX32_CONST(2));
        uVar1 = Sprite_GetAnimationFrame(param0->unk_34);
        uVar2 = (u32)uVar1;
        switch (uVar2) {
        case 10:
            if (param0->unk_3B == 0) {
                ov91_0225D500(param1, 0x80, 0x60, 0x80, 0x40, 2);
                PlaySE(0x64f);
                param0->unk_3B = 1;
            }
            break;
        case 11:
            param0->unk_39 = param0->unk_39 + 1;
            if (param0->unk_39 >= 2) {
                param0->unk_3A = 3;
            } else {
                param0->unk_3A = 2;
                param0->unk_3C = 0x10;
            }
            ov91_02260D00(&param1->unk_1AB4.unk_6A34);
            break;
        }
        ov91_02260CE0(&param1->unk_1AB4.unk_6A34, 0x80, ov91_02261C84[uVar2]);
        break;
    case 2:
        iVar3 = param0->unk_3C + -1;
        param0->unk_3C = iVar3;
        if (iVar3 <= 0) {
            param0->unk_3A = 0;
        }
        break;
    case 3:
        iVar3 = ov91_0225E6D0(param1, param1);
        if (iVar3 == 0) {
            Sprite_SetDrawFlag(param0->unk_34, 0);
            return 1;
        }
    }
    ov91_0225E40C(param1);
    ov91_0225E62C((u8 *)param1);
    return 0;
}
