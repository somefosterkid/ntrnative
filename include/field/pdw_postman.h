// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/pdw_postman.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_FIELD_PDW_POSTMAN_H
#define BWPORT_FIELD_PDW_POSTMAN_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

// What the postman does for each kind of mystery gift, in order of MysteryGift.kind (Black's are in
// ov021_021b69b0.c, with the mystery-gift script command)
typedef struct {
    u32 kind;
    BOOL (*canReceive)(FieldScriptEnv *env, GameData *gameData, MysteryGift *gift);
    void (*receive)(FieldScriptEnv *env, GameData *gameData, MysteryGift *gift);
    u32 (*unk0C)(FieldScriptEnv *env, GameData *gameData, MysteryGift *gift);
    // These set the words of a message and return the message
    u32 (*unk10)(WordSet *wordSet, MysteryGift *gift, FieldScriptEnv *env);
    u32 (*unk14)(WordSet *wordSet, MysteryGift *gift, FieldScriptEnv *env);
} PdwPostmanGiftHandler;

FieldActor *FindMysteryGiftDeliveryManActor(Field *field);
s32 FindMysteryGiftDeliveryManNPCID(GameData *gameData);

u32 func_ov021_021b76f8(GameData *gameData, HeapID heapId, DreamWorldSave *dreamWorld);
GameEvent *func_ov021_021b7794(GameSystem *gsys);
void func_ov021_021b76b4(GameData *gameData, HeapID heapId, DreamWorldSave *dreamWorld);
u16 func_ov021_021b7740(GameData *gameData, HeapID heapId, DreamWorldSave *dreamWorld, u32 position);
MysteryGift *func_ov021_021b7834(MysteryGiftSave *param_1, s32 *param_2, MysteryGift *param_3);
void func_ov021_021b785c(MysteryGiftSave *save, u32 slot);
u8 func_ov021_021b7864(MysteryGiftSave *param_1);
u32 func_ov021_021b6d0c(u32 kind, FieldScriptEnv *env, GameData *gameData, MysteryGift *gift);
u32 func_ov021_021b6d30(u32 kind, MysteryGift *gift, FieldScriptEnv *env);
u32 func_ov021_021b6d64(u32 kind, MysteryGift *gift, FieldScriptEnv *env);
u32 func_ov021_021b6d98(u32 kind, FieldScriptEnv *env, GameData *gameData, MysteryGift *gift);
u32 func_ov021_021b6dc0(u32 kind, FieldScriptEnv *env, GameData *gameData, MysteryGift *gift);
u32 func_ov021_021b6eb0(MysteryGift *param_1, s32 param_2);
u32 func_ov021_021b6de4(Field *param_1, GameData *param_2);
s32 func_ov021_021b6e0c(Field *param_1);

#endif // BWPORT_FIELD_PDW_POSTMAN_H
