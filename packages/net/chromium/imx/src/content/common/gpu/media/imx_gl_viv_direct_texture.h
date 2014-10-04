#ifndef IMX_GL_VIV_DIRECT_TEXTURE_H
#define IMX_GL_VIV_DIRECT_TEXTURE_H

#include "ui/gl/gl_bindings.h"
#include "ui/gl/gl_context.h"


/* GL_VIV_direct_texture */
#ifndef GL_VIV_direct_texture
#define GL_VIV_YV12                                             0x8FC0
#define GL_VIV_NV12                                             0x8FC1
#define GL_VIV_YUY2                                             0x8FC2
#define GL_VIV_UYVY                                             0x8FC3
#define GL_VIV_NV21                                             0x8FC4
#define GL_VIV_I420                                             0x8FC5
#endif


#ifndef GL_APICALL
#define GL_APICALL  KHRONOS_APICALL
#endif

#ifndef GL_APIENTRY
#define GL_APIENTRY KHRONOS_APIENTRY
#endif

#ifndef GL_APIENTRYP
#define GL_APIENTRYP GL_APIENTRY*
#endif


/* GL_VIV_direct_texture */
#ifndef GL_VIV_direct_texture
#define GL_VIV_direct_texture 1

typedef void (GL_APIENTRYP PFNGLTEXDIRECTVIVPROC)           (GLenum Target, GLsizei Width, GLsizei Height, GLenum Format, GLvoid ** Pixels);
typedef void (GL_APIENTRYP PFNGLTEXDIRECTVIVMAPPROC)        (GLenum Target, GLsizei Width, GLsizei Height, GLenum Format, GLvoid ** Logical, const GLuint * Physical);
typedef void (GL_APIENTRYP PFNGLTEXDIRECTTILEDMAPVIVPROC)   (GLenum Target, GLsizei Width, GLsizei Height, GLenum Format, GLvoid ** Logical, const GLuint * Physical);
typedef void (GL_APIENTRYP PFNGLTEXDIRECTINVALIDATEVIVPROC) (GLenum Target);

#endif

struct GLESVIVDirectTextureProcs
{
	PFNGLTEXDIRECTVIVPROC           TexDirectVIV;
	PFNGLTEXDIRECTVIVMAPPROC        TexDirectVIVMap;
	PFNGLTEXDIRECTTILEDMAPVIVPROC   TexDirectTiledMapVIV;
	PFNGLTEXDIRECTINVALIDATEVIVPROC TexDirectInvalidateVIV;
};


bool init_viv_direct_texture(gfx::GLContext &context, GLESVIVDirectTextureProcs &procs);


#endif
