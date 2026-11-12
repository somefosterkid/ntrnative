// The PC build compiles src/ov096/tr_ai.c through this file (tools/port_sources.py). Three of the trainer AI's script
// commands read a table's offset in the expression that adds it to the script pointer:
//     const u16 *table = (const u16 *)(vm->pc + (s32)VM_Read32(vm));
// VM_Read32 moves vm->pc on by 4. C leaves open whether vm->pc is read before or after the call; Metrowerks reads it
// after (the ROM's scripts are laid out for that), clang before, so on the PC the table starts 4 bytes early. In
// AIJumpByMoveEffect (func_ov096_02189fc0) that turns the jump into a wild one: the AI's machine halts on a bad command
// without AIEnd, TrAI_RunScripts runs it again every frame until its time limit, and the trainer never picks a move.
// The first trainer battle with a damaging move to score (N's in Accumula Town) then waits forever at the command menu
// (2026-11-04, a test iPhone; the speed settings and renderers had nothing to do with it). Wild battles run no AI
// script, so they never met it. The two list commands (AIIfResultInList / NotInList) read their list the same way.
// Each is replaced here by the same command with the offset read first. The function-like macros rename only the
// game's prototypes and definitions (written `name(`), not the command table's entries (`name,`), so the table points
// at the versions below. The DS build never sees this file.
BOOL func_ov096_02188b1c(VM *vm, void *work);
BOOL func_ov096_02188b64(VM *vm, void *work);
BOOL func_ov096_02189fc0(VM *vm, void *work);
#pragma push_macro("func_ov096_02188b1c")
#define func_ov096_02188b1c(vm, work) func_ov096_02188b1c_game(vm, work)
#pragma push_macro("func_ov096_02188b64")
#define func_ov096_02188b64(vm, work) func_ov096_02188b64_game(vm, work)
#pragma push_macro("func_ov096_02189fc0")
#define func_ov096_02189fc0(vm, work) func_ov096_02189fc0_game(vm, work)
#ifdef BWPORT_RENAME_W_H
// White's compile (tools/port_rename.py): the macros above hide the names' w_ renames, so the table's bare entries name
// the w_ versions below by their symbol (a parenthesized name is not a macro call)
BOOL (func_ov096_02188b1c)(VM *vm, void *work) __asm__(BWPORT_RENAMED("func_ov096_02188b1c"));
BOOL (func_ov096_02188b64)(VM *vm, void *work) __asm__(BWPORT_RENAMED("func_ov096_02188b64"));
BOOL (func_ov096_02189fc0)(VM *vm, void *work) __asm__(BWPORT_RENAMED("func_ov096_02189fc0"));
#endif
#include "../../../../src/ov096/tr_ai.c"
#pragma pop_macro("func_ov096_02188b1c")
#pragma pop_macro("func_ov096_02188b64")
#pragma pop_macro("func_ov096_02189fc0")

// pokebw2's AIIfResultInList
BOOL func_ov096_02188b1c(VM *vm, void *work) {
    TrAIContext *wk = work;
    s32 listOffset = VM_Read32(vm);
    const u16 *list = (const u16 *)(vm->pc + listOffset);
    s32 offset = VM_Read32(vm);
    s32 i = 0;
    u32 value;

    do {
        value = list[i] | (list[i + 1] << 16);
        if (wk->result == value) {
            VM_Jump(vm, vm->pc + offset);
            break;
        }
        i += 2;
    } while (value != 0xffffffff);
    return wk->cmdReturn;
}

// pokebw2's AIIfResultNotInList
BOOL func_ov096_02188b64(VM *vm, void *work) {
    TrAIContext *wk = work;
    s32 listOffset = VM_Read32(vm);
    const u16 *list = (const u16 *)(vm->pc + listOffset);
    s32 offset = VM_Read32(vm);
    s32 i = 0;
    u32 value;

    do {
        value = list[i] | (list[i + 1] << 16);
        if (wk->result == value) {
            return wk->cmdReturn;
        }
        i += 2;
    } while (value != 0xffffffff);
    VM_Jump(vm, vm->pc + offset);
    return wk->cmdReturn;
}

// pokebw2's AIJumpByMoveEffect
BOOL func_ov096_02189fc0(VM *vm, void *work) {
    TrAIContext *wk = work;
    u32 mode = VM_Read32(vm);
    s32 max = VM_Read32(vm);
    s32 tableOffset = VM_Read32(vm);
    const u16 *table = (const u16 *)(vm->pc + tableOffset);
    s32 index;
    BOOL end = FALSE;

    if (mode == 0) {
        index = GetMoveData(wk, wk->moveId, MOVE_PARAM_EFFECT) * 2;
        if (index / 2 > max) {
            end = TRUE;
        }
    } else {
        end = TRUE;
    }
    if (end == TRUE) {
        return AIEnd(vm, wk);
    }
    VM_Jump(vm, (const u8 *)table + (table[index] | (table[index + 1] << 16)));
    return wk->cmdReturn;
}
