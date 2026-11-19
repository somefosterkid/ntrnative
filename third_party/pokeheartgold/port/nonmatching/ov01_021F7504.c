#include "follow_mon.h"
#include "map_object.h"
#include "map_object_manager.h"
#include "overlay_01_021F944C.h"
#include "overlay_01_022053EC.h"
#include "party.h"
#include "pokemon.h"
#include "script_pokemon_util.h"
#include "unk_02062108.h"
#include "field_system.h"
#include "field_types_def.h"
#include "follow_mon.h"
#include "map_object.h"
#include "overlay_01_021F944C.h"
#include "overlay_01_022053EC.h"
#include "party.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "script_pokemon_util.h"
#include "sprite.h"
#include "unk_02062108.h"
#include "wild_encounter.h"
s32 ov01_02205564(LocalMapObject *arg0);
void ov01_021F8E70(LocalMapObject *, u32, VecFx32 *);
void ov01_021F9510(LocalMapObject *, s32 *);
void ov01_021F9630(s32, s32 *);
void ov01_02205808(u32, LocalMapObject *, s32);
void ov01_0220589C(u32, LocalMapObject *, s32);
void ov01_021F7504(LocalMapObject *param_1) {
  FieldSystem *fieldSystem;
  s32 *piVar3;
  u32 uVar4;
  MapObjectManager *manager;
  Pokemon *mon;
  Party *party;
  u8 gender;
  u32 uVar5;
  u32 uVar6;
  BOOL BVar7;
  s32 sVar8;
  VecFx32 VStack_20;
  VecFx32 VStack_2c;
  u32 shiny;
  u8 form;
  
  piVar3 = (s32 *)sub_0205F40C(param_1);
  if (((((u32)*(u8 *)((u32)piVar3 + 0x17) << 0x1e) >> 0x1f) == 0) &&
     (uVar4 = MapObject_GetID(param_1), uVar4 == 0xfd)) {
    manager = MapObject_GetManager(param_1);
    fieldSystem = MapObjectManager_GetFieldSystem(manager);
    party = SaveArray_Party_Get(fieldSystem->saveData);
    mon = GetFirstAliveMonInParty_CrashIfNone(party);
    uVar4 = GetMonData(mon,5,(void *)0x0);
    uVar5 = GetMonData(mon,0x70,(void *)0x0);
    gender = GetMonGender(mon);
    uVar6 = FollowMon_GetSpriteID(uVar4,uVar5,gender);
    shiny = MonIsShiny(mon);
    form = uVar5;
    *(u8 *)((s32)piVar3 + 0x17) |= 2;
    FollowMon_SetObjectParams(param_1,uVar4,form,shiny);
    MapObject_SetSpriteID(param_1,uVar6);
    sub_0205FCD4(param_1);
    MapObject_SetFlagsBits(param_1,MAPOBJECTFLAG_START_MOVEMENT);
    MapObject_CopyPositionVector(param_1,&VStack_20);
    uVar6 = MapObject_GetXCoord(param_1);
    VStack_20.x = uVar6 * 0x10000 + 0x8000;
    MapObject_SetPreviousX(param_1,uVar6);
    uVar6 = MapObject_GetYCoord(param_1);
    MapObject_SetPreviousY(param_1,uVar6);
    uVar6 = MapObject_GetZCoord(param_1);
    VStack_20.z = uVar6 * 0x10000 + 0x8000;
    MapObject_SetPreviousZ(param_1,uVar6);
    MapObject_SetPositionVector(param_1,&VStack_20);
    MapObject_ClearHeldMovement(param_1);
    if (fieldSystem->followMon.species != uVar4 ||
        fieldSystem->followMon.form != uVar5 ||
        fieldSystem->followMon.shiny != shiny ||
        fieldSystem->followMon.gender != gender) {
      FieldSystem_SetFollowerPokeParam(fieldSystem,uVar4,uVar5,shiny,gender);
      MapObject_SetFacingDirectionDirect(param_1,1);
    }
    uVar5 = GetMonData(mon,0,(void *)0x0);
    FieldSystem_UnkSub108_Set(fieldSystem->unk108,mon,uVar4,uVar5);
  }
  BVar7 = ov01_021FA2D4(param_1);
  if (BVar7 != 1) {
    if (*piVar3 == 0) {
      ov01_021F9510(param_1,piVar3);
    }
    if (*piVar3 != 0) {
      *(u8 *)((s32)piVar3 + 0x17) = *(u8 *)((s32)piVar3 + 0x17) & ~1 | 1;
      VecFx32 afStack_38 = {0, 0, 0};
      MapObject_CopyFacingVector(param_1,&VStack_2c);
      MapObject_GetSpriteID(param_1);
      uVar4 = MapObject_GetFacingDirection(param_1);
      ov01_021F8E70(param_1,uVar4,&afStack_38);
      VStack_2c.x = afStack_38.x;
      VStack_2c.z = afStack_38.z;
      MapObject_SetFacingVector(param_1,&VStack_2c);
      sVar8 = ov01_02205564(param_1);
      if (sVar8 != 0) {
        ov01_0220589C(1,param_1,*piVar3);
      } else {
        BVar7 = ov01_0220553C(param_1);
        if (BVar7 != 0) {
          ov01_02205808(1,param_1,*piVar3);
        }
      }
      ov01_021F9630(*piVar3,piVar3 + 1);
      ov01_021FA3E8(param_1,(Sprite *)*piVar3);
      MapObject_ClearFlagsBits(param_1,MAPOBJECTFLAG_UNK21);
      sub_0205F484(param_1);
    }
  }
  return;
}
