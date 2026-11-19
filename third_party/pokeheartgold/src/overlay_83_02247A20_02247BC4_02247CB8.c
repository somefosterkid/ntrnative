#include "global.h"

#include "constants/heap.h"

#include "palette.h"
#include "touchscreen_list_menu.h"

struct SpriteList;

TouchscreenListMenuSpawner *ov83_02247CB8(struct SpriteList *spriteList, PaletteData *param_2);

TouchscreenListMenuSpawner *ov83_02247CB8(struct SpriteList *spriteList, PaletteData *param_2) {
    return TouchscreenListMenuSpawner_Create(HEAP_ID_107, param_2);
}
