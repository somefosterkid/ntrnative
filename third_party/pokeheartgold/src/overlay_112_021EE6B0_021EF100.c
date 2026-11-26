#include "global.h"
struct UnkStruct_ov112_021EA230;

#include "heap.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "system.h"
#include "unk_0200B150.h"
#include "overlay_112_unk_struct_ov112_021_ea230.h"
#include "overlay_112_pokewalker_app.h"

s32 ov112_021F051C(void *);

s32 sub_02093354(void *);

s32 ov112_021EF100(Ov112PokewalkerApp *arg0) {
    s32 v0;

    arg0->link.active = 0;
    Main_SetVBlankIntrCB(NULL, NULL);
    sub_02093354(arg0->link.unk_4);
    ov112_021F051C(arg0->link.subScreen);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    Heap_Free(arg0->bgConfig);
    ov112_021E7768(arg0);
    if (*((s32 *)&arg0->link.unk_14) == 1) {
        v0 = 0;
    } else {
        v0 = 3;
    }
    arg0->unk_4 = v0;
    return 1;
}
