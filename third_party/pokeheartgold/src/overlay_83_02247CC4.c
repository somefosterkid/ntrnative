#include "global.h"

#include "sys_task.h"
#include "touchscreen_list_menu.h"

void TouchscreenListMenuSpawner_Destroy(TouchscreenListMenuSpawner *spawner);

void ov83_02247CC4(TouchscreenListMenuSpawner *spawner);

void ov83_02247CC4(TouchscreenListMenuSpawner *spawner) {
    TouchscreenListMenuSpawner_Destroy(spawner);
}
