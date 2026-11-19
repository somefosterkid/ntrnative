u32 _d2f(void);
void _ddiv(s32, s32, u32, u32);
u64 _dmul(u32, u32, s32, s32);
u64 _dsub(s32, s32, u32, u32);
u64 _f2d(void);
u32 _ffix(void);
void _fflt(s32);
void _fmul(u32, u32);
void _s32_div_f(s32, u32);
extern u32 ov96_0221E5A0;

extern s32 ov96_0221DC10[2];
typedef struct { u32 x, y, z; } OV96_Vec3; /* invented */
extern const OV96_Vec3 ov96_0221C0E8;
void ov96_021F61C8(s32 param_1)
{
  OV96_Vec3 vec;
  u32 cmd3c;
  u32 cmd38;
  u32 cmd34;
  u32 vtx2c[2];
  u32 vtx24[2];
  u32 vtx1c[2];
  u32 vtx14[2];
  u32 color;
  u32 vtx8[2];
  u32 vtx0[2];
  s32 *p;
  s32 i;
  float f;
  vec = ov96_0221C0E8;
  NNS_G3dGeBufferOP_N(0x1b, (u32 *) (&vec), 3);
  cmd3c = 0x310081;
  NNS_G3dGeBufferOP_N(0x29, &cmd3c, 1);
  cmd38 = 0xbf;
  NNS_G3dGeBufferOP_N(0x20, &cmd38, 1);
  NNS_G3dGlbFlushP();
  cmd34 = 3;
  NNS_G3dGeBufferOP_N(0x40, &cmd34, 1);
  vtx2c[1] = (ov96_0221E5A0 = 0);
  ov96_0221DC10[1] = 10;
  ov96_0221DC10[0] = 20;
  vtx2c[0] = 0xe000;
  NNS_G3dGeBufferOP_N(0x23, vtx2c, 2);
  vtx24[0] = 0xe800e000;
  vtx24[1] = 0;
  NNS_G3dGeBufferOP_N(0x23, vtx24, 2);
  p = (s32 *) (param_1 + 4);
  for (i = 1; i <= 128; i++)
  {
    f = (float) (((((double) ((float) i)) - 64.0) * 4.0) / 128.0);
    vtx1c[0] = ((u16) ((s32) (f * 4096.0f))) | (((u32) ((u16) ((s16) p[0x67]))) << 16);
    vtx1c[1] = 0;
    NNS_G3dGeBufferOP_N(0x23, vtx1c, 2);
    vtx14[0] = ((u16) ((s32) (f * 4096.0f))) | 0xe8000000;
    vtx14[1] = 0;
    NNS_G3dGeBufferOP_N(0x23, vtx14, 2);
    color = (u16) ((ov96_0221E5A0 | (ov96_0221DC10[1] << 5)) | (ov96_0221DC10[0] << 10));
    NNS_G3dGeBufferOP_N(0x20, &color, 1);
    ov96_0221E5A0 = ((s32) (ov96_0221E5A0 + 1)) % 31;
    ov96_0221DC10[1] = (ov96_0221DC10[1] + 1) % 31;
    ov96_0221DC10[0] = (ov96_0221DC10[0] + 1) % 31;
    p++;
  }

  vtx8[0] = 0xe000;
  vtx8[1] = 0;
  NNS_G3dGeBufferOP_N(0x23, vtx8, 2);
  vtx0[0] = 0xe800e000;
  vtx0[1] = 0;
  NNS_G3dGeBufferOP_N(0x23, vtx0, 2);
  NNS_G3dGeBufferOP_N(0x41, 0L, 0);
}

