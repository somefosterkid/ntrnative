#include "global.h"
#include "sprite_resources_header.h"

#include "constants/heap.h"
#include "pm_string.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_91_unk_struct_ov115_02261_adc.h"
#include "msgdata.h"
#include "struct_defs/resdat_narc_structs.h"

typedef enum HeapID HeapID;

struct CameraPositionHistory;

struct CharTransferAllocation;

struct G3DPipelineBuffers;

struct MessageBank;

struct MessageBankEntry;

struct Sprite;

struct String;

struct StringTemplateArg;

struct StringTemplateArgHeader;

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
} UnkStruct_ov115_02264BA0;

void ov91_02260C48(UnkStruct_ov115_02261ADC *param0, UnkStruct_ov115_02264BA0 *param1, u32 param2, u32 param3);

void ov91_02260C48(UnkStruct_ov115_02261ADC *param0, UnkStruct_ov115_02264BA0 *param1, u32 param2, u32 param3) {
    SpriteTemplate v0 = { 0 };

    v0.spriteList = param0->unk_1C;
    v0.header = &param0->unk_760.unk_70;
    v0.scale.x = ((fx32)0x0000000000001000L);
    v0.scale.y = ((fx32)0x0000000000001000L);
    v0.scale.z = ((fx32)0x0000000000001000L);
    v0.drawPriority = 32;
    v0.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    v0.heapID = (HeapID)(param3);

    param1->unk_04 = Sprite_CreateAffine(&v0);

    Sprite_SetDrawFlag(param1->unk_04, 0);
    Sprite_SetPaletteOverride(param1->unk_04, param2);

    param1->unk_00 = 0;
}
