#ifndef POKEHEARTGOLD_FRONTIER_LAUNCH_H
#define POKEHEARTGOLD_FRONTIER_LAUNCH_H

#include "overlay_80_field_frontier_dto.h"

// compat alias for files still held by other workers (remove once they are free)
typedef FieldFrontierDTO FrontierLaunchArgs;

struct BattleFrontier;
struct FrontierGraphics;

FieldFrontierDTO *Frontier_GetLaunchArgs(struct BattleFrontier *frontier);
struct FrontierGraphics *sub_0209680C(struct BattleFrontier *frontier);

#endif // POKEHEARTGOLD_FRONTIER_LAUNCH_H
