// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/stadium_script.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_FIELD_STADIUM_SCRIPT_H
#define BWPORT_FIELD_STADIUM_SCRIPT_H

#include "types.h"
#include "struct_decls.h"

// Black 2's; twinned code confirms the offsets of objCode and trainerId in Black
struct StadiumTrainerEntry {
    u16 unk00;
    u16 unk02;
    u16 objCode;
    u16 trainerId;
};

BOOL s01E2_StadiumFreeTrainerTable(VM *vm, FieldScriptEnv *env);
BOOL s01E3_StadiumSetupActorSingle(VM *vm, FieldScriptEnv *env);
BOOL s01E0_StadiumSetupActorsDouble(VM *vm, FieldScriptEnv *env);
BOOL s01E0_StadiumSetupActorsTriple(VM *vm, FieldScriptEnv *env);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
u32 FindStadiumTrainerIndex(StadiumTrainerEntry *trainers, u16 a, u16 b);
BOOL s01E1_StadiumLoadTrainerTable(VM *vm, FieldScriptEnv *env);
BOOL s01E5_StadiumResetTrainerFlags(VM *vm, FieldScriptEnv *env);
// Picks six different numbers from 0x50 to 0x83 into the script variables
BOOL func_ov010_0215f52c(VM *vm, FieldScriptEnv *env);

#endif // BWPORT_FIELD_STADIUM_SCRIPT_H
