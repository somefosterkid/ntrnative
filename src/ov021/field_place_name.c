// Black's code at ov021 0x021bc550-0x021bd068. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/field.h"
#include "field/field_event.h"
#include "field/field_status.h"
#include "field/intrude_comm.h"
#include "field/intrude_work.h"
#include "field/zone.h"
#include "gfl/arc.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "nitro/ioreg.h"
#include "nnsys/g2d.h"
#include "struct_decls.h"
#include "system/bmp_oam.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/gf_font.h"
#include "system/wordset.h"
#include "field/ov021_021bd068.h"
#include "field/field_place_name.h"

void func_ov021_021bca80(PlaceName *placeName);
void func_ov021_021bca6c(PlaceName *placeName);
void func_ov021_021bc654(PlaceName *placeName);
void func_ov021_021bc720(PlaceName *placeName);
void func_ov021_021bc6d0(PlaceName *placeName);
static const ClActSurfaceSetup data_ov021_021d9b78 = { 0, 0, 256, 192, 0, 1 };
static const PlaceNameWidths data_ov021_021d9b88 = { { 256.0f, 9.0f, -20.0f, 0.0f, 14.0f, 9.0f, 0.0f, 0.0f }, NULL, 0 };
void func_ov021_021bcab0(PlaceName *placeName);
void func_ov021_021bc848(PlaceName *placeName);
void func_ov021_021bca64(PlaceName *placeName);
void func_ov021_021bca78(PlaceName *placeName);
u32 func_ov021_021bca74(PlaceName *placeName);
u32 func_ov021_021bca88(PlaceName *placeName);
u32 IsLocNameIdxChanged(PlaceName *placeName);
u32 func_ov021_021bcb48(PlaceName *placeName, u16 param_2);
void func_ov021_021bcbc8(PlaceName *placeName);


void func_ov021_021bc550(PlaceName *placeName, s32 zoneId) {
    SetPlaceNameDispZone(placeName, zoneId);
    if (func_ov021_021bd140(zoneId) != 0) {
        func_ov021_021bcbc8(placeName);
        func_ov021_021bca78(placeName);
    }
}

void func_ov021_021bc578(PlaceName *placeName, u32 zoneId) {
    SetPlaceNameDispZone(placeName, zoneId);
    if (func_ov021_021bd140(zoneId) != 0) {
        func_ov021_021bcbc8(placeName);
        func_ov021_021bca64(placeName);
    }
}

void BeginForcePlaceNameDisp(PlaceName *placeName, u32 zoneId) {
    SetPlaceNameDispZone(placeName, zoneId);
    func_ov021_021bcbc8(placeName);
    func_ov021_021bca64(placeName);
}



void func_ov021_021bc5bc(PlaceName *placeName) {
    func_ov021_021bcb9c(placeName, 0);
    func_ov021_021bca80(placeName);
    func_ov021_021bca6c(placeName);
}

void func_ov021_021bc5d4(PlaceName *placeName) {
    func_ov021_021bc654(placeName);
    func_ov021_021bc720(placeName);
}

void func_ov021_021bc5e4(PlaceName *placeName) {
    u8 bg;
    BmpWin *window;

    func_ov021_021bc6d0(placeName);
    func_ov021_021bc6e8(placeName);
    func_ov021_021bd01c(placeName);
    window = placeName->unk_24;
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    bg = BmpWin_GetBGIndex(window);
    GFL_BGSysLoadScr(bg);
    GFL_BGSysSetBGPriority(3, 1);
    GFL_BGSysSetBGEnabled(3, 0);
    G2_SetBlendAlpha(8, 0x11, 0x10, 4);
}

void func_ov021_021bc63c(PlaceName *placeName) {
    GFL_BGSysClearBG(3);
    func_ov021_021bc704(placeName);
    func_ov021_021bd07c(placeName);
}

void func_ov021_021bc654(PlaceName *placeName) {
    func_ov021_021bc63c(placeName);
    func_ov021_021bc5e4(placeName);
}

void func_ov021_021bc664(PlaceName *placeName) {
    HeapID heapId;
    ArcTool *handle;
    u32 size;
    void *dest;

    heapId = func_ov021_021bd090(placeName);
    handle = func_ov021_021bd094(placeName);
    size = GFL_ArcToolGetDataLength(handle, 1);
    dest = GFL_HeapAllocate(heapId, size, 0, "field_place_name.c", 0x2c8);
    placeName->unk_14c = dest;
    GFL_ArcToolRead(handle, 1, dest);
    NNS_G2dGetUnpackedPaletteData(placeName->unk_14c, &placeName->unk_150);
}

void func_ov021_021bc6b4(PlaceName *placeName) {
    if (placeName->unk_14c != NULL) {
        GFL_HeapFree(placeName->unk_14c);
        placeName->unk_14c = NULL;
        placeName->unk_150 = NULL;
    }
}

void func_ov021_021bc6d0(PlaceName *placeName) {
    GFL_BGSysUploadStdPalette(3, placeName->unk_150->pRawData, 0x20, 0);
}

void func_ov021_021bc6e8(PlaceName *placeName) {
    if (placeName->unk_28 == 0xffffffff) {
        placeName->unk_28 = GFL_BGSysAllocChar(3, 0x20, 0);
    }
}

void func_ov021_021bc704(PlaceName *placeName) {
    if (placeName->unk_28 != 0xffffffff) {
        GFL_BGSysFreeCharMemory(3, placeName->unk_28, 0x20);
        placeName->unk_28 = 0xffffffff;
    }
}

void func_ov021_021bc720(PlaceName *placeName) {
    GFLBitmap *bitmap;

    bitmap = BmpWin_GetBitmap(placeName->unk_24);
    GFL_BitmapFill(bitmap, 3);
}

void func_ov021_021bc730(PlaceName *placeName) {
    ClActRenderer *pCVar1;

    pCVar1 = func_0204b034(&data_ov021_021d9b78, 1, placeName->unk_c);
    placeName->unk_38 = pCVar1;
}

void func_ov021_021bc748(PlaceName *placeName) {
    if (placeName->unk_38 != NULL) {
        func_0204b084(placeName->unk_38);
        placeName->unk_38 = NULL;
    }
}

void func_ov021_021bc75c(PlaceName *placeName) {
    HeapID heapId;
    ArcTool *arc;

    heapId = func_ov021_021bd090(placeName);
    arc = func_ov021_021bd094(placeName);
    placeName->unk_2c = func_0204ac18(arc, 3, 0, 0x20, 0, 1, heapId);
}

void func_ov021_021bc788(PlaceName *placeName) {
    func_0204ada4(placeName->unk_2c);
}

void func_ov021_021bc794(PlaceName *placeName) {
    HeapID heapId;
    ClActUnit *unit;

    heapId = func_ov021_021bd090(placeName);
    unit = func_0204b100(0x1e, 1, heapId);
    placeName->unk_34[0] = unit;
    func_0204b22c(unit, 1);
}

void func_ov021_021bc7b0(PlaceName *placeName) {
    func_0204b1cc(placeName->unk_34[0]);
}

void func_ov021_021bc7bc(PlaceName *placeName, s32 param_2) {
    if (placeName->unk_38 != NULL) {
        func_0204b258(placeName->unk_34[param_2], placeName->unk_38);
    }
}

void func_ov021_021bc7d4(PlaceName *placeName) {
    s32 i;

    for (i = 0; i < 0x1e; i++) {
        placeName->unk_50[i] = func_ov021_021bd148(placeName->unk_c, placeName->unk_30, 0, placeName->unk_2c);
    }
}


void func_ov021_021bc7f8(PlaceName *placeName) {
    s32 i;

    i = 0;
    do {
        func_ov021_021bd184(placeName->unk_50[i]);
        i = i + 1;
    } while (i < 0x1e);
}

void func_ov021_021bc810(PlaceName *placeName) {
    HeapID heapId;
    ArcTool *pAVar1;

    heapId = func_ov021_021bd090(placeName);
    pAVar1 = GFL_ArcSysCreateFileHandle(0x54, heapId);
    placeName->unk_10 = pAVar1;
}

void func_ov021_021bc824(PlaceName *placeName) {
    if (placeName->unk_10 != NULL) {
        GFL_ArcToolFree(placeName->unk_10);
        placeName->unk_10 = NULL;
    }
}

void func_ov021_021bc838(PlaceName *placeName) {
    placeName->unk_148 = 0;
    placeName->unk_14b = 0;
}

void func_ov021_021bc848(PlaceName *placeName) {
    s32 idx = placeName->unk_148;
    u16 str[0x20];
    PlaceNameWidths args;
    s32 i;

    GFL_StrBufStoreString((StrBuf *)func_ov021_021bd134(placeName), str, 0x20);
    args = data_ov021_021d9b88;
    args.unk_20 = placeName->unk_18;
    args.unk_24 = str[idx];
    for (i = 0; i < idx; i++) {
        if (placeName->unk_c8[i] == NULL) {
            args.unk_0[4] += GFL_FontGetCharWidth(placeName->unk_18, 0x20);
        } else {
            args.unk_0[4] += (s32)func_ov021_021bd1ec(placeName->unk_c8[i]);
        }
        args.unk_0[4] += 1.0f;
    }
    if (args.unk_24 == 0x20) {
        placeName->unk_c8[idx] = NULL;
        placeName->unk_14b++;
    } else {
        placeName->unk_c8[idx] = placeName->unk_50[idx - placeName->unk_14b];
        func_ov021_021bd19c(placeName->unk_c8[idx], &args);
    }
    placeName->unk_148++;
}


u32 func_ov021_021bc910(PlaceName *placeName) {
    if (func_ov021_021bd138(placeName) <= placeName->unk_148) {
        return 1;
    }
    return 0;
}

void func_ov021_021bc92c(PlaceName *placeName) {
    s32 i;

    for (i = 0; i < 0x1e; i++) {
        func_ov021_021bd1e4(placeName->unk_50[i], FALSE);
    }
}

void func_ov021_021bc948(PlaceName *placeName) {
    u8 idx = placeName->unk_149;

    placeName->unk_149 = idx + 1;
    if (placeName->unk_c8[idx] != NULL) {
        func_ov021_021bd1d4(placeName->unk_c8[idx]);
        func_ov021_021bd1e4(placeName->unk_c8[idx], TRUE);
    }
}

void func_ov021_021bc970(PlaceName *placeName) {
    s32 i;
    s32 count;

    count = func_ov021_021bd138(placeName);
    for (i = 0; i < count; i++) {
        if (placeName->unk_c8[i] != NULL) {
            func_ov021_021bd1cc(placeName->unk_c8[i]);
        }
    }
}




void func_ov021_021bc998(PlaceName *placeName) {
    HeapID heapId;
    BmpOamSys *pBVar1;

    heapId = func_ov021_021bd090(placeName);
    pBVar1 = func_02025538(heapId, placeName->unk_34[0]);
    placeName->unk_30 = pBVar1;
}

void func_ov021_021bc9ac(PlaceName *placeName) {
    if (placeName->unk_30 != NULL) {
        func_02025588(placeName->unk_30);
        placeName->unk_30 = NULL;
    }
}

void func_ov021_021bc9c0(PlaceName *placeName) {
    placeName->unk_14a = 0;
}

void func_ov021_021bc9cc(PlaceName *placeName) {
    Ov021Work *work = placeName->unk_c8[placeName->unk_14a];

    if (work != NULL) {
        GFLBitmap *src = func_ov021_021bd1dc(work);
        GFLBitmap *dest = BmpWin_GetBitmap(placeName->unk_24);
        u16 destX = func_ov021_021bd1fc(work);
        u16 destY = func_ov021_021bd204(work) - 8;
        u16 width = func_ov021_021bd1ec(work);
        u16 height = func_ov021_021bd1f4(work);

        GFL_BitmapCopyArea(src, dest, 0, 0, destX, destY, width, height, 0);
    }
    placeName->unk_14a++;
}

BOOL func_ov021_021bca48(PlaceName *placeName) {
    s32 count = func_ov021_021bd138(placeName);

    if (count <= placeName->unk_14a) {
        return TRUE;
    }
    return FALSE;
}



void func_ov021_021bca64(PlaceName *placeName) {
    placeName->unk_4c = 1;
}

void func_ov021_021bca6c(PlaceName *placeName) {
    placeName->unk_4c = 0;
}

u32 func_ov021_021bca74(PlaceName *placeName) {
    return placeName->unk_48;
}

void func_ov021_021bca78(PlaceName *placeName) {
    placeName->unk_48 = 1;
}

void func_ov021_021bca80(PlaceName *placeName) {
    placeName->unk_48 = 0;
}

u32 func_ov021_021bca88(PlaceName *placeName) {
    return placeName->unk_4c;
}

u32 IsLocNameIdxChanged(PlaceName *placeName) {
    if (ZoneData_GetPlaceNameID(placeName->unk_44) != ZoneData_GetPlaceNameID(placeName->unk_46)) {
        return 1;
    }
    return 0;
}

void func_ov021_021bcab0(PlaceName *placeName) {
    u16 zoneId = func_ov021_021bd118(placeName);
    u32 messageId = ZoneData_GetPlaceNameID(zoneId);

    if (messageId >= 0x75) {
        messageId = 0;
    }
    if (messageId == 0) {
        func_ov021_021bc5bc(placeName);
        return;
    }
    if (func_ov021_021bcb48(placeName, zoneId) != 0) {
        GameCommSys *commSys = GSYS_GetGameCommSystem(placeName->unk_0);
        u8 netId;
        PlayerInfo *playerInfo;
        StrBuf *src;

        func_ov010_02158344(commSys);
        netId = func_ov010_0215841c(placeName->unk_4);
        playerInfo = func_ov010_021583ec(placeName->unk_4, netId);
        src = GFL_MsgDataLoadStrbufNew(placeName->unk_14, 3);
        GFL_MsgDataLoadStrbuf(placeName->unk_14, messageId, placeName->unk_1c);
        copyVarForText(placeName->unk_8, 0, playerInfo);
        loadLocationNameToStrbuf(placeName->unk_8, 1, messageId);
        GFL_WordSetFormatStrbuf(placeName->unk_8, placeName->unk_1c, src);
        GFL_StrBufFree(src);
    } else {
        GFL_MsgDataLoadStrbuf(placeName->unk_14, messageId, placeName->unk_1c);
    }
    placeName->unk_20 = GFL_StrBufGetCharCount(placeName->unk_1c);
}


u32 func_ov021_021bcb48(PlaceName *placeName, u16 param_2) {
    u8 result;
    GameCommSys *commSys;
    IntrudeComm *pIVar2;
    FieldStatus *status;
    u32 result2;
    BOOL BVar4;

    commSys = GSYS_GetGameCommSystem(placeName->unk_0);
    pIVar2 = func_ov010_02158344(commSys);
    status = GameData_GetFieldStatus(placeName->unk_4);
    result2 = func_02012b58(placeName->unk_4);
    if (pIVar2 != NULL) {
        result = func_ov010_0215841c(placeName->unk_4);
        BVar4 = func_0201444c(status);
        if (BVar4 == 1) {
            return 1;
        }
        BVar4 = IsZoneEntralinkHub(param_2);
        if ((BVar4 != 0) && (result2 != result)) {
            return 1;
        }
    }
    return 0;
}

void func_ov021_021bcb9c(PlaceName *placeName, u32 param_2) {
    func_ov021_021bd0a0(placeName, param_2);
    func_ov021_021bd0a8(placeName);
    func_ov021_021bd0c8(placeName);
    if (param_2 == 0) {
        GFL_BGSysSetBGEnabled(3, 0);
        func_ov021_021bc92c(placeName);
    }
}

void func_ov021_021bcbc8(PlaceName *placeName) {
    u32 mode;
    int val;

    switch (placeName->unk_3c) {
    case 0:
    case 4:
    case 6:
        return;
    case 3:
    case 5:
        mode = 5;
        val = 30;
        break;
    case 2:
        mode = 6;
        val = (int)(20.0f * (1.0f - (f32)placeName->unk_42 / 10.0f));
        break;
    case 1:
        mode = 0;
        val = 0;
        break;
    }
    func_ov021_021bcb9c(placeName, mode);
    placeName->unk_42 = val;
}

void func_ov021_021bcc3c(PlaceName *placeName) {
    BOOL flag = FALSE;

    if (func_ov021_021bca88(placeName) != 0) {
        flag = TRUE;
        func_ov021_021bca6c(placeName);
    } else if (func_ov021_021bca74(placeName) != 0) {
        if (IsLocNameIdxChanged(placeName) != 0) {
            flag = TRUE;
        }
        func_ov021_021bca80(placeName);
    }
    if (flag) {
        func_ov021_021bcb9c(placeName, 1);
    }
}




void func_ov021_021bcc80(PlaceName *placeName) {
    u8 bg;
    BmpWin *window;

    switch(func_ov021_021bd0c0(placeName)) {
    case 0:
        func_ov021_021bcab0(placeName);
        func_ov021_021bd0d0(placeName);
        return;
    case 1:
        GFL_BGSysClearChar(3);
        GFL_BGSysClearScr(3);
        func_ov021_021bc6d0(placeName);
        func_ov021_021bd0d0(placeName);
        return;
    case 2:
        func_ov021_021bc720(placeName);
        window = placeName->unk_24;
        BmpWin_FlushChar(window);
        BmpWin_FlushMap(window);
        bg = BmpWin_GetBGIndex(window);
        GFL_BGSysLoadScr(bg);
        GFL_BGSysSetBGPriority(3, 1);
        GFL_BGSysSetBGEnabled(3, 0);
        G2_SetBlendAlpha(8, 0x11, 0x10, 4);
        func_ov021_021bc838(placeName);
        func_ov021_021bd0d0(placeName);
        return;
    case 3:
        func_ov021_021bc848(placeName);
        if (func_ov021_021bc910(placeName) != 0) {
            func_ov021_021bcb9c(placeName, 2);
        }
    }
}

void func_ov021_021bcd34(PlaceName *placeName) {
    switch (func_ov021_021bd0c0(placeName)) {
    case 0:
        func_ov021_021bceb4(placeName);
        GFL_BGSysSetBGEnabled(3, 1);
        func_ov021_021bd0d0(placeName);
        break;
    case 1:
        if (placeName->unk_42 > 10) {
            func_ov021_021bcb9c(placeName, 3);
        }
        break;
    }
}


void func_ov021_021bcd70(PlaceName *placeName) {
    if (10 < placeName->unk_42) {
        func_ov021_021bcb9c(placeName, 4);
    }
}

void func_ov021_021bcd84(PlaceName *placeName) {
    switch (func_ov021_021bd0c0(placeName)) {
    case 0:
        placeName->unk_149 = 0;
        func_ov021_021bd0d0(placeName);
        break;
    case 1:
        if (placeName->unk_42 % 3 == 0) {
            func_ov021_021bc948(placeName);
            if (placeName->unk_20 <= placeName->unk_149) {
                func_ov021_021bcb9c(placeName, 5);
            }
        }
        break;
    }
}

void func_ov021_021bcdd8(PlaceName *placeName) {
    BOOL flag = TRUE;
    s32 i;

    switch (func_ov021_021bd0c0(placeName)) {
    case 0:
        for (i = 0; i < placeName->unk_20; i++) {
            if (placeName->unk_c8[i] != NULL && func_ov021_021bd20c(placeName->unk_c8[i]) != 0) {
                flag = FALSE;
                break;
            }
        }
        if (flag) {
            func_ov021_021bc9c0(placeName);
            func_ov021_021bd0d0(placeName);
            return;
        }
        break;
    case 1:
        func_ov021_021bc9cc(placeName);
        if (func_ov021_021bca48(placeName)) {
            func_ov021_021bd0d0(placeName);
            return;
        }
        break;
    case 2:
        BmpWin_FlushChar(placeName->unk_24);
        func_ov021_021bd0d0(placeName);
        return;
    case 3:
        func_ov021_021bc92c(placeName);
        func_ov021_021bd0d0(placeName);
        return;
    case 4:
        if (placeName->unk_42 > 0x1e) {
            func_ov021_021bcb9c(placeName, 6);
        }
        break;
    }
}

void func_ov021_021bce88(PlaceName *placeName) {
    if (placeName->unk_42 > 0x14) {
        func_ov021_021bcb9c(placeName, 0);
        GFL_BGSysSetBGEnabled(3, 0);
        func_ov021_021bc92c(placeName);
    }
}




void func_ov021_021bceac(PlaceName *placeName) {
}

void func_ov021_021bceb0(PlaceName *placeName) {
}

void func_ov021_021bceb4(PlaceName *placeName) {
    f32 t = placeName->unk_42 / 10.0f;

    G2_SetBlendAlpha(8, 0x11, (int)(16.0f * t), (int)(4.0f + 12.0f * (1.0f - t)));
}

void func_ov021_021bcf18(PlaceName *placeName) {
}

void func_ov021_021bcf1c(PlaceName *placeName) {
}

void func_ov021_021bcf20(PlaceName *placeName) {
}

void func_ov021_021bcf24(PlaceName *placeName) {
    f32 t = placeName->unk_42 / 20.0f;

    G2_SetBlendAlpha(8, 0x11, (int)(16.0f * (1.0f - t)), (int)(4.0f + 12.0f * t));
}

PlaceName *func_ov021_021bcf88(HeapID heapId) {
    return GFL_HeapAllocate(heapId, sizeof(PlaceName), 1, "field_place_name.c", 0x73c);
}

void func_ov021_021bcfa4(PlaceName *placeName, GameSystem *gsys) {
    MI_CpuFill8(placeName, 0, sizeof(PlaceName));
    placeName->unk_0 = gsys;
    placeName->unk_4 = GSYS_GetGameData(gsys);
    placeName->unk_28 = 0xffffffff;
    placeName->unk_4c = 0;
    placeName->unk_48 = 0;
    placeName->unk_44 = 0x1ab;
    placeName->unk_46 = 0x1ab;
}

void func_ov021_021bcfd8(PlaceName *placeName) {
    GFL_HeapFree(placeName);
}


void func_ov021_021bcfe0(PlaceName *placeName) {
    HeapID heapId;
    WordSet *pWVar1;

    heapId = func_ov021_021bd090(placeName);
    pWVar1 = GFL_WordSetSystemCreateDefault(heapId);
    placeName->unk_8 = pWVar1;
}

void func_ov021_021bcff0(PlaceName *placeName) {
    HeapID heapId;
    MsgData *pMVar1;

    heapId = func_ov021_021bd090(placeName);
    pMVar1 = GFL_MsgSysLoadData(0, 2, 0x59, heapId);
    placeName->unk_14 = pMVar1;
}

void func_ov021_021bd008(PlaceName *placeName) {
    HeapID heapId;
    StrBuf *pSVar1;

    heapId = func_ov021_021bd090(placeName);
    pSVar1 = GFL_StrBufCreate(0x21, heapId);
    placeName->unk_1c = pSVar1;
}

void func_ov021_021bd01c(PlaceName *placeName) {
    BmpWin *pBVar1;

    func_ov021_021bd090(placeName);
    pBVar1 = BmpWin_CreateDynamic(3, 0, 1, 0x20, 2, 0, 0);
    placeName->unk_24 = pBVar1;
}

void func_ov021_021bd040(PlaceName *placeName) {
    if (placeName->unk_8 != NULL) {
        GFL_WordSetSystemFree(placeName->unk_8);
        placeName->unk_8 = NULL;
    }
}

void func_ov021_021bd054(PlaceName *placeName) {
    if (placeName->unk_14 != NULL) {
        GFL_MsgDataFree(placeName->unk_14);
        placeName->unk_14 = NULL;
    }
}
