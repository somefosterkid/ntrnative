// port/test/ovl/run.sh's test: overlay 1's and 21's data dirtied, then loads reset theirs and nothing else
#include <stdio.h>
#include <string.h>
#include "nitro.h"
#include "port_ovl.h"

extern int ov1_counter, ov1_bss[4], ov1_common;
extern const char *ov1_ptr;
int ov1_get_static(void), ov1_get_zero(void);
void ov1_dirty(void);
extern int ov21a_val, ov21a_bss[100];
void ov21a_dirty(void);
char *ov21b_get(void);
long long ov21b_sum(void);
void ov21b_dirty(void);

int main_val = 3;
int main_bss[8];

static int fails;
#define CHECK(c) do { if (!(c)) { printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); fails++; } } while (0)

int main(void) {
    FSOverlayInfo info;

    CHECK(PortOvl_Count() == 237);
    CHECK(PortOvl_Owner(&ov1_counter) == 1);
    CHECK(PortOvl_Owner(&ov1_bss[2]) == 1);
    CHECK(PortOvl_Owner(&ov1_common) == 1);
    CHECK(PortOvl_Owner(&ov21a_val) == 21);
    CHECK(PortOvl_Owner(ov21b_get()) == 21);
    CHECK(PortOvl_Owner(&ov21a_bss[99]) == 21);
    CHECK(PortOvl_Owner(&main_val) == -1);
    CHECK(PortOvl_Owner(&main_bss[0]) == -1);

    CHECK(FS_LoadOverlayInfo(&info, MI_PROCESSOR_ARM9, 21));
    CHECK((uintptr_t)info.header.ram_address == 0x02187f00 && info.header.bss_size == 0x10040);
    CHECK(info.header.ram_size == 0x021de700 - 0x02187f00);
    CHECK(!FS_LoadOverlayInfo(&info, MI_PROCESSOR_ARM9, 237));
    CHECK(!FS_LoadOverlayInfo(&info, MI_PROCESSOR_ARM7, 1));

    CHECK(FS_LoadOverlay(MI_PROCESSOR_ARM9, 1));
    CHECK(FS_LoadOverlay(MI_PROCESSOR_ARM9, 21));
    ov1_dirty();
    ov21a_dirty();
    ov21b_dirty();
    main_val = 30;
    main_bss[0] = 1;
    CHECK(ov1_get_static() == 70 && ov21a_val == 0 && ov21b_get()[0] == 'X');

    // ov001 again: its data back, its bss zero; ov021's and main's untouched
    CHECK(FS_UnloadOverlay(MI_PROCESSOR_ARM9, 1));
    CHECK(FS_LoadOverlay(MI_PROCESSOR_ARM9, 1));
    CHECK(ov1_counter == 5 && ov1_get_static() == 7 && ov1_ptr != NULL && strcmp(ov1_ptr, "hello") == 0);
    CHECK(ov1_bss[0] == 0 && ov1_bss[3] == 0 && ov1_common == 0 && ov1_get_zero() == 0);
    CHECK(ov21a_val == 0 && ov21a_bss[99] == 3 && ov21b_get()[0] == 'X' && ov21b_sum() == 103);
    CHECK(main_val == 30 && main_bss[0] == 1);

    // ov021 again
    CHECK(FS_LoadOverlay(MI_PROCESSOR_ARM9, 21));
    CHECK(ov21a_val == 21 && ov21a_bss[99] == 0 && strcmp(ov21b_get(), "field") == 0 && ov21b_sum() == 6);
    CHECK(main_val == 30 && main_bss[0] == 1);
    CHECK(!FS_LoadOverlay(MI_PROCESSOR_ARM9, 999));
    printf("%s (%d failed)\n", fails ? "FAILED" : "ok", fails);
    return fails != 0;
}
