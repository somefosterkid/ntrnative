#include "global.h"

#include "overlay_115.h"
#include "player_data.h"
#include "field_system.h"
#include "overlay_01_encounter_effect.h"
#include "sys_task.h"

s32 ov01_021EFCDC(EncounterEffect *, SysTask *);

extern GymLeaderEncounterParam ov115_022603B0;

void ov115_0225F704(SysTask *arg0, EncounterEffect *arg1) {
    if (ov115_0225F220(arg1, HEAP_ID_FIELD1, &ov115_022603B0) == 1) {
        ov01_021EFCDC(arg1, arg0);
    }
}

extern GymLeaderEncounterParam ov115_022603C4;

void ov115_0225F728(SysTask *arg0, EncounterEffect *arg1) {
    if (ov115_0225F220(arg1, HEAP_ID_FIELD1, &ov115_022603C4) == 1) {
        ov01_021EFCDC(arg1, arg0);
    }
}

extern GymLeaderEncounterParam ov115_022603D8;

void ov115_0225F74C(SysTask *arg0, EncounterEffect *arg1) {
    if (ov115_0225F220(arg1, HEAP_ID_FIELD1, &ov115_022603D8) == 1) {
        ov01_021EFCDC(arg1, arg0);
    }
}

extern GymLeaderEncounterParam ov115_022603EC;

void ov115_0225F770(SysTask *arg0, EncounterEffect *arg1) {
    if (ov115_0225F220(arg1, HEAP_ID_FIELD1, &ov115_022603EC) == 1) {
        ov01_021EFCDC(arg1, arg0);
    }
}

extern GymLeaderEncounterParam ov115_02260400;

void ov115_0225F794(SysTask *arg0, EncounterEffect *arg1) {
    if (ov115_0225F220(arg1, HEAP_ID_FIELD1, &ov115_02260400) == 1) {
        ov01_021EFCDC(arg1, arg0);
    }
}

extern GymLeaderEncounterParam ov115_02260414;

void ov115_0225F7B8(SysTask *arg0, EncounterEffect *arg1) {
    if (ov115_0225F220(arg1, HEAP_ID_FIELD1, &ov115_02260414) == 1) {
        ov01_021EFCDC(arg1, arg0);
    }
}

extern GymLeaderEncounterParam ov115_02260428;

void ov115_0225F7DC(SysTask *arg0, EncounterEffect *arg1) {
    if (ov115_0225F220(arg1, HEAP_ID_FIELD1, &ov115_02260428) == 1) {
        ov01_021EFCDC(arg1, arg0);
    }
}

extern GymLeaderEncounterParam ov115_0226043C;

void ov115_0225F800(SysTask *arg0, EncounterEffect *arg1) {
    if (ov115_0225F220(arg1, HEAP_ID_FIELD1, &ov115_0226043C) == 1) {
        ov01_021EFCDC(arg1, arg0);
    }
}

extern GymLeaderEncounterParam ov115_02260450;

void ov115_0225F824(SysTask *arg0, EncounterEffect *arg1) {
    if (ov115_0225F220(arg1, HEAP_ID_FIELD1, &ov115_02260450) == 1) {
        ov01_021EFCDC(arg1, arg0);
    }
}

extern GymLeaderEncounterParam ov115_02260464;

void ov115_0225F848(SysTask *arg0, EncounterEffect *arg1) {
    if (ov115_0225F220(arg1, HEAP_ID_FIELD1, &ov115_02260464) == 1) {
        ov01_021EFCDC(arg1, arg0);
    }
}

extern GymLeaderEncounterParam ov115_02260478;

void ov115_0225F86C(SysTask *arg0, EncounterEffect *arg1) {
    if (ov115_0225F220(arg1, HEAP_ID_FIELD1, &ov115_02260478) == 1) {
        ov01_021EFCDC(arg1, arg0);
    }
}

extern GymLeaderEncounterParam ov115_0226048C;

void ov115_0225F890(SysTask *arg0, EncounterEffect *arg1) {
    if (ov115_0225F220(arg1, HEAP_ID_FIELD1, &ov115_0226048C) == 1) {
        ov01_021EFCDC(arg1, arg0);
    }
}

extern GymLeaderEncounterParam ov115_022604A0;

void ov115_0225F8B4(SysTask *arg0, EncounterEffect *arg1) {
    if (ov115_0225F220(arg1, HEAP_ID_FIELD1, &ov115_022604A0) == 1) {
        ov01_021EFCDC(arg1, arg0);
    }
}

extern GymLeaderEncounterParam ov115_022604B4;

void ov115_0225F8D8(SysTask *arg0, EncounterEffect *arg1) {
    if (ov115_0225F220(arg1, HEAP_ID_FIELD1, &ov115_022604B4) == 1) {
        ov01_021EFCDC(arg1, arg0);
    }
}

extern GymLeaderEncounterParam ov115_022604C8;

void ov115_0225F8FC(SysTask *arg0, EncounterEffect *arg1) {
    if (ov115_0225F220(arg1, HEAP_ID_FIELD1, &ov115_022604C8) == 1) {
        ov01_021EFCDC(arg1, arg0);
    }
}

extern GymLeaderEncounterParam ov115_022604DC;

void ov115_0225F920(SysTask *arg0, EncounterEffect *arg1) {
    if (ov115_0225F220(arg1, HEAP_ID_FIELD1, &ov115_022604DC) == 1) {
        ov01_021EFCDC(arg1, arg0);
    }
}

extern GymLeaderEncounterParam _02260374;

void ov115_0225F944(SysTask *arg0, EncounterEffect *arg1) {
    if (ov115_0225F220(arg1, HEAP_ID_FIELD1, &_02260374) == 1) {
        ov01_021EFCDC(arg1, arg0);
    }
}

void ov115_0225F968(void *arg0) {
    PlayerProfile_GetTrainerGender(Save_PlayerData_GetProfile(((FieldSystem *)arg0)->saveData));
}
