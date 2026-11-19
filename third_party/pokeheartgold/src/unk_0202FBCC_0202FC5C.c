#include "global.h"

#include "save.h"

extern SaveData *_021D2AF8;

SaveData *sub_0202FC5C(void);

SaveData *sub_0202FC5C(void) {
    ((_021D2AF8 != 0L) ? (void)0 : GF_AssertFail());
    return _021D2AF8;
}
