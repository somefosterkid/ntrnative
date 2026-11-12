// Test overlay 21, second file (stands for src/ov021/field_map_chunk_gc.c)
static char ov21b_name[16] = "field";
static long long ov21b_big[3] = {1, 2, 3};
char *ov21b_get(void) { return ov21b_name; }
long long ov21b_sum(void) { return ov21b_big[0] + ov21b_big[1] + ov21b_big[2]; }
void ov21b_dirty(void) { ov21b_name[0] = 'X'; ov21b_big[2] = 100; }
