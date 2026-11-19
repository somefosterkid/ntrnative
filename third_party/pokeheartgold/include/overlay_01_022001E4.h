#ifndef POKEHEARTGOLD_OVERLAY_01_022001E4_H
#define POKEHEARTGOLD_OVERLAY_01_022001E4_H

#include "billboard.h"
#include "overlay_01_021F1348.h"
#include "player_avatar.h"
#include "sys_task.h"

void *ov01_022001E4(FieldEffectManager *fieldEffMan);
void ov01_022001F8(void *ctx);

// The context of the field effect renderer 9 (sizeof == 0x14 at 32 bits): a user count and a
// "resources loaded" flag for each of the two emote kinds.
typedef struct UnkOv01_022001E4 {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    FieldEffectManager *fieldEffMan;
} UnkOv01_022001E4;

// userData of the NPC emote anim manager made by ov01_02200540 (0x10 bytes at 32 bits).
typedef struct NpcEmoteAnimArgs {
    int emoteKind;
    FieldEffectManager *fieldEffMan;
    UnkOv01_022001E4 *rendererCtx;
    LocalMapObject *mapObject;
} NpcEmoteAnimArgs;

// ctx of that anim manager (0x44 bytes at 32 bits): a copy of the args at +0x30.
typedef struct NpcEmoteAnimCtx {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    VecFx32 unk_18;
    VecFx32 unk_24;
    NpcEmoteAnimArgs args;
    Billboard *billboard;
} NpcEmoteAnimCtx;

void ov01_02200208(UnkOv01_022001E4 *ctx);
void ov01_02200210(UnkOv01_022001E4 *ctx);
void ov01_02200220(UnkOv01_022001E4 *ctx);
void ov01_02200228(UnkOv01_022001E4 *ctx);
void ov01_02200238(UnkOv01_022001E4 *ctx, int emoteKind);
void ov01_0220024C(UnkOv01_022001E4 *ctx, int emoteKind);
void ov01_02200260(UnkOv01_022001E4 *ctx);
void ov01_022002AC(UnkOv01_022001E4 *ctx);
void ov01_022002FC(UnkOv01_022001E4 *ctx);
void ov01_0220032C(UnkOv01_022001E4 *ctx);
void ov01_0220038C(UnkOv01_022001E4 *ctx);
void ov01_0220039C(UnkOv01_022001E4 *ctx, int emoteKind);
void ov01_022003B0(UnkOv01_022001E4 *ctx, int emoteKind);
void ov01_022004EC(OverworldAnimManager *animMan, NpcEmoteAnimCtx *ctx);

// The DS forms hand back whatever the callee left in r0 (nothing reads it).
#if defined(SDK_PORT) || defined(NONMATCHING)
void ov01_0220035C(UnkOv01_022001E4 *ctx);
void ov01_0220036C(UnkOv01_022001E4 *ctx);
void ov01_0220037C(UnkOv01_022001E4 *ctx);
#else
u32 ov01_0220035C(UnkOv01_022001E4 *ctx);
int ov01_0220036C(UnkOv01_022001E4 *ctx);
unsigned char ov01_0220037C(UnkOv01_022001E4 *ctx);
#endif

OverworldAnimManager *ov01_02200540(LocalMapObject *obj, int a0, int a1);
BOOL ov01_022003F4(OverworldAnimManager *animMan);

#endif // POKEHEARTGOLD_OVERLAY_01_022001E4_H
