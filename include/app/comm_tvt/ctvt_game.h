// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/comm_tvt/ctvt_game.h: the lines in use
// so far.
#ifndef BWPORT_APP_COMM_TVT_CTVT_GAME_H
#define BWPORT_APP_COMM_TVT_CTVT_GAME_H

#include "types.h"

// Black 2's, not checked against Black
// One of the others in the call, and the pictures of them that go on their faces and balloons
struct CtvtGameMember {
    BOOL hasCamera;
    BOOL canExchangePhotos;
    u8 netId;
    void *pictures[3];
};

#endif // BWPORT_APP_COMM_TVT_CTVT_GAME_H
