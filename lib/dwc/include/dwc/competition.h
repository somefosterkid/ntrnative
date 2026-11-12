// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/competition.h: its types and
// constants, and the prototypes of Black's named functions.
#ifndef BWPORT_DWC_COMPETITION_H
#define BWPORT_DWC_COMPETITION_H

#include "types.h"

typedef struct ScClient ScClient;
typedef struct ScReport ScReport;

// The result of the service's calls (the GameSpy SDK's SCResult)
typedef enum {
    SCResult_NO_ERROR = 0,
    SCResult_NO_AVAILABILITY_CHECK,
    SCResult_INVALID_PARAMETERS,
    SCResult_NOT_INITIALIZED,
    SCResult_CORE_NOT_INITIALIZED,
    SCResult_OUT_OF_MEMORY,
    SCResult_CALLBACK_PENDING,
    SCResult_HTTP_ERROR,
    SCResult_UNKNOWN_RESPONSE,
    SCResult_RESPONSE_INVALID,
    SCResult_REPORT_INCOMPLETE,
    SCResult_REPORT_INVALID,
    SCResult_SUBMISSION_FAILED,
    SCResult_UNKNOWN_ERROR,
    SCResultMax
} SCResult;

// Called with the client, the status of the request, the error (0 if none) and the user data
typedef void (*ScClientFunc)(ScClient *client, int status, int error, void *userData);

// The client of the service: the functions to call back and the flags that tell that a request is running, each for
// the three requests, then the report being submitted, the game, and the IDs of the session and of the connection
struct ScClient {
    ScClient *self;
    ScClientFunc createSessionFunc;
    ScClientFunc reportIntentionFunc;
    ScClientFunc submitReportFunc;
    int reportIntentionBusy;
    int createSessionBusy;
    int submitReportBusy;
    int unk_1c;
    int unk_20;
    int unk_24;
    void *reportIntentionData;
    void *createSessionData;
    void *submitReportData;
    int unk_34;
    int unk_38;
    int unk_3c;
    void *report;
    int reportSize;
    int ready;
    int gameId;
    int valid;
    int unk_54;
    int unk_58;
    char csid[0x28];
    char ccid[0x28];
};

// The report: the phase of the data being added, the group being added to, the keys that have a record and the
// buffer, which starts with a header of 0x44 bytes. The data has the header's player and team sections, then the
// records of the game, of the players and of the teams
struct ScReport {
    int phase;
    int groupOffset;
    u16 groupCount;
    int unk_0c;
    u32 keyNum;
    int keys[0x40];
    int unk_114;
    int unk_118;
    int bufferSize;
    int used;
    u8 *buffer;
};

// The phases of a report, which are those of the records' sections
typedef enum ScReportPhase {
    SC_REPORT_PHASE_NEW = 1,
    SC_REPORT_PHASE_GAME = 2,
    SC_REPORT_PHASE_PLAYER = 3,
    SC_REPORT_PHASE_TEAM = 4,
} ScReportPhase;

#endif // BWPORT_DWC_COMPETITION_H
