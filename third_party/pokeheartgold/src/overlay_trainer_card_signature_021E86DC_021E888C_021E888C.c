#include "global.h"

#include "gf_gfx_planes.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "signature_app.h"

void ov52_021E888C(SignatureApp *signatureApp);

void ov52_021E888C(SignatureApp *signatureApp) {
    SpriteTemplate sp2C;

    CreateSpriteResourcesHeader(&signatureApp->dummyResourcesHeader, 0, 0, 0, 0, -1, -1, 0, 0, signatureApp->spriteResourceCollection[0], signatureApp->spriteResourceCollection[1], signatureApp->spriteResourceCollection[2], signatureApp->spriteResourceCollection[3], NULL, NULL);
    CreateSpriteResourcesHeader(&signatureApp->spriteResourcesHeader, 1, 1, 1, 1, -1, -1, 0, 0, signatureApp->spriteResourceCollection[0], signatureApp->spriteResourceCollection[1], signatureApp->spriteResourceCollection[2], signatureApp->spriteResourceCollection[3], NULL, NULL);
    sp2C.spriteList = signatureApp->spriteList;
    sp2C.header = &signatureApp->dummyResourcesHeader;
    sp2C.position.z = 0;
    sp2C.scale.x = 0x1000;
    sp2C.scale.y = 0x1000;
    sp2C.scale.z = 0x1000;
    sp2C.rotation = 0;
    sp2C.drawPriority = 1;
    sp2C.whichScreen = (NNS_G2D_VRAM_TYPE)1;
    sp2C.heapID = (enum HeapID)0x27;
    sp2C.header = &signatureApp->spriteResourcesHeader;
    sp2C.position.x = 0xE0000;
    sp2C.position.y = 0xAF000;
    signatureApp->sprite[0] = Sprite_CreateAffine((SpriteTemplate *)&sp2C);
    Sprite_SetAnimActiveFlag(signatureApp->sprite[0], 1);
    Sprite_SetAnimCtrlSeq(signatureApp->sprite[0], 0);
    Sprite_SetPaletteOverride(signatureApp->sprite[0], 0);
    Sprite_SetPriority(signatureApp->sprite[0], 2U);
    GfGfx_EngineATogglePlanes(0x10U, 1U);
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
}
