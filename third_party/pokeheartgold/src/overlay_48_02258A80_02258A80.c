#include "global.h"

#include "constants/heap.h"
#include "overlay_48_unk_struct_ov69_0225_d35_c.h"
#include "sys_task.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_48_unk_struct_ov69_0225ef54.h"
#include "overlay_45_wifi_plaza_system.h"
#include "bg_window.h"
#include "camera.h"
#include "gf_3d_loader.h"
#include "filesystem_files_def.h"
#include "struct_defs/resdat_narc_structs.h"
#include "filesystem.h"
#include "field/overlay_01_021FB878.h"
#include "sprite.h"
#include "message_format.h"

struct Camera;

struct CameraAngle;

struct CameraPositionHistory;

struct Field3dModel;

struct Field3dObject;

struct G3DPipelineBuffers;

struct MessageBank;

struct MessageBankEntry;

struct NARC;

struct PlaceRecord;

struct Sprite;

struct SpriteResource;

struct String;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct SysTask;

struct UnkStruct_ov69_0225C980_t;

struct Window;

typedef struct PlaceRecord {
    u16 type;
    s16 x;
    s16 y;
} PlaceRecord;

typedef struct UnkStruct_ov69_0225C980_t {
    s16 unk_00;
    s16 unk_02;
} UnkStruct_ov69_0225C980;

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

NARC *NARC_New(enum NarcId narcID, enum HeapID heapID);

void NARC_Delete(NARC *narc);

void *GfGfxLoader_LoadFromOpenNarc_GetSizeOut(NARC *narc, u32 narcMemberIdx, BOOL compressed, enum HeapID heapID, BOOL allocAtEnd, u32 *fileSize);

typedef struct G3DPipelineBuffers {
    enum HeapID heapID;
    void *paletteBuffer;
    void *textureBuffer;
} G3DPipelineBuffers;

u32 GeonetLocations_GetLocationListCount(void);

u32 GeonetLocations_GetCountry(u32 index);

u32 GeonetLocations_GetNarcMemberIndex(u32 index);

void Heap_Free(void *ptr);

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
    s16 unk_00;
    s16 unk_02;
    MtxFx33 unk_04;
    u16 unk_28;
    u16 unk_2A;
    u16 unk_2C;
} UnkStruct_ov69_0225CA7C_sub1_sub1;

typedef struct {
    u32 unk_00;
    UnkStruct_ov69_0225CA7C_sub1_sub1 unk_04[1024];
} UnkStruct_ov69_0225CA7C_sub1;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    s16 unk_0A;
    s16 unk_0C;
    u16 unk_0E;
} UnkStruct_ov69_0225ECF0;

typedef struct {
    u16 unk_00[2];
    u16 unk_04;
    u16 unk_06;
    u16 unk_08;
    u16 unk_0A;
} UnkStruct_ov69_0225F118;

typedef struct {
    u32 unk_00_0 : 1;
    u32 unk_00_1 : 1;
    u32 : 30;
    u32 unk_04_0 : 31;
    u32 : 1;
} UnkStruct_ov69_0225DA74;

typedef struct {
    UnkStruct_ov69_0225CA7C_sub1 unk_00;
    VecFx32 unk_B804;
    Field3dObject unk_B810[3];
    Field3dModel unk_B978[3];
} UnkStruct_ov69_0225CA7C;

typedef struct {
    Camera *camera;
    CameraAngle unk_04;
    fx32 unk_0C;
    u16 unk_10;
    u16 unk_12;
} UnkStruct_ov69_0225DAEC;

typedef struct {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
} UnkStruct_ov69_0225E0C0;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    u8 unk_04[4];
    void *unk_08[2];
    NNSG2dScreenData *unk_10[2];
    u16 unk_18;
    u16 unk_1A;
    void *unk_1C;
    NNSG2dPaletteData *unk_20;
    u8 unk_24;
    u8 unk_25;
    u8 unk_26;
    u8 unk_27;
} UnkStruct_ov69_0225E7DC;

typedef struct {
    u8 unk_00;
    s8 unk_01;
    u8 unk_02[2];
} UnkStruct_ov69_0225E084_sub1;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    u16 unk_08;
    u16 unk_0A;
    const UnkStruct_ov69_0225ECF0 *unk_0C;
} UnkStruct_ov69_0225ED90;

typedef struct {
    UnkStruct_ov69_0225ED90 unk_00[3];
    s16 unk_30;
    s16 unk_32;
    u8 unk_34[4];
    u8 unk_38[4];
    Sprite *unk_3C[12];
    SpriteResource *unk_6C[12][4];
} UnkStruct_ov69_0225EB60;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    s16 unk_0A;
    SysTask *unk_0C;
} UnkStruct_ov69_0225E478;

typedef struct {
    u32 unk_00;
    UnkStruct_ov69_0225E0C0 unk_04[4];
    u16 unk_34;
    u16 unk_36;
    u32 unk_38;
    u16 unk_3C;
    u16 unk_3E;
    UnkStruct_ov69_0225E084_sub1 unk_40[18];
    u32 unk_88[3];
    void *unk_94;
    NNSG2dScreenData *unk_98;
    u16 unk_9C;
    u16 unk_9E;
    UnkStruct_ov69_0225E7DC unk_A0[5];
    Window unk_168[3];
    Window unk_198;
    Window unk_1A8;
    UnkStruct_ov69_0225EB60 unk_1B8;
    UnkStruct_ov69_0225E478 unk_2E4;
} UnkStruct_ov69_0225E084;

void ov69_0225CA7C(UnkStruct_ov69_0225CA7C *param0, u32 param1, s16 param2, s16 param3, u16 param4, u16 param5, const UnkStruct_ov66_02231560 *param6);

void ov48_02259BC0(UnkStruct_ov69_0225DAEC *param0, UnkStruct_ov69_0225DA74 param1, enum HeapID heapID);

void ov69_0225DEC0(UnkStruct_ov69_0225E084 *param0, UnkStruct_ov69_0225D35C *param1, UnkStruct_ov69_0225EF54 *param2, enum HeapID heapID);

void ov48_0225A894(UnkStruct_ov69_0225E7DC *param0, UnkStruct_ov69_0225D35C *param1, const UnkStruct_ov69_0225F118 *param2, u16 param3, enum HeapID heapID);

void ov48_02258A80(UnkStruct_ov69_0225CA7C *param0, const UnkStruct_ov66_02231560 *param1, enum HeapID heapID);

void ov48_02258B7C(UnkStruct_ov69_0225CA7C *param0, u32 param1, s16 param2, s16 param3, u16 param4, u16 param5, const UnkStruct_ov66_02231560 *param6);

void ov48_02258A80(UnkStruct_ov69_0225CA7C *param0, const UnkStruct_ov66_02231560 *param1, enum HeapID heapID) {
    NARC *narc = NARC_New(NARC_a_0_9_8, heapID);

    param0->unk_00.unk_00 = 0;

    {
        void *v1;
        PlaceRecord *v2;
        u32 v3;
        int v4, v5;

        v1 = GfGfxLoader_LoadFromOpenNarc_GetSizeOut(narc, 18, FALSE, heapID, FALSE, &v3);
        v2 = (PlaceRecord *)v1;
        v5 = v3 / 6;

        v2++;

        for (v4 = 1; v4 < v5; v4++) {
            if (v2->type != 2) {
                ov48_02258B7C(param0, param0->unk_00.unk_00, v2->x, v2->y, v4, 0, param1);
                param0->unk_00.unk_00++;
            }

            v2++;
        }

        Heap_Free(v1);
    }
    {
        void *v6;
        UnkStruct_ov69_0225C980 *v7;
        u32 v8, v9;
        int v10, v11, v12, v13;

        v12 = 1;
        v11 = GeonetLocations_GetLocationListCount();

        while (v12 < v11) {
            v9 = GeonetLocations_GetNarcMemberIndex(v12);
            v6 = GfGfxLoader_LoadFromOpenNarc_GetSizeOut(narc, v9, FALSE, heapID, FALSE, &v8);
            v7 = (UnkStruct_ov69_0225C980 *)v6;
            v13 = v8 / 4;

            v7++;

            for (v10 = 1; v10 < v13; v10++) {
                ov48_02258B7C(param0, param0->unk_00.unk_00, v7->unk_00, v7->unk_02, GeonetLocations_GetCountry(v12), v10, param1);
                param0->unk_00.unk_00++;
                v7++;
            }

            Heap_Free(v6);
            v12++;
        }
    }

    NARC_Delete(narc);
}
