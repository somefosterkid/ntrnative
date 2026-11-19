/* PORTOK hk39 */
#include "global.h"
#include "constants/sndseq.h"
#include "field/overlay_01_021FB878.h"
#include "field_system.h"
#include "unk_02005D10.h"

typedef struct Ov02Field3dThing {
    Field3dObject obj;               // 0x00
    Field3dModel model;              // 0x78
    Field3DModelAnimation anims[4];  // 0x88
    u32 animCount;                   // 0xD8
} Ov02Field3dThing;                  // 0xDC

typedef struct Ov02Work_0224D488 {
    Ov02Field3dThing things[2];      // 0x000
    NNSFndAllocator alloc;           // 0x1B8
    u16 loaded;                      // 0x1C8
    u16 state;                       // 0x1CA
} Ov02Work_0224D488;

// D0C8's matched C declares 6 params (param1 NARC * used as a file id, param5 unused); by the asm it takes these 5
void ov02_0224D0C8(Ov02Field3dThing *thing, u32 modelFile, u32 animFile, u32 animCount, NNSFndAllocator *alloc);
void ov02_0224D0AC(PlayerAvatar *playerAvatar, Field3dObject *obj);
BOOL ov02_0224D178(Ov02Field3dThing *thing);
void ov02_0224D1AC(Ov02Field3dThing *thing);
extern const u8 ov02_022538EC[];

void ov02_0224D488(void *unused, FieldSystem *fieldSystem, void *param2) {
    Ov02Work_0224D488 *work = param2;
    BOOL done;
    u8 files[4];
    u8 *animFiles;
    u8 n;

    switch (work->state) {
    case 0:
        files[2] = ov02_022538EC[2];
        files[3] = ov02_022538EC[3];
        files[0] = ov02_022538EC[4];
        files[1] = ov02_022538EC[5];
        animFiles = &files[2];
        n = work->loaded;
        ov02_0224D0C8(&work->things[n], animFiles[n], files[n], 2, &work->alloc);
        ov02_0224D0AC(fieldSystem->playerAvatar, &work->things[n].obj);
        work->loaded++;
        if (work->loaded >= 2) {
            work->state++;
        }
        break;
    case 1:
        Field3dObject_SetActiveFlag(&work->things[1].obj, FALSE);
        PlaySE(SEQ_SE_DP_FW463);
        work->state++;
        // fallthrough
    case 2:
        done = ov02_0224D178(&work->things[0]);
        ov02_0224D0AC(fieldSystem->playerAvatar, &work->things[0].obj);
        if (done == TRUE) {
            Field3dObject_SetActiveFlag(&work->things[1].obj, TRUE);
            Field3dObject_SetActiveFlag(&work->things[0].obj, FALSE);
            ov02_0224D0AC(fieldSystem->playerAvatar, &work->things[1].obj);
            work->state++;
        }
        break;
    case 3:
        ov02_0224D1AC(&work->things[1]);
        ov02_0224D0AC(fieldSystem->playerAvatar, &work->things[1].obj);
        break;
    }
}
