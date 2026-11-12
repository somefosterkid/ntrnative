// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/scrcmd_shop.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_FIELD_SCRCMD_SHOP_H
#define BWPORT_FIELD_SCRCMD_SHOP_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
BOOL s0149_CallFriendlyShopBuy(VM *vm, FieldScriptEnv *env);

#endif // BWPORT_FIELD_SCRCMD_SHOP_H
