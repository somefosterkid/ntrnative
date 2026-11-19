#include "global.h"

#include "field/area_data.h"
#include "field/map_prop.h"
#include "field/map_prop_animation.h"
#include "field/overlay_01_02204004.h"

#include "filesystem.h"
#include "map_matrix.h"
#include "sys_task.h"
#include "overlay_01_map_load_manager.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov01_021F630C(int index, const MapLoadManager *mapLoadManager, MapPropManager **mapPropMan);
#else
void ov01_021F630C(const u8 index, const MapLoadManager *mapLoadManager, MapPropManager **mapPropMan);
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov01_021F630C(int index, const MapLoadManager *mapLoadManager, MapPropManager **mapPropMan) {
#else
void ov01_021F630C(const u8 index, const MapLoadManager *mapLoadManager, MapPropManager **mapPropMan) {
#endif
    *mapPropMan = mapLoadManager->maps[index]->propManager;
}
