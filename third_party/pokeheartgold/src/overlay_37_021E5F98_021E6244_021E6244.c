#include "global.h"
#include "sprite_resources_header.h"

#include "unk_02095EAC.h"
#include "constants/heap.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "options.h"
#include "pm_string.h"
#include "unk_0200A090.h"

void Sprite_SetDrawFlag(Sprite *sprite, BOOL draw);

void Sprite_SetAnimActiveFlag(Sprite *sprite, BOOL animate);

void Sprite_SetPriority(Sprite *sprite, u8 priority);

void Sprite_SetDrawPriority(Sprite *sprite, u32 priority);

void CreateSpriteResourcesHeader(SpriteResourcesHeader *resourceHeader,
    int charResourceID,
    int plttResourceID,
    int cellResourceID,
    int animResourceID,
    int mcellResourceID,
    int manimResourceID,
    BOOL vramTransfer,
    int priority,
    GF_2DGfxResMan *charResources,
    GF_2DGfxResMan *plttResources,
    GF_2DGfxResMan *cellResources,
    GF_2DGfxResMan *animResources,
    GF_2DGfxResMan *mcellResources,
    GF_2DGfxResMan *manimResources);

extern const u16 ov37_021E7A80[12][3];

void ov37_021E6244(UnkStruct_02095EAC *param0);

void ov37_021E6244(UnkStruct_02095EAC *param0) {
    int v0;

    CreateSpriteResourcesHeader(&param0->unk_1F4, 0, 0, 0, 0, 0xffffffff, 0xffffffff, 0, 0, param0->unk_1C4[0], param0->unk_1C4[1], param0->unk_1C4[2], param0->unk_1C4[3], NULL, NULL);
    CreateSpriteResourcesHeader(&param0->unk_218, 1, 1, 1, 1, 0xffffffff, 0xffffffff, 0, 0, param0->unk_1C4[0], param0->unk_1C4[1], param0->unk_1C4[2], param0->unk_1C4[3], NULL, NULL);

    {
        SpriteTemplate v1;

        v1.spriteList = param0->unk_34;
        v1.header = &param0->unk_1F4;
        v1.position.z = 0;
        v1.scale.x = FX32_ONE;
        v1.scale.y = FX32_ONE;
        v1.scale.z = FX32_ONE;
        v1.rotation = 0;
        v1.drawPriority = 1;
        v1.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
        v1.heapID = HEAP_ID_39;

        for (v0 = 0; v0 < 5; v0++) {
            v1.header = &param0->unk_218;
            v1.position.x = FX32_ONE * (24 + v0 * 40);
            v1.position.y = FX32_ONE * (32 + 32);
            v1.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;

            param0->unk_23C[v0] = Sprite_CreateAffine(&v1);

            Sprite_SetAnimActiveFlag(param0->unk_23C[v0], 1);
            Sprite_SetAnimCtrlSeq(param0->unk_23C[v0], v0);
            Sprite_SetDrawFlag(param0->unk_23C[v0], FALSE);
        }

        for (v0 = 0; v0 < 9 + 3; v0++) {
            v1.header = &param0->unk_218;
            v1.position.x = FX32_ONE * (ov37_021E7A80[v0][0]);
            v1.position.y = FX32_ONE * (ov37_021E7A80[v0][1]);

            param0->unk_2AC[v0] = Sprite_CreateAffine(&v1);

            Sprite_SetAnimActiveFlag(param0->unk_2AC[v0], 1);
            Sprite_SetAnimCtrlSeq(param0->unk_2AC[v0], ov37_021E7A80[v0][2]);

            if (v0 >= 8) {
                Sprite_SetPriority(param0->unk_2AC[v0], 2);
            }
        }

        Sprite_SetAnimCtrlSeq(param0->unk_2AC[0], 6);

        for (v0 = 0; v0 < 5; v0++) {
            v1.position.x = FX32_ONE * 24;
            v1.position.y = FX32_ONE * (32 + 32 * v0) + (256 * FX32_ONE);

            param0->unk_274[v0] = Sprite_CreateAffine(&v1);

            Sprite_SetAnimActiveFlag(param0->unk_274[v0], 1);
            Sprite_SetAnimCtrlSeq(param0->unk_274[v0], v0);
            Sprite_SetDrawPriority(param0->unk_274[v0], 1);
            Sprite_SetDrawFlag(param0->unk_274[v0], FALSE);
        }
    }
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, 1);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, 1);
}
