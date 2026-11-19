#ifndef POKEHEARTGOLD_OVERLAY_02_FOLLOW_APP_H
#define POKEHEARTGOLD_OVERLAY_02_FOLLOW_APP_H

// Ov02FollowApp: hk65 per-app struct: size from the asm (alloc / reach of the asm closure), embedded typed records at the shifts
// generic callees take them, pads elsewhere (split into fields where code accesses them).

typedef struct Ov02FollowApp {
    u8 unk_0[0x10];

    struct String *unk_10;

    u8 unk_14[0x3EC];
    u8 unk_400[0x400];
    u8 unk_800[0xC];
    s32 unk_80C;
    u8 unk_810[0x5E];
    u16 unk_86E;
    u8 unk_870[0x10];
} Ov02FollowApp;

// sizeof(Ov02FollowApp) == 0x880

#endif // POKEHEARTGOLD_OVERLAY_02_FOLLOW_APP_H
