#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "unk_02077678.h"

int ov45_0222AAEC(void *, s32);

u32 ov45_0222D7C0(int param0);

extern u32 ov49_022696F8;

extern u32 ov49_0226978C;

void ov49_0225BEA0(void *param_1, void *param_2, void **param_3, enum HeapID param_4, void *param_5);

void ov49_0225BEA0(void *param_1, void *param_2, void **param_3, enum HeapID param_4, void *param_5) {
    s32 iStack_24;
    u16 moveType;
    NarcId narcId;
    s32 iVar2;
    void *pNcgrFile;
    u32 destY;
    u16 *puStack_28;
    u8 *buffer;
    u16 sVar1;
    NNSG2dCharacterData *pNStack_18;

    buffer = (unsigned char *)(&ov49_0226978C);
    iStack_24 = 0;
    puStack_28 = (u16 *)&ov49_022696F8;
    destY = 2;
    do {
        sVar1 = ov45_0222AAEC(param_5, iStack_24);
        if (sVar1 != 0) {
            moveType = ov45_0222D7C0(sVar1);
            narcId = TypeIconGfx_GetNarcId();
            iVar2 = TypeIconGfx_GetCharFileId(moveType);
            pNcgrFile = GfGfxLoader_LoadFromNarc(narcId, iVar2, 1, param_4, 1);
            NNS_G2dGetUnpackedCharacterData(pNcgrFile, &pNStack_18);
            BG_LoadCharTilesData((BgConfig *)*param_3, 6, pNStack_18->pRawData, 0x100, (u32)*puStack_28);
            Heap_Free(pNcgrFile);
            CopyToBgTilemapRect((BgConfig *)*param_3, 6, 0x1a, destY, 4, 2, buffer, 0, 0, 4, 2);
            iVar2 = TypeIconGfx_GetPlttOverride(moveType);
            BgTilemapRectChangePalette((BgConfig *)*param_3, 6, 0x1a, destY, 4, 2, iVar2 + 0xb);
            ScheduleBgTilemapBufferTransfer((BgConfig *)*param_3, 6);
        }
        buffer = buffer + 0x10;
        puStack_28 = puStack_28 + 1;
        destY = destY + 2;
        iStack_24 = iStack_24 + 1;
    } while (iStack_24 < 2);
    return;
}
