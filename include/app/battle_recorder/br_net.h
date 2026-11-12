// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/battle_recorder/br_net.h, the lines in
// use so far.
#ifndef BWPORT_APP_BATTLE_RECORDER_BR_NET_H
#define BWPORT_APP_BATTLE_RECORDER_BR_NET_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"
#include "save/musical_savedata.h"

// The Battle Recorder's connection to the Global Link (br_net.c), used by its online modes. pokebw2's guessed names
// are in the comments

// The data of a request to the Global Link
typedef struct {
    union {
        // The battle video download and favorite requests, 3 and 8, send this number
        u64 dataNumber;
        struct {
            union {
                // The musical photo upload, request 0, sends the photo
                void *data;
                struct {
                    // The species whose musical photos request 1 downloads
                    u16 species;
                    u8 unk2;
                    u8 unk3;
                };
            };
            u32 unk4;
        };
    };
} BrNetRequestParam;

// pokebw2's BrNet_StartRequest, BrNet_IsRequestEnd, BrNet_GetResultMsg and BrNet_CheckError
void func_ov186_021ff54c(BrNet *net, s32 type, const BrNetRequestParam *param);
BOOL func_ov186_021ff648(BrNet *net);
BOOL func_ov186_021ff72c(BrNet *net, u32 *msgID);
u32 func_ov186_021ff88c(BrNet *net);

// Black 2's; twinned code confirms the values of BR_NET_REQUEST_2, BR_NET_REQUEST_3 and BR_NET_REQUEST_8 in Black
// The requests BrNet_StartRequest starts
enum {
    BR_NET_REQUEST_UPLOAD_MUSICAL_SHOT,
    BR_NET_REQUEST_DOWNLOAD_MUSICAL_SHOT,
    BR_NET_REQUEST_2,
    BR_NET_REQUEST_3,
    BR_NET_REQUEST_4,
    BR_NET_REQUEST_5,
    BR_NET_REQUEST_6,
    BR_NET_REQUEST_7,
    BR_NET_REQUEST_8,
    BR_NET_REQUEST_MAX,
};

// pokebw2's BrNet_GetUploadVideoNumber
// Once the video's upload is finished, TRUE and the video's number
BOOL func_ov186_021ff67c(BrNet *net, u64 *number);

// pokebw2's BrNet_GetDownloadVideo
// Once a video is downloaded, TRUE, a pointer to it and the value that goes with it
BOOL func_ov186_021ff698(BrNet *net, void **video, u32 *value);

// What a search for battle videos asks for
typedef struct {
    // The species the videos have, or BR_NET_VIDEO_SEARCH_MONSNO_NONE
    u16 monsno;
    // Where the videos' players live, or BR_NET_VIDEO_SEARCH_AREA_NONE in both
    u8 country;
    u8 area;
    // The videos' battle mode, a line of the search screen's list, or BR_NET_VIDEO_SEARCH_MODE_NONE
    u32 mode;
} BrNetVideoSearchData;

// pokebw2's BrNet_Main
void func_ov186_021ff52c(BrNet *net);
// pokebw2's BrNet_GetDownloadMusicalShot
// TRUE once the photos are downloaded, then giving up to tblMax of them in tbl and their number in recvNum
BOOL func_ov186_021ff660(BrNet *net, BrMusicalShotRecv **tbl, int tblMax, int *recvNum);
// pokebw2's BrNet_GetDownloadVideoListA
// Once a list of videos is downloaded, TRUE with up to listMax of its entries in list and their number in num. The
// three are for the three lists the Battle Recorder can ask for
BOOL func_ov186_021ff6b4(BrNet *net, void *list, int listMax, int *num);
// pokebw2's BrNet_IsFailed
// TRUE once the connection has failed
BOOL func_ov186_021ff8d4(BrNet *net);

#define BR_NET_VIDEO_SEARCH_MONSNO_NONE 0xffff
#define BR_NET_VIDEO_SEARCH_AREA_NONE 0xff
#define BR_NET_VIDEO_SEARCH_MODE_NONE 19

// A musical photo downloaded from the Global Link, after the profile of the player who sent it
struct BrMusicalShotRecv {
    // A GdsProfile, whose layout isn't recovered yet
    u8 profile[0x80];
    MusicalShot shot;
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BrNet_Init
BrNet *func_ov186_021ff408(GameData *gameData, void *loginBuffer, HeapID heapId);
// pokebw2's BrNet_Exit
void func_ov186_021ff4e4(BrNet *net);
// pokebw2's BrNet_GetDownloadVideoListB
BOOL func_ov186_021ff6dc(BrNet *net, void *list, int listMax, int *num);
// pokebw2's BrNet_GetDownloadVideoListC
BOOL func_ov186_021ff704(BrNet *net, void *list, int listMax, int *num);

#endif // BWPORT_APP_BATTLE_RECORDER_BR_NET_H
