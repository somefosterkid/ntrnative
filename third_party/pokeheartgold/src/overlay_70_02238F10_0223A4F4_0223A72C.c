#include "global.h"

#include "gf_gfx_loader.h"
#include "heap.h"
#include "msgdata.h"
#include "pm_string.h"
#include "pokemon.h"
#include "unk_02013FDC.h"

s32 ov70_02245084(void *, String *, s32, s32, s32, s32);

void ov70_0223A72C(MsgData *arg0, u8 *arg1, u16 *arg2, Pokemon *arg3, u8 *arg4);

void ov70_0223A72C(MsgData *msgData, u8 *arg1, u16 *arg2, Pokemon *arg3, u8 *arg4) {
    String *string;
    Pokemon *mon;
    u16 *src;
    String *v0;
    String *v1;
    String *v2;
    String *v3;

    src = arg2;
    mon = arg3;
    v1 = String_New(0x10U, HEAP_ID_61);
    v2 = String_New(0x10U, HEAP_ID_61);
    v0 = NewString_ReadMsgData(msgData, 0x31);
    string = v0;
    CopyU16ArrayToString(v1, src);
    v3 = NewString_ReadMsgData(msgData, 0xB4);
    GetMonData(mon, 0x91, v2);
    ov70_02245084(arg1, string, 0, 0, 0, 0xF0200);
    ov70_02245084(arg1 + 0x10, v1, 0, 0, 0, 0x10200);
    ov70_02245084(arg4, v3, 0, 0, 0, 0xF0200);
    ov70_02245084(arg4 + 0x10, v2, 0, 0, 0, 0x10200);
    String_Delete(v0);
    String_Delete(v1);
    String_Delete(v3);
    String_Delete(v2);
}

void ov70_0223A7E4(Pokemon *arg0);

void ov70_0223A7E4(Pokemon *mon) {
    PokepicTemplate pokepicTemplate;
    u32 pid;
    void *v0;

    v0 = Heap_Alloc(HEAP_ID_61, 0xC80U);
    GetPokemonSpriteCharAndPlttNarcIds(&pokepicTemplate, mon, 2U);
    pid = GetMonData(mon, 0, NULL);
    sub_02014494((enum NarcId)pokepicTemplate.narcID, (s32)pokepicTemplate.charDataID, HEAP_ID_61, 0, 0, 0xA, 0xA, v0, pid, 0, 2, (s32)GetMonData(mon, 5, NULL));
    DC_FlushRange(v0, 0xC80U);
    GX_LoadOBJ(v0, 0x4A00U, 0xC80U);
    GfGfxLoader_GXLoadPal((enum NarcId)pokepicTemplate.narcID, (s32)pokepicTemplate.palDataID, GF_PAL_LOCATION_MAIN_OBJ, GF_PAL_SLOT_13_OFFSET, 0x20U, HEAP_ID_61);
    Heap_Free(v0);
}
