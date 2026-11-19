#include "global.h"

#include "sprite.h"
#include "sprite_transfer.h"
#include "unk_02005D10.h"
#include "unk_0200A090.h"
#include "overlay_48_unk_struct_ov69_0225_d35_c.h"
#include "overlay_48_app_0225ac34.h"

void ov48_0225AC34(void *arg0, void *arg1);

void ov48_0225AC34(void *arg0, void *arg1) {
    s32 v2;
    void *v1;
    void *v0;
    s32 v3;
    s32 v4;
    struct SpriteResourceCollection **v5;
    struct SpriteResource **v6;
    void *v7;

    v0 = arg0;
    v1 = arg1;
    StopSE(0x5D8U, 0);
    v7 = v0;
    v2 = 0;
    do {
        Sprite_Delete(*(Sprite **)((u8 *)v0 + 0x3C));
        SpriteTransfer_DeleteCharTransferTask(*(SpriteResource **)((u8 *)v7 + 0x6C));
        if (v2 == 0) {
            SpriteTransfer_DeletePlttTransferTask(*(SpriteResource **)((u8 *)v7 + 0x70));
            DestroySingle2DGfxResObj(((GF_2DGfxResMan *)((UnkStruct_ov69_0225D35C *)v1)->unk_194[1]), *(SpriteResource **)((u8 *)v7 + 0x70));
        }
        v5 = (struct SpriteResourceCollection **)v1;
        v4 = 0;
        v6 = (struct SpriteResource **)v7;
loop_4:
        if (v4 != 1) {
            DestroySingle2DGfxResObj(*(GF_2DGfxResMan **)((u8 *)v5 + offsetof(UnkStruct_ov69_0225D35C, unk_194)), *(SpriteResource **)((u8 *)v6 + offsetof(Ov48App_0225AC34, unk_6C)));
        }
        v4 += 1;
        v6 += 1;
        v5 += 1;
        if (v4 < 4) {
            goto loop_4;
        }
        v7 += 0x10;
        v0 += 4;
        v3 = v2 + 1;
        v2 = v3;
    } while (v3 < 0xC);
}
