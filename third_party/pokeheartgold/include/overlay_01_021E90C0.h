#ifndef POKEHEARTGOLD_OVERLAY_01_021E90C0_H
#define POKEHEARTGOLD_OVERLAY_01_021E90C0_H

#include "field_system.h"
#include "script.h"
#include "task.h"

// The task data of the map prop (door) animations ov01_021E90E4 / ov01_021E9374, hung on FieldEnv::unk18 as its
// FieldEnvSubUnk18 head (other transitions keep a bare 8 byte FieldEnvSubUnk18 there). 0x20 bytes on the DS; the camera
// translation wrapper is a pointer, so the host block is wider.
typedef struct MapPropDoorAnimEnv {
    FieldEnvSubUnk18 base; // state, x
    u32 unk_08;
    u8 unk_0C;
    u8 unk_0D;
    u16 unk_0E;
    int unk_10;
    int unk_14;
    struct GFCameraTranslationWrapper *wrapper;
    u16 unk_1C;
    u16 unk_1E;
} MapPropDoorAnimEnv;

FieldEnvSubUnk18 *ov01_021E90C0();
BOOL ov01_021E98F0(FieldSystem *fieldSystem, FieldEnvSubUnk18 *a1, u8 direction);
void ov01_021E90D4(FieldEnvSubUnk18 *a0);
void *ov01_021E9C30();
BOOL ov01_021EA128(TaskManager *taskMan);
BOOL ov01_021E9EEC(TaskManager *taskMan);
BOOL ov01_021E9F78(TaskManager *taskMan);
BOOL ov01_021E9FF8(TaskManager *taskMan);
void ov01_021E90DC(u32 x, u32 y, FieldEnvSubUnk18 *a2);
BOOL ov01_021E90E4(FieldSystem *fieldSystem, FieldEnvSubUnk18 *a1);
BOOL ov01_021E9374(FieldSystem *fieldSystem, FieldEnvSubUnk18 *a1);

#endif // POKEHEARTGOLD_OVERLAY_01_021E90C0_H
