#include "global.h"
struct FieldSystem;

#include "party.h"
#include "pokemon.h"
#include "script_pokemon_util.h"
#include "field_system.h"
#include "follow_mon_interact_work.h"

s32 ov02_0224F324(Pokemon *, FollowMonInteractWork *);

s32 ov02_0224F4BC(void *, FollowMonInteractWork *);

s32 ov02_0224F580(void *, FollowMonInteractWork *);

s32 ov02_0224F5D0(void *, FollowMonInteractWork *);

s32 ov02_0224F5FC(void *, FollowMonInteractWork *);

s32 ov02_0224F644(void *, FollowMonInteractWork *);

s32 ov02_0224F64C(void *, FollowMonInteractWork *);

s32 ov02_0224F698(void *, FollowMonInteractWork *);

s32 ov02_0224F6AC(void *, int, int, FollowMonInteractWork *);

s32 ov02_0224F728(void *, FollowMonInteractWork *);

s32 ov02_0224F76C(u16, FollowMonInteractWork *);

void ov02_0224F058(struct FieldSystem *arg0, FollowMonInteractWork *arg1);

void ov02_0224F058(FieldSystem *arg0, FollowMonInteractWork *arg1) {
    Pokemon *mon;
    void *v0;

    mon = GetFirstAliveMonInParty_CrashIfNone(SaveArray_Party_Get(arg0->saveData));
    *(u16 *)((u8 *)(*((void * *)&arg0->unk120)) + 0x87E) = (u16)GetMonData(mon, 5, NULL);
    *(u8 *)((u8 *)(*((void * *)&arg0->unk120)) + 0x87D) = (u8)GetMonData(mon, 0x70, NULL);
    ov02_0224F324(mon, arg1);
    ov02_0224F4BC(arg0, arg1);
    ov02_0224F580(arg0, arg1);
    ov02_0224F5D0(arg0, arg1);
    ov02_0224F5FC(arg0, arg1);
    ov02_0224F644(arg0, arg1);
    ov02_0224F64C(arg0, arg1);
    ov02_0224F698(arg0, arg1);
    v0 = *((void * *)&arg0->unk120);
    ov02_0224F6AC(arg0, *(u16 *)((u8 *)v0 + 0x87E), *(u8 *)((u8 *)v0 + 0x87D), arg1);
    ov02_0224F728(arg0, arg1);
    ov02_0224F76C(*(u16 *)((u8 *)(*((void * *)&arg0->unk120)) + 0x87E), arg1);
}
