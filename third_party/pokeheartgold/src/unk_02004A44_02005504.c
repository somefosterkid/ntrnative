#include "global.h"

#include "sys_task.h"

BOOL GF_SndWorkMicCounterFull(void);

u16 GF_GetBankBySeqNo(int seqID);
BOOL sub_02005518(void);

u16 GF_GetBankBySeqNo(int seqID) {
    const NNSSndSeqParam *param = NNS_SndArcGetSeqParam(seqID);
    if (param == 0L) {
        return 0;
    }

    return param->bankNo;
}

BOOL sub_02005518(void) {
    return GF_SndWorkMicCounterFull();
}
