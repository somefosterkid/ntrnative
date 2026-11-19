#include "global.h"

#include "constants/items.h"
#include "constants/save_arrays.h"

#include "error_handling.h"
#include "unk_02031B0C.h"

int ApricornBox_GiveApricorn(SaveApricornBox *apricornBox, int apricorn, u8 amount);

int ApricornBox_GiveApricorn(SaveApricornBox *apricornBox, int apricorn, u8 amount) {
    int quantity = amount;
    if (apricorn >= 7) {
        ((0) ? (void)0 : GF_AssertFail());
        return 0;
    }
    quantity += apricornBox->apricorns[apricorn];
    if (quantity > 99) {
        apricornBox->apricorns[apricorn] = 99;
    } else {
        apricornBox->apricorns[apricorn] = quantity;
    }
    return apricornBox->apricorns[apricorn];
}
