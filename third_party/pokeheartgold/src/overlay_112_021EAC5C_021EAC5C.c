#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "pokemon_storage_system.h"
#include "pokewalker.h"
#include "save_arrays.h"
#include "screen_fade.h"
#include "overlay_112_pokewalker_app.h"

typedef enum HeapID HeapID;

typedef enum FadeType FadeType;

typedef enum FadeMode FadeMode;

void ov112_021E9C10(void *work, int a1);

void ov112_021E9A78(void *work, u8 a1);

void ov112_021EADD0(void *work, int a1);

int ov112_021EAC5C(Ov112PokewalkerApp *work);

int ov112_021EAC5C(Ov112PokewalkerApp *work) {
    void *storage = SaveArray_PCStorage_Get(work->saveData);
    u16 state[2];

    BeginNormalPaletteFade((FadeMode)(0), (FadeType)(1), (FadeType)(1), (HeapID)(0), (FadeMode)(6), 1, (HeapID)(0x9A));
    if (PCStorage_CountEmptySpotsInAllBoxes(storage) < 5) {
        ov112_021E9C10(work, 3);
        ov112_021E9A78(work, 1);
        return 0x14;
    }
    if (PCStorage_CountMonsInAllBoxes(storage) == 0 && sub_0203263C(work->pokewalker) == 0) {
        ov112_021E9C10(work, 3);
        ov112_021E9A78(work, 1);
        return 0x17;
    }
    sub_02032688(work->pokewalker, &state[1], &state[0]);
    switch (state[1]) {
    case 0:
        work->unk_4 = 3;
        work->unk_1D77C = 0;
        work->unk_10 = 0;
        ov112_021E9C10(work, 3);
        ov112_021E9A78(work, 1);
        FillBgTilemapRect(work->bgConfig, 1, 0, 0, 0, 0x20, 0x18, 0x10);
        BgCommitTilemapBufferToVram(work->bgConfig, 1);
        sub_020326A4(work->pokewalker, 0, 0);
        return 4;
    case 1:
        work->unk_1D77C = 0;
        work->unk_10 = 0;
        ov112_021E9C10(work, 3);
        ov112_021E9A78(work, 7);
        FillBgTilemapRect(work->bgConfig, 1, 0, 0, 0, 0x20, 0x18, 0x10);
        BgCommitTilemapBufferToVram(work->bgConfig, 1);
        ov112_021EADD0(work, 0);
        return 0xF;
    case 2:
        ov112_021E9C10(work, 3);
        ov112_021E9A78(work, 1);
        FillBgTilemapRect(work->bgConfig, 1, 0, 0, 0, 0x20, 0x18, 0x10);
        BgCommitTilemapBufferToVram(work->bgConfig, 1);
        ov112_021EADD0(work, 1);
        return 0xA;
    }
    return 1;
}
