// The fast 3D renderer on GL (src/g10_fast_gl.c, docs/gpu-hwraster.md): g10_fast.vert with the vertex as three attributes (GLES
// 3.0 has no storage buffers) and the polygons' edge functions from a data texture. GL's row 0 is the bottom one and the passes
// put DS row 0 there (gl_FragCoord.y = row), so y is not flipped as Metal's is.
layout(location = 0) in vec4 aP; // src/g10_fast.c's vertex: (x, y, z, w), (c0, c1, c2, s), (t, k, flags, 0)
layout(location = 1) in vec4 aA;
layout(location = 2) in vec4 aB;
uniform highp sampler2D uFastE; // GpuFast.pe, RGBA32F, 1024 texels a row (gpu_gl_data_tex)
uniform vec2 uGrid;             // the grid's width and height
out vec3 vCol;
out vec2 vSt;
out float vW;
flat out int vK;
flat out int vF;
flat out vec3 vE0; // the fan triangle's polygon edges i - 1, i, i + 1 (none: never near)
flat out vec3 vE1;
flat out vec3 vE2;
flat out vec3 vE3; // and edges 0 and nv - 1 (g10_fast.vert)
flat out vec3 vE4;
vec4 edge(int i) { return texelFetch(uFastE, ivec2(i & 1023, i >> 10), 0); }
void main() {
  float w = aP.w;
  // z: GL's window depth is (ndc + 1) / 2 (Metal's is ndc), so gl_FragCoord.z is again the DS's z / 2^24 (g10_fast.frag)
  gl_Position = vec4((aP.x / uGrid.x * 2.0 - 1.0) * w, (aP.y / uGrid.y * 2.0 - 1.0) * w, (aP.z / 16777215.0 * 2.0 - 1.0) * w, w);
  vCol = aA.xyz;
  vSt = vec2(aA.w, aB.x);
  vW = w;
  vK = int(aB.y);
  vF = int(aB.z);
  int nv = (vF >> 4) & 15, t = vF >> 8;
  vE0 = vE1 = vE2 = vE3 = vE4 = vec3(0.0, 0.0, 1e9);
  if (nv > 0) {
    int k10 = vK * 10;
    vec4 h0 = edge(k10 + (t + nv - 1) % nv), h1 = edge(k10 + t), h2 = edge(k10 + (t + 1) % nv), h3 = edge(k10), h4 = edge(k10 + nv - 1);
    vE0 = h0.xyz;
    vE1 = h1.xyz;
    vE2 = h2.xyz;
    vE3 = h3.xyz;
    vE4 = h4.xyz;
    // which of them are inner (shared with a neighbour, src/g10_fast.c fast_seams): bits 16-20 of vF, for g10_fast.frag's rules
    vF |= (h0.w != 0.0 ? 1 : 0) << 16 | (h1.w != 0.0 ? 1 : 0) << 17 | (h2.w != 0.0 ? 1 : 0) << 18 | (h3.w != 0.0 ? 1 : 0) << 19 |
          (h4.w != 0.0 ? 1 : 0) << 20;
  }
}
