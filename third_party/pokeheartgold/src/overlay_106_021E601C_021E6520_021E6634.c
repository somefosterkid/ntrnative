#include "global.h"

#include "bg_window.h"
#include "pm_string.h"
#include "overlay_106_arceus_cinematic.h"

void Sprite_DeleteAndFreeResources(void *taskData);

void ov106_021E6634(ArceusCinematic *arceus, u32 param1);

void ov106_021E6634(ArceusCinematic *arceus, u32 param1) {
    if (arceus->sprites[param1]) {
        Sprite_DeleteAndFreeResources(arceus->sprites[param1]);
        arceus->sprites[param1] = 0L;
    }
}
