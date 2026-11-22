// port/gpu 2D upscaling: a style's analysis at 1x (g11_up.glsl; src/g07_compose.c draws it as "g11_up.frag#1+UP_<STYLE>",
// 256 x 384 into the info texture, after the plan). OWNED BY THE 2D FILTER.
#ifdef UP
layout(location = 0) out uvec4 oInfo;
void main() { oInfo = up_analyse(ivec2(gl_FragCoord.xy)); }
#endif
