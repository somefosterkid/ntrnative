// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/scrcmd_net.h.
#ifndef BWPORT_FIELD_SCRCMD_NET_H
#define BWPORT_FIELD_SCRCMD_NET_H

// Overlay 21's scrcmd_net.c: the script commands of the network. Names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0), whose numbers are the commands' in Black 2; Black's entry in
// its command table, FIELD_SCRIPT_SUB_EVENT_FINISH_FUNCS, follows each

#include "types.h"
#include "struct_decls.h"

BOOL s013A_GameCommGetStatus(VM *vm, FieldScriptEnv *env); // 0x149
// pokebw2's func_ov036_021adc94 and func_ov036_021adcb4
BOOL func_ov021_021b4c14(VM *vm, FieldScriptEnv *env); // 0x16E
BOOL func_ov021_021b4c34(VM *vm, FieldScriptEnv *env); // 0x16F
BOOL s015D_NetConnectWiFiClub(VM *vm, FieldScriptEnv *env); // 0x170
BOOL s015E_NetConnectGTS(VM *vm, FieldScriptEnv *env); // 0x171
BOOL s0162_NetConnectGTSNegotiation(VM *vm, FieldScriptEnv *env); // 0x175

#endif // BWPORT_FIELD_SCRCMD_NET_H
