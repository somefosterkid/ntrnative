#include "global.h"

void GF_NNS_SndPlayerGetSeqNo(NNSSndHandle *arg0);

u16 GF_GetBankBySeqNo(int seqID);
void GF_GetBankInfoBySeqNo(int seqID);

void GF_NNS_SndPlayerGetSeqNo(NNSSndHandle *handle) {
    NNS_SndPlayerGetSeqNo(handle);
}

void GF_GetBankInfoBySeqNo(int seqID) {
    NNS_SndArcGetBankInfo(GF_GetBankBySeqNo(seqID));
}
