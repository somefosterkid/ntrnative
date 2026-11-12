// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/wifi_p2pmatch.h: the lines in use so
// far.
#ifndef BWPORT_APP_WIFI_P2PMATCH_H
#define BWPORT_APP_WIFI_P2PMATCH_H

#include "types.h"
#include "struct_decls.h"

// pokebw2's WifiP2PMatch_RecvPlayerInfo
// The handlers of the match board's commands (wifi_p2pmatch_net.c has the table), with the work as the match board's
void func_ov099_021c1288(int netId, int size, const void *data, void *work, NetHandle *handle);
// pokebw2's WifiP2PMatch_RecvCancel
void func_ov099_021c12d0(int netId, int size, const void *data, void *work, NetHandle *handle);
// pokebw2's func_ov099_021c12ec
// Where the data of func_ov099_021c12cc's command is received
void *func_ov099_021c12ec(int netId, void *work, int size);
// pokebw2's WifiP2PMatch_RecvNextSeq
void func_ov099_021c12fc(int netId, int size, const void *data, void *work, NetHandle *handle);
// pokebw2's WifiP2PMatch_RecvRegulation
void func_ov099_021c1334(int netId, int size, const void *data, void *work, NetHandle *handle);
// pokebw2's WifiP2PMatch_RecvConnectData
void func_ov099_021c1350(int netId, int size, const void *data, void *work, NetHandle *handle);

// A constant of wifi_p2pmatch.c that no code of the ROM reads. It lies among the file's data that code refers to,
// which MWCC does for a const global only when some code reads it before its definition, even code it never emits:
// pokebw2's accessor (WifiP2PMatch_GetUnk100), reconstructed for that
extern const u16 data_ov099_021c3718;

static inline u16 WifiP2PMatch_GetUnk100(void) {
    return data_ov099_021c3718;
}

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's func_ov099_021c12cc
void func_ov099_021c12cc(int netId, int size, const void *data, void *work, NetHandle *handle);

#endif // BWPORT_APP_WIFI_P2PMATCH_H
