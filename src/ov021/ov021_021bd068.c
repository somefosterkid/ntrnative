// Black's code at ov021 0x021bd068-0x021bd214. No name is known: the module and address.

#include "types.h"
#include "field/ov021_021bd068.h"
#include "field/ov021_021bd2b8.h"
#include "field/place_name_letter.h"
#include "gfl/bmpwin.h"
#include "gfl/str.h"
#include "save/player_savedata.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "system/game_system.h"

void func_ov021_021bd068(PlaceName *placeName) {
    if (placeName->unk_1c != NULL) {
        GFL_StrBufFree(placeName->unk_1c);
        placeName->unk_1c = NULL;
    }
}

void func_ov021_021bd07c(PlaceName *placeName) {
    if (placeName->unk_24 != NULL) {
        BmpWin_Free(placeName->unk_24);
        placeName->unk_24 = NULL;
    }
}

HeapID func_ov021_021bd090(PlaceName *placeName) {
    return placeName->unk_c;
}

ArcTool *func_ov021_021bd094(PlaceName *placeName) {
    return placeName->unk_10;
}

void func_ov021_021bd098(PlaceName *placeName, HeapID heapId) {
    placeName->unk_c = heapId;
}

void func_ov021_021bd09c(PlaceName *placeName, Font *font) {
    placeName->unk_18 = font;
}

void func_ov021_021bd0a0(PlaceName *placeName, u32 param_2) {
    placeName->unk_3c = param_2;
}

u32 func_ov021_021bd0a4(PlaceName *placeName) {
    return placeName->unk_3c;
}

void func_ov021_021bd0a8(PlaceName *placeName) {
    placeName->unk_42 = 0;
}

void func_ov021_021bd0b0(PlaceName *placeName) {
    placeName->unk_42++;
}


u8 func_ov021_021bd0c0(PlaceName *placeName) {
    return placeName->unk_40;
}

void func_ov021_021bd0c8(PlaceName *placeName) {
    placeName->unk_40 = 0;
}

void func_ov021_021bd0d0(PlaceName *placeName) {
    placeName->unk_40++;
}


void func_ov021_021bd0e0(PlaceName *placeName) {
    GameData *gameData;
    SaveControl *save;
    PlayerSave *playerSave;

    gameData = GSYS_GetGameData(placeName->unk_0);
    save = GameData_GetSaveControl(gameData);
    playerSave = SaveControl_GetPlayerSave(save);
    placeName->unk_46 = PlayerSave_GetPlaceNameZoneID(playerSave);
}

void func_ov021_021bd0fc(PlaceName *placeName) {
    GameData *gameData;
    SaveControl *save;
    PlayerSave *playerSave;

    gameData = GSYS_GetGameData(placeName->unk_0);
    save = GameData_GetSaveControl(gameData);
    playerSave = SaveControl_GetPlayerSave(save);
    PlayerSave_SetPlaceNameZoneID(playerSave, placeName->unk_46);
}

u16 func_ov021_021bd118(PlaceName *placeName) {
    return placeName->unk_46;
}

void SetPlaceNameDispZone(PlaceName *placeName, u16 zoneId) {
    placeName->unk_44 = placeName->unk_46;
    placeName->unk_46 = zoneId;
}


StrBuf *func_ov021_021bd134(PlaceName *placeName) {
    return placeName->unk_1c;
}

u8 func_ov021_021bd138(PlaceName *placeName) {
    return placeName->unk_20;
}

u32 func_ov021_021bd140(u16 zoneId) {
    return func_02013e6c(zoneId);
}

Ov021Work *func_ov021_021bd148(HeapID heapID, BmpOamSys *x, u16 y, u32 z) {
    Ov021Work *work = func_ov021_021bd214(heapID);
    func_ov021_021bd234(work);
    func_ov021_021bd2bc(work, heapID);
    func_ov021_021bd250(work);
    func_ov021_021bd278(work, x, y, z);
    func_ov021_021bd5f8(work, FALSE);
    return work;
}


void func_ov021_021bd184(Ov021Work *work) {
    func_ov021_021bd2a4(work);
    func_ov021_021bd264(work);
    func_ov021_021bd22c(work);
}

void func_ov021_021bd19c(Ov021Work *work, PlaceNameWidths *args) {
    func_ov021_021bd2c4(work, args->unk_20);
    func_ov021_021bd2d0(work, args->unk_24);
    func_ov021_021bd30c(work);
    func_ov021_021bd57c(work);
    func_ov021_021bd588(work);
    func_ov021_021bd554(work, args);
}



void func_ov021_021bd1cc(Ov021Work *work) {
    func_ov021_021bd3bc(work);
}

void func_ov021_021bd1d4(Ov021Work *work) {
    func_ov021_021bd474(work);
}

GFLBitmap *func_ov021_021bd1dc(Ov021Work *work) {
    return func_ov021_021bd308(work);
}

void func_ov021_021bd1e4(Ov021Work *work, BOOL flag) {
    func_ov021_021bd5f8(work, flag);
}

u32 func_ov021_021bd1ec(Ov021Work *work) {
    return func_ov021_021bd2f4(work);
}

u32 func_ov021_021bd1f4(Ov021Work *work) {
    return func_ov021_021bd2fc(work);
}

u32 func_ov021_021bd1fc(Ov021Work *work) {
    return func_ov021_021bd2dc(work);
}

u32 func_ov021_021bd204(Ov021Work *work) {
    return func_ov021_021bd2e8(work);
}

u32 func_ov021_021bd20c(Ov021Work *work) {
    return func_ov021_021bd2d8(work);
}










