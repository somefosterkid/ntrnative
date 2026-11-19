#include "global.h"
struct UnkStruct_0208C06C;

#include "overlay_40.h"
#include "touch_hitbox_controller.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

typedef struct UnkStruct_ov40_0223A83C_v0 {
    u8 unk_0[0x194];
    void *unk_194;
    u8 unk_198[0x8C];
    void *unk_224;
} UnkStruct_ov40_0223A83C_v0;

SysTask *sub_02030938(void *);

SysTask *sub_020314BC(void *);

void ov40_0223A83C(struct UnkStruct_0208C06C *arg0);

void ov40_0223A83C(UnkStruct_0208C06C *arg0) {
    UnkStruct_ov40_0223A83C_v0 *v0;

    v0 = arg0->unk_860;
    sub_02030938(v0->unk_224);
    sub_020314BC(v0->unk_194);
}

s32 ov40_0223A85C(UnkStruct_0208C06C *arg0) {
    TouchHitboxController_IsTriggered(*(TouchHitboxController **)((u8 *)(arg0->unk_860) + 0x190));
    return 0;
}
