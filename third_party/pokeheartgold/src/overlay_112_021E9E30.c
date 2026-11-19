#include "global.h"

#include "pm_string.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021E9E30(Ov112PokewalkerApp *arg0);

u8 ov112_021E9FA4(void *, s32, String *, s32, s32);
void ov112_021E9F40(void *arg0, s32 arg1);

void ov112_021E9E30(Ov112PokewalkerApp *arg0) {
    String_Delete(arg0->unk_1E458);
    String_Delete(arg0->unk_1E45C);
    String_Delete(arg0->unk_1E460);
    String_Delete(arg0->unk_1E464);
    String_Delete(arg0->unk_1E468);
    String_Delete(arg0->unk_1E46C);
    String_Delete(arg0->unk_1E470);
    String_Delete(arg0->unk_1E474);
    String_Delete(arg0->unk_1E494);
    String_Delete(arg0->unk_1E49C);
    String_Delete(arg0->unk_1E498);
    String_Delete(arg0->unk_1E4A0);
    String_Delete(arg0->unk_1E4A4);
    String_Delete(arg0->unk_1E4A8);
    String_Delete(arg0->unk_1E4AC);
    String_Delete(arg0->unk_1E4B0);
    String_Delete(arg0->unk_1E4B4);
    String_Delete(arg0->unk_1E4BC);
    String_Delete(arg0->unk_1E50C);
    String_Delete(arg0->unk_1E510);
    String_Delete(arg0->unk_1E514);
    String_Delete(arg0->unk_1E518);
}

void ov112_021E9F40(void *arg0, s32 arg1) {
    ov112_021E9FA4(arg0, arg1, *(void **)((u8 *)arg0 + 0x1E474), 4, 0xF0100);
}
