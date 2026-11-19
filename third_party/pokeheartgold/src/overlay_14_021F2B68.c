#include "global.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
struct IrcBattleMatch;
struct UnkStruct_ov14_boxapp;

#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_irc_battle_match.h"
struct UnkStruct_ov14_boxsys;

void ov14_021F2B68(struct IrcBattleMatch *arg0);

void ov14_021F2B68(IrcBattleMatch *arg0) {
    SpriteSystem_FreeResourcesAndManager(*((SpriteSystem * *)&arg0->unk_2F4), *((SpriteManager * *)&arg0->unk_2F8));
    SpriteSystem_Free(*((SpriteSystem * *)&arg0->unk_2F4));
}

s32 ov14_021F2C84(UnkStruct_ov14_boxsys *);

s32 ov14_021F34EC(struct UnkStruct_ov14_boxsys *);

s32 ov14_021F3714(UnkStruct_ov14_boxsys *);

s32 ov14_021F3C08(struct UnkStruct_ov14_boxsys *);

s32 ov14_021F3DE8(UnkStruct_ov14_boxsys *);

s32 ov14_021F41E4(struct UnkStruct_ov14_boxsys *arg0);

s32 ov14_021F42EC(struct UnkStruct_ov14_boxsys *);

void ov14_021F2B88(void *arg0);

void ov14_021F2B88(void *arg0) {
    ov14_021F41E4(arg0);
    ov14_021F42EC(arg0);
    ov14_021F2C84(arg0);
    ov14_021F3DE8(arg0);
    ov14_021F3714(arg0);
    ov14_021F34EC(arg0);
    ov14_021F3C08(arg0);
}

s32 ov14_021F2D1C(UnkStruct_ov14_boxsys *);

s32 ov14_021F35BC(void *);

s32 ov14_021F37F4(struct UnkStruct_ov14_boxsys *);

s32 ov14_021F3CB4(struct UnkStruct_ov14_boxsys *);

s32 ov14_021F3E70(UnkStruct_ov14_boxsys *);

s32 ov14_021F4278(u8 *);

s32 ov14_021F4380(u8 *);

void ov14_021F2BB8(struct UnkStruct_ov14_boxapp *arg0);

void ov14_021F2BB8(UnkStruct_ov14_boxapp *arg0) {
    ov14_021F4278(*((void **)&arg0->sys));
    ov14_021F4380(*((void **)&arg0->sys));
    ov14_021F2D1C(arg0->sys);
    ov14_021F3E70(*((void **)&arg0->sys));
    ov14_021F37F4(*((void **)&arg0->sys));
    ov14_021F35BC(arg0);
    ov14_021F3CB4(*((void **)&arg0->sys));
}
