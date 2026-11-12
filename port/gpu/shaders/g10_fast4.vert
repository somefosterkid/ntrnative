// The fast 3D renderer with 4 samples (src/g10_fast_mtl.inc, docs/gpu-hwraster.md): a fragment per output pixel, the pixel's four
// samples (the 2N grid's points) covered and shaded in g10_fast4.frag. A triangle of src/g10_fast.c (vertex pulling: the
// triangle's three vertices from hwV) is drawn grown by three quarters of a 2N pixel on every side, so that every output pixel
// with a sample in it gets a fragment; the fragment then takes the samples inside the triangle itself as the rasterizer would on
// the 2N grid (the top-left rule, in integers: G, twice the 2N grid's coordinates), and the attributes from planes (flat): the
// 24-bit z and 1 / w screen-linear, the colour and texture coordinates over w (perspective-correct, the DS's 1 / lerp(1 / w)).
#ifdef GPU_SPIRV
layout(std430, binding = 2) readonly buffer HwV { vec4 hv[]; };
layout(std140, binding = 3) uniform HwU { vec4 hu; }; // the 2N grid's width and height
layout(std430, binding = 4) readonly buffer HwE { vec4 he[]; }; // polygon k's edge functions (src/g10_fast.h GpuFast.pe)
#define HV(i) hv[i]
#define HE(i) he[i]
#define G10_VARY(n) layout(location = n)
#else // GL (src/g10_fast_gl.c): GLES 3.0 has no storage buffers: both from data textures (RGBA32F, 1024 texels a row)
uniform highp sampler2D uFastV;
uniform highp sampler2D uFastE;
uniform vec4 hu;
#define HV(i) texelFetch(uFastV, ivec2((i) & 1023, (i) >> 10), 0)
#define HE(i) texelFetch(uFastE, ivec2((i) & 1023, (i) >> 10), 0)
#define G10_VARY(n) // (GLSL ES 3.00 and 3.30 have no locations on the varyings: they go by name)
#endif
G10_VARY(0) flat out ivec3 vA; // the triangle's edges j (vertex j to j + 1) in G relative to vertex 0: A x + B y + C,
G10_VARY(1) flat out ivec3 vB; // positive inside
G10_VARY(2) flat out ivec3 vC;
G10_VARY(3) flat out ivec2 vO; // vertex 0 in G
G10_VARY(4) flat out vec3 vZ;  // planes over G - vO: (value at vO, d/dx, d/dy): z
G10_VARY(5) flat out vec3 vIW; // 1 / w
G10_VARY(6) flat out vec3 vR;  // colour / w
G10_VARY(7) flat out vec3 vG;
G10_VARY(8) flat out vec3 vBl;
G10_VARY(9) flat out vec3 vS; // s / w, t / w
G10_VARY(10) flat out vec3 vT;
G10_VARY(11) flat out int vK;
G10_VARY(12) flat out int vF;
G10_VARY(13) flat out vec3 vE0; // the fan triangle's polygon edges i - 1, i, i + 1 (none: never near), on the 2N grid
G10_VARY(14) flat out vec3 vE1;
G10_VARY(15) flat out vec3 vE2;
vec3 plane(vec3 f, vec2 d1, vec2 d2, float D) {
  return vec3(f.x, ((f.y - f.x) * d2.y - (f.z - f.x) * d1.y) / D, ((f.z - f.x) * d1.x - (f.y - f.x) * d2.x) / D);
}
void main() {
  int t = gl_VertexID / 3, c = gl_VertexID - t * 3;
  vec4 p[3], a[3], b[3];
  for (int i = 0; i < 3; i++) {
    p[i] = HV((t * 3 + i) * 3);
    a[i] = HV((t * 3 + i) * 3 + 1);
    b[i] = HV((t * 3 + i) * 3 + 2);
  }
  ivec2 G[3];
  for (int i = 0; i < 3; i++) G[i] = ivec2(round(p[i].xy * 2.0));
  ivec2 e1 = G[1] - G[0], e2 = G[2] - G[0];
  int D = e1.x * e2.y - e1.y * e2.x; // twice the area, positive: clockwise on screen (y down)
  float sg = D < 0 ? -1.0 : 1.0;
  // the edges, positive inside; grown by 1.5 (|A| + |B|): three quarters of a 2N pixel either way
  vec2 q = vec2(G[c]);
  ivec3 A, B, C;
  vec3 gA, gB, gC;
  for (int j = 0; j < 3; j++) {
    ivec2 u = G[j], v = G[(j + 1) % 3];
    int s = D < 0 ? -1 : 1;
    A[j] = -(v.y - u.y) * s;
    B[j] = (v.x - u.x) * s;
    C[j] = -(A[j] * (u.x - G[0].x) + B[j] * (u.y - G[0].y));
    gA[j] = float(A[j]);
    gB[j] = float(B[j]);
    gC[j] = float(C[j]) + 1.5 * (abs(gA[j]) + abs(gB[j]));
  }
  if (D != 0) { // vertex c: where its two edges' grown lines meet (edge c - 1 into c, edge c out of it)
    int j0 = (c + 2) % 3, j1 = c;
    float det = gA[j0] * gB[j1] - gA[j1] * gB[j0];
    vec2 r = vec2(-(gC[j0] * gB[j1] - gC[j1] * gB[j0]), -(gA[j0] * gC[j1] - gA[j1] * gC[j0])) / det;
    q = r + vec2(G[0]);
  }
  vA = A;
  vB = B;
  vC = C;
  vO = G[0];
  vec2 d1 = vec2(e1), d2 = vec2(e2);
  float Df = float(D == 0 ? 1 : D);
  vec3 iw = vec3(1.0 / p[0].w, 1.0 / p[1].w, 1.0 / p[2].w);
  vZ = plane(vec3(p[0].z, p[1].z, p[2].z), d1, d2, Df);
  vIW = plane(iw, d1, d2, Df);
  vR = plane(vec3(a[0].x, a[1].x, a[2].x) * iw, d1, d2, Df);
  vG = plane(vec3(a[0].y, a[1].y, a[2].y) * iw, d1, d2, Df);
  vBl = plane(vec3(a[0].z, a[1].z, a[2].z) * iw, d1, d2, Df);
  vS = plane(vec3(a[0].w, a[1].w, a[2].w) * iw, d1, d2, Df);
  vT = plane(vec3(b[0].x, b[1].x, b[2].x) * iw, d1, d2, Df);
  vK = int(b[0].y);
  vF = int(b[0].z);
  vec2 g = q * 0.5; // back on the 2N grid
#ifdef GPU_SPIRV
  gl_Position = vec4(g.x / hu.x * 2.0 - 1.0, 1.0 - g.y / hu.y * 2.0, 0.0, 1.0);
#else // (GL's row 0 is DS row 0: not flipped)
  gl_Position = vec4(g.x / hu.x * 2.0 - 1.0, g.y / hu.y * 2.0 - 1.0, 0.0, 1.0);
#endif
  int nv = (vF >> 4) & 15, ti = vF >> 8;
  vE0 = vE1 = vE2 = vec3(0.0, 0.0, 1e9);
  if (nv > 0) {
    int k10 = vK * 10;
    vE0 = HE(k10 + (ti + nv - 1) % nv).xyz;
    vE1 = HE(k10 + ti).xyz;
    vE2 = HE(k10 + (ti + 1) % nv).xyz;
  }
}
