#include "global.h"
#include "sprite_resources_header.h"
#include "constants/heap.h"
#include "pm_string.h"
#include "unk_02013534.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_91_unk_struct_ov115_02261_adc.h"
#include "struct_defs/resdat_narc_structs.h"

struct CameraPositionHistory;

struct CharTransferAllocation;

struct G3DPipelineBuffers;

struct MessageBank;

struct MessageBankEntry;

struct Sprite;

struct String;

struct StringTemplateArg;

struct StringTemplateArgHeader;

enum AffineOverwriteMode {
    AFFINE_OVERWRITE_MODE_NONE = 0,
    AFFINE_OVERWRITE_MODE_NORMAL,
    AFFINE_OVERWRITE_MODE_DOUBLE,
};

Sprite *Sprite_CreateAffine(const SpriteTemplate *template);

void Sprite_SetDrawFlag(Sprite *sprite, BOOL draw);

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
    BOOL unk_00;
    Sprite *unk_04;
    VecFx32 unk_08;
    s32 unk_14;
    BOOL unk_18;
} UnkStruct_ov115_02264D80;

typedef struct {
    UnkStruct_ov115_02264D80 unk_00[8];
} UnkStruct_ov115_02264C90;

void ov91_02260D14(UnkStruct_ov115_02261ADC *param0, UnkStruct_ov115_02264C90 *param1, u32 param2, u32 param3);

void ov91_02260D14(UnkStruct_ov115_02261ADC *param0, UnkStruct_ov115_02264C90 *param1, u32 param2, u32 param3) {
    int v0;
    SpriteTemplate v1 = { 0 };

    v1.spriteList = param0->unk_1C;
    v1.header = &param0->unk_760.unk_70;
    v1.scale.x = 9;
    v1.scale.y = 9;
    v1.scale.z = 9;
    v1.drawPriority = 32;
    v1.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    v1.heapID = (enum HeapID)(param3);

    for (v0 = 0; v0 < 8; v0++) {
        param1->unk_00[v0].unk_04 = Sprite_CreateAffine(&v1);
        Sprite_SetDrawFlag(param1->unk_00[v0].unk_04, 0);
        Sprite_SetAffineOverwriteMode(param1->unk_00[v0].unk_04, (enum AffineOverwriteMode)(1));
        Sprite_SetPaletteOverride(param1->unk_00[v0].unk_04, param2);
        param1->unk_00[v0].unk_00 = 0;
    }
}
