#include "global.h"

#include "map_object.h"
#include "overlay_01_022001E4.h"

// the emote movement's data: the same record MapObjectMovementCmd075_Step1 reads (unk_02062108_02062F6C.c)
typedef struct EmoteMovementData {
    int unk_00;
    OverworldAnimManager *unk_04;
} EmoteMovementData;

void sub_02062F48(LocalMapObject *arg0, s32 arg1);

void sub_02062F48(LocalMapObject *object, s32 a0) {
    EmoteMovementData *v0;

    v0 = (EmoteMovementData *)sub_0205F3C0(object, sizeof(EmoteMovementData));
    v0->unk_00 = a0;
    v0->unk_04 = ov01_02200540(object, a0, 1);
    MapObject_IncrementMovementStep(object);
}
