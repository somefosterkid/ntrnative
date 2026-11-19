#include "global.h"

#include "constants/heap.h"

#include "field/area_data.h"
#include "field/area_light.h"
#include "field/draw_map_name.h"
#include "field/dynamic_terrain_height.h"
#include "field/field_3d_object_task.h"
#include "field/fog.h"
#include "field/hblank_system.h"
#include "field/map_prop.h"
#include "field/map_prop_animation.h"
#include "field/model_attributes.h"
#include "field/overlay_01_021EAFD4.h"
#include "field/overlay_01_02204004.h"
#include "field/signpost.h"

#include "bag_cursor.h"
#include "bg_window.h"
#include "bug_contest_internal.h"
#include "camera.h"
#include "dex_mon_measures.h"
#include "field_system.h"
#include "field_types_def.h"
#include "filesystem.h"
#include "frontier_data.h"
#include "gear_phone.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "map_events_internal.h"
#include "map_matrix.h"
#include "map_object.h"
#include "menu_input_state.h"
#include "msgdata.h"
#include "overlay_01_021EFB38.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_unk_struct_ov5_021de47c.h"
#include "overlay_01_unk_struct_ov5_021de5a4.h"
#include "overlay_109.h"
#include "overlay_manager.h"
#include "photo_album.h"
#include "photo_types_def.h"
#include "player_avatar.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "save_link_ruleset.h"
#include "save_pokegear.h"
#include "scrcmd_9.h"
#include "screen_fade.h"
#include "script.h"
#include "seal_case.h"
#include "sprite.h"
#include "sys_task.h"
#include "task.h"
#include "terrain_attributes.h"
#include "unk_02009D48.h"
#include "unk_0202F370.h"
#include "unk_02055418.h"
#include "unk_02092BB8.h"
#include "wild_encounter.h"
#include "overlay_01_encounter_effect.h"
#include "overlay_01_unk_struct_ov83_0223_d9_ec.h"
#include "overlay_01_unk_struct_ov83_0223_da94.h"
#include "linear_interpolation_task.h"

typedef enum FadeType FadeType;

typedef enum FadeMode FadeMode;

struct AreaDataManager;

struct AreaLightManager;

struct AsyncWriteManager;

struct BUGMON;

struct Background;

struct BagCursor;

struct BagCursorBattle;

struct BagCursorField;

struct BgConfig;

struct BgEvent;

struct BicycleSlopeAnimation;

struct BoxPokemon;

struct BugContest;

struct BugContestant;

struct BugContestantData;

struct CAPSULE;

struct Camera;

struct CameraAngle;

struct CameraHistory;

struct CameraLookAt;

struct CameraPerspective;

struct CoordEvent;

struct DynamicTerrainHeightManager;

struct EncounterData;

struct EncounterDataSlot;

struct Field3dObjectTask;

struct Field3dObjectTaskManager;

struct Field3dObjectTaskTemplate;

struct Field3dRenderObj;

struct Field3dRenderObjManager;

struct FieldDrawMapNameInfo;

struct FieldEffectManager;

struct FieldProcessManager;

struct FieldSystem;

struct FieldSystemUnkSub104;

struct FieldSystemUnkSub104_Sub8;

struct FieldSystemUnkSub108;

struct FieldSystemUnkSub120;

struct FieldSystemUnkSub120_Sub7E4;

struct FieldSystemUnkSub120_Sub818;

struct FieldSystemUnkSub4;

struct FieldSystemUnkSubC8;

struct FieldSystemUnkSubCC_Sub0;

struct FieldTextureManager;

struct FieldViewPhoto;

struct FogData;

struct FollowMon;

struct FrontierData;

struct FrontierFieldSystem;

struct FrontierMon;

struct FrontierMultiBattleAllyData;

struct FrontierTrainer;

struct GearPhoneRingManager;

struct HBlankSystem;

struct LandEncounterData;

struct LinkBattleRuleset;

struct LocalMapObject;

struct Location;

struct MAPDATA;

struct Mail;

struct MailMessage;

struct MapEvents;

struct MapLoadManager;

struct MapLoadMode;

struct MapMarkingsSave;

struct MapMatrix;

struct MapMatrixData;

struct MapObjectManager;

struct MapObjectsToPreload;

struct MapPropAnimation;

struct MapPropAnimationData;

struct MapPropAnimationManager;

struct MapPropManager;

struct MapPropOneShotAnimation;

struct MapPropOneShotAnimationList;

struct MapPropOneShotAnimationManager;

struct MenuInputStateMgr;

struct ModelAttributes;

struct MsgData;

struct NARC;

struct ObjectEvent;

struct Ov114_Counter;

struct Ov114_Single;

struct Ov114_Tween;

struct Ov114_Work;

struct OverlayManager;

struct OverlayManagerTemplate;

struct Party;

struct PartyCore;

struct PartyExtra;

struct PartyExtraSub;

struct PartyPokemon;

struct PhoneBookEntry;

struct PhoneCallPersistentState;

struct PhoneRematch;

struct Photo;

struct PhotoAlbum;

struct PhotoAlbumArgs;

struct PhotoCameraParam;

struct PhotoMon;

struct PlayerAvatar;

struct PlayerSaveData;

struct PokedexData;

struct PokegearRingingTask;

struct SEAL;

struct SaveArrayHeader;

struct SaveData;

struct SavePokegear;

struct SaveSlotSpec;

struct Signpost;

struct Sprite;

struct SpriteList;

struct String;

struct SysTask;

struct SysTaskQueue;

struct TaskManager;

struct TaskManagerUnkSub1C;

struct TerrainAttributes;

struct TextureSlot;

struct UnkPokegearSub8List;

struct UnkStruct_020556FC;

struct UnkStruct_02059E1C;

struct UnkStruct_0205AC88;

struct UnkStruct_02092BB8;

struct WarpEvent;

struct Window;

struct _MAT;

struct _MAT_ENTRY;

typedef struct CAPSULE {
    SEAL seals[8];
} CAPSULE;

typedef struct Ov114_Counter {
    int unk_00;
    u8 filler_04[0x10];
} Ov114_Counter;

typedef struct Ov114_Tween {
    int unk_00;
    u8 filler_04[0x14];
} Ov114_Tween;

typedef UnkStruct_ov5_021DE47C Ov114_Renderer; // the field renderer that ov01_021F05C4 etc. take

typedef struct Ov114_Single {
    Ov114_Counter unk_00;
    Ov114_Tween unk_14;
    Ov114_Renderer unk_2C;
    struct UnkStruct_ov5_021DE5A4 unk_168;
    Sprite *unk_19C;
    u8 filler_1A0[4];
} Ov114_Single;

typedef struct Ov114_Work {
    int unk_00;
    int unk_04;
    u8 filler_08[4];
    void *unk_0C;
    FieldSystem *unk_10;
    int *unk_14;
    u8 filler_18[8];
    void *unk_20;
} Ov114_Work;

void ov01_021F05C4(UnkStruct_ov5_021DE47C *param0, s32 param1, s32 param2);

Sprite *ov01_021F0718(UnkStruct_ov5_021DE47C *param0, UnkStruct_ov5_021DE5A4 *param1, fx32 param2, fx32 param3, fx32 param4, int param5);

void ov01_021EFCF8(s32 arg0, s32 arg1, s32 arg2, s32 *arg3, s32 arg4);

void ov01_021EFE34(UnkStruct_ov83_0223D9EC *param0, fx32 param1, fx32 param2, int param3);

BOOL ov01_021EFE44(LinearInterpolationTaskS32 *task);

void ov01_021EFEC8(UnkStruct_ov83_0223DA94 *param0, fx32 param1, fx32 param2, fx32 param3, int param4);

BOOL ov01_021EFF28(UnkStruct_ov83_0223DA94 *param0);

void ov01_021F06EC(UnkStruct_ov5_021DE47C *param0, UnkStruct_ov5_021DE5A4 *param1);

void ov01_021F05F4(UnkStruct_ov5_021DE47C *param0);

void ov01_021EFCDC(EncounterEffect *encEffect, SysTask *effectTask);

void ov114_0225F020(void *a0, Ov114_Work *work);

void ov114_0225F020(void *a0, Ov114_Work *work) {
    Ov114_Single *single = work->unk_0C;
    BOOL tweenDone;
    VecFx32 scale5;
    VecFx32 scale6;
    VecFx32 tmp5;
    VecFx32 tmp6;

    switch (work->unk_00) {
    case 0:
        work->unk_0C = Heap_Alloc(HEAP_ID_FIELD1, sizeof(Ov114_Single));
        memset(work->unk_0C, 0, sizeof(Ov114_Single));
        single = work->unk_0C;
        ov01_021F05C4(&single->unk_2C, 1, 1);
        ov01_021F0614(work->unk_20, &single->unk_2C, &single->unk_168, 0, 1, 7, 9, 8, 600000);
        single->unk_19C = ov01_021F0718(&single->unk_2C, &single->unk_168, 2 << 18, 6 << 16, 0, 0);
        Sprite_SetDrawFlag(single->unk_19C, 0);
        Sprite_SetOamMode(single->unk_19C, (GXOamMode)(1));
        GfGfx_EngineATogglePlanes(0x10, 1);
        work->unk_00++;
        break;
    case 1:
        ov01_021EFCF8(1, 0x10, -0x10, &work->unk_04, 2);
        work->unk_00++;
        break;
    case 2:
        if (work->unk_04 == 0) {
            break;
        }
        work->unk_00++;
        break;
    case 3:
        ov01_021EFE34((struct UnkStruct_ov83_0223D9EC *)(&single->unk_00), 0, 0x10, 0xC);
        G2_SetBlendAlpha(0, 0xF, single->unk_00.unk_00, 0x10 - single->unk_00.unk_00);
        Sprite_SetDrawFlag(single->unk_19C, 1);
        work->unk_00++;
        break;
    case 4:
        tweenDone = ov01_021EFE44((struct LinearInterpolationTaskS32 *)(&single->unk_00));
        reg_G2_BLDALPHA = ((0x10 - single->unk_00.unk_00) << 8) | single->unk_00.unk_00;
        if (tweenDone == 1) {
            reg_G2_BLDCNT = 0;
            Sprite_SetOamMode(single->unk_19C, (GXOamMode)(0));
            work->unk_00++;
        }
        break;
    case 5: {
        ov01_021EFEC8((struct UnkStruct_ov83_0223DA94 *)(&single->unk_14), 1 << 12, 0x19A, 1, 6);
        Sprite_SetAffineOverwriteMode(single->unk_19C, 2);
        tmp5 = ov01_021F074C(single->unk_14.unk_00, single->unk_14.unk_00, single->unk_14.unk_00);
        scale5 = tmp5;
        Sprite_SetAffineScale(single->unk_19C, &scale5);
        HBlankSystem_Stop(work->unk_10->unk4->hBlankSystem);
        BeginNormalPaletteFade((FadeMode)(3), (FadeType)(0x10), (FadeType)(0), 0, 6, 1, HEAP_ID_FIELD1);
        work->unk_00++;
        break;
    }
    case 6: {
        tweenDone = ov01_021EFF28((struct UnkStruct_ov83_0223DA94 *)(&single->unk_14));
        tmp6 = ov01_021F074C(single->unk_14.unk_00, single->unk_14.unk_00, single->unk_14.unk_00);
        scale6 = tmp6;
        Sprite_SetAffineScale(single->unk_19C, &scale6);
        if (tweenDone == 1 && IsPaletteFadeFinished() == 1) {
            work->unk_00++;
        }
        break;
    }
    case 7:
        sub_0200FBF4((PMLCDTarget)(1), 0);
        HBlankSystem_Start(work->unk_10->unk4->hBlankSystem);
        if (work->unk_14 != 0L) {
            *work->unk_14 = 1;
        }
        Sprite_Delete(single->unk_19C);
        ov01_021F06EC(&single->unk_2C, &single->unk_168);
        ov01_021F05F4(&single->unk_2C);
        ov01_021EFCDC((struct EncounterEffect *)(work), a0);
        break;
    default:
        break;
    }
    if (work->unk_00 != 7) {
        SpriteList_RenderAndAnimateSprites(single->unk_2C.unk_00);
    }
}
