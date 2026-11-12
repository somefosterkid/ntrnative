// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/wf2dmap/wf2dmap_judge.h: the lines in
// use so far.
#ifndef BWPORT_APP_WF2DMAP_WF2DMAP_JUDGE_H
#define BWPORT_APP_WF2DMAP_WF2DMAP_JUDGE_H

#include "types.h"
#include "app/wf2dmap/wf2dmap_common.h"
#include "app/wf2dmap/wf2dmap_map.h"
#include "app/wf2dmap/wf2dmap_obj.h"

// pokebw2's WF2DMAP_JUDGESysCmdJudge
// Fills act and returns TRUE when the request is granted
BOOL func_ov127_021c5370(const WF2DMAP_MAPSYS *mapSys, const WF2DMAP_OBJSYS *objSys, const WF2DMAP_REQCMD *req,
                              WF2DMAP_ACTCMD *act);

#endif // BWPORT_APP_WF2DMAP_WF2DMAP_JUDGE_H
