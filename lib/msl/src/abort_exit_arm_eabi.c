// MSL's abort_exit_arm_eabi.c (zeldaret-st's name), as Black has it: zeldaret-st's copy (CC0-1.0) is another version.
// Black's has atexit only, which registers its functions in the runtime's destructor chain (runtime/runtime.c); exit
// lies before it in an object of its own (exit.c, built with another compiler). The name is pokebw2's.
#include <critical_regions.h>

int __register_atexit(void (*func)(void));

int atexit(void (*func)(void)) {
    int result;

    __begin_critical_region(atexit_funcs_access);
    result = __register_atexit(func);
    __end_critical_region(atexit_funcs_access);
    return result;
}
