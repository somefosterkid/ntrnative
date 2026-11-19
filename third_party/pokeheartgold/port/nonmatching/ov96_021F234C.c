#include "error_handling.h"

typedef struct { /* invented */
  u8 items[12];
  u32 count;
} Ov96NodeStack;

typedef struct { /* invented */
  VecFx32 *acc;
  u8 nbrs[12];
  u16 count;
  u16 idx;
  VecFx32 vecs[12];
} Ov96Node;

extern u8 _0221DCA0[];

u8 ov96_021F2780(Ov96NodeStack *stack, u8 node);

u8 ov96_021F27A8(Ov96NodeStack *stack);

void ov96_021F27B8(VecFx32 *acc, VecFx32 *vec, VecFx32 *dir);

fx32 ov96_021F2814(VecFx32 *acc, VecFx32 *vec);

void ov96_021F234C(u8 *param) {
  u8 *obj;
  u8 *ent;
  u8 count;
  Ov96Node *node;
  u8 i;
  u8 j;
  u8 m;
  u8 cur;
  s32 n;
  s32 r;
  BOOL fresh;
  Ov96Node nodes[12];
  VecFx32 accum[12][12];
  VecFx32 deltas[12];
  u8 list[12];
  u8 visited[12];
  Ov96NodeStack stack;
  VecFx32 dir;
  VecFx32 diff;
  VecFx32 tmp;

  count = 0;
  for (i = 0; i < 12; i++) {
    obj = param + 0x20 + (i / 3) * 0x1b0 + (i % 3) * 0x90;
    if (obj[0x41] != 0 && *(s32 *)(obj + 0x18) == 1) {
      list[count++] = i;
    }
    VEC_Subtract((VecFx32 *)(obj + 0x1c), (VecFx32 *)(obj + 0x28), &deltas[i]);
  }
  if (count == 0) {
    return;
  }
  MIi_CpuClear32(0, accum, sizeof(accum));
  for (m = 0; m < count; m++) {
    cur = list[m];
    stack.count = 0;
    MI_CpuFill8(nodes, 0, sizeof(nodes));
    for (j = 0; j < 12; j++) {
      nodes[j].acc = &accum[m][j];
    }
    MI_CpuFill8(visited, 0, sizeof(visited));
    dir = *(VecFx32 *)(param + 0x20 + (cur / 3) * 0x1b0 + (cur % 3) * 0x90 + 0x34);
    fresh = TRUE;
    do {
      obj = param + 0x48 + (cur / 3) * 0x1b0;
      r = (cur % 3) * 0x90;
      visited[cur] = 1;
      if (fresh) {
        nodes[cur].count = 0;
        nodes[cur].idx = 0;
        for (j = 0, ent = _0221DCA0 + cur * 0xc0; j < 12; j++) {
          if (*(u32 *)(ent + j * 16) != 0) {
            VEC_Subtract((VecFx32 *)(*(u8 **)(ent + j * 16 + 8) + 0x28), (VecFx32 *)(obj + r), &diff);
            if (VEC_DotProduct(&dir, &diff) > 0) {
              nodes[cur].nbrs[nodes[cur].count] = j;
              nodes[cur].vecs[nodes[cur].count] = diff;
              nodes[cur].count++;
            }
          }
        }
        VEC_Add(nodes[cur].acc, &dir, nodes[cur].acc);
      }
      node = &nodes[cur];
      fresh = FALSE;
      if (node->idx >= node->count) {
        visited[cur] = 0;
        cur = ov96_021F27A8(&stack);
      } else if (visited[node->nbrs[node->idx]]) {
        node->idx++;
      } else {
        if (ov96_021F2780(&stack, cur) == 0xff) {
          break;
        }
        if (ov96_021F2814(node->acc, &node->vecs[node->idx]) >= FX_SinCosTable_[0x401]) {
          dir = *node->acc;
        } else {
          ov96_021F27B8(node->acc, &node->vecs[node->idx], &dir);
        }
        fresh = TRUE;
        cur = node->nbrs[node->idx++];
      }
    } while (cur != 0xff);
  }
  for (j = 0; j < count; j++) {
    for (i = 0; i < 12; i++) {
      obj = param + 0x20 + (i / 3) * 0x1b0 + (i % 3) * 0x90;
      VEC_Add((VecFx32 *)(obj + 0x28), &accum[j][i], (VecFx32 *)(obj + 0x28));
      if (VEC_Mag(&accum[j][i]) != 0 && obj[0x41] == 0) {
        *(VecFx32 *)(obj + 0x1c) = *(VecFx32 *)(obj + 0x28);
      }
    }
    cur = list[j];
    obj = param + 0x20 + (cur / 3) * 0x1b0 + (cur % 3) * 0x90;
    if (*(u16 *)(obj + 0x8e) == 0) {
      for (n = 0; n < 4; n++) {
        if (cur == param[0x720 + n]) {
          param[0x720 + n] = 12;
          break;
        }
      }
      GF_ASSERT(*(s32 *)(obj + 0x18) == 1);
      obj[0x44] = 1;
      obj[0x45] = 0x3c;
      *(VecFx32 *)(obj + 0x1c) = *(VecFx32 *)(obj + 0x28);
      *(u32 *)(obj + 0x34) = 0;
      *(u32 *)(obj + 0x38) = 0;
      obj[0x41] = 0;
    }
  }
  for (i = 0; i < 12; i++) {
    obj = param + 0x20 + (i / 3) * 0x1b0 + (i % 3) * 0x90;
    VEC_Subtract((VecFx32 *)(obj + 0x1c), (VecFx32 *)(obj + 0x28), &tmp);
    if (VEC_DotProduct(&deltas[i], &tmp) < 0) {
      *(VecFx32 *)(obj + 0x1c) = *(VecFx32 *)(obj + 0x28);
    }
  }
}
