#include "global.h"

#include "field/overlay_01_02204004.h"

int IsFanfarePlaying(const MapPropAnimation *animation);

BOOL ov80_02235408(const MapPropAnimation *animation);

BOOL ov80_02235408(const MapPropAnimation *animation) {
    return IsFanfarePlaying(animation) == 0;
}
