// port/gpu SMAA, pass 2: the blending weights (docs/gpu.md "Antialiasing: SMAA"). Ported from the SMAA reference
// implementation (SMAA.hlsl, github.com/iryoku/smaa: SMAABlendingWeightCalculationVS/PS, SMAASearch*, SMAAArea; Jimenez,
// Echevarria, Sousa, Gutierrez, "SMAA: Enhanced Subpixel Morphological Antialiasing", 2012) under its MIT licence:
//   Copyright (C) 2013 Jorge Jimenez, Jose I. Echevarria, Belen Masia, Fernando Navarro, Diego Gutierrez
//   (the full notice is in port/gpu/src/g08_smaa.c).
// SMAA_PRESET_MEDIUM: 8 search steps, no diagonal and no corner detection; SMAA 1x (subsample indices 0). The reference's
// vertex shader offsets are computed here (the passes share g00_full.vert). Texture row 0 is DS row 0 (the top), as the
// reference's v = 0 is, so the area and search textures go in as their bytes are.
uniform sampler2D aEdges;  // pass 1's RG8, linear, clamped
uniform sampler2D aArea;   // the area texture, RG8 160 x 560, linear
uniform sampler2D aSearch; // the search texture, R8 64 x 16, linear
// aMetrics: g00_head.glsl's block
layout(location = 0) out vec4 oWeights;

#define SMAA_MAX_SEARCH_STEPS 8
#define SMAA_AREATEX_MAX_DISTANCE 16
#define SMAA_AREATEX_PIXEL_SIZE (1.0 / vec2(160.0, 560.0))
#define SMAA_AREATEX_SUBTEX_SIZE (1.0 / 7.0)
#define SMAA_SEARCHTEX_SIZE vec2(66.0, 33.0)
#define SMAA_SEARCHTEX_PACKED_SIZE vec2(64.0, 16.0)

float a_search_length(vec2 e, float offset) {
  // the texture is flipped vertically, with left and right cases taking half of the space horizontally
  vec2 scale = SMAA_SEARCHTEX_SIZE * vec2(0.5, -1.0);
  vec2 bias = SMAA_SEARCHTEX_SIZE * vec2(offset, 1.0);
  scale += vec2(-1.0, 1.0); // to texel centres
  bias += vec2(0.5, -0.5);
  scale *= 1.0 / SMAA_SEARCHTEX_PACKED_SIZE; // the texture is cropped
  bias *= 1.0 / SMAA_SEARCHTEX_PACKED_SIZE;
  return textureLod(aSearch, scale * e + bias, 0.0).r;
}

// the texcoords are offset by (-0.25, -0.125) so one bilinear fetch reads two edges (the reference's @PSEUDO_GATHER4)
float a_search_x_left(vec2 texcoord, float end) {
  vec2 e = vec2(0.0, 1.0);
  while (texcoord.x > end && e.g > 0.8281 && e.r == 0.0) { // some edge not activated, no crossing edge
    e = textureLod(aEdges, texcoord, 0.0).rg;
    texcoord = -vec2(2.0, 0.0) * aMetrics.xy + texcoord;
  }
  float offset = -(255.0 / 127.0) * a_search_length(e, 0.0) + 3.25;
  return aMetrics.x * offset + texcoord.x;
}

float a_search_x_right(vec2 texcoord, float end) {
  vec2 e = vec2(0.0, 1.0);
  while (texcoord.x < end && e.g > 0.8281 && e.r == 0.0) {
    e = textureLod(aEdges, texcoord, 0.0).rg;
    texcoord = vec2(2.0, 0.0) * aMetrics.xy + texcoord;
  }
  float offset = -(255.0 / 127.0) * a_search_length(e, 0.5) + 3.25;
  return -aMetrics.x * offset + texcoord.x;
}

float a_search_y_up(vec2 texcoord, float end) {
  vec2 e = vec2(1.0, 0.0);
  while (texcoord.y > end && e.r > 0.8281 && e.g == 0.0) {
    e = textureLod(aEdges, texcoord, 0.0).rg;
    texcoord = -vec2(0.0, 2.0) * aMetrics.xy + texcoord;
  }
  float offset = -(255.0 / 127.0) * a_search_length(e.gr, 0.0) + 3.25;
  return aMetrics.y * offset + texcoord.y;
}

float a_search_y_down(vec2 texcoord, float end) {
  vec2 e = vec2(1.0, 0.0);
  while (texcoord.y < end && e.r > 0.8281 && e.g == 0.0) {
    e = textureLod(aEdges, texcoord, 0.0).rg;
    texcoord = vec2(0.0, 2.0) * aMetrics.xy + texcoord;
  }
  float offset = -(255.0 / 127.0) * a_search_length(e.gr, 0.5) + 3.25;
  return -aMetrics.y * offset + texcoord.y;
}

vec2 a_area(vec2 dist, float e1, float e2, float offset) {
  // rounding prevents precision errors of bilinear filtering
  vec2 texcoord = vec2(SMAA_AREATEX_MAX_DISTANCE) * round(4.0 * vec2(e1, e2)) + dist;
  texcoord = SMAA_AREATEX_PIXEL_SIZE * texcoord + 0.5 * SMAA_AREATEX_PIXEL_SIZE; // to texel space
  texcoord.y = SMAA_AREATEX_SUBTEX_SIZE * offset + texcoord.y; // the subpixel offset's part
  return textureLod(aArea, texcoord, 0.0).rg;
}

void main() {
  vec2 texcoord = gl_FragCoord.xy * aMetrics.xy;
  vec2 pixcoord = gl_FragCoord.xy;
  vec4 offset0 = aMetrics.xyxy * vec4(-0.25, -0.125, 1.25, -0.125) + texcoord.xyxy;
  vec4 offset1 = aMetrics.xyxy * vec4(-0.125, -0.25, -0.125, 1.25) + texcoord.xyxy;
  vec4 offset2 = aMetrics.xxyy * (vec4(-2.0, 2.0, -2.0, 2.0) * float(SMAA_MAX_SEARCH_STEPS)) +
                 vec4(offset0.xz, offset1.yw);
  vec4 weights = vec4(0.0);
  vec2 e = texelFetch(aEdges, ivec2(gl_FragCoord.xy), 0).rg;
  if (e.g > 0.0) { // an edge at the north
    vec2 d;
    vec3 coords;
    coords.x = a_search_x_left(offset0.xy, offset2.x);
    coords.y = offset1.y; // texcoord.y - 0.25 a pixel (the reference's @CROSSING_OFFSET)
    d.x = coords.x;
    float e1 = textureLod(aEdges, coords.xy, 0.0).r;
    coords.z = a_search_x_right(offset0.zw, offset2.y);
    d.y = coords.z;
    d = abs(round(aMetrics.zz * d - pixcoord.xx));
    vec2 sqrt_d = sqrt(d);
    float e2 = textureLodOffset(aEdges, coords.zy, 0.0, ivec2(1, 0)).r;
    weights.rg = a_area(sqrt_d, e1, e2, 0.0);
  }
  if (e.r > 0.0) { // an edge at the west
    vec2 d;
    vec3 coords;
    coords.y = a_search_y_up(offset1.xy, offset2.z);
    coords.x = offset0.x; // texcoord.x - 0.25 a pixel
    d.x = coords.y;
    float e1 = textureLod(aEdges, coords.xy, 0.0).g;
    coords.z = a_search_y_down(offset1.zw, offset2.w);
    d.y = coords.z;
    d = abs(round(aMetrics.ww * d - pixcoord.yy));
    vec2 sqrt_d = sqrt(d);
    float e2 = textureLodOffset(aEdges, coords.xz, 0.0, ivec2(0, 1)).g;
    weights.ba = a_area(sqrt_d, e1, e2, 0.0);
  }
  oWeights = weights;
}
