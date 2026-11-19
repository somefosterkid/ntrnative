#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "msgdata.h"
#include "pm_string.h"
#include "sprite.h"
#include "sys_task.h"
#include "unk_0200A090.h"


typedef struct ChooseStarterRotation {
    fx32 verticalOffset;
    fx32 verticalOffsetMultiplier;
    int frameCountMax;
    int frameCount;
} ChooseStarterRotation;

typedef struct ChooseStarterCursor {
    Sprite *sprite;
    SpriteResource *resources[6];
    VecFx32 position;
    SysTask *movementTask;
    ChooseStarterRotation starterRotation;
} ChooseStarterCursor;

void ov48_0225B0D4(MsgData **msgDataPtr, s32 strno, String *dest);

void ov48_0225B0D4(MsgData **msgDataPtr, s32 strno, String *dest) {
    ReadMsgDataIntoString(*msgDataPtr, strno, dest);
}
