// port/gpu SMAA's neighbourhood blending into a layer of its own, for the readbacks only (gpu_read_3d: gpu_render --hi, the
// tools); the frame does it in pass C (shaders/g08_smaa.glsl, docs/gpu.md "Antialiasing: SMAA").
uniform highp usampler2D aLayer; // pass P's 3D layer
uniform sampler2D aWeights;      // the weights pass's RGBA8
layout(location = 0) out uvec4 oOut;

void main() { oOut = smaa_blend(aLayer, aWeights, ivec2(gl_FragCoord.xy)); }
