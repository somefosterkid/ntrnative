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

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov02_0224F76C(s8, FollowMonInteractWork *);
#else
s32 ov02_0224F76C(u16, FollowMonInteractWork *);
#endif

void ov02_0224F058(struct FieldSystem *arg0, FollowMonInteractWork *arg1);

void ov02_0224F058(FieldSystem *arg0, FollowMonInteractWork *arg1) {
    Pokemon *mon;
    FieldSystemUnkSub120 *v0;

    mon = GetFirstAliveMonInParty_CrashIfNone(SaveArray_Party_Get(arg0->saveData));
    arg0->unk120->unk87E = (u16)GetMonData(mon, 5, NULL);
    arg0->unk120->unk87D = (u8)GetMonData(mon, 0x70, NULL);
    ov02_0224F324(mon, arg1);
    ov02_0224F4BC(arg0, arg1);
    ov02_0224F580(arg0, arg1);
    ov02_0224F5D0(arg0, arg1);
    ov02_0224F5FC(arg0, arg1);
    ov02_0224F644(arg0, arg1);
    ov02_0224F64C(arg0, arg1);
    ov02_0224F698(arg0, arg1);
    v0 = arg0->unk120;
    ov02_0224F6AC(arg0, v0->unk87E, v0->unk87D, arg1);
    ov02_0224F728(arg0, arg1);
    ov02_0224F76C(arg0->unk120->unk87E, arg1);
}
