#include "global.h"

struct ApplicationManager;

struct ApplicationManagerTemplate;

struct DWCstInetControl;

struct Options;

struct SaveData;

struct String;

struct WorldExchangeTrainer;

struct WorldExchangeTrainerError;

struct tagDWCWDSData;

typedef enum {
    DWC_WDS_STATE_IDLE = 0,
    DWC_WDS_STATE_PROCESS = 1,
    DWC_WDS_STATE_COMPLETED = 2,
    DWC_WDS_STATE_FAILED = 3,
    DWC_WDS_STATE_ERROR = 4
} DWCWDSState;

typedef struct tagDWCWDSData {
    u8 ssid[32];
    u8 wep[32];
    int wepMode;
    char apnum[10];
} DWCWDSData;

typedef struct DWCstInetControl {
    volatile int ac_state;
    u16 state;
    u16 online;
    u16 dmaNo;
    u16 powerMode;
    u8 wontExecuteWDS;
    DWCWDSState processWDSstate;
    DWCWDSData nspotInfo;

} DWCInetControl;

typedef struct WorldExchangeTrainer {
    u8 gameCode;
    u8 language;
    u8 country;
    u8 unk_03;
    u32 trainerId;
    u16 trainerName[7 + 1];
    u32 unk_10;
    u8 macAddress[6];
    u8 unk_1A[2];
    char email[56];
    u32 emailInitialised;
    u16 rngValue;
    u16 unk_5A;
} WorldExchangeTrainer;

typedef struct WorldExchangeTrainerError {
    u32 validationError;
    u32 systemError;
} WorldExchangeTrainerError;

typedef struct SaveData SaveData;

typedef struct Options Options;

typedef struct ApplicationManager ApplicationManager;

typedef BOOL (*OverlayFunc)(ApplicationManager *appMan, int *state);

typedef struct ApplicationManagerTemplate {
    OverlayFunc init;
    OverlayFunc main;
    OverlayFunc exit;
    const FSOverlayID overlayID;
} ApplicationManagerTemplate;

struct ApplicationManager {
    ApplicationManagerTemplate template;
    int execState;
    int procState;
    void *args;
    void *data;
    ApplicationManager *parent;
    ApplicationManager *child;
};

typedef struct String String;

typedef struct {
    ApplicationManager *appMan;
    SaveData *saveData;
    Options *options;
    void *unk_0C;
    NNSFndHeapHandle unk_10;
    DWCInetControl unk_14;
    u16 unk_7C;
    u8 unk_7E;
    u8 padding_7F;
    int unk_80;
    int unk_84;
    u8 padding_88[4];
    int unk_8C;
    int unk_90;
    u8 padding_94[4];
    void *unk_98;
    WorldExchangeTrainer unk_9C;
    WorldExchangeTrainerError unk_100;
    u32 unk_108;
    u32 unk_10C;
    u8 unk_110;
    u8 unk_111[3];
    String *unk_114;
    String *unk_118;
    BOOL unk_11C;
} UnkStruct_ov98_02246E88;

void sub_02031780(SaveData *saveData, int param1, u32 param2);

void ov75_02246C8C(UnkStruct_ov98_02246E88 *param0);

void ov75_02246C8C(UnkStruct_ov98_02246E88 *param0) {
    sub_02031780(param0->saveData, 3, param0->unk_108);
}

void ov75_02246CD8(UnkStruct_ov98_02246E88 *param0, u32 param1);

u32 sub_0203186C(SaveData *saveData, WorldExchangeTrainer *param1);

void ov75_02246CA0(UnkStruct_ov98_02246E88 *param0);

void ov75_02246CA0(UnkStruct_ov98_02246E88 *param0) {
    u32 v0 = sub_0203186C(param0->saveData, &param0->unk_9C);
    ov75_02246CD8(param0, v0);
}
