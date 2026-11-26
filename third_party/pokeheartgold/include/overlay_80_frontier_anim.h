#ifndef POKEHEARTGOLD_OVERLAY_80_FRONTIER_ANIM_H
#define POKEHEARTGOLD_OVERLAY_80_FRONTIER_ANIM_H

struct UnkStruct_ov63_0222D77C_t;
struct UnkStruct_ov104_0223C634;

typedef struct FrontierAnimCmd {
    u16 action;
    u16 count;
} FrontierAnimCmd;

// Movement task data: allocated by ov80_0222C7B0, run and freed by ov80_0222EEC8.
// DS layout: sizeof == 0x18.
typedef struct FrontierAnimData {
    u8 state;
    u8 elapsed;
    s16 delay;
    u16 localID;
    u8 *totalMovementCount;
    const FrontierAnimCmd *animCmd;
    struct UnkStruct_ov63_0222D77C_t *unk_10;
    struct UnkStruct_ov104_0223C634 *frontierObj;
} FrontierAnimData;

#endif // POKEHEARTGOLD_OVERLAY_80_FRONTIER_ANIM_H
