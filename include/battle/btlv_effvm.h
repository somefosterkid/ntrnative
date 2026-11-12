// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btlv_effvm.h: the lines in use so
// far.
#ifndef BWPORT_BATTLE_BTLV_EFFVM_H
#define BWPORT_BATTLE_BTLV_EFFVM_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

// pokebw2's BtlvEffvm_GetScriptKind
s32 func_ov094_021f9824(VM *vm);

// What btlv_effect.c tells BtlvEffvm_Start of the effect, 0x10 bytes; BtlvEffvmParam_Clear clears it. The scripts
// read the fields as their variables 10 to 16
struct BtlvEffvmParam {
    u8 unk0;    // 0x0  a byte of the move's parameter 27
    u8 variant; // 0x1  which of the script's variants runs, the turn of a two-turn move
    u8 unk2;    // 0x2
    u8 unk3;    // 0x3
    u32 unk4;   // 0x4
    u32 unk8;   // 0x8
    u16 itemNo; // 0xc  the ball the ball effects show
};

// pokebw2's BtlvEffvm_Delete
void func_ov094_021f94d8(VM *vm);
// pokebw2's BtlvEffvm_Main
BOOL func_ov094_021f9478(VM *vm);
// pokebw2's BtlvEffvmParam_Clear
void func_ov094_021f9844(BtlvEffvmParam *param);
// pokebw2's BtlvEffvm_Stop
void func_ov094_021f97e4(VM *vm);
// pokebw2's BtlvEffvm_Resume
void func_ov094_021f9814(VM *vm);
// pokebw2's BtlvEffvm_ReleaseVoices
void func_ov094_021f9a7c(VM *vm);
// pokebw2's BtlvEffvm_ClearLastEffect
void func_ov094_021f9aa4(VM *vm);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BtlvEffvm_Start
void func_ov094_021f94f8(VM *vm, u32 attacker, u32 defender, u16 move, BtlvEffvmParam *param);
// pokebw2's BtlvEffvm_PlaySEAt
void func_ov094_021f9858(VM *vm, u32 se, u32 player, u32 pan, int arg4, int arg5, int vol, int pitch, int wait);
// pokebw2's BtlvEffvm_SlideSE
void func_ov094_021f9948(VM *vm, u32 player, u32 type, u32 param, int start, int end, int delay, int frames, int stepWait,
                       int count);
// A script command, global in the original: starts the task that opens or closes window 0 step by step
BOOL BtlvEffect_QueueCommands(VM *vm, void *env);
// pokebw2's BtlvEffvm_Create
VM *func_ov094_021f93f4(TCBManager *tcbManager, HeapID heapId);


#endif // BWPORT_BATTLE_BTLV_EFFVM_H
