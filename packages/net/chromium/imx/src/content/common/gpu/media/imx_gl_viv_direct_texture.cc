#include "ui/gl/gl_implementation.h"
#include "imx_gl_viv_direct_texture.h"


bool init_viv_direct_texture(gfx::GLContext &context, GLESVIVDirectTextureProcs &procs)
{
	VLOG(1) << "Initializing Vivante direct texture GLES extension";

	gfx::GLImplementation glimpl = gfx::GetGLImplementation();
	if (glimpl != gfx::kGLImplementationEGLGLES2)
	{
		LOG(INFO) << "Cannot initialize direct textures - GL implementation is "
		          << gfx::GetGLImplementationName(glimpl)
		          << ", expected " <<
		          gfx::GetGLImplementationName(gfx::kGLImplementationEGLGLES2);
		return false;
	}

	// Newer Vivante drivers call the extension GL_VIV_tex_direct instead of GL_VIV_direct_texture,
	// even though it is the same extension
	if (context.HasExtension("GL_VIV_direct_texture"))
		VLOG(1) << "GL_VIV_direct_texture supported";
	else if (context.HasExtension("GL_VIV_tex_direct"))
		VLOG(1) << "GL_VIV_tex_direct supported";
	else
	{
		VLOG(1) << "Neither GL_VIV_direct_texture nor GL_VIV_tex_direct supported";
		return false;
	}

	procs.TexDirectVIV           = reinterpret_cast < PFNGLTEXDIRECTVIVPROC >           (gfx::GetGLProcAddress("glTexDirectVIV"));
	procs.TexDirectVIVMap        = reinterpret_cast < PFNGLTEXDIRECTVIVMAPPROC >        (gfx::GetGLProcAddress("glTexDirectVIVMap"));
	procs.TexDirectTiledMapVIV   = reinterpret_cast < PFNGLTEXDIRECTTILEDMAPVIVPROC >   (gfx::GetGLProcAddress("glTexDirectTiledMapVIV"));
	procs.TexDirectInvalidateVIV = reinterpret_cast < PFNGLTEXDIRECTINVALIDATEVIVPROC > (gfx::GetGLProcAddress("glTexDirectInvalidateVIV"));

	return true;
}

