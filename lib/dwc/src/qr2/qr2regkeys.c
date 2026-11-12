// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/src/qr2/qr2regkeys.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "dwc/qr2regkeys.h"
#include "dwc/ext.h"
#include "dwc/gsDebug.h"
#include "dwc/nonport.h"
#include "gs/nonport.h"
#include "string.h"
// Declarations from the references' headers that the functions below need (tools/ref_decls.py): they
// belong in the library's header
#define APIENTRY
#define APIENTRYP APIENTRY *
typedef float khronos_float_t;
typedef khronos_float_t GLfloat;
typedef void(APIENTRYP PFNGLCLEARDEPTHFPROC)(GLfloat d);


// GameSpy's QR2 key names (qr2regkeys.c). The ROM names neither the file nor its functions: the file,
// func_ov009_0219d158 and data_ov009_021a7c90 are GameSpy's, as pret's pokerevo Wii build has them (qr2regkeys.s),
// and func_ov009_0219d114 is the later SDK's

// pokebw2's qr2_registered_key_list
const char *data_ov009_021a7c90[MAX_REGISTERED_KEYS] = {
    "",           "hostname",  "gamename",     "gamever",  "hostport", "mapname",   "gametype",      "gamevariant",
    "numplayers", "numteams",  "maxplayers",   "gamemode", "teamplay", "fraglimit", "teamfraglimit", "timeelapsed",
    "timelimit",  "roundtime", "roundelapsed", "password", "groupid",  "player_",   "score_",        "skill_",
    "ping_",      "team_",     "deaths_",      "pid_",     "team_t",   "score_t",   "nn_groupid",    "country",
    "region",
};

// GameSpy keeps a list of the unicode keys it allocated so it can free them when QR2 shuts down. Black's link
// drops the functions that use it, but the list keeps its place in the file's data
typedef struct QR2KeyListNodeS {
    char *mKeyData;
    struct QR2KeyListNodeS *mNextKey;
} QR2KeyListNode;

typedef struct QR2KeyListS {
    QR2KeyListNode *mHead;
} QR2KeyList;

static QR2KeyList qr2_internal_key_list = {NULL};

// pokebw2's func_ov009_0219d114
gsi_bool func_ov009_0219d114(const char *key) {
    if (strcmp(key, data_ov009_021a7c90[31]) == 0 || strcmp(key, data_ov009_021a7c90[32]) == 0) {
        return gsi_true;
    }
    return gsi_false;
}

void func_ov009_0219d158(int keyid, const char *key) {
    gsDebugFormat(GSIDebugCat_QR2, GSIDebugType_Misc, GSIDebugLevel_StackTrace, "qr2_register_keyA()\r\n");
    if (keyid < 50 || keyid > 254) {
        gsDebugFormat(GSIDebugCat_QR2, GSIDebugType_Misc, GSIDebugLevel_WarmError,
                      "Attempted to register invalid key %d - %s\r\n", keyid, key);
        return;
    }
    gsDebugFormat(GSIDebugCat_QR2, GSIDebugType_Misc, GSIDebugLevel_Comment, "Registered key %d - %s\r\n", keyid, key);
    data_ov009_021a7c90[keyid] = key;
}



void qr2_internal_key_list_append(char *theKey) {
    QR2KeyListNode *aNewNode;
    aNewNode = (QR2KeyListNode *)gsimalloc(sizeof(QR2KeyListNode));
    aNewNode->mKeyData = theKey;
    aNewNode->mNextKey = NULL;
    if (qr2_internal_key_list.mHead == NULL) {
        qr2_internal_key_list.mHead = aNewNode;
    } else {
        QR2KeyListNode *aInsertPlace = qr2_internal_key_list.mHead;
        while (aInsertPlace->mNextKey != NULL) {
            aInsertPlace = aInsertPlace->mNextKey;
        }
        aInsertPlace->mNextKey = aNewNode;
    }
}

void qr2_internal_key_list_free(void) {
    QR2KeyListNode *aNodeToFree;
    QR2KeyListNode *aNextNode;
    aNodeToFree = qr2_internal_key_list.mHead;
    while (aNodeToFree != NULL) {
        aNextNode = aNodeToFree->mNextKey;
        gsifree(aNodeToFree->mKeyData);
        gsifree(aNodeToFree);
        aNodeToFree = aNextNode;
    }
    qr2_internal_key_list.mHead = NULL;
}
