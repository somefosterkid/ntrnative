#ifndef POKEHEARTGOLD_FIELD_OVERLAY_01_021EAF00_H
#define POKEHEARTGOLD_FIELD_OVERLAY_01_021EAF00_H

void FieldSystem_InitBottomScreen(FieldSystem *fieldSystem);
BOOL FieldSystem_IsBottomScreenRunningDummy(FieldSystem *fieldSystem);
void FieldSystem_EndBottomScreen(FieldSystem *fieldSystem);
BOOL FieldSystem_IsBottomScreenDone(FieldSystem *fieldSystem);

// Per-bottom-screen-mode handler table (ov01_022066DC in overlay 1 rodata, indexed by ov01_021EAF00()).
typedef struct BottomScreenHandlers {
    void (*initFn)(FieldSystem *);
    BOOL (*isRunningDummyFn)(FieldSystem *);
    void (*endFn)(FieldSystem *);
    BOOL (*isDoneFn)(FieldSystem *);
} BottomScreenHandlers;

void ov01_021F6840(FieldSystem *fieldSystem);
void ov01_021F6864(FieldSystem *fieldSystem);
BOOL ov01_021F6874(FieldSystem *fieldSystem);
void ov01_021F6894(FieldSystem *fieldSystem);
void ov01_021F68B8(FieldSystem *fieldSystem);
BOOL ov01_021F68C0(FieldSystem *fieldSystem);
void ov01_021EAF8C(FieldSystem *fieldSystem);
void ov01_021EAF90(FieldSystem *fieldSystem);
BOOL ov01_021EAF94(FieldSystem *fieldSystem);
void ov01_021EAF98(FieldSystem *fieldSystem);
void ov01_021EAFB4(FieldSystem *fieldSystem);
BOOL ov01_021EAFD0(FieldSystem *fieldSystem);

extern const BottomScreenHandlers ov01_022066DC[6];

#endif // POKEHEARTGOLD_FIELD_OVERLAY_01_021EAF00_H
