#ifndef POKEHEARTGOLD_OVERLAY_07_BATTLE_ANIM_FUNCS_H
#define POKEHEARTGOLD_OVERLAY_07_BATTLE_ANIM_FUNCS_H

#include "constants/heap.h"

#include "overlay_07_battle_anim_common.h"
#include "overlay_07_battle_anim_system.h"
#include "sys_task.h"

// Accessors and task makers every per-move effect of overlay 7 goes through. They take the BattleAnimSystem as a
// pointer (8 bytes on the PC); pointer results are typed, not s32.

struct ManagedSprite;
struct PaletteData;
struct ParticleSystem;
struct Pokepic;
struct SPLEmitter;
struct SpriteManager;
struct SpriteSystem;

enum HeapID ov07_0221BFD0(BattleAnimSystem *system);
SysTask *ov07_0221C3F4(BattleAnimSystem *system, SysTaskFunc func, void *param, s32 priority);
SysTask *ov07_0221C410(BattleAnimSystem *system, SysTaskFunc func, void *param);
SysTask *ov07_0221C42C(BattleAnimSystem *system, SysTaskFunc func, void *param, s32 priority);
void ov07_0221C448(BattleAnimSystem *system, SysTask *task);
u16 ov07_0221C468(BattleAnimSystem *system);
u16 ov07_0221C470(BattleAnimSystem *system);
struct ParticleSystem *ov07_0221C478(BattleAnimSystem *system);
struct ParticleSystem *ov07_0221C488(BattleAnimSystem *system, s32 index);
struct SPLEmitter *ov07_0221C494(BattleAnimSystem *system, s32 index);
struct BgConfig *ov07_0221C4A0(BattleAnimSystem *system);
s32 ov07_0221C4A8(BattleAnimSystem *system, int id);
struct ManagedSprite *ov07_0221C4E8(BattleAnimSystem *system, int index);
struct SpriteManager *ov07_0221C514(BattleAnimSystem *system);
struct SpriteManager *ov07_0221C528(BattleAnimSystem *system);
struct SpriteSystem *ov07_0221C530(BattleAnimSystem *system);
struct Pokepic *ov07_0221FA48(BattleAnimSystem *system, int battler);
struct PaletteData *ov07_0221FA78(BattleAnimSystem *system);
s32 ov07_0221FA80(BattleAnimSystem *system, int battler);
enum NarcId ov07_0221FA90(BattleAnimSystem *system, int battler);
s32 ov07_0221FAA0(BattleAnimSystem *system, int battler);
s32 ov07_0221FAB0(BattleAnimSystem *system);
s32 ov07_02222004(BattleAnimSystem *system, int battler);
s32 ov07_0222202C(BattleAnimSystem *system, int battler);
int ov07_0223197C(BattleAnimSystem *system, int battler);
void ov07_02231E74(BattleAnimSystem *system, int alphaBase, int alphaEffect);
void ov07_02231FA0(struct Pokepic *sprite, Point2D *pos);
void ov07_02231FE4(BattleAnimSystem *system, BattleAnimScriptFuncCommon *common);
void *ov07_022324D8(BattleAnimSystem *system, u32 size);

#endif // POKEHEARTGOLD_OVERLAY_07_BATTLE_ANIM_FUNCS_H
