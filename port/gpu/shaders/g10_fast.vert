// The fast 3D renderer (src/g10_fast.h, docs/gpu-hwraster.md): a triangle vertex of src/g10_fast.c, pulled from its buffer by
// the vertex index. The position on the N k grid to clip space with z screen-linear (the DS's z-buffer: z w / w), the
// attributes interpolated by w (perspective-correct, the DS's 1 / lerp(1 / w))
layout(std430, binding = 2) readonly buffer HwV { vec4 hv[]; };
layout(std140, binding = 3) uniform HwU { vec4 hu; }; // the grid's width and height
layout(std430, binding = 4) readonly buffer HwE { vec4 he[]; }; // polygon k's edge functions (src/g10_fast.h GpuFast.pe)
layout(location = 0) out vec3 vCol;
layout(location = 1) out vec2 vSt;
layout(location = 2) out float vW;
layout(location = 3) flat out int vK;
layout(location = 4) flat out int vF;
layout(location = 5) flat out vec3 vE0; // the fan triangle's polygon edges i - 1, i, i + 1 (none: never near)
layout(location = 6) flat out vec3 vE1;
layout(location = 7) flat out vec3 vE2;
void main() {
  int i = gl_VertexID * 3;
  vec4 p = hv[i], a = hv[i + 1], b = hv[i + 2];
  float w = p.w;
  float nx = p.x / hu.x * 2.0 - 1.0, ny = 1.0 - p.y / hu.y * 2.0;
  gl_Position = vec4(nx * w, ny * w, p.z / 16777215.0 * w, w);
  vCol = a.xyz;
  vSt = vec2(a.w, b.x);
  vW = w;
  vK = int(b.y);
  vF = int(b.z);
  int nv = (vF >> 4) & 15, t = vF >> 8;
  vE0 = vE1 = vE2 = vec3(0.0, 0.0, 1e9);
  if (nv > 0) {
    int k10 = vK * 10;
    vE0 = he[k10 + (t + nv - 1) % nv].xyz;
    vE1 = he[k10 + t].xyz;
    vE2 = he[k10 + (t + 1) % nv].xyz;
  }
}
