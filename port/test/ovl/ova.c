// Test overlay 1 (stands for src/ov001/event_gtsnego.c)
int ov1_counter = 5;
static int ov1_static = 7;
int ov1_bss[4];
int ov1_common; // tentative: COMMON with -fcommon
const char *ov1_ptr = "hello"; // a pointer in .data (a relocation in PIE)
static int ov1_zero_static;

int ov1_get_static(void) { return ov1_static; }
int ov1_get_zero(void) { return ov1_zero_static; }
void ov1_dirty(void) {
    ov1_static = 70;
    ov1_counter = 50;
    ov1_bss[0] = 1;
    ov1_bss[3] = 4;
    ov1_common = 9;
    ov1_ptr = 0;
    ov1_zero_static = 11;
}
