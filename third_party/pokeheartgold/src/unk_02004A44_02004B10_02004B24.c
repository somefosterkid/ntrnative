#include "global.h"

#include "constants/sndseq.h"

#include "error_handling.h"
#include "heap.h"
#include "sound.h"

BOOL sub_02004B24(int a0);

BOOL sub_02004B24(int a0) {
    BOOL result;

    switch (a0) {
    case 1:
    case 9:
    case 10:
    case 17:
    case 20:
    case 23:
        result = GF_Snd_LoadGroup(GROUP_SE_FIELD);
        break;
    case 19:
        result = GF_Snd_LoadGroup(GROUP_SE_FIELD);
        GF_Snd_LoadSeqEx(SEQ_SE_PL_BALLOON02, 1);
        GF_Snd_LoadSeqEx(SEQ_SE_PL_BALLOON03_2, 1);
        GF_Snd_LoadSeqEx(SEQ_SE_PL_BALLOON05, 1);
        GF_Snd_LoadSeqEx(SEQ_SE_PL_BALLOON01, 1);
        GF_Snd_LoadSeqEx(SEQ_SE_PL_BALLOON07, 1);
        GF_Snd_LoadSeqEx(SEQ_SE_PL_ALERT4, 1);
        GF_Snd_LoadSeqEx(SEQ_SE_DP_FW104, 1);
        GF_Snd_LoadSeqEx(SEQ_SE_PL_NOMI02, 1);
        GF_Snd_LoadSeqEx(SEQ_SE_DP_023, 1);
        GF_Snd_LoadSeqEx(SEQ_SE_PL_POINT1, 1);
        GF_Snd_LoadSeqEx(SEQ_SE_PL_POINT2, 1);
        GF_Snd_LoadSeqEx(SEQ_SE_PL_POINT3, 1);
        GF_Snd_LoadSeqEx(SEQ_SE_PL_BALLOON05_2, 1);
        GF_Snd_LoadSeqEx(SEQ_SE_DP_HAMARU, 1);
        GF_Snd_LoadSeqEx(SEQ_SE_DP_CON_016, 1);
        GF_Snd_LoadSeqEx(SEQ_SE_PL_KIRAKIRA, 1);
        GF_Snd_LoadSeqEx(SEQ_SE_PL_FCALL, 1);
        break;
    case 14:
        result = GF_Snd_LoadGroup(GROUP_SE_NUTMIXER);
        break;
    case 2:
    case 13:
        result = GF_Snd_LoadGroup(GROUP_SE_BATTLE);
        break;
    case 21:
        GF_Snd_LoadBank(BANK_SE_HIROBA);
        result = GF_Snd_LoadWaveArc(WAVE_ARC_SE_HIROBA);
        break;
    case 3:
        result = GF_Snd_LoadGroup(GROUP_SE_TRADE);
        break;
    case 4:
    case 22:
        result = GF_Snd_LoadGroup(GROUP_SE_FIELD);
        break;
    case 5:
        result = GF_Snd_LoadGroup(GROUP_SE_BATTLE);
        break;
    case 11:
        result = GF_Snd_LoadGroup(GROUP_SE_FIELD);
        break;
    case 6:
        result = GF_Snd_LoadGroup(GROUP_SE_CONTEST);
        break;
    case 8:
        result = GF_Snd_LoadGroup(GROUP_SE_FIELD);
        break;
    case 12:
        result = GF_Snd_LoadGroup(GROUP_SE_NUTMIXER);
        break;
    case 16:
        GF_Snd_LoadGroup(GROUP_SE_FIELD);
        result = GF_Snd_LoadGroup(GROUP_SE_DIG);
        break;
    case 15:
        result = GF_Snd_LoadGroup(GROUP_SE_FIELD);
        break;
    case 24:
        GF_Snd_LoadBank(BANK_SE_THLON);
        result = GF_Snd_LoadWaveArc(WAVE_ARC_SE_THLON);
        break;
    case 25:
        GF_Snd_LoadBank(BANK_SE_THLON_OPED);
        result = GF_Snd_LoadWaveArc(WAVE_ARC_SE_THLON_OPED);
        break;
    case 51:
        result = GF_Snd_LoadGroup(GROUP_SE_BAG);
        break;
    case 64:
        result = GF_Snd_LoadGroup(GROUP_SE_SLOT);
        break;
    case 52:
    case 67:
        result = GF_Snd_LoadGroup(GROUP_SE_NAMEIN);
        break;
    case 7:
    case 53:
        result = GF_Snd_LoadGroup(GROUP_SE_IMAGE);
        break;
    case 54:
        result = GF_Snd_LoadGroup(GROUP_SE_ZUKAN);
        break;
    case 55:
    case 65:
        GF_Snd_LoadBank(BANK_SE_TOWNMAP);
        result = GF_Snd_LoadWaveArc(WAVE_ARC_SE_TOWNMAP);
        break;
    case 56:
        result = GF_Snd_LoadGroup(GROUP_SE_TRCARD);
        break;
    case 57:
        result = GF_Snd_LoadGroup(GROUP_SE_POKELIST);
        break;
    case 58:
        result = GF_Snd_LoadGroup(GROUP_SE_DIG);
        break;
    case 59:
        result = GF_Snd_LoadGroup(GROUP_SE_CUSTOM);
        break;
    case 60:
        result = GF_Snd_LoadGroup(GROUP_SE_BAG);
        break;
    case 61:
        result = GF_Snd_LoadGroup(GROUP_SE_NAMEIN);
        break;
    case 62:
        result = GF_Snd_LoadGroup(GROUP_SE_CUSTOM);
        break;
    case 63:
        result = GF_Snd_LoadGroup(GROUP_SE_CLIMAX);
        break;
    case 66:
        GF_Snd_LoadBank(BANK_SE_SCRATCH);
        result = GF_Snd_LoadWaveArc(WAVE_ARC_SE_SCRATCH);
        break;
    case 69:
        GF_Snd_LoadBank(BANK_SE_PLANTER);
        result = GF_Snd_LoadWaveArc(WAVE_ARC_SE_PLANTER);
        break;
    case 68:
        GF_Snd_LoadBank(BANK_SE_LINEAR);
        result = GF_Snd_LoadWaveArc(WAVE_ARC_SE_LINEAR);
        break;
    case 70:
        GF_Snd_LoadBank(BANK_SE_COIN);
        result = GF_Snd_LoadWaveArc(WAVE_ARC_SE_COIN);
        break;
    case 71:
        GF_Snd_LoadBank(BANK_SE_DENDO);
        result = GF_Snd_LoadWaveArc(WAVE_ARC_SE_DENDO);
        break;
    case 72:
        GF_Snd_LoadBank(BANK_SE_JUICE);
        result = GF_Snd_LoadWaveArc(WAVE_ARC_SE_JUICE);
        break;
    case 73:
        GF_Snd_LoadBank(BANK_SE_PHC);
        result = GF_Snd_LoadWaveArc(WAVE_ARC_SE_PHC);
        break;
    case 74:
        GF_Snd_LoadBank(BANK_SE_SEKIBAN);
        result = GF_Snd_LoadWaveArc(WAVE_ARC_SE_SEKIBAN);
        break;
    case 75:
        GF_Snd_LoadBank(BANK_SE_EVENT);
        result = GF_Snd_LoadWaveArc(WAVE_ARC_SE_EVENT);
        break;
    default:
        GF_AssertFail();
        result = 0;
        break;
    }
    return result;
}
