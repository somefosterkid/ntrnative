// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/battle_recorder.h: the lines in use so
// far.
#ifndef BWPORT_APP_BATTLE_RECORDER_H
#define BWPORT_APP_BATTLE_RECORDER_H

#include "types.h"
#include "gfl/proc.h"
#include "struct_decls.h"

// The parameter of br_main.c's proc
typedef struct {
    u32 mode;
    GameData *gameData;
    // The login work battle_video_proc.c lends (its loginBuffer), for br_net.c
    void *loginBuffer;
    // 2 when the online connection failed
    u32 result;
} BattleRecorderParam;

// Black 2's, not checked against Black
// What the Battle Recorder is opened for
enum {
    // The Battle Recorder in the bag: the player's own videos and records
    BR_MODE_BROWSE,
    // The Global Link's Battle Videos, online
    BR_MODE_GLOBAL_BV,
    // The Global Link's musical photos, online
    BR_MODE_GLOBAL_MUSICAL,
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BR_MAIN_PROC_FUNCTIONS
extern const GameProcFunctions data_ov187_0220b294;

#endif // BWPORT_APP_BATTLE_RECORDER_H
