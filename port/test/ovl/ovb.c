// Test overlay 21, first file (stands for src/ov021/field_map_chunk_wb.c)
int ov21a_val = 21;
int ov21a_bss[100];
void ov21a_dirty(void) { ov21a_val = 0; ov21a_bss[99] = 3; }
