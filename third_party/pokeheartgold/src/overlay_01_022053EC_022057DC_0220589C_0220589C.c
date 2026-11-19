#include "global.h"

#include "constants/heap.h"

#include "field_types_def.h"
#include "filesystem.h"
#include "heap.h"
#include "map_object.h"
#include "map_object_tex_tint_task.h"
#include "palette.h"
#include "sys_task_api.h"
#include "vram_transfer_manager.h"

s32 ov01_02206088(u32);

s32 sub_02023FA0(void *arg0);

s32 sub_02023FB0(void *arg0);

void *sub_02020838(NNSG3dResTex *, const u8);

void *sub_02020888(NNSG3dResTex *, const u8);

void ov01_0220596C(SysTask *task, void *param);

void ov01_0220589C(u32 param_1, LocalMapObject *param_2, void *param_3);

void ov01_0220589C(u32 param_1, LocalMapObject *param_2, void *param_3) {
    MapObjectTexTintTask *dest;
    u32 file_id;
    MapObjectManager *manager;
    NARC *narc;
    NNSG3dResFileHeader *header;
    NNSG3dResTex *pNVar1;
    void *srcp;
    void *uVar2;
    u32 uVar3;
    u32 uVar4;

    dest = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(MapObjectTexTintTask));
    MI_CpuFill8(&dest->resFile, 0, sizeof(dest->resFile));
    file_id = ov01_02206088((((u32)MapObject_GetParam(param_2, 2)) << 15) >> 16);
    manager = MapObject_GetManager(param_2);
    narc = MapObjectManager_GetMapModelNarc(manager);
    header = NARC_AllocAndReadWholeMember(narc, file_id, HEAP_ID_FIELD2);
    dest->resFile = header;
    pNVar1 = NNS_G3dGetTex(header);
    srcp = (void *)sub_02020888(pNVar1, 0);
    uVar2 = sub_02020838(pNVar1, 0);
    dest->texData = uVar2;
    MIi_CpuCopy16(srcp, dest->pltt, sizeof(dest->pltt));
    TintPalette_GrayScale(dest->pltt, 0x10);
    TintPalette_CustomTone(dest->pltt, 0x10, 0x100, 0xb4, 0);
    uVar3 = sub_02023FB0(param_3);
    GF_CreateNewVramTransferTask(NNS_GFD_DST_3D_TEX_PLTT, (uVar3 & 0xffff) << 3, dest->pltt, sizeof(dest->pltt));
    uVar3 = sub_02023FA0(param_3);
    uVar4 = sub_02023FA0(param_3);
    GF_CreateNewVramTransferTask(NNS_GFD_DST_3D_TEX_VRAM, (uVar4 & 0xffff) << 3, dest->texData, ((uVar3 & 0x7fff0000) >> 0x10) << 4);
    SysTask_CreateOnVWaitQueue(ov01_0220596C, dest, 0);
    return;
}
