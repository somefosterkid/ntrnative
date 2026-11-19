#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_91_unk_struct_ov115_02261_c18.h"
#include "overlay_91_unk_struct_ov115_02261_adc.h"
#include "constants/heap.h"
#include "struct_defs/resdat_narc_structs.h"
#include "pm_string.h"
#include "player_data.h"
#include "unk_02013534.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_41_sprite_resource_collection.h"
#include "touch_pad_data_buffer.h"

struct CameraPerspective;

struct CameraPositionHistory;

struct CharTransferAllocation;

struct G3DPipelineBuffers;

struct MessageBank;

struct MessageBankEntry;

struct Sprite;

struct SpriteResource;

struct SpriteResourcesHeader;

struct String;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct TouchPadDataBuffer;

struct PlayerProfile;

struct UnkStruct_ov115_02260D78_t;

typedef struct UnkStruct_ov115_02260D78_t UnkStruct_ov115_02260D78;

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

typedef struct CameraPositionHistory {
    int historySize;
    int currentHistoryIndex;
    int nextHistoryIndex;
    int delay;
    BOOL delayReached;
    BOOL delayX;
    BOOL delayY;
    BOOL delayZ;
    VecFx32 *positions;
} CameraPositionHistory;

typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;

typedef struct G3DPipelineBuffers {
    enum HeapID heapID;
    void *paletteBuffer;
    void *textureBuffer;
} G3DPipelineBuffers;

u32 MTRandom(void);

typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;

typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;

typedef struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
} StringTemplateArgHeader;

typedef struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
} StringTemplateArg;

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

extern void ov91_0225E728(const UnkStruct_ov115_022622F8 *param0, VecFx32 *param1);

extern void ov91_0225EA54(UnkStruct_ov115_022622F8 *param0, const VecFx32 *param1);

void ov91_0225E7E8(UnkStruct_ov115_022622F8 *param0, UnkStruct_ov115_02260D78 *param1, BOOL param2, fx32 param3, BOOL param4);

void ov91_0225E7E8(UnkStruct_ov115_022622F8 *param0, UnkStruct_ov115_02260D78 *param1, BOOL param2, fx32 param3, BOOL param4) {
    fx32 v1;
    VecFx32 v2;
    u32 v3;

    if (param2) {
        param0->unk_04.unk_04.x = -param0->unk_04.unk_04.x;
        param0->unk_04.unk_04.z = -param0->unk_04.unk_04.z;

        if (param4) {
            v3 = MTRandom();

            if ((((param0->unk_04.unk_04.x) < 0) ? (-(param0->unk_04.unk_04.x)) : (param0->unk_04.unk_04.x)) <= (((param0->unk_04.unk_04.z) < 0) ? (-(param0->unk_04.unk_04.z)) : (param0->unk_04.unk_04.z))) {
                v1 = FX_MulInline(param0->unk_04.unk_04.z, ((fx32)(((1.0f) > 0) ? ((1.0f) * (1 << 12) + 0.5f) : ((1.0f) * (1 << 12) - 0.5f))));

                if ((v3 % 2) == 1) {
                    param0->unk_04.unk_04.x = -v1;
                } else {
                    param0->unk_04.unk_04.x = v1;
                }
            } else {
                v1 = FX_MulInline(param0->unk_04.unk_04.x, ((fx32)(((1.0f) > 0) ? ((1.0f) * (1 << 12) + 0.5f) : ((1.0f) * (1 << 12) - 0.5f))));

                if ((v3 % 2) == 1) {
                    param0->unk_04.unk_04.z = -v1;
                } else {
                    param0->unk_04.unk_04.z = v1;
                }
            }
        }
    }

    param0->unk_04.unk_10 = FX_MulInline(param0->unk_04.unk_10, param3);
    param0->unk_02 = 0;
    param0->unk_00 = 5;

    ov91_0225E728(param0, &v2);
    ov91_0225EA54(param0, &v2);

    param0->unk_02++;
}
