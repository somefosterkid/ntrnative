#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_91_unk_struct_ov115_02261_adc.h"
#include "constants/heap.h"
#include "struct_defs/resdat_narc_structs.h"
#include "pm_string.h"
#include "unk_02013534.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_41_sprite_resource_collection.h"

struct CameraPerspective;

struct CameraPositionHistory;

struct CharTransferAllocation;

struct Field3DModelAnimation;

struct Field3dModel;

struct Field3dObject;

struct G3DPipelineBuffers;

struct MessageBank;

struct MessageBankEntry;

struct NARC;

struct String;

struct StringTemplateArg;

struct StringTemplateArgHeader;

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

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

void sub_02018030(Field3dModel *model, NARC *narc, u32 memberIndex, u32 heapID);

void sub_020180BC(Field3DModelAnimation *anim, const Field3dModel *model, NARC *narc, u32 memberIndex, enum HeapID heapID, NNSFndAllocator *allocator);

void sub_020181B0(Field3dObject *obj, Field3dModel *model);

void sub_020181D4(Field3dObject *obj, Field3DModelAnimation *anim);

void sub_020182A0(Field3dObject *obj, BOOL visible);

void sub_020182A8(Field3dObject *obj, fx32 x, fx32 y, fx32 z);

void sub_020182C4(Field3dObject *obj, fx32 x, fx32 y, fx32 z);

#if defined(SDK_PORT) || defined(NONMATCHING)
void sub_020182E0(Field3dObject *obj, s16 angle, enum RotationAxis axis);
#else
void sub_020182E0(Field3dObject *obj, u16 angle, enum RotationAxis axis);
#endif

typedef struct G3DPipelineBuffers {
    enum HeapID heapID;
    void *paletteBuffer;
    void *textureBuffer;
} G3DPipelineBuffers;

u16 GF_DegreeToSinCosIdxNoWrap(u16 degrees);

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
    Field3dObject unk_00[3];
    Field3dModel unk_168[3];
    Field3DModelAnimation unk_198[3];
    fx32 unk_1D4;
    u8 unk_1D8;
    u8 unk_1D9;
    s16 unk_1DA;
    fx32 unk_1DC;
    fx32 unk_1E0;
} UnkStruct_ov115_02264FA0;

extern const u16 ov91_02261D64[4][4];

void ov91_02260F50(UnkStruct_ov115_02264FA0 *param0, UnkStruct_ov115_02261ADC *param1, NARC *param2, int param3, u32 param4, u32 heapID, NNSFndAllocator *param6);

void ov91_02260F50(UnkStruct_ov115_02264FA0 *param0, UnkStruct_ov115_02261ADC *param1, NARC *param2, int param3, u32 param4, u32 heapID, NNSFndAllocator *param6) {
    extern const u16 ov91_02261C0A[4];
    extern const u16 ov91_02261BF8[3];
    extern const u16 ov91_02261BFE[3];
    u32 v3;
    int v4;

    for (v4 = 0; v4 < 3; v4++) {
        if (v4 == 0) {
            v3 = 36;
        } else if (v4 == 1) {
            v3 = 37;
        } else {
            v3 = ov91_02261C0A[param3 - 1];
        }

        sub_02018030(&param0->unk_168[v4], param2, v3, heapID);
        sub_020181B0(&param0->unk_00[v4], &param0->unk_168[v4]);
        sub_020182A8(&param0->unk_00[v4], 0, ((fx32)(((-90) > 0) ? ((-90) * (1 << 12) + 0.5f) : ((-90) * (1 << 12) - 0.5f))) - ((fx32)(((8) > 0) ? ((8) * (1 << 12) + 0.5f) : ((8) * (1 << 12) - 0.5f))), 0);
        sub_020182C4(&param0->unk_00[v4], ((fx32)(((1.50f) > 0) ? ((1.50f) * (1 << 12) + 0.5f) : ((1.50f) * (1 << 12) - 0.5f))), ((fx32)(((1.50f) > 0) ? ((1.50f) * (1 << 12) + 0.5f) : ((1.50f) * (1 << 12) - 0.5f))), ((fx32)(((1.50f) > 0) ? ((1.50f) * (1 << 12) + 0.5f) : ((1.50f) * (1 << 12) - 0.5f))));

        if (v4 == 1) {
            sub_020182A0(&param0->unk_00[v4], 0);
        }

        if ((v4 == 0) || (v4 == 1)) {
            sub_020182E0(&param0->unk_00[v4], ov91_02261D64[param3 - 1][param4], (enum RotationAxis)(1));
        } else {
            sub_020182E0(&param0->unk_00[v4], GF_DegreeToSinCosIdxNoWrap(180), (enum RotationAxis)(1));
        }
    }

    for (v4 = 0; v4 < 3; v4++) {
        sub_020180BC(&param0->unk_198[v4], &param0->unk_168[ov91_02261BFE[v4]], param2, ov91_02261BF8[v4], (enum HeapID)(heapID), param6);
        sub_020181D4(&param0->unk_00[ov91_02261BFE[v4]], &param0->unk_198[v4]);
    }

    param0->unk_1D4 = ((fx32)(((1) > 0) ? ((1) * (1 << 12) + 0.5f) : ((1) * (1 << 12) - 0.5f)));
}
