#include "global.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"

#include "overlay_01_021F944C.h"
#include "billboard.h"
#include "billboard_gfx_sequence.h"

s32 ov01_021F96E4(void *);


BOOL ov01_021FA314(BillboardList *list);

void ov01_021F94A0(void *arg0);

struct UnkStruct_ov5_021ED0A4_t;
Billboard *ov01_021F9528(LocalMapObject *param0, int param1);
UnkStruct_ov5_021ED0A4 *ov01_021FA3DC(const LocalMapObject *param0);
void ov01_021FA75C(UnkStruct_ov5_021ED0A4 *param0, LocalMapObject *param1, Billboard **param2, int param3);

void ov01_021F94A0(void *arg0) {
    ov01_021FA8F8(arg0);
    ov01_021F96E4(arg0);
    ov01_021FA314(ov01_021FA1D0(arg0));
    ov01_021F9890(arg0);
}

int ov01_021F94C0(LocalMapObject *param0, Billboard **param1, int param2) {
    int v0;
    UnkStruct_ov5_021ED0A4 *v1;

    *param1 = 0L;

    v1 = ov01_021FA3DC(param0);
    v0 = ov01_021F9DA4(v1, param2);

    if (v0 == 0) {
        ov01_021F9654(param0, param1, param2);
        return v0;
    }

    if ((v0 == 3) || (v0 == 4)) {
        ov01_021FA75C(v1, param0, param1, param2);
        return v0;
    }

    *param1 = ov01_021F9528(param0, param2);
    return v0;
}
