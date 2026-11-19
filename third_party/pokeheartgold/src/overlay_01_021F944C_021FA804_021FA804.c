#include "global.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"

struct LocalMapObject;

struct UnkStruct_ov5_021ED0A4_t;




Billboard *ov01_021F9528(struct LocalMapObject *param0, int param1);

void ov01_021FA2B8(struct LocalMapObject *param0, int param1);

BOOL MapObject_TestFlagsBits(const struct LocalMapObject *mapObj, u32 flag);

void sub_0205F484(struct LocalMapObject *mapObj);

void sub_0205F4C0(struct LocalMapObject *mapObj);


void ov01_021FA804(UnkStruct_ov5_021ED0A4 *param0, UnkStruct_ov5_021EE294 *param1);

void ov01_021FA804(UnkStruct_ov5_021ED0A4 *param0, UnkStruct_ov5_021EE294 *param1) {
    if (ov01_021F9EC4(param0, param1->unk_00) != 2) {
        *(param1->unk_08) = ov01_021F9528(param1->unk_04, param1->unk_00);
        ((*(param1->unk_08) != 0L) ? (void)0 : GF_AssertFail());

        ov01_021FA2B8(param1->unk_04, 0);

        if (MapObject_TestFlagsBits(param1->unk_04, (1 << 21)) == 1) {
            sub_0205F4C0(param1->unk_04);
        } else {
            sub_0205F484(param1->unk_04);
        }

        param1->unk_04 = 0L;
        param1->unk_08 = 0L;
    }
}
