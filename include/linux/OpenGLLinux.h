#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <GL/glx.h>
#include <GL/glext.h>

//vbo
extern PFNGLGENBUFFERSARBPROC glGenBuffers;
extern PFNGLBINDBUFFERARBPROC glBindBuffer;
extern PFNGLBUFFERSUBDATAARBPROC glBufferSubData;
extern PFNGLBUFFERDATAARBPROC glBufferData;
extern PFNGLDELETEBUFFERSARBPROC glDeleteBuffers;
//edit vbo
extern PFNGLMAPBUFFERARBPROC glMapBuffer;
extern PFNGLUNMAPBUFFERARBPROC glUnmapBuffer;
//vba
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC  glBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
#if !defined(__linux__)
    //multi texture
    extern PFNGLACTIVETEXTUREPROC glActiveTexture;
    extern PFNGLCLIENTACTIVETEXTUREPROC glClientActiveTexture;
    extern PFNGLDRAWRANGEELEMENTSEXTPROC glDrawRangeElements;
    //blend effect
    extern PFNGLBLENDEQUATIONPROC		glBlendEquation;
    //vsync
    typedef BOOL (WINAPI * PFNWGLSWAPINTERVALEXTPROC)(int interval);
    extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
#endif
//FBO
extern PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT ;
extern PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffersEXT;
extern PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT ;
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT ;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT ;
//shader
extern PFNGLENABLEVERTEXATTRIBARRAYPROC	    glEnableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC	        glVertexAttribPointer;
extern PFNGLCREATESHADERPROC                glCreateShader;
extern PFNGLDELETESHADERPROC				glDeleteShader;
extern PFNGLSHADERSOURCEPROC                glShaderSource;
extern PFNGLCOMPILESHADERPROC               glCompileShader;
extern PFNGLCREATEPROGRAMPROC               glCreateProgram;
extern PFNGLDELETEPROGRAMPROC               glDeleteProgram;
extern PFNGLATTACHSHADERPROC                glAttachShader;
extern PFNGLDETACHSHADERPROC				glDetachShader;
extern PFNGLLINKPROGRAMPROC                 glLinkProgram;
extern PFNGLUSEPROGRAMPROC                  glUseProgram;
extern PFNGLGETUNIFORMLOCATIONPROC          glGetUniformLocation;
extern PFNGLBINDATTRIBLOCATIONPROC     		glBindAttribLocation;
extern PFNGLUNIFORM1FPROC                   glUniform1f;
extern PFNGLUNIFORM2FPROC                   glUniform2f;
extern PFNGLUNIFORM3FPROC                   glUniform3f;
extern PFNGLUNIFORM4FPROC                   glUniform4f;
extern PFNGLUNIFORM1IPROC                   glUniform1i;
extern PFNGLUNIFORM4UIPROC					glUniform4ui;
extern PFNGLUNIFORM4UIVPROC				    glUniform4uiv;
extern PFNGLUNIFORM1IVPROC                  glUniform1iv;
extern PFNGLUNIFORM2IVPROC                  glUniform2iv;
extern PFNGLUNIFORM3IVPROC                  glUniform3iv;
extern PFNGLUNIFORM4IVPROC				    glUniform4iv;
extern PFNGLUNIFORM1FVPROC                  glUniform1fv;
extern PFNGLUNIFORM2FVPROC                  glUniform2fv;
extern PFNGLUNIFORM3FVPROC                  glUniform3fv;
extern PFNGLUNIFORM4FVPROC				    glUniform4fv;
extern PFNGLUNIFORMMATRIX4FVPROC		    glUniformMatrix4fv;
//Info Log
extern PFNGLGETSHADERINFOLOGPROC     glGetShaderInfoLog;
extern PFNGLGETSHADERIVPROC			 glGetShaderiv;
extern PFNGLGETPROGRAMIVPROC		 glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC	 glGetProgramInfoLog;

namespace Sphere {
	//init openGL2
	extern void initOpenGL2();
};

#endif
