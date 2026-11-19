#include "global.h"
struct EvolutionData;

#include "error_handling.h"
#include "sprite_system.h"
#include "evolution_data.h"

void sub_02075770(struct EvolutionData *arg0);

void sub_02075770(EvolutionData *arg0) {
    SpriteManager *spriteManager;
    SpriteSystem *spriteSystem;

    spriteSystem = arg0->unk_AC;
    spriteManager = arg0->unk_B0;
    if (arg0->unk_B4 == NULL) {
        GF_AssertFail();
    }
    Sprite_DeleteAndFreeResources(arg0->unk_B4);
    SpriteSystem_FreeResourcesAndManager(spriteSystem, spriteManager);
    SpriteSystem_Free(spriteSystem);
    arg0->unk_B8 = FALSE;
}
