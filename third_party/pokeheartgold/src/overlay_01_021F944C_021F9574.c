#include "global.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"
#include "field_types_def.h"
#include "overlay_01_021F944C.h"

struct UnkStruct_ov5_021ED0A4_t;
BOOL sub_02023DA4(Billboard *billboard);
const MapObjectManager *MapObject_GetManager(const LocalMapObject *mapObj);
void ov01_021F9778(UnkStruct_ov5_021ED0A4 *param0, u32 param1);
int ov01_021F97BC(const MapObjectManager *param0, const LocalMapObject *param1, int param2);
UnkStruct_ov5_021ED0A4 *ov01_021FA3DC(const LocalMapObject *param0);

void ov01_021F9574(LocalMapObject *param0, Billboard **param1, int param2) {
    int v0;

    sub_02023DA4(*param1);
    *param1 = 0L;

    v0 = ov01_021F97BC(MapObject_GetManager(param0), param0, param2);

    if (v0 == 0) {
        ov01_021F9778(ov01_021FA3DC(param0), param2);
    }
}
