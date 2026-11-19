#include "global.h"

#include "sys_task.h"

int GF_SndPlayerCountPlayingSeqByPlayerNo(int playerID);

int sub_02006190(int playerID);

int sub_02006190(int playerID) {
    return GF_SndPlayerCountPlayingSeqByPlayerNo(playerID);
}
