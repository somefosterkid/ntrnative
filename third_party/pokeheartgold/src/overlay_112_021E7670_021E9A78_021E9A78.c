#include "global.h"

#include "bg_window.h"
#include "error_handling.h"
#include "overlay_112_pokewalker_app.h"

extern u8 ov112_021FEC80[];

extern WindowTemplate *ov112_021FF5D0[2];

#if defined(SDK_PORT) || defined(NONMATCHING)
extern struct WindowTemplate ov112_021FEC30;
extern struct WindowTemplate ov112_021FEC28;

WindowTemplate * ov112_021FF5D0[2] = { &ov112_021FEC30, &ov112_021FEC28 };
#endif

extern WindowTemplate *ov112_021FF5D8[6];

#if defined(SDK_PORT) || defined(NONMATCHING)
extern struct WindowTemplate ov112_021FEC68;
extern struct WindowTemplate ov112_021FEC60;
extern struct WindowTemplate ov112_021FEC58;
extern struct WindowTemplate ov112_021FEC50;
extern struct WindowTemplate ov112_021FEC48;
extern struct WindowTemplate ov112_021FEC40;

WindowTemplate * ov112_021FF5D8[6] = {
    &ov112_021FEC68, &ov112_021FEC60, &ov112_021FEC58, &ov112_021FEC50, &ov112_021FEC48,
    &ov112_021FEC40,
};
#endif

extern WindowTemplate *ov112_021FF5F0[6];

#if defined(SDK_PORT) || defined(NONMATCHING)
extern struct WindowTemplate ov112_021FEB08;
extern struct WindowTemplate ov112_021FEAF8;
extern struct WindowTemplate ov112_021FEAE8;
extern struct WindowTemplate ov112_021FEAD8;
extern struct WindowTemplate ov112_021FEAC8;
extern struct WindowTemplate ov112_021FEAB8;

WindowTemplate * ov112_021FF5F0[6] = {
    &ov112_021FEB08, &ov112_021FEAF8, &ov112_021FEAE8, &ov112_021FEAD8, &ov112_021FEAC8,
    &ov112_021FEAB8,
};
#endif

extern WindowTemplate *ov112_021FF608[7];

#if defined(SDK_PORT) || defined(NONMATCHING)
extern struct WindowTemplate ov112_021FEAB0;
extern struct WindowTemplate ov112_021FEAA8;
extern struct WindowTemplate ov112_021FEAF0;
extern struct WindowTemplate ov112_021FEAC0;
extern struct WindowTemplate ov112_021FEA98;
extern struct WindowTemplate ov112_021FEAA0;
extern struct WindowTemplate ov112_021FEC78;

WindowTemplate * ov112_021FF608[7] = {
    &ov112_021FEAB0, &ov112_021FEAA8, &ov112_021FEAF0, &ov112_021FEAC0, &ov112_021FEA98,
    &ov112_021FEAA0, &ov112_021FEC78,
};
#endif

extern WindowTemplate *ov112_021FF624[7];

#if defined(SDK_PORT) || defined(NONMATCHING)
extern struct WindowTemplate ov112_021FEB88;
extern struct WindowTemplate ov112_021FEB80;
extern struct WindowTemplate ov112_021FEC70;
extern struct WindowTemplate ov112_021FEB70;
extern struct WindowTemplate ov112_021FEB68;
extern struct WindowTemplate ov112_021FEC38;
extern struct WindowTemplate ov112_021FEB58;

WindowTemplate * ov112_021FF624[7] = {
    &ov112_021FEB88, &ov112_021FEB80, &ov112_021FEC70, &ov112_021FEB70, &ov112_021FEB68,
    &ov112_021FEC38, &ov112_021FEB58,
};
#endif

extern WindowTemplate *ov112_021FF640[8];

#if defined(SDK_PORT) || defined(NONMATCHING)
extern struct WindowTemplate ov112_021FEC20;
extern struct WindowTemplate ov112_021FEC18;
extern struct WindowTemplate ov112_021FEC10;
extern struct WindowTemplate ov112_021FEC08;
extern struct WindowTemplate ov112_021FEC00;
extern struct WindowTemplate ov112_021FEBF8;
extern struct WindowTemplate ov112_021FEBF0;
extern struct WindowTemplate ov112_021FEBE8;

WindowTemplate * ov112_021FF640[8] = {
    &ov112_021FEC20, &ov112_021FEC18, &ov112_021FEC10, &ov112_021FEC08, &ov112_021FEC00,
    &ov112_021FEBF8, &ov112_021FEBF0, &ov112_021FEBE8,
};
#endif

extern WindowTemplate *ov112_021FF660[9];

#if defined(SDK_PORT) || defined(NONMATCHING)
extern struct WindowTemplate ov112_021FEBD8;
extern struct WindowTemplate ov112_021FEBD0;
extern struct WindowTemplate ov112_021FEBC8;
extern struct WindowTemplate ov112_021FEBC0;
extern struct WindowTemplate ov112_021FEBB8;
extern struct WindowTemplate ov112_021FEBB0;
extern struct WindowTemplate ov112_021FEBA8;
extern struct WindowTemplate ov112_021FEBA0;
extern struct WindowTemplate ov112_021FEB98;

WindowTemplate * ov112_021FF660[9] = {
    &ov112_021FEBD8, &ov112_021FEBD0, &ov112_021FEBC8, &ov112_021FEBC0, &ov112_021FEBB8,
    &ov112_021FEBB0, &ov112_021FEBA8, &ov112_021FEBA0, &ov112_021FEB98,
};
#endif

extern WindowTemplate *ov112_021FF684[16];

#if defined(SDK_PORT) || defined(NONMATCHING)
extern struct WindowTemplate ov112_021FEB48;
extern struct WindowTemplate ov112_021FEB40;
extern struct WindowTemplate ov112_021FEB38;
extern struct WindowTemplate ov112_021FEBE0;
extern struct WindowTemplate ov112_021FEB28;
extern struct WindowTemplate ov112_021FEB20;
extern struct WindowTemplate ov112_021FEB18;
extern struct WindowTemplate ov112_021FEB10;
extern struct WindowTemplate ov112_021FEB90;
extern struct WindowTemplate ov112_021FEB00;
extern struct WindowTemplate ov112_021FEB78;
extern struct WindowTemplate ov112_021FEB60;
extern struct WindowTemplate ov112_021FEB50;
extern struct WindowTemplate ov112_021FEAE0;
extern struct WindowTemplate ov112_021FEB30;
extern struct WindowTemplate ov112_021FEAD0;

WindowTemplate * ov112_021FF684[16] = {
    &ov112_021FEB48, &ov112_021FEB40, &ov112_021FEB38, &ov112_021FEBE0, &ov112_021FEB28,
    &ov112_021FEB20, &ov112_021FEB18, &ov112_021FEB10, &ov112_021FEB90, &ov112_021FEB00,
    &ov112_021FEB78, &ov112_021FEB60, &ov112_021FEB50, &ov112_021FEAE0, &ov112_021FEB30,
    &ov112_021FEAD0,
};
#endif

void ov112_021E9A78(Ov112PokewalkerApp *param_1, u8 param_2) {
    u8 uVar1;
    u32 unaff_r4;
    WindowTemplate **puVar2;
    Window *pWVar3;

    if ((s32)param_2 >= 8) {
        GF_AssertFail();
    }
    uVar1 = param_2;
    switch (param_2) {
    case 0:
        puVar2 = ov112_021FF608;
        unaff_r4 = 0;
        pWVar3 = param_1->windows2;
        do {
            AddWindow(param_1->bgConfig, pWVar3, *puVar2);
            unaff_r4 = unaff_r4 + 1;
            puVar2 = puVar2 + 1;
            pWVar3 = pWVar3 + 1;
        } while (unaff_r4 < 7);
        param_1->unk_1EC4B = uVar1;
        break;
    case 1:
        puVar2 = ov112_021FF5D8;
        unaff_r4 = 0;
        pWVar3 = param_1->windows;
        do {
            AddWindow(param_1->bgConfig, pWVar3, *puVar2);
            unaff_r4 = unaff_r4 + 1;
            puVar2 = puVar2 + 1;
            pWVar3 = pWVar3 + 1;
        } while (unaff_r4 < 6);
        param_1->unk_1EC4A = uVar1;
        break;
    case 2:
        puVar2 = ov112_021FF5D0;
        unaff_r4 = 0;
        pWVar3 = param_1->windows2;
        do {
            AddWindow(param_1->bgConfig, pWVar3, *puVar2);
            unaff_r4 = unaff_r4 + 1;
            puVar2 = puVar2 + 1;
            pWVar3 = pWVar3 + 1;
        } while (unaff_r4 < 2);
        param_1->unk_1EC4B = uVar1;
        break;
    case 3:
        puVar2 = ov112_021FF640;
        unaff_r4 = 0;
        pWVar3 = param_1->windows;
        do {
            AddWindow(param_1->bgConfig, pWVar3, *puVar2);
            unaff_r4 = unaff_r4 + 1;
            puVar2 = puVar2 + 1;
            pWVar3 = pWVar3 + 1;
        } while (unaff_r4 < 8);
        param_1->unk_1EC4A = uVar1;
        break;
    case 4:
        puVar2 = ov112_021FF660;
        unaff_r4 = 0;
        pWVar3 = param_1->windows;
        do {
            AddWindow(param_1->bgConfig, pWVar3, *puVar2);
            unaff_r4 = unaff_r4 + 1;
            puVar2 = puVar2 + 1;
            pWVar3 = pWVar3 + 1;
        } while (unaff_r4 < 9);
        param_1->unk_1EC4A = uVar1;
        break;
    case 5:
        puVar2 = ov112_021FF624;
        unaff_r4 = 0;
        pWVar3 = param_1->windows;
        do {
            AddWindow(param_1->bgConfig, pWVar3, *puVar2);
            unaff_r4 = unaff_r4 + 1;
            puVar2 = puVar2 + 1;
            pWVar3 = pWVar3 + 1;
        } while (unaff_r4 < 7);
        param_1->unk_1EC4A = uVar1;
        break;
    case 6:
        puVar2 = ov112_021FF684;
        unaff_r4 = 0;
        pWVar3 = param_1->windows;
        do {
            AddWindow(param_1->bgConfig, pWVar3, *puVar2);
            unaff_r4 = unaff_r4 + 1;
            puVar2 = puVar2 + 1;
            pWVar3 = pWVar3 + 1;
        } while (unaff_r4 < 0x10);
        param_1->unk_1EC4A = uVar1;
        break;
    case 7:
        puVar2 = ov112_021FF5F0;
        unaff_r4 = 0;
        pWVar3 = param_1->windows;
        do {
            AddWindow(param_1->bgConfig, pWVar3, *puVar2);
            unaff_r4 = unaff_r4 + 1;
            puVar2 = puVar2 + 1;
            pWVar3 = pWVar3 + 1;
        } while (unaff_r4 < 6);
        param_1->unk_1EC4B = uVar1;
    }
    (&param_1->unk_1EC48)[ov112_021FEC80[param_2]] = unaff_r4;
    return;
}
