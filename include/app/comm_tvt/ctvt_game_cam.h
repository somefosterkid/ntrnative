// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/comm_tvt/ctvt_game_cam.h: the lines in
// use so far.
#ifndef BWPORT_APP_COMM_TVT_CTVT_GAME_CAM_H
#define BWPORT_APP_COMM_TVT_CTVT_GAME_CAM_H

#include "types.h"
#include "app/comm_tvt/ctvt_game.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
struct CtvtGameCam {
    BOOL cameraEnabled;
    // The character whose pictures are this machine's without a camera
    u8 character;
    // This machine's pictures
    void *pictures[3];
    CtvtGameMember members[3];
    CameraSystem *cameraSystem;
    // CTVT_GAME_TYPE_*
    int type;
};

#endif // BWPORT_APP_COMM_TVT_CTVT_GAME_CAM_H
