#include "global.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"

#include "error_handling.h"
#include "sys_task.h"
#include "map_object.h"

s32 ov45_022331E8(void);

s32 ov45_02233220(PPW_LOBBY_RESULT result);

s32 ov45_0222E7CC(void);

struct MapObject;
struct ObjectEventGfxRenderDetailsEntry;
s32 ov45_02233204(s32 error);
typedef struct MapObject MapObject;
void ov45_0222E810(MapObject *mapObj, int param1);
s32 ov45_0222E7FC(s32 error);
void ov45_0222E804(MapObject *mapObj);

s32 ov45_0222E7CC(void) {
    if (_022577C0 == NULL) {
        GF_AssertFail();
    }
    if (_022577C0->result != 0) {
        return ov45_02233220(_022577C0->result);
    }
    return ov45_022331E8();
}

s32 ov45_0222E7FC(s32 error) {
    return ov45_02233204(error);
}

void ov45_0222E804(MapObject *mapObj) {
    ov45_0222E810(mapObj, 0);
}
