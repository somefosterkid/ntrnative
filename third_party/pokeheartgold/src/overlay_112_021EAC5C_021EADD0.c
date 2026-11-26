#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "heap.h"
#include "message_format.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "pokewalker.h"
#include "string_util.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021E7CA4(Ov112PokewalkerApp *, u32, u32);

void ov112_021E9290(BoxPokemon *, void *, void *, u32);

void ov112_021EA010(Ov112PokewalkerApp *, u32, String *, u32, u32);

void ov112_021EADD0(Ov112PokewalkerApp *param_1, s32 param_2) {
    s32 num;
    Pokemon *mon;
    BoxPokemon *boxMon;
    u32 uStack_18[2];
    sub_02032674(param_1->pokewalker, &uStack_18[1], &uStack_18[0]);
    BufferIntegerAsString(param_1->msgFormat, 7, uStack_18[0], 7, PRINTING_MODE_LEFT_ALIGN, 1);
    BufferIntegerAsString(param_1->msgFormat, 6, uStack_18[1], 7, PRINTING_MODE_LEFT_ALIGN, 1);
    num = ov112_021E9610(param_1, uStack_18[0]);
    BufferIntegerAsString(param_1->msgFormat, 0xc, num, 7, PRINTING_MODE_LEFT_ALIGN, 1);
    ov112_021E7CA4(param_1, 5, 0x10);
    ov112_021E98E8(param_1, 1);
    param_1->unk_1F2E0 = 0;
    ov112_021EA010(param_1, 1, param_1->unk_1E50C, 0, 0x10200);
    ov112_021EA044(param_1, 2, 0x53, 0x10200);
    ov112_021EA010(param_1, 3, param_1->unk_1E510, 0, 0x10200);
    ov112_021EA044(param_1, 4, 0x54, 0x10200);
    if (num < 0) {
        ov112_021EA044(param_1, 0, 0x58, 0x10200);
    } else if (num == 0) {
        ov112_021EA044(param_1, 0, 0x57, 0x10200);
    } else {
        ov112_021EA044(param_1, 0, 0x56, 0x10200);
    }
    if (param_2 != 0) {
        mon = AllocMonZeroed(HEAP_ID_154);
        boxMon = Mon_GetBoxMon(mon);
        Pokewalker_TryGetBoxMon(param_1->pokewalker, boxMon);
        BufferBoxMonNickname(param_1->msgFormat, 0xd, boxMon);
        ov112_021EA044(param_1, 5, 0x59, 0x30400);
        ov112_021E9290(boxMon, param_1->unk_1D7AC, param_1->unk_1D79C, 1);
        ov112_021EA670(param_1, 7);
        Heap_Free(mon);
    } else {
        ov112_021EA688(param_1, 7);
        FillWindowPixelBuffer(&param_1->windows2[5], 0);
        CopyWindowToVram(&param_1->windows2[5]);
    }
    ov112_021EA64C(param_1);
    return;
}
