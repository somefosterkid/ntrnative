#include "global.h"
#include "unk_02014DA0.h"
#include "heap.h"
#include "overlay_59_poffin_cutscene_mon_sprite.h"
#include "unk_struct_ov59_0223c5b0.h"

ParticleSystem *ov59_0223C53C(enum HeapID);

void ov59_0223C584(void *arg0);

void ov59_0223C584(void *arg0) {
    ParticleSystem *temp_r0;

    temp_r0 = ov59_0223C53C(*(enum HeapID *)arg0);
    *((ParticleSystem **)&((PoffinCutsceneMonSprite *)arg0)->unk_24) = temp_r0;
    sub_02015528(temp_r0, 1);
    sub_0201526C(*((ParticleSystem **)&((PoffinCutsceneMonSprite *)arg0)->unk_24), sub_02015264(NARC_a_2_4_2, 0xA, *(enum HeapID *)arg0), 0xAU, 1);
}

void ov59_0223C5B0(UnkStruct_ov59_0223C5B0 *arg0);

void ov59_0223C5B0(UnkStruct_ov59_0223C5B0 *arg0) {
#if defined(SDK_PORT) || defined(UBFIX)
    // sub_02014EBC frees the ParticleSystem itself (the DS freed it a second time here); the buffer ov59_0223C53C gave
    // sub_02014DB4 was never freed
    void *particleHeap = sub_020154D0(arg0->unk_24);

    sub_02014EBC(arg0->unk_24);
    Heap_Free(particleHeap);
#else
    sub_02014EBC(arg0->unk_24);
    Heap_Free(arg0->unk_24);
#endif
}
