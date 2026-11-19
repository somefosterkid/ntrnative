// port/gpu: display capture at N x (docs/gpu.md "Display capture"), GL and Metal (tools/gpu_msl.py's PROGRAMS: under
// GPU_SPIRV the plain uniforms are block GpuK's, src/gpu_mtl.m's gK; the same values as src/g07_capture.c gives GL).
// The default program writes DISPCAPCNT's capture at N x (p11_capture.c's vc_capture_line, the same integers): source A =
// engine A's picture without master brightness (pass C again, kSrcA) or the 3D layer (bit 24), source B = a VRAM bank at
// N x (kBank) where its rows hold an N x capture (kRows), else the bank's 1x bytes (kBank1x) nearest. Out: R16UI BGR555 |
// alpha bit 15, 256 N x (rows N), copied into the destination bank's texture afterwards. With K_SHOW: engine A's half of
// pass C's output drawn again from the bank it shows (display mode 2) where kRows says the bank's row is N x, with master
// brightness (p10's vc_display_line); other rows keep pass C's 1x line.
uniform highp usampler2D kBank; // a bank at N x, R16UI 256 N x 256 N (BGR555 | alpha bit 15)
#ifdef GPU_SPIRV
uniform GpuK { // src/gpu_mtl.h's GpuK, std140
  uvec4 kRows4[2];
  int kN;
  uint kBright;
  int kY0;
  int k3dScale;
  uint kCnt;
  int kRoff;
  uint kHaveB;
};
#define K_ROWS(i) kRows4[(i) >> 2][(i) & 3]
#else
uniform uint kRows[8];          // the bank rows kBank holds (bit r & 31 of kRows[r >> 5])
#define K_ROWS(i) kRows[i]
uniform int kN;                 // N
#endif
#ifdef K_SHOW
#ifndef GPU_SPIRV
uniform uint kBright;           // engine A's MASTER_BRIGHT
uniform int kY0;                // engine A's half: its first row in the output
#endif
layout(location = 0) out vec4 oColor;
#else
uniform sampler2D kSrcA;          // engine A without master brightness, RGBA8 (v6 << 2 | v6 >> 4), 256 N x 192 N
uniform highp usampler2D k3d;     // the 3D layer, RGBA8UI (r6, g6, b6, a5), k3dScale x the DS size
uniform highp usampler2D kBank1x; // source B's bank as the game left it, R16UI 256 x 256
#ifndef GPU_SPIRV
uniform int k3dScale;             // 0: no 3D layer
uniform uint kCnt;                // DISPCAPCNT
uniform int kRoff;                // source B's first bank row (the read offset; 0 in display mode 2)
uniform uint kHaveB;              // 1: source B is a bank in LCDC mode (else 0, as vcore's)
#endif
layout(location = 0) out uint oCap;
#endif

bool k_row(int r) { return ((K_ROWS(r >> 5) >> uint(r & 31)) & 1u) != 0u; }

#ifdef K_SHOW
uint k_mk(int r, int g, int b) { return uint(r) | (uint(g) << 8) | (uint(b) << 16); }
// MASTER_BRIGHT (p10): up v + (63 - v) F / 16 rounded down, down v - (v F + 15) / 16 (g07_compose.frag's c_bright)
uint k_bright(uint px) {
  uint bm = (kBright >> 14) & 3u;
  int f = min(int(kBright & 31u), 16);
  if ((bm != 1u && bm != 2u) || f == 0) return px;
  int r = int(px & 63u), g = int((px >> 8) & 63u), b = int((px >> 16) & 63u);
  if (bm == 1u) return k_mk(r + (((63 - r) * f) >> 4), g + (((63 - g) * f) >> 4), b + (((63 - b) * f) >> 4));
  return k_mk(r - ((r * f + 15) >> 4), g - ((g * f + 15) >> 4), b - ((b * f + 15) >> 4));
}

void main() {
  ivec2 p = ivec2(gl_FragCoord.xy);
  p.y -= kY0;
  int y = int((float(p.y) + 0.5) / float(kN)); // the DS row (as pass C's quotient)
  if (!k_row(y)) discard;
  uint v = texelFetch(kBank, p, 0).r;
  uint c = k_bright(((v & 31u) << 1) | (((v >> 5) & 31u) << 9) | (((v >> 10) & 31u) << 17));
  uint r = c & 63u, g = (c >> 8) & 63u, b = (c >> 16) & 63u;
  uvec3 v8 = uvec3((r << 2) | (r >> 4), (g << 2) | (g >> 4), (b << 2) | (b >> 4));
  oColor = vec4(vec3(v8) / 255.0, 1.0);
}
#else
void main() {
  ivec2 p = ivec2(gl_FragCoord.xy); // the N x pixel of the capture
  ivec2 d = ivec2((vec2(p) + 0.5) / float(kN));
  uint mode = (kCnt >> 29) & 3u;
  uint pa = 0u, pb = 0u;
  if (mode != 1u) {
    if ((kCnt & (1u << 24)) != 0u) {
      if (k3dScale != 0) {
        uvec4 t = texelFetch(k3d, k3dScale == kN ? p : p * k3dScale / kN, 0);
        pa = ((t.r >> 1) & 31u) | (((t.g >> 1) & 31u) << 5) | (((t.b >> 1) & 31u) << 10);
        if ((t.a & 31u) != 0u) pa |= 0x8000u;
      }
    } else {
      uvec3 v = uvec3(texelFetch(kSrcA, p, 0).rgb * 255.0 + 0.5); // v6 << 2 | v6 >> 4: v6 >> 1 is v >> 3
      pa = (v.r >> 3) | ((v.g >> 3) << 5) | ((v.b >> 3) << 10) | 0x8000u;
    }
  }
  if (mode != 0u && kHaveB != 0u) {
    int r = (kRoff + d.y) & 255;
    pb = k_row(r) ? texelFetch(kBank, ivec2(p.x, r * kN + p.y - d.y * kN), 0).r : texelFetch(kBank1x, ivec2(d.x, r), 0).r;
  }
  uint o = mode == 0u ? pa : pb;
  if (mode >= 2u) { // vc_cap_blend
    int eva = min(int(kCnt & 31u), 16), evb = min(int((kCnt >> 8) & 31u), 16);
    int aa = int((pa >> 15) & 1u), ab = int((pb >> 15) & 1u);
    o = 0u;
    for (int c = 0; c < 3; c++) {
      int ca = int((pa >> uint(5 * c)) & 31u), cb = int((pb >> uint(5 * c)) & 31u);
      o |= uint(min(31, (ca * aa * eva + cb * ab * evb + 8) >> 4)) << uint(5 * c);
    }
    if ((aa != 0 && eva > 0) || (ab != 0 && evb > 0)) o |= 0x8000u;
  }
  oCap = o;
}
#endif
