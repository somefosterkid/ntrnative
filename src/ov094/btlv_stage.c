// Black's code at ov094 0x021f6560-0x021f6b04. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "battle/btlv_stage.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "struct_decls.h"
#include "gfl/arc.h"
#include "nitro/fx/fx_mtx.h"



BtlvStage *func_ov094_021f6560(s32 kind, s32 tblIdx, s32 fileIdx, HeapID heapId) {
    BtlvStage *stage;
    BtlvStageResTable *tbl;
    BtlvStageResTable *entry;
    u32 fileId;
    s32 i;
    s32 j;
    s32 k;
    NNSG3dResTex *tex;
    u32 size;

    stage = GFL_HeapAllocate(heapId, sizeof(BtlvStage), 1, "btlv_stage.c", 99);
    tbl = GFL_ArcSysReadHeapNew(0x98, 2, (heapId & 0x7fff) | 0x8000);
    stage->heapId = heapId;
    if (kind == 3) {
        stage->unk_00 = GFL_G3DSysReadArcSysResource(0xb, 0x56);
    } else {
        fileId = tbl[tblIdx].files[0][fileIdx];
        if (fileId != 0xffffffff) {
            stage->unk_00 = GFL_G3DSysReadArcSysResource(0xb, fileId);
        } else {
            stage->unk_00 = GFL_G3DSysReadArcSysResource(0xb, tbl[tblIdx].files[0][0]);
        }
    }
    GFL_G3DResUploadTexData(stage->unk_00);
    stage->unk_04 = 0;
    for (i = 1; i < 4; i++) {
        if (tbl[tblIdx].files[i][fileIdx] != 0xffffffff) {
            stage->unk_04++;
        }
    }
    entry = &tbl[tblIdx];
    stage->unk_1C[0] = GFL_G3DMdlCreate(stage->unk_00, 0, stage->unk_00);
    stage->unk_1C[1] = GFL_G3DMdlCreate(stage->unk_00, 0, stage->unk_00);
    if (kind == 3) {
        stage->unk_04 = 1;
        stage->unk_10 = GFL_HeapAllocate(stage->heapId, sizeof(void *), 0, "btlv_stage.c", 147);
        stage->unk_10[0] = GFL_G3DSysReadArcSysResource(0xb, 0x57);
        for (i = 0; i < 2; i++) {
            stage->unk_14[i] = GFL_HeapAllocate(stage->heapId, sizeof(void *), 0, "btlv_stage.c", 152);
            stage->unk_08[i] = GFL_HeapAllocate(stage->heapId, sizeof(BtlvStageAnm), 1, "btlv_stage.c", 153);
            stage->unk_14[i][0] = GFL_G3DAnmCreate(stage->unk_1C[i], stage->unk_10[0], 0);
            stage->unk_08[i][0].mode = 1;
            stage->unk_24[i] = GFL_G3DActorCreate(stage->unk_1C[i], stage->unk_14[i], (u16)stage->unk_04);
            GFL_G3DActorBindAnm(stage->unk_24[i], 0);
        }
    } else if (stage->unk_04 != 0) {
        stage->unk_10 = GFL_HeapAllocate(stage->heapId, stage->unk_04 * sizeof(void *), 0, "btlv_stage.c", 167);
        for (i = 0; i < 2; i++) {
            stage->unk_14[i] =
                GFL_HeapAllocate(stage->heapId, stage->unk_04 * sizeof(void *), 0, "btlv_stage.c", 171);
            stage->unk_08[i] =
                GFL_HeapAllocate(stage->heapId, stage->unk_04 * sizeof(BtlvStageAnm), 1, "btlv_stage.c", 172);
        }
        k = 0;
        for (i = 1; i < 4; i++) {
            if (tbl[tblIdx].files[i][fileIdx] != 0xffffffff) {
                stage->unk_10[k] = GFL_G3DSysReadArcSysResource(0xb, tbl[tblIdx].files[i][fileIdx]);
                for (j = 0; j < 2; j++) {
                    stage->unk_14[j][k] = GFL_G3DAnmCreate(stage->unk_1C[j], stage->unk_10[k], 0);
                    stage->unk_08[j][k].mode = 0;
                }
                k++;
            }
        }
        for (j = 0; j < 2; j++) {
            stage->unk_24[j] = GFL_G3DActorCreate(stage->unk_1C[j], stage->unk_14[j], (u16)stage->unk_04);
            for (k = 0; k < stage->unk_04; k++) {
                GFL_G3DActorBindAnm(stage->unk_24[j], k);
            }
        }
    } else {
        stage->unk_24[0] = GFL_G3DActorCreate(stage->unk_1C[0], 0, 0);
        stage->unk_24[1] = GFL_G3DActorCreate(stage->unk_1C[1], 0, 0);
    }
    tex = NNS_G3dGetTex(GFL_G3DResGetResData(stage->unk_00));
    size = tex->plttInfo.sizePltt << 3;
    stage->unk_A4.resources = GFL_HeapAllocate(stage->heapId, sizeof(void *), 0, "btlv_stage.c", 214);
    stage->unk_A4.palettes = GFL_HeapAllocate(stage->heapId, sizeof(void *), 0, "btlv_stage.c", 215);
    *stage->unk_A4.resources = stage->unk_00;
    *stage->unk_A4.palettes = GFL_HeapAllocate(stage->heapId, size, 0, "btlv_stage.c", 218);
    stage->unk_A4.active = 0;
    stage->unk_A4.count = 1;
    stage->unk_2C[0].translation.x = 0;
    stage->unk_2C[0].translation.y = 0;
    stage->unk_2C[0].translation.z = 0x572f;
    stage->unk_2C[0].scale.x = 0x1000;
    stage->unk_2C[0].scale.y = 0x1000;
    stage->unk_2C[0].scale.z = 0x1000;
    MTX_Identity33_(&stage->unk_2C[0].rotation);
    stage->unk_2C[1].translation.x = 0;
    stage->unk_2C[1].translation.y = 0;
    stage->unk_2C[1].translation.z = 0xffff3483;
    stage->unk_2C[1].scale.x = 0x1000;
    stage->unk_2C[1].scale.y = 0x1000;
    stage->unk_2C[1].scale.z = 0x1000;
    MTX_Identity33_(&stage->unk_2C[1].rotation);
    if (kind == 3) {
        stage->unk_2C[0].translation.z = 0xa000;
        stage->unk_2C[1].translation.z = 0xffff1000;
    }
    G3X_SetEdgeColorTable(entry->unk_00);
    GFL_HeapFree(tbl);
    return stage;
}

void func_ov094_021f6898(BtlvStage *stage) {
    s32 i;
    s32 j;

    if (stage->unk_04 != 0) {
        for (j = 0; j < stage->unk_04; j++) {
            for (i = 0; i < 2; i++) {
                GFL_G3DAnmFree(stage->unk_14[i][j]);
            }
            GFL_G3DResFree(stage->unk_10[j]);
        }
        for (j = 0; j < 2; j++) {
            GFL_HeapFree(stage->unk_14[j]);
            GFL_HeapFree(stage->unk_08[j]);
        }
        GFL_HeapFree(stage->unk_10);
    }
    for (j = 0; j < 2; j++) {
        GFL_G3DActorFree(stage->unk_24[j]);
        GFL_G3DMdlFree(stage->unk_1C[j]);
    }
    GFL_G3DResFree(stage->unk_00);
    GFL_HeapFree(*stage->unk_A4.palettes);
    GFL_HeapFree(stage->unk_A4.palettes);
    GFL_HeapFree(stage->unk_A4.resources);
    GFL_HeapFree(stage);
}

void func_ov094_021f6934(BtlvStage *stage) {
    int i;
    int j;
    fx32 speed;

    if (stage->unk_04 != 0) {
        for (i = 0; i < stage->unk_04; i++) {
            for (j = 0; j < 2; j++) {
                speed = 0;
                switch (stage->unk_08[j][i].mode) {
                case 0:
                    speed = FX32_ONE;
                    break;
                case 1:
                    if (stage->unk_08[j][i].active == TRUE) {
                        speed = stage->unk_08[j][i].speed;
                        stage->unk_08[j][i].frames--;
                        if (stage->unk_08[j][i].frames == 0) {
                            stage->unk_08[j][i].active = FALSE;
                        }
                    }
                    break;
                }
                if (speed != 0) {
                    GFL_G3DActorStepAnmFrameLoop(stage->unk_24[j], i, speed);
                }
            }
        }
    }
    BtlvEffect_UpdateTexPaletteFade(&stage->unk_A4);
}


void func_ov094_021f69b0(BtlvStage *stage) {
    int i;

    for (i = 0; i < 2; i++) {
        if (!(stage->hidden & (1 << i))) {
            GFL_G3DSysDrawObj(stage->unk_24[i], &stage->unk_2C[i]);
        }
    }
}


void func_ov094_021f69e8(BtlvStage *stage, u8 startEvy, u8 targetEvy, u8 wait, u16 color) {
    stage->unk_A4.active = 1;
    stage->unk_A4.evy = startEvy;
    stage->unk_A4.targetEvy = targetEvy;
    stage->unk_A4.wait = 0;
    stage->unk_A4.waitFrames = wait;
    stage->unk_A4.color = color;
}

BOOL func_ov094_021f6a18(BtlvStage *stage) {
    if (stage->unk_A4.active != 0) {
        return TRUE;
    }
    return FALSE;
}

void func_ov094_021f6a28(BtlvStage *stage, u32 side, BOOL hide) {
    if (side == 2) {
        if (hide) {
            stage->hidden = 3;
        } else {
            stage->hidden = 0;
        }
    } else if (hide) {
        stage->hidden |= 1 << side;
    } else {
        stage->hidden &= (1 << side) ^ 3;
    }
}


void func_ov094_021f6aa8(BtlvStage *stage, u32 side, u32 anm, fx32 speed, s32 frames) {
    if (stage->unk_08[side][anm].mode == 1) {
        stage->unk_08[side][anm].active = TRUE;
        stage->unk_08[side][anm].speed = speed;
        stage->unk_08[side][anm].frames = frames;
    }
}


BOOL func_ov094_021f6ad0(BtlvStage *stage) {
    s32 i;

    for (i = 0; i < stage->unk_04; i++) {
        if (stage->unk_08[0][i].active == TRUE || stage->unk_08[1][i].active == TRUE) {
            return TRUE;
        }
    }
    return FALSE;
}
