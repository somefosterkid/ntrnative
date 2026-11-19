#include "global.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"
#include "field_types_def.h"

struct MapObject;
struct UnkStruct_ov5_021ED0A4_t;
typedef struct MapObject MapObject;
BOOL sub_02023DA4(Billboard *billboard);
const MapObjectManager *MapObject_GetManager(const MapObject *mapObj);
void ov01_021F9688(const MapObject *param0);
void ov01_021F9778(UnkStruct_ov5_021ED0A4 *param0, u32 param1);
int ov01_021F97BC(const MapObjectManager *param0, const MapObject *param1, int param2);
UnkStruct_ov5_021ED0A4 *ov01_021FA3DC(const MapObject *param0);

void ov01_021F95CC(const MapObject *param0, Billboard **param1, int param2);

void ov01_021F95CC(const MapObject *param0, Billboard **param1, int param2) {
    int v0;

    ov01_021F9688(param0);

    if ((*param1) != 0L) {
        sub_02023DA4(*param1);
        *param1 = 0L;
    }

    if (param2 != 0xffff) {
        v0 = ov01_021F97BC(MapObject_GetManager(param0), param0, param2);

        if (v0 == 0) {
            ov01_021F9778(ov01_021FA3DC(param0), param2);
        }
    }
}
