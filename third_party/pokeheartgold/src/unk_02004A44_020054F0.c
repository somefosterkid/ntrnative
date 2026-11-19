#include "global.h"

void GF_NNS_SndPlayerGetSeqNo(NNSSndHandle *arg0);

u16 GF_GetBankBySeqNo();
void GF_GetBankInfoBySeqNo(void);

void GF_NNS_SndPlayerGetSeqNo(NNSSndHandle *handle) {
    NNS_SndPlayerGetSeqNo(handle);
}

void GF_GetBankInfoBySeqNo(void) {
    NNS_SndArcGetBankInfo(GF_GetBankBySeqNo());
}
