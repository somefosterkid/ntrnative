#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"

void ov18_021EE35C(PokedexAppData *param_1, WindowTemplate *param_2, u32 param_3);

void ov18_021EE35C(PokedexAppData *param_1, WindowTemplate *param_2, u32 param_3) {
    Window *window;
    u32 uVar1;

    uVar1 = 0;
    if (param_3 != 0) {
        window = param_1->windows;
        do {
            AddWindow(param_1->bgConfig, window, param_2);
            uVar1 = uVar1 + 1;
            param_2 = param_2 + 1;
            window = window + 1;
        } while (uVar1 < param_3);
    }
    return;
}

void ov18_021EE388(PokedexAppData *pokedexApp) {
    Window *window;
    u32 uVar1;

    uVar1 = 0;
    window = pokedexApp->windows;
    do {
#if defined(SDK_PORT) || defined(NONMATCHING)
        if (window->pixelBuffer != NULL) {
            RemoveWindow(window);
        }
        uVar1 = uVar1 + 1;
        window = window + 1;
#else
        if (pokedexApp->windows[0].pixelBuffer != NULL) {
            RemoveWindow(window);
        }
        uVar1 = uVar1 + 1;
        pokedexApp = (PokedexAppData *)&pokedexApp->windows[0].bgId;
        window = window + 1;
#endif
    } while (uVar1 < 0x65);
    return;
}
