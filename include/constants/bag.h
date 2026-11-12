// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/constants/bag.h: the lines in use so far.
#ifndef BWPORT_CONSTANTS_BAG_H
#define BWPORT_CONSTANTS_BAG_H

#include "types.h"

// The bag's pockets, in the order the bag keeps them. Black's items, medicine and berries: b_bag_item.c's
// matched table of them
#define BAG_POCKET_ITEMS 0
#define BAG_POCKET_MEDICINE 1
#define BAG_POCKET_BERRIES 3

#define BAG_POCKET_TMS_HMS 2
#define BAG_POCKET_KEY_ITEMS 4

// The count of Black's pockets (the Free Space is not one of them)
#define BAG_POCKET_COUNT 5

// Black 2's, not checked against Black
// The items the player moved to the Free Space, which stay in their own pockets
#define BAG_POCKET_FREE_SPACE 5

#endif // BWPORT_CONSTANTS_BAG_H
