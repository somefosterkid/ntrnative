#include "global.h"
#include "constants/heap.h"
#include "filesystem.h"

struct NARC;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    u32 unk_04;
    u8 unk_08;
    u8 unk_09;
    u8 unk_0A;
    u8 unk_0B;
} UnkStruct_ov113_02260818;

BOOL ov89_0225C88C(int param0, int param1, BOOL param2);

void *GfGfxLoader_LoadFromOpenNarc(NARC *narc, u32 narcMemberIdx, BOOL compressed, enum HeapID heapID, BOOL allocAtEnd);

void Heap_Free(void *ptr);

void ov89_0225AA24(NNSG3dResTex *param0, NARC *param1, NARC *param2, const UnkStruct_ov113_02260818 *param3, BOOL param4);

void ov89_0225AA24(NNSG3dResTex *param0, NARC *param1, NARC *param2, const UnkStruct_ov113_02260818 *param3, BOOL param4) {
    u16 *v0;
    u8 *v1, *v2;
    NNSG2dCharacterData *v3;
    int v4 = ov89_0225C88C(param3->unk_02, param3->unk_08, param4);
    v1 = (u8 *)((u8 *)param0 + param0->texInfo.ofsTex);

    if (v4 == 1) {
        v2 = GfGfxLoader_LoadFromOpenNarc(param2, 3 + param3->unk_02, 1, (enum HeapID)125, 1);
        NNS_G2dGetUnpackedCharacterData(v2, &v3);
    } else {
        v2 = GfGfxLoader_LoadFromOpenNarc(param1, 16, 0, (enum HeapID)125, 1);
        NNS_G2dGetUnpackedCharacterData(v2, &v3);
    }

    MI_CpuClear16(v1, (16 * 16 / 4));

    {
        int v6, v7, v8, v9, v10;
        u8 *v11, *v12;
        u16 *v13;

        v11 = v3->pRawData;
        v12 = v11;
        v13 = (u16 *)v1;

        for (v9 = 0; v9 < 4; v9++) {
            switch (v9) {
            case 0:
                if (v4 == 1) {
                    v12 = &v11[0x20 * 4];
                } else {
                    v12 = &v11[0x20 * 0];
                }

                v13 = (u16 *)v1;
                break;
            case 1:
                if (v4 == 1) {
                    v12 = &v11[0x20 * 5];
                } else {
                    v12 = &v11[0x20 * 1];
                }

                v13 = (u16 *)v1;
                v13++;
                break;
            case 2:
                if (v4 == 1) {
                    v12 = v11;
                } else {
                    v12 = &v11[0x20 * 2];
                }

                v13 = (u16 *)v1;
                v13 = &v13[2 * 8];
                break;
            case 3:
                if (v4 == 1) {
                    v12 = &v11[0x20 * 1];
                } else {
                    v12 = &v11[0x20 * 3];
                }

                v13 = (u16 *)v1;
                v13 = &v13[2 * 8 + 1];

                break;
            }

            v7 = 0;

            for (v6 = 0; v6 < 8; v6++) {
                v10 = 0;

                for (v8 = 0; v8 < 4; v8++) {
                    if (v12[v7] & 0xf) {
                        *v13 |= 1 << v10;
                    }

                    v10 += 2;

                    if (v12[v7] & 0xf0) {
                        *v13 |= 1 << v10;
                    }

                    v10 += 2;
                    v7++;
                }

                v13++;
                v13++;
            }
        }
    }

    v0 = (u16 *)((u8 *)param0 + param0->plttInfo.ofsPlttData);
    v0[1] = param3->unk_00;

    Heap_Free(v2);
}
