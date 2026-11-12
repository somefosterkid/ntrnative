// Black's code at ov174 0x021b6c70-0x021b7380. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "nnsys/g2d.h"
#include "struct_decls.h"
#include "local_tvt_chara.h"
#include "gfl/random.h"
#include "save/player_info.h"
#include "system/game_data.h"
#include "gfl/bmp.h"
#include "gfl/msg.h"
#include "gfl/str.h"
#include "system/wordset.h"


LocalTvtFace *func_ov174_021b6c70(Ov174Work *work, u32 idx) {
    LocalTvtFace *face;

    if (work->header->persons[idx] == 0xff) {
        return NULL;
    }
    face = GFL_HeapAllocate(work->heapId, sizeof(LocalTvtFace), FALSE, "local_tvt_chara.c", 122);
    face->unk_00 = idx;
    face->unk_02 = work->header->persons[idx];
    face->unk_03 = work->header->zones[idx];
    face->unk_04 = 0;
    face->unk_05 = 0;
    face->unk_14 = 0;
    face->unk_34 = 0;
    face->unk_30 = GFL_RandomLCAlt(75);
    if (face->unk_02 == 0) {
        if (getTrainerGender(GetGameDataPlayerInfo(work->unk_30->gameData)) == 1) {
            face->unk_02 = 5;
        }
    }
    func_ov174_021b7004(work, face);
    func_ov174_021b6f3c(work, face);
    func_ov174_021b71b8(work, face);
    face->unk_08 = 0;
    face->unk_10 = NULL;
    face->unk_06 = GFL_RandomLCAlt(40);
    face->unk_38 = 0;
    return face;
}

// Bytes 0x21b7480-0x21b7496: three tables read at 0x21b7481, 0x21b7484, 0x21b7485 and 0x21b7486 (overlapping labels).
const u8 data_ov174_021b7480[0x16] = {
    0x00, 0x06, 0x03, 0x0c, 0x14, 0x1c, 0x33, 0x15, 0x1e, 0x37, 0x16, 0x20, 0x3b, 0x12, 0x18, 0x27,
    0x17, 0x22, 0x2b, 0x13, 0x1a, 0x2f,
};

void func_ov174_021b6d00(Ov174Work *work, LocalTvtFace *face) {
    func_0204b3b4(face->unk_28);
    func_0204b3b4(face->unk_2c);
    func_0204ada4(face->unk_1c);
    func_0204a8d4(face->unk_20);
    func_0204afd8(face->unk_24);
    BmpWin_Free(face->unk_18);
    if (face->unk_10 != NULL) {
        GFL_HeapFree(face->unk_10);
    }
    GFL_HeapFree(face);
}

void func_ov174_021b6d3c(Ov174Work *work, LocalTvtFace *face) {
    u32 size;
    s32 dstBase;
    s32 srcBase;
    s32 offset;

    size = (work->unk_08 == 2) ? 0x6000 : 0x3000;
    dstBase = (work->unk_0d == 0) ? 0 : 0xc000;
    srcBase = (work->unk_08 == 2) ? 0 : 0x1c00;
    switch (face->unk_08) {
    case 1:
        face->unk_01 = 0;
        func_ov174_021b7180(work, face, face->unk_04 + (data_ov174_021b7480 + 5)[3 * face->unk_02]);
        face->unk_08 = 2;
        break;
    case 2:
        offset = 0x3000 * face->unk_01;
        func_ov174_021b71a0(work, face, srcBase + offset,
                            dstBase + (offset + (size * face->unk_00 + 0x10000)), 0x3000);
        face->unk_01++;
        if (0x3000 * face->unk_01 >= size) {
            face->unk_08 = 3;
            GFL_HeapFree(face->unk_10);
            face->unk_10 = NULL;
        }
        break;
    case 3:
        face->unk_01 = 0;
        func_ov174_021b7180(work, face, face->unk_05 + (data_ov174_021b7480 + 1)[2 * face->unk_03] + 2 * work->unk_10);
        face->unk_08 = 4;
        break;
    case 4:
        offset = 0x3000 * face->unk_01;
        func_ov174_021b71a0(work, face, srcBase + offset,
                            dstBase + (offset + (size * face->unk_00 + 0x28000)), 0x3000);
        face->unk_01++;
        if (0x3000 * face->unk_01 >= size) {
            face->unk_08 = 5;
            GFL_HeapFree(face->unk_10);
            face->unk_10 = NULL;
        }
        break;
    }
    if (face->unk_06 < 0x28) {
        face->unk_06++;
    } else if (face->unk_08 == 5 || face->unk_08 == 0) {
        face->unk_06 = 0;
        face->unk_04 = face->unk_04 == 0;
        face->unk_05 = face->unk_05 == 0;
    }
    if (face->unk_14 == 1 && func_0201c6b4(work->unk_78) == 1) {
        BmpWin_FlushChar(face->unk_18);
        face->unk_14 = 0;
    }
    face->unk_30 += GFL_RandomLC(2) + 1;
    if (face->unk_30 >= 0x96) {
        func_0204b95c(face->unk_28);
        func_0204b9a0(face->unk_28);
        face->unk_30 = 0;
    }
    if (face->unk_34 == 1 || face->unk_38 != 0) {
        if (func_0204b98c(face->unk_2c) == 0) {
            func_0204b95c(face->unk_2c);
        }
    } else if (func_0204b98c(face->unk_2c) == 1 && func_0204b918(face->unk_2c) == 0) {
        func_0204b974(face->unk_2c);
    }
    if (face->unk_38 != 0) {
        face->unk_38--;
    }
}


void func_ov174_021b6f24(Ov174Work *work, LocalTvtFace *face, u8 param_3) {
    face->unk_08 = 1;
}

u32 func_ov174_021b6f2c(Ov174Work *work, LocalTvtFace *face) {
    if (face->unk_08 == 5) {
        return 1;
    }
    return 0;
}

void func_ov174_021b6f3c(Ov174Work *work, LocalTvtFace *face) {
    u8 height = 24;

    if (work->unk_08 != 2) {
        height = 12;
    }
    GFL_G2DIOLoadArcNCLR(work->arc, (data_ov174_021b7480 + 4)[face->unk_02 * 3], 2, 0, face->unk_00 * 0x200 + 0x4000, 0x200, work->heapId);
    GFL_G2DIOLoadArcNCLR(work->arc, work->unk_10 + data_ov174_021b7480[face->unk_03 * 2], 2, 0, face->unk_00 * 0x200 + 0x6000, 0x200, work->heapId);
    GFL_BGSysSetScrPaletteNo(2, (face->unk_00 % 2) * 16, (face->unk_00 / 2) * height, 16, height, face->unk_00);
    GFL_BGSysSetScrPaletteNo(3, (face->unk_00 % 2) * 16, (face->unk_00 / 2) * height, 16, height, face->unk_00);
    GFL_BGSysQueueScrLoad(3);
    GFL_BGSysQueueScrLoad(2);
}

void func_ov174_021b7004(Ov174Work *work, LocalTvtFace *face) {
    u32 size;
    u32 srcOffset;
    ClActorSetup setup;

    if (work->unk_08 == 2) {
        size = 0x6000;
        srcOffset = 0;
    } else {
        size = 0x3000;
        srcOffset = 0x1c00;
    }
    func_ov174_021b7180(work, face, (data_ov174_021b7480 + 5)[face->unk_02 * 3]);
    func_ov174_021b71a0(work, face, srcOffset, size * face->unk_00 + 0x10000, size);
    GFL_HeapFree(face->unk_10);
    func_ov174_021b7180(work, face, (data_ov174_021b7480 + 1)[face->unk_03 * 2] + work->unk_10 * 2);
    func_ov174_021b71a0(work, face, srcOffset, size * face->unk_00 + 0x28000, size);
    GFL_HeapFree(face->unk_10);
    face->unk_1c = func_0204acec(work->arc, (data_ov174_021b7480 + 6)[face->unk_02 * 3], 0, (face->unk_00 * 2 + 4) * 0x20, work->heapId);
    face->unk_20 = func_0204a6c8(work->arc, (data_ov174_021b7480 + 6)[face->unk_02 * 3] + 1, 0, 0, work->heapId);
    face->unk_24 = func_0204af28(work->arc, (data_ov174_021b7480 + 6)[face->unk_02 * 3] + 2, (data_ov174_021b7480 + 6)[face->unk_02 * 3] + 3, work->heapId);
    setup.priority = 10;
    setup.bgPriority = 1;
    setup.x = (face->unk_00 % 2) * 128 + 64;
    setup.y = (face->unk_00 / 2) * 96 + 48;
    if (work->unk_08 == 2) {
        setup.y += 56;
    }
    setup.sequence = 0;
    face->unk_28 = func_0204b294(work->unk_50, face->unk_20, face->unk_1c, face->unk_24, &setup, 0, work->heapId);
    func_0204b3dc(face->unk_28, TRUE);
    func_0204b92c(face->unk_28, TRUE);
    func_0204b974(face->unk_28);
    setup.sequence = 1;
    face->unk_2c = func_0204b294(work->unk_50, face->unk_20, face->unk_1c, face->unk_24, &setup, 0, work->heapId);
    func_0204b3dc(face->unk_2c, TRUE);
    func_0204b92c(face->unk_2c, TRUE);
    func_0204b974(face->unk_2c);
}



void func_ov174_021b7180(Ov174Work *work, LocalTvtFace *face, u32 fileId) {
    face->unk_10 = GFL_G2DIOReadBGNCGRArc(work->arc, fileId, 0, &face->unk_0c, work->heapId);
}

void func_ov174_021b71a0(Ov174Work *work, LocalTvtFace *face, u32 srcOffset, u32 dstOffset, u32 size) {
    MIi_CpuCopy32((u8 *)face->unk_0c->pRawData + srcOffset, (u8 *)HW_BG_VRAM + dstOffset, size);
}

void func_ov174_021b71b8(Ov174Work *work, LocalTvtFace *face) {
    StrBuf *src;
    StrBuf *formatted;
    WordSet *wordSet;
    PlayerInfo *playerInfo;

    face->unk_18 = BmpWin_CreateDynamic(0, data_ov174_021b7380[face->unk_00].x + 2,
                                        data_ov174_021b7380[face->unk_00].y + 1, 12, 2, 12, TRUE);
    GFL_BitmapFill(BmpWin_GetBitmap(face->unk_18), 0);
    BmpWin_FlushChar(face->unk_18);
    BmpWin_FlushMap(face->unk_18);
    GFL_BGSysLoadScr(0);
    src = GFL_MsgDataLoadStrbufNew(work->unk_68, face->unk_02);
    if (face->unk_02 == 0 || face->unk_02 == 5) {
        formatted = GFL_StrBufCreate(0x80, work->heapId);
        playerInfo = GetGameDataPlayerInfo(work->unk_30->gameData);
        wordSet = GFL_WordSetSystemCreateDefault(work->heapId);
        copyVarForText(wordSet, 0, playerInfo);
        GFL_WordSetFormatStrbuf(wordSet, formatted, src);
        GFL_StrBufFree(src);
        GFL_WordSetSystemFree(wordSet);
        src = formatted;
    }
    face->unk_07 = GFL_FontGetBlockWidth(src, work->unk_60, 0);
    func_0201c724(work->unk_78, BmpWin_GetBitmap(face->unk_18), (s16)((0x60 - face->unk_07) / 2 + 1), 1, src, work->unk_60, PRINT_COLOR(15, 0, 0));
    func_0201c724(work->unk_78, BmpWin_GetBitmap(face->unk_18), (s16)((0x60 - face->unk_07) / 2 - 1), 1, src, work->unk_60, PRINT_COLOR(15, 0, 0));
    func_0201c724(work->unk_78, BmpWin_GetBitmap(face->unk_18), (s16)((0x60 - face->unk_07) / 2), 2, src, work->unk_60, PRINT_COLOR(15, 0, 0));
    func_0201c724(work->unk_78, BmpWin_GetBitmap(face->unk_18), (s16)((0x60 - face->unk_07) / 2), 0, src, work->unk_60, PRINT_COLOR(15, 0, 0));
    func_0201c724(work->unk_78, BmpWin_GetBitmap(face->unk_18), (s16)((0x60 - face->unk_07) / 2), 1, src, work->unk_60, PRINT_COLOR(1, 0, 0));
    GFL_StrBufFree(src);
    face->unk_14 = 1;
}


void func_ov174_021b7350(Ov174Work *work, LocalTvtFace *face) {
    BmpWin_FlushMap(face->unk_18);
    GFL_BGSysQueueScrLoad(0);
}

u8 func_ov174_021b7360(LocalTvtFace *face) {
    return face->unk_07;
}

void func_ov174_021b7364(LocalTvtFace *face, s32 speaking) {
    if (face->unk_34 == 1 && speaking == 0) {
        face->unk_38 = 0x46;
    }
    face->unk_34 = speaking;
}

void func_ov174_021b7378(LocalTvtFace *face) {
    face->unk_38 = 0;
    face->unk_34 = 0;
}
