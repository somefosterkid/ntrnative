// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov168/btlv_field.c, Black 2/White 2's version in
// overlay 168. Black's has no seasons: no reload, and its table holds a 32-bit file ID for each of four variants.
// The battle view's field: the ground and weather model around the stage, with its animations and palette fade. The
// name is the ROM's string, from GFL_HeapAllocate's asserts. BtlvField_Create is swan's name
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0); the others are Black's dsd names

#include "battle/btlv_field.h"
#include "types.h"
#include "battle/btlv_effect.h"
#include "gfl/arc.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_mtx.h"
#include "nnsys/g3d.h"

// The battle's 3D graphics archive, and the archive of the field table
#define BTLV_FIELD_GRA_ARC 11
#define BTLV_FIELD_TABLE_ARC 0x98

#define BTLV_FIELD_RES_NONE 0xffffffff

// A field's resources in file 1 of archive 0x98: the model, then three animations, each by variant;
// BTLV_FIELD_RES_NONE for none
typedef u32 BtlvFieldResIds[4][4];

// An animation's playback, 0x10 bytes
typedef struct {
    u32 mode;    // 0x0  0: loops at normal speed; 1: steps by speed while playing
    u32 playing; // 0x4
    fx32 speed;  // 0x8
    s32 frames;  // 0xc  mode 1: the steps left
} BtlvFieldAnm;

// The field's work, 0x70 bytes
struct BtlvField {
    void *res;                  // 0x00  the model and its textures
    s32 anmCount;               // 0x04
    BtlvFieldAnm *anmParams;    // 0x08
    void **anmRes;              // 0x0c
    void **anms;                // 0x10
    G3DModel *mdl;              // 0x14
    G3DActor *actor;            // 0x18
    SRTMatrix status;           // 0x1c
    BtlvTexPaletteFade palFade; // 0x58
    // 0x68
    u32 hidden : 1;
    u32 pauseAnm : 1; // skips one animation step
    HeapID heapId;    // 0x6c
};

BtlvField *BtlvField_Create(BOOL arg0, u8 fieldId, u8 variant, HeapID heapId) {
    BtlvField *field;
    BtlvFieldResIds *table;
    u32 palSize;
    int i;
    int j;

    field = GFL_HeapAllocate(heapId, sizeof(BtlvField), TRUE, "btlv_field.c", 0x56);
    table = GFL_ArcSysReadHeapNew(BTLV_FIELD_TABLE_ARC, 1, HEAPID_TAIL(heapId));

    field->heapId = heapId;
    if (table[fieldId][0][variant] != BTLV_FIELD_RES_NONE) {
        field->res = GFL_G3DSysReadArcSysResource(BTLV_FIELD_GRA_ARC, table[fieldId][0][variant]);
    } else {
        field->res = GFL_G3DSysReadArcSysResource(BTLV_FIELD_GRA_ARC, table[fieldId][0][0]);
    }
    GFL_G3DResUploadTexData(field->res);

    field->anmCount = 0;
    for (i = 1; i < 4; i++) {
        if (table[fieldId][i][variant] != BTLV_FIELD_RES_NONE) {
            field->anmCount++;
        }
    }

    j = 0;
    field->mdl = GFL_G3DMdlCreate(field->res, 0, field->res);
    if (field->anmCount != 0) {
        field->anmRes = GFL_HeapAllocate(field->heapId, field->anmCount * sizeof(void *), FALSE, "btlv_field.c", 0x7e);
        field->anms = GFL_HeapAllocate(field->heapId, field->anmCount * sizeof(void *), FALSE, "btlv_field.c", 0x7f);
        field->anmParams =
            GFL_HeapAllocate(field->heapId, field->anmCount * sizeof(BtlvFieldAnm), TRUE, "btlv_field.c", 0x80);
        for (i = 1; i < 4; i++) {
            if (table[fieldId][i][variant] != BTLV_FIELD_RES_NONE) {
                field->anmRes[j] = GFL_G3DSysReadArcSysResource(BTLV_FIELD_GRA_ARC, table[fieldId][i][variant]);
                field->anms[j] = GFL_G3DAnmCreate(field->mdl, field->anmRes[j], 0);
                field->anmParams[j].mode = 0;
                j++;
            }
        }
        // GFL_G3DActorCreate's count is a u16
        field->actor = GFL_G3DActorCreate(field->mdl, field->anms, (u16)field->anmCount);
        for (i = 0; i < field->anmCount; i++) {
            GFL_G3DActorBindAnm(field->actor, i);
        }
    } else {
        field->actor = GFL_G3DActorCreate(field->mdl, NULL, 0);
    }

    field->status.translation.x = 0;
    field->status.translation.y = 0;
    field->status.translation.z = 0;
    field->status.scale.x = FX32_ONE;
    field->status.scale.y = FX32_ONE;
    field->status.scale.z = FX32_ONE;
    MTX_Identity33_(&field->status.rotation);

    palSize = NNS_G3dGetTex(GFL_G3DResGetResData(field->res))->plttInfo.sizePltt << 3;
    field->palFade.resources = GFL_HeapAllocate(field->heapId, sizeof(void *), FALSE, "btlv_field.c", 0xaa);
    field->palFade.palettes = GFL_HeapAllocate(field->heapId, sizeof(void *), FALSE, "btlv_field.c", 0xab);
    field->palFade.resources[0] = field->res;
    field->palFade.palettes[0] = GFL_HeapAllocate(field->heapId, palSize, FALSE, "btlv_field.c", 0xae);
    field->palFade.active = FALSE;
    field->palFade.count = 1;

    GFL_HeapFree(table);
    return field;
}

// Unlike Black 2's, frees the animations itself, as it has no reload to share that with
void func_ov094_021f6cf0(BtlvField *field) {
    int i;

    GFL_G3DActorFree(field->actor);
    if (field->anmCount != 0) {
        for (i = 0; i < field->anmCount; i++) {
            GFL_G3DAnmFree(field->anms[i]);
            GFL_G3DResFree(field->anmRes[i]);
        }
        GFL_HeapFree(field->anmParams);
        GFL_HeapFree(field->anms);
        GFL_HeapFree(field->anmRes);
    }
    GFL_G3DMdlFree(field->mdl);
    GFL_G3DResFree(field->res);
    GFL_HeapFree(field->palFade.palettes[0]);
    GFL_HeapFree(field->palFade.palettes);
    GFL_HeapFree(field->palFade.resources);
    GFL_HeapFree(field);
}

void func_ov094_021f6d5c(BtlvField *field) {
    int i;
    fx32 step;

    if (field->pauseAnm) {
        field->pauseAnm = FALSE;
        return;
    }
    if (field->anmCount != 0) {
        for (i = 0; i < field->anmCount; i++) {
            step = 0;
            switch (field->anmParams[i].mode) {
            case 0:
                step = FX32_ONE;
                break;
            case 1:
                if (field->anmParams[i].playing == TRUE) {
                    step = field->anmParams[i].speed;
                    field->anmParams[i].frames--;
                    if (field->anmParams[i].frames == 0) {
                        field->anmParams[i].playing = FALSE;
                    }
                }
                break;
            }
            if (step != 0) {
                GFL_G3DActorStepAnmFrameLoop(field->actor, i, step);
            }
        }
    }
    BtlvEffect_UpdateTexPaletteFade(&field->palFade);
}

void func_ov094_021f6dd4(BtlvField *field) {
    if (!field->hidden) {
        GFL_G3DSysDrawObj(field->actor, &field->status);
    }
}

void func_ov094_021f6dec(BtlvField *field, u8 evy, u8 targetEvy, u8 waitFrames, u16 color) {
    field->palFade.active = TRUE;
    field->palFade.evy = evy;
    field->palFade.targetEvy = targetEvy;
    field->palFade.wait = 0;
    field->palFade.waitFrames = waitFrames;
    field->palFade.color = color;
}

BOOL func_ov094_021f6e1c(BtlvField *field) {
    if (field->palFade.active) {
        return TRUE;
    }
    return FALSE;
}

void func_ov094_021f6e2c(BtlvField *field, BOOL hide) {
    field->hidden = hide;
}
