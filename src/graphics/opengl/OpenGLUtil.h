/*
 * Copyright 2015 Arx Libertatis Team (see the AUTHORS file)
 *
 * This file is part of Arx Libertatis.
 *
 * Arx Libertatis is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Arx Libertatis is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Arx Libertatis.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ARX_GRAPHICS_OPENGL_OPENGLUTIL_H
#define ARX_GRAPHICS_OPENGL_OPENGLUTIL_H

#include "platform/Platform.h"
#include "Configure.h"

#if ARX_PLATFORM == ARX_PLATFORM_WIN32
// Make sure we get the APIENTRY define from windows.h first to avoid a re-definition warning
#include <windows.h>
#endif

#if ARX_HAVE_EPOXY

#include <epoxy/gl.h>

#define ARX_HAVE_GL_VER(x, y) (epoxy_gl_version() >= x##y)
#define ARX_HAVE_GL_EXT(name) epoxy_has_gl_extension("GL_" #name)
#define ARX_HAVE_GLES_VER(x, y) ARX_HAVE_GL_VER(x, y)
#define ARX_HAVE_GLES_EXT(name) ARX_HAVE_GL_EXT(name)

#elif ARX_HAVE_GLEW

#include <GL/glew.h>

#define ARX_HAVE_GL_VER(x, y) (glewIsSupported("GL_VERSION_" #x "_" #y) != 0)
#define ARX_HAVE_GL_EXT(name) (glewIsSupported("GL_" #name) != 0)
#define ARX_HAVE_GLES_VER(x, y) (false)
#define ARX_HAVE_GLES_EXT(name) (false)

#elif defined(__MORPHOS__) || defined(__amigaos4__)

#include <GL/gl.h>
#ifdef __amigaos4__
#include <GL/glext.h>
#endif
#ifdef __MORPHOS__
#define glMultiTexCoord2f glMultiTexCoord2fARB
#define glActiveTexture glActiveTextureARB
#define glClientActiveTexture glClientActiveTextureARB
#define glDrawRangeElementsBaseVertex(mode, start, end, count, type, indices, basevertex)
#define glDrawElementsBaseVertex(mode, count, type, indices, basevertex)
#define GL_MAX_TEXTURE_UNITS GL_MAX_TEXTURE_UNITS_ARB
#define GL_GENERATE_MIPMAP                0x8191
#define GL_FOG_COORDINATE_ARRAY           0x8457
#define GL_SAMPLE_BUFFERS                 0x80A8
#define GL_SAMPLES                        0x80A9
#define GL_FOG_DISTANCE_MODE_NV           0x855A
#define GL_FOG_COORDINATE_SOURCE          0x8450
#define GL_FOG_COORDINATE                 0x8451
#define GL_FRAGMENT_DEPTH                 0x8452
#define GL_SAMPLE_ALPHA_TO_COVERAGE       0x809E
#undef glGetString
#define glGetString(name) ((const GLubyte*)GLGetString(__tglContext, name))
/*#undef glDrawElements
#define glDrawElements(mode, count, type, indices) GLDrawElements(__tglContext, mode, count, type, (GLvoid*)indices)
#undef glDrawRangeElements
#define glDrawRangeElements(mode, start, end, count, type, indices) GLDrawRangeElements(__tglContext, mode, start, end, count, type, (GLvoid*)indices)*/
#endif

#define glFogCoordPointer(x,y,z)

#define ARX_HAVE_GL_VER(x, y) ((x) == 1)
#define ARX_HAVE_GL_EXT(name) (false)
#define ARX_HAVE_GLES_VER(x, y) (false)
#define ARX_HAVE_GLES_EXT(name) (false)

#else
#error "OpenGL renderer not supported: need ARX_HAVE_EPOXY or ARX_HAVE_GLEW"
#endif

#endif // ARX_GRAPHICS_OPENGL_OPENGLUTIL_H
