#include "global.h"

#include "text.h"
#include "unk_02005D10.h"
#include "bg_window.h"
#include "message_format.h"
#include "pm_string.h"
#include "heap.h"
#include "pokemon.h"
#include "pokewalker.h"
#include "overlay_112_pokewalker_app.h"

s32 ov112_021EA838(Ov112PokewalkerApp *);

void ov112_021EAAE4(Ov112PokewalkerApp *, void *);

u32 ov112_021EDF08(Ov112PokewalkerApp *param_1);

s32 ov112_021E7464(void);
u8 ov112_021EA08C(Ov112PokewalkerApp *, s32, s32);
void *ov112_021ED610(void *);
s32 ov112_021EDF4C(Ov112PokewalkerApp *arg0);

s32 ov112_021EDFE8(Ov112PokewalkerApp *arg0);

u32 ov112_021EDF08(Ov112PokewalkerApp *param_1) {
    u8 uVar1;
    s32 iVar2;
    BOOL BVar3;

    ov112_021EAAE4(param_1, param_1->unk_1F2C0);
    uVar1 = TextPrinterCheckActive((u8)param_1->unk_1E524);
    if (((uVar1 == 0) && (iVar2 = ov112_021EA838(param_1), iVar2 == 0)) && (BVar3 = IsFanfarePlaying(), BVar3 == 0)) {
        param_1->unk_4 = 5;
        return 0x42;
    }
    return 0x24;
}

s32 ov112_021EDF4C(Ov112PokewalkerApp *arg0) {
    String *string;
    s32 v0;

    string = String_New(0xCU, HEAP_ID_154);
    FillBgTilemapRect(arg0->bgConfig, 2U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    BgCommitTilemapBufferToVram(arg0->bgConfig, 2U);
    if (ov112_021ED610(arg0) != 0) {
        CopyU16ArrayToStringN(string, arg0->unk_9D54, 0xCU);
        BufferString(arg0->msgFormat, 1U, string, (s32)((u32)(arg0->unk_9D51 << 0x19) >> 0x1E), 1, 1);
        v0 = ov112_021EA08C(arg0, 2, 0x1B);
    } else {
        v0 = ov112_021EA08C(arg0, 2, 0x20);
    }
    arg0->unk_1E524 = v0;
    ov112_021E7464();
    String_Delete(string);
    return 0x26;
}

s32 ov112_021EDFE8(Ov112PokewalkerApp *arg0) {
    s32 v2;
    BoxPokemon *mon;
    u32 v0;
    u32 v1;

    v2 = 1;
    mon = (BoxPokemon *)AllocMonZeroed(HEAP_ID_154);
    if (Pokewalker_TryGetBoxMon(arg0->pokewalker, mon) == 0) {
        v2 = 0;
    }
    v1 = GetMonData((Pokemon *)mon, 5, NULL);
    v0 = GetMonData((Pokemon *)mon, 0x70, NULL);
    if ((arg0->unk_9D44 != v1) || (((u32)(arg0->unk_9D51 << 0x1B) >> 0x1B) != v0)) {
        v2 = 0;
    }
    Heap_Free(mon);
    return v2;
}
