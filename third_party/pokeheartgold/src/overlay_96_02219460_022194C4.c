#include "global.h"

#include "constants/heap.h"

#include "pokeathlon/pokeathlon.h"

#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "pokemon_icon_idx.h"
#include "sprite_system.h"
#include "overlay_96_ov96_digit_work.h"

void ov96_02219A08(Ov96DigitWork *work);

Ov96DigitWork *ov96_022194C4(SpriteSystem *spriteSystem, SpriteManager *spriteManager, struct UnkStruct_ov96_021E9A78 *resSrc, BgConfig *bgConfig, MsgData *msgData, MessageFormat *msgFormat, u8 playerIdx, enum HeapID heapID, PokeathlonCourseData *course);

Ov96DigitWork *ov96_022194C4(SpriteSystem *spriteSystem, SpriteManager *spriteManager, struct UnkStruct_ov96_021E9A78 *resSrc, BgConfig *bgConfig, MsgData *msgData, MessageFormat *msgFormat, u8 playerIdx, enum HeapID heapID, PokeathlonCourseData *course) {
    Ov96DigitWork *work;
    u8 *participant;
    u32 index;
    int i;

    GF_ASSERT(spriteSystem);
    GF_ASSERT(spriteManager);
    GF_ASSERT(resSrc);
    GF_ASSERT(bgConfig);
    GF_ASSERT(msgData);
    GF_ASSERT(msgFormat);

    work = Heap_Alloc(heapID, sizeof(Ov96DigitWork));
    MI_CpuFill8(work, 0, sizeof(Ov96DigitWork));
    work->spriteSystem = spriteSystem;
    work->spriteManager = spriteManager;
    work->resSrc = resSrc;
    work->playerIdx = playerIdx;
    work->heapId = heapID;
    work->bgConfig = bgConfig;
    work->msgData = msgData;
    work->msgFormat = msgFormat;
    work->course = course;
    work->narc = NARC_New(NARC_poketool_icongra_poke_icon, work->heapId);
    work->scrnBuf = GfGfxLoader_GetScrnData(NARC_a_2_4_0, 7, 1, &work->screenData, work->heapId);
    work->plttResId = SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_poketool_icongra_poke_icon, sub_02074490(), 0, 3, 2, 0x2714);
    index = ov96_021E5F24(course);
    participant = PokeathlonCourse_GetParticipantUnk04(course, index);
    i = 0;
    for (; i < 3; i++, participant += 40) {
        work->names[i] = String_New(11, work->heapId);
        CopyU16ArrayToString(work->names[i], (u16 *)(participant + 0x12));
    }
    ov96_02219A08(work);
    return work;
}
