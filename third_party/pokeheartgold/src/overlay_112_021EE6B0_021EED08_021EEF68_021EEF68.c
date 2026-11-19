#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "overlay_112_pokewalker_app.h"
#include "party.h"
#include "pokemon_storage_system.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "save_arrays.h"
#include "system.h"

void ov112_021E77E4(Ov112PokewalkerApp *app);

void ov112_021EF15C(void);

void ov112_021EF19C(void);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov112_021EF1CC(void);

void ov112_021EF17C(void);
#else
// The DS code loads r0 before these two calls (the link address, the bg config), but their definitions take no parameter.
void ov112_021EF1CC(Ov112PokewalkerLink *link);

void ov112_021EF17C(BgConfig *bgConfig);
#endif

void ov112_021EF310(void *param0);

Ov112SubScreen *ov112_021F039C(enum HeapID heapID);

void ov112_021F03BC(Ov112SubScreen *subScreen, BgConfig *bgConfig);

void *sub_020932E0(s32, u8, s8);

s32 ov112_021EF298(PCStorage *storage, u32 boxno, u32 slotno, void *arg3);

void ov112_021EF300(String *dest, PCStorage *pcStorage, u32 boxno);

void ov112_021EF3F8(int box, int slot, Ov112PokewalkerApp *work);

s32 sub_02093440(void *, BgConfig *, PCStorage *, Party *, s32, s32, s32, s32, void *, void *, void *, void *, void *);

s32 ov112_021EEF68(Ov112PokewalkerApp *app, s32 arg1);

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov112_021EEF68(Ov112PokewalkerApp *app, s32 arg1) {
    PCStorage *pcStorage;

    switch (app->link.active) {
    case 0:
        ov112_021E77E4(app);
        app->link.active = 1;
        break;

    case 1:
        app->bgConfig = BgConfig_Alloc(HEAP_ID_154);
        ov112_021EF15C();
        app->link.unk_4 = sub_020932E0(0x9A, 1, 0x12);
        app->link.subScreen = ov112_021F039C(HEAP_ID_154);
#if defined(SDK_PORT) || defined(NONMATCHING)
        ov112_021EF17C();
        ov112_021EF19C();
        ov112_021EF1CC();
#else
        ov112_021EF17C(app->bgConfig);
        ov112_021EF19C();
        ov112_021EF1CC(&app->link);
#endif
        app->link.active = 0;
        pcStorage = SaveArray_PCStorage_Get(app->saveData);
        sub_02093440(app->link.unk_4, app->bgConfig, pcStorage, SaveArray_Party_Get(app->saveData), 0, 0, 1, 0, ov112_021EF298, ov112_021EF300, &app->link.unk_C, ov112_021EF3F8, app);
        ov112_021F03BC(app->link.subScreen, app->bgConfig);
        Main_SetVBlankIntrCB(ov112_021EF310, &app->link);
        app->link.active = 0;
        return 2;
    }

    return 1;
}
#else
// DS form: the 0x1D764 link offset stays in r6 and the link fields are reached as r6 + 4 / 8 / 12.
s32 ov112_021EEF68(Ov112PokewalkerApp *arg0, s32 arg1) {
    Ov112PokewalkerApp *new_var2;
    PCStorage *temp_r4;
    Ov112SubScreen **new_var;

    switch (*((s32 *)(((u8 *)arg0) + 0x1D764))) {
    case 0:
        ov112_021E77E4(arg0);
        *((s32 *)(((u8 *)arg0) + 0x1D764)) = 1;
        break;

    case 1:
        arg0->bgConfig = BgConfig_Alloc(HEAP_ID_154);
        ov112_021EF15C();
        arg0->link.unk_4 = sub_020932E0(0x9A, 1, 0x12);
        new_var = &arg0->link.subScreen;
        *new_var = ov112_021F039C(HEAP_ID_154);
        ov112_021EF17C(arg0->bgConfig);
        new_var2 = arg0;
        ov112_021EF19C();
        ov112_021EF1CC(&arg0->link);
        new_var2->link.active = 0;
        temp_r4 = SaveArray_PCStorage_Get(new_var2->saveData);
        sub_02093440(new_var2->link.unk_4, new_var2->bgConfig, temp_r4, SaveArray_Party_Get(new_var2->saveData), 0, 0, 1, 0, ov112_021EF298, ov112_021EF300, &arg0->link.unk_C, ov112_021EF3F8, arg0);
        ov112_021F03BC(*new_var, new_var2->bgConfig);
        Main_SetVBlankIntrCB(ov112_021EF310, &arg0->link);
        *((s32 *)(((u8 *)arg0) + 0x1D764)) = 0;
        return 2;
    }

    return 1;
}
#endif
