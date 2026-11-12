/* port/gpu: the GL function table, shader programs, data textures (docs/gpu.md). OWNED BY THE SKELETON (+ G1).
 *
 * The functions are loaded by name (gpu_gl_load) so that one build serves EGL/GLES (the tools, iOS through ANGLE)
 * and libntr's desktop GL context (the port): call them as GL(Name)(...). Only GLES 3.0 functions, which desktop GL
 * 3.3 core has too. The types and enums come from the Khronos GLES 3 header (prototypes off). */
#ifndef GPU_GL_H
#define GPU_GL_H

#define GL_GLES_PROTOTYPES 0
#include <GLES3/gl3.h>

#include "gpu.h"
#include "gpu_uni.h"

/* gpu_set_pass_hook's function (NULL: none) and the brackets the passes put around their GL calls */
extern GpuPassHook gpu_pass_hook;
#define GPU_PASS(pass, begin)                                                                                         \
  do {                                                                                                                \
    if (gpu_pass_hook != NULL) gpu_pass_hook((pass), (begin));                                                        \
  } while (0)

#define GPU_GL_LIST(X)                                                                                                \
  X(PFNGLACTIVETEXTUREPROC, ActiveTexture)                                                                           \
  X(PFNGLATTACHSHADERPROC, AttachShader)                                                                             \
  X(PFNGLBINDFRAMEBUFFERPROC, BindFramebuffer)                                                                       \
  X(PFNGLBINDTEXTUREPROC, BindTexture)                                                                               \
  X(PFNGLBINDVERTEXARRAYPROC, BindVertexArray)                                                                       \
  X(PFNGLBEGINTRANSFORMFEEDBACKPROC, BeginTransformFeedback)                                                         \
  X(PFNGLCHECKFRAMEBUFFERSTATUSPROC, CheckFramebufferStatus)                                                         \
  X(PFNGLCOMPILESHADERPROC, CompileShader)                                                                           \
  X(PFNGLCREATEPROGRAMPROC, CreateProgram)                                                                           \
  X(PFNGLCREATESHADERPROC, CreateShader)                                                                             \
  X(PFNGLDELETEFRAMEBUFFERSPROC, DeleteFramebuffers)                                                                 \
  X(PFNGLDELETEPROGRAMPROC, DeleteProgram)                                                                           \
  X(PFNGLDELETESHADERPROC, DeleteShader)                                                                             \
  X(PFNGLDELETETEXTURESPROC, DeleteTextures)                                                                         \
  X(PFNGLDELETEVERTEXARRAYSPROC, DeleteVertexArrays)                                                                 \
  X(PFNGLDISABLEPROC, Disable)                                                                                       \
  X(PFNGLDRAWARRAYSPROC, DrawArrays)                                                                                 \
  X(PFNGLDRAWBUFFERSPROC, DrawBuffers)                                                                               \
  X(PFNGLENDTRANSFORMFEEDBACKPROC, EndTransformFeedback)                                                             \
  X(PFNGLENABLEPROC, Enable)                                                                                         \
  X(PFNGLFINISHPROC, Finish)                                                                                         \
  X(PFNGLFRAMEBUFFERTEXTURE2DPROC, FramebufferTexture2D)                                                             \
  X(PFNGLGENFRAMEBUFFERSPROC, GenFramebuffers)                                                                       \
  X(PFNGLGENTEXTURESPROC, GenTextures)                                                                               \
  X(PFNGLGENVERTEXARRAYSPROC, GenVertexArrays)                                                                       \
  X(PFNGLGETERRORPROC, GetError)                                                                                     \
  X(PFNGLGETINTEGERVPROC, GetIntegerv)                                                                               \
  X(PFNGLGETPROGRAMINFOLOGPROC, GetProgramInfoLog)                                                                   \
  X(PFNGLGETPROGRAMIVPROC, GetProgramiv)                                                                             \
  X(PFNGLGETSHADERINFOLOGPROC, GetShaderInfoLog)                                                                     \
  X(PFNGLGETSHADERIVPROC, GetShaderiv)                                                                               \
  X(PFNGLGETSTRINGPROC, GetString)                                                                                   \
  X(PFNGLGETUNIFORMLOCATIONPROC, GetUniformLocation)                                                                 \
  X(PFNGLLINKPROGRAMPROC, LinkProgram)                                                                               \
  X(PFNGLPIXELSTOREIPROC, PixelStorei)                                                                               \
  X(PFNGLREADBUFFERPROC, ReadBuffer)                                                                                 \
  X(PFNGLREADPIXELSPROC, ReadPixels)                                                                                 \
  X(PFNGLSHADERSOURCEPROC, ShaderSource)                                                                             \
  X(PFNGLTEXIMAGE2DPROC, TexImage2D)                                                                                 \
  X(PFNGLTEXPARAMETERIPROC, TexParameteri)                                                                           \
  X(PFNGLUNIFORM1IPROC, Uniform1i)                                                                                   \
  X(PFNGLUNIFORM1UIPROC, Uniform1ui)                                                                                 \
  X(PFNGLUNIFORM1UIVPROC, Uniform1uiv)                                                                               \
  X(PFNGLUSEPROGRAMPROC, UseProgram)                                                                                 \
  X(PFNGLBINDBUFFERPROC, BindBuffer)                                                                               \
  X(PFNGLBINDBUFFERBASEPROC, BindBufferBase)                                                                       \
  X(PFNGLBINDBUFFERRANGEPROC, BindBufferRange)                                                                     \
  X(PFNGLGENQUERIESPROC, GenQueries)                                                                               \
  X(PFNGLDELETEQUERIESPROC, DeleteQueries)                                                                         \
  X(PFNGLBEGINQUERYPROC, BeginQuery)                                                                               \
  X(PFNGLENDQUERYPROC, EndQuery)                                                                                   \
  X(PFNGLGETQUERYOBJECTUIVPROC, GetQueryObjectuiv)                                                                 \
  X(PFNGLGETBUFFERPARAMETERIVPROC, GetBufferParameteriv)                                                           \
  X(PFNGLBLITFRAMEBUFFERPROC, BlitFramebuffer)                                                                     \
  X(PFNGLBUFFERDATAPROC, BufferData)                                                                               \
  X(PFNGLBUFFERSUBDATAPROC, BufferSubData)                                                                         \
  X(PFNGLMAPBUFFERRANGEPROC, MapBufferRange)                                                                       \
  X(PFNGLUNMAPBUFFERPROC, UnmapBuffer)                                                                             \
  X(PFNGLCLEARPROC, Clear)                                                                                         \
  X(PFNGLCLEARBUFFERFVPROC, ClearBufferfv)                                                                         \
  X(PFNGLCLEARBUFFERIVPROC, ClearBufferiv)                                                                         \
  X(PFNGLCLEARBUFFERUIVPROC, ClearBufferuiv)                                                                       \
  X(PFNGLCLEARCOLORPROC, ClearColor)                                                                               \
  X(PFNGLCOLORMASKPROC, ColorMask)                                                                                 \
  X(PFNGLDELETEBUFFERSPROC, DeleteBuffers)                                                                         \
  X(PFNGLDEPTHMASKPROC, DepthMask)                                                                                 \
  X(PFNGLFLUSHPROC, Flush)                                                                                         \
  X(PFNGLFRAMEBUFFERTEXTURELAYERPROC, FramebufferTextureLayer)                                                     \
  X(PFNGLGENBUFFERSPROC, GenBuffers)                                                                               \
  X(PFNGLGETATTRIBLOCATIONPROC, GetAttribLocation)                                                                 \
  X(PFNGLGETBOOLEANVPROC, GetBooleanv)                                                                             \
  X(PFNGLGETFLOATVPROC, GetFloatv)                                                                                 \
  X(PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC, GetFramebufferAttachmentParameteriv)                             \
  X(PFNGLGETSTRINGIPROC, GetStringi)                                                                               \
  X(PFNGLGETUNIFORMBLOCKINDEXPROC, GetUniformBlockIndex)                                                           \
  X(PFNGLISENABLEDPROC, IsEnabled)                                                                                 \
  X(PFNGLSCISSORPROC, Scissor)                                                                                     \
  X(PFNGLSTENCILFUNCPROC, StencilFunc)                                                                             \
  X(PFNGLSTENCILOPPROC, StencilOp)                                                                                 \
  X(PFNGLTEXIMAGE3DPROC, TexImage3D)                                                                               \
  X(PFNGLTEXSTORAGE2DPROC, TexStorage2D)                                                                           \
  X(PFNGLTEXSUBIMAGE2DPROC, TexSubImage2D)                                                                         \
  X(PFNGLTEXSUBIMAGE3DPROC, TexSubImage3D)                                                                         \
  X(PFNGLUNIFORM1FPROC, Uniform1f)                                                                                 \
  X(PFNGLUNIFORM1FVPROC, Uniform1fv)                                                                               \
  X(PFNGLUNIFORM1IVPROC, Uniform1iv)                                                                               \
  X(PFNGLUNIFORM2FPROC, Uniform2f)                                                                                 \
  X(PFNGLUNIFORM2IPROC, Uniform2i)                                                                                 \
  X(PFNGLUNIFORM2IVPROC, Uniform2iv)                                                                               \
  X(PFNGLUNIFORM2UIPROC, Uniform2ui)                                                                               \
  X(PFNGLUNIFORM3IPROC, Uniform3i)                                                                                 \
  X(PFNGLUNIFORM4FPROC, Uniform4f)                                                                                 \
  X(PFNGLUNIFORM4FVPROC, Uniform4fv)                                                                               \
  X(PFNGLUNIFORM4IPROC, Uniform4i)                                                                                 \
  X(PFNGLUNIFORM4IVPROC, Uniform4iv)                                                                               \
  X(PFNGLUNIFORM4UIPROC, Uniform4ui)                                                                               \
  X(PFNGLUNIFORM4UIVPROC, Uniform4uiv)                                                                             \
  X(PFNGLUNIFORMBLOCKBINDINGPROC, UniformBlockBinding)                                                             \
  X(PFNGLUNIFORMMATRIX4FVPROC, UniformMatrix4fv)                                                                   \
  X(PFNGLTRANSFORMFEEDBACKVARYINGSPROC, TransformFeedbackVaryings)                                                 \
  X(PFNGLVIEWPORTPROC, Viewport)                                                                                     \
  X(PFNGLENABLEVERTEXATTRIBARRAYPROC, EnableVertexAttribArray)                                                     \
  X(PFNGLVERTEXATTRIBPOINTERPROC, VertexAttribPointer)

typedef struct GpuGL {
#define GPU_GL_MEMBER(type, name) type name;
  GPU_GL_LIST(GPU_GL_MEMBER)
#undef GPU_GL_MEMBER
  int api; /* GPU_API_ES / GPU_API_GL */
  /* optional (NULL when absent): GLES 3.0 core, desktop GL 4.3 or ARB_invalidate_subdata (macOS's 4.1 has none) */
  PFNGLINVALIDATEFRAMEBUFFERPROC InvalidateFramebuffer;
  /* desktop GL 4.2-4.3 (the fast 3D renderer's image modes, g10_fast_gl.c, which checks the version before using them) */
  void (*BindImageTexture)(GLuint unit, GLuint tex, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum fmt);
  void (*MemoryBarrier)(GLbitfield barriers);
  void (*FramebufferParameteri)(GLenum target, GLenum pname, GLint param);
} GpuGL;
extern GpuGL gpu_gl;
#define GL(name) (gpu_gl.name)

/* fills gpu_gl; 0 on success, else the names of the missing functions in err. The API actually in use is read from
 * the context's GL_VERSION ("OpenGL ES 3.x ..." = ES, else desktop GL), so `api` is only the fallback if that string
 * is unreadable: a context made by libntr or by GPU_API=gl gets the right GLSL header either way. A current context
 * is needed (the loaders of some platforms return NULL for the GL 1.1 functions without one). */
int gpu_gl_load(GpuGetProc getproc, int api, char *err, size_t errlen);
/* the bound framebuffer's colour attachments 0..n-1 are about to be overwritten whole: their old contents are not needed
 * (a tile-based GPU then does not load them into tile memory); nothing where glInvalidateFramebuffer is absent */
void gpu_gl_invalidate(int n);

/* GL errors as text */
const char *gpu_gl_error_name(GLenum e);
/* drains glGetError: 0 when clean, else the number of errors, with "where: GL_INVALID_VALUE, ..." in err */
int gpu_gl_errors(const char *where, char *err, size_t errlen);

/* the embedded shader sources (gpu_shaders.c, generated from shaders/; BWPORT_GPU_SHADERS=DIR reads DIR instead) */
typedef struct GpuShaderSrc {
  const char *name, *text;
} GpuShaderSrc;
extern const GpuShaderSrc gpu_shader_src[];

/* a program from the vertex main vs and the fragment main fs (file names in shaders/): the fragment shader is the
 * API's header + g00_head.glsl + every other *.glsl by name + fs. 0 on failure (the compiler's log in err) */
GLuint gpu_gl_program(const char *vs, const char *fs, char *err, size_t errlen); /* made once per pair (a cache) */
/* a transform-feedback program: vertex shader vs (g00_head.glsl before it) writing the nvar varyings interleaved, rasterizer
 * discard expected (an empty fragment shader; fs only names the cache entry) */
GLuint gpu_gl_program_tf(const char *vs, const char *fs, const char *const *varyings, int nvar, char *err, size_t errlen);
/* the two shaders' text exactly as gpu_gl_program (tf = 0) or gpu_gl_program_tf (tf = 1) would compile them for gpu_gl.api,
 * without a context: malloc'd, the caller frees them; -1 when a file is missing (tools/gpu_glsl.c, the strict-ES gate) */
int gpu_gl_program_text(const char *vs, const char *fs, int tf, char **vtext, char **ftext, char *err, size_t errlen);
/* a program from gpu_gl_program is no longer used: deleted unless the cache keeps it */
void gpu_gl_program_done(GLuint p);
/* the context the programs belong to is going away (gpu_ctx_destroy) */
void gpu_gl_programs_forget(void);
/* glGetUniformLocation(prog, name) looked up once: a table by program and
 * the name's address, so name must be a string literal (or live as long); emptied when a program is made or the functions
 * loaded (a deleted program's id comes back) */
GLint gpu_gl_uniform(GLuint prog, const char *name);

/* the next draw's uniforms: u into the next buffer of a ring (orphaned and filled: never one a draw in flight reads), bound
 * at GL_UNIFORM_BUFFER binding 0, where every program's block GpuU is (shaders/g00_head.glsl). _release deletes the ring
 * (gpu_destroy; the next call makes it again). */
void gpu_gl_uniforms(const GpuU *u);
void gpu_gl_uniforms_release(void);

/* a data texture 1024 texels wide (docs/gpu.md, "The data"): n texels of texel_bytes each from data, uploaded into a
 * persistent texture (*tex made on the first call, grown when n grows; texels past n keep older data); ifmt/fmt/type as glTexImage2D wants them. NEAREST, no mipmaps. */
void gpu_gl_data_tex(GLuint *tex, GLenum ifmt, GLenum fmt, GLenum type, int texel_bytes, const void *data, int n);
/* the same, keeping a copy of the texture's content: only the rows that differ from the last call's are uploaded (VRAM
 * pages, palettes: mostly unchanged from frame to frame). n is the same on each call. */
void gpu_gl_data_tex_diff(GLuint *tex, GLenum ifmt, GLenum fmt, GLenum type, int texel_bytes, const void *data, int n);
/* the data a frame writes (the polygon, band and list tables, pass S's setup, a texture build) goes into one of GPU_RING
 * textures in turn, and the frame's passes read the one it wrote: on Apple's GL a TexSubImage2D into a texture an earlier
 * frame (or its in-between) still reads waits in the driver until the GPU is done with it (GLDContextRec::finishResource:
 * the GL thread stalled whenever the GPU was behind); a frame never writes a texture one of the two before it reads. */
enum { GPU_RING = 3 };
/* the texture is deleted: drop its record (the row count and the _diff copy) */
void gpu_gl_data_forget(GLuint tex);
/* uploads through a pixel-unpack buffer (docs/gpu-cost.md P3; Apple's GL and GLES convert client memory to the texture's
 * layout on the CPU inside glTexSubImage2D, a buffer's on the GPU). _map binds the next buffer of a small ring to
 * GL_PIXEL_UNPACK_BUFFER and maps `bytes` of it for writing (MAP_INVALIDATE_BUFFER: the GPU's reads of its last use never
 * stall the map); NULL when that failed, nothing bound (upload from client memory). _unmap: 0, the buffer still bound,
 * its offsets are TexSubImage2D's data; -1 when the content was lost (nothing bound, upload from client memory).
 * _done unbinds; _release deletes the ring (gpu_destroy; a later _map makes it again). Only pass C's 1.2 MB stage and the
 * CPU's span records (g02_setup.c, when pass S is off) go through it: the small per-present tables and the texture rows cost
 * more GPU time that way than they saved (P3). */
unsigned char *gpu_gl_unpack_map(size_t bytes);
int gpu_gl_unpack_unmap(void);
void gpu_gl_unpack_done(void);
void gpu_gl_unpack_release(void);
enum { GPU_DATA_W = 1024 };

#endif
