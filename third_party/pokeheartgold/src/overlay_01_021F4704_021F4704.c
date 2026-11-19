#include "global.h"

#include "field/map_prop_animation.h"

#include "filesystem.h"
#include "map_matrix.h"
#include "sys_task.h"
#include "overlay_01_map_load_manager.h"

struct AreaDataManager;
struct MapPropManager;
void ov01_021FB354(SysTask *sysTask);
void ov01_021F6614(SysTask *sysTask);

BOOL ov01_021F4704(const int lhsX, const int rhsX, const int mapMatrixWidth);
BOOL ov01_021F4728(const int lhsY, const int rhsY, const int mapMatrixWidth);
void ov01_021F474C(MapLoadManager *mapLoadManager, const u8 index);
void ov01_021F477C(MapLoadSlot *loader);

BOOL ov01_021F4704(const int lhsX, const int rhsX, const int mapMatrixWidth) {
    return lhsX % mapMatrixWidth == rhsX % mapMatrixWidth;
}

BOOL ov01_021F4728(const int lhsY, const int rhsY, const int mapMatrixWidth) {
    return lhsY / mapMatrixWidth == rhsY / mapMatrixWidth;
}

void ov01_021F474C(MapLoadManager *mapLoadManager, const u8 index) {
    if (mapLoadManager->slots[index].tasks.modelLoading != 0) {
        ov01_021F6614(mapLoadManager->slots[index].modelTask);
    }

    if (mapLoadManager->slots[index].tasks.bdhcLoading != 0) {
        ov01_021FB354(mapLoadManager->slots[index].bdhcTask);
    }

    mapLoadManager->slots[index].finished = 1;
}

void ov01_021F477C(MapLoadSlot *loader) {
    if (loader->tasks.modelLoading != 0) {
        ov01_021F6614(loader->modelTask);
    }

    if (loader->tasks.bdhcLoading != 0) {
        ov01_021FB354(loader->bdhcTask);
    }

    loader->tasks.modelLoading = 0;
    loader->tasks.bdhcLoading = 0;
}
