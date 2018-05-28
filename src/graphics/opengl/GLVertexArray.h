/*
 * Copyright 2011-2012 Arx Libertatis Team (see the AUTHORS file)
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

#ifndef ARX_GRAPHICS_OPENGL_GLVERTEXARRAY_H
#define ARX_GRAPHICS_OPENGL_GLVERTEXARRAY_H

#include <algorithm>
#include <cstring>
#include <vector>
#include <limits>

#include "graphics/VertexBuffer.h"
#include "graphics/Vertex.h"
#include "graphics/Math.h"
#include "graphics/opengl/OpenGLRenderer.h"
#include "graphics/opengl/OpenGLUtil.h"

#include "io/log/Logger.h"

template <class Vertex>
static void setVertexArray(OpenGLRenderer * renderer, const Vertex * vertex, const void * ref);

// cached vertex array definitions
enum GLArrayClientState {
	GL_NoArray,
	GL_TexturedVertex,
	GL_SMY_VERTEX,
	GL_SMY_VERTEX3
};

void setVertexArrayTexCoord(int index, const void * coord, size_t stride);
bool switchVertexArray(GLArrayClientState type, const void * ref, int texcount);
void clearVertexArray(const void * ref);

template <>
inline void setVertexArray(OpenGLRenderer * renderer, const TexturedVertex * vertices, const void * ref) {
	
	ARX_UNUSED(vertices);

	if(!switchVertexArray(GL_TexturedVertex, ref, 1)) {
		return;
	}
	
	glVertexPointer(4, GL_FLOAT, sizeof(*vertices), &vertices->p);
	
	if(renderer->hasVertexFogCoordinate()) {
		// Use clip.w == view.z as the fog depth to match other vertex types
		// TODO remove GL_FOG_COORDINATE_* uses once vertices are provided in view-space coordinates
		glEnableClientState(GL_FOG_COORDINATE_ARRAY);
		glFogCoordPointer(GL_FLOAT, sizeof(*vertices), &vertices->w);
	}
	
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(*vertices), &vertices->color);
	
	setVertexArrayTexCoord(0, &vertices->uv, sizeof(*vertices));
}

template <>
inline void setVertexArray(OpenGLRenderer * renderer, const SMY_VERTEX * vertices, const void * ref) {
	
	if(!switchVertexArray(GL_SMY_VERTEX, ref, 1)) {
		return;
	}
	
	glVertexPointer(3, GL_FLOAT, sizeof(*vertices), &vertices->p.x);
	
	if(renderer->hasVertexFogCoordinate()) {
		glDisableClientState(GL_FOG_COORDINATE_ARRAY);
	}
	
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(*vertices), &vertices->color);
	
	setVertexArrayTexCoord(0, &vertices->uv, sizeof(*vertices));
}

template <>
inline void setVertexArray(OpenGLRenderer * renderer, const SMY_VERTEX3 * vertices, const void * ref) {
	
	if(!switchVertexArray(GL_SMY_VERTEX3, ref, 3)) {
		return;
	}
	
	glVertexPointer(3, GL_FLOAT, sizeof(*vertices), &vertices->p.x);
	
	if(renderer->hasVertexFogCoordinate()) {
		glDisableClientState(GL_FOG_COORDINATE_ARRAY);
	};
	
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(*vertices), &vertices->color);
	
	setVertexArrayTexCoord(0, &vertices->uv[0], sizeof(*vertices));
	setVertexArrayTexCoord(1, &vertices->uv[1], sizeof(*vertices));
	setVertexArrayTexCoord(2, &vertices->uv[2], sizeof(*vertices));
}

extern const GLenum arxToGlPrimitiveType[];

extern std::vector<GLushort> glShortIndexBuffer;
extern std::vector<GLuint> glIntIndexBuffer;

template <class Vertex>
class GLVertexArray : public VertexBuffer<Vertex> {
	
	typedef VertexBuffer<Vertex> Base;
	
public:
	
	using Base::capacity;
	
	GLVertexArray(OpenGLRenderer * renderer, size_t size, Renderer::BufferUsage usage)
		: Base(size)
		, m_renderer(renderer)
		, m_buffer(new Vertex[size])
	{ }
	
	void setData(const Vertex * vertices, size_t count, size_t offset, BufferFlags flags) {
		
		ARX_UNUSED(flags);
		
		arx_assert(offset < capacity());
		arx_assert(offset + count <= capacity());
		
		std::copy(vertices, vertices + count, m_buffer + offset);
	}
	
	Vertex * lock(BufferFlags flags, size_t offset, size_t count) {
		ARX_UNUSED(flags), ARX_UNUSED(count);
		return m_buffer + offset;
	}
	
	void unlock() {
		// nothing to do
	}
	
	void draw(Renderer::Primitive primitive, size_t count, size_t offset) const {
		
		arx_assert(offset + count <= capacity());
		
		m_renderer->beforeDraw<Vertex>();
		
		setVertexArray<Vertex>(m_renderer, m_buffer, this);
		
		glDrawArrays(arxToGlPrimitiveType[primitive], offset, count);
		
	}
	
	void drawIndexed(Renderer::Primitive primitive, size_t count, size_t offset, const unsigned short * indices, size_t nbindices) const {
		
		arx_assert(offset < capacity());
		arx_assert(offset + count <= capacity());
		arx_assert(indices != NULL);
		
		m_renderer->beforeDraw<Vertex>();
		
		setVertexArray<Vertex>(m_renderer, m_buffer, this);
		
		GLenum mode = arxToGlPrimitiveType[primitive];
		GLenum type = GL_UNSIGNED_SHORT;
		const void * data = indices;
		
		if(offset != 0) {
			if(offset + count - 1 <= std::numeric_limits<GLushort>::max()) {
				glShortIndexBuffer.resize(std::max(glShortIndexBuffer.size(), size_t(nbindices)));
				for(size_t i = 0; i < nbindices; i++) {
					glShortIndexBuffer[i] = GLushort(indices[i] + offset);
				}
				type = GL_UNSIGNED_SHORT;
				data = &glShortIndexBuffer[0];
			} else {
				glIntIndexBuffer.resize(std::max(glIntIndexBuffer.size(), size_t(nbindices)));
				for(size_t i = 0; i < nbindices; i++) {
					glIntIndexBuffer[i] = GLuint(indices[i] + offset);
				}
				type = GL_UNSIGNED_INT;
				data = &glIntIndexBuffer[0];
			}
		}
		
		if(m_renderer->hasDrawRangeElements()) {
			glDrawRangeElements(mode, offset, offset + count - 1, nbindices, type, (GLvoid*)data);
		} else {
			glDrawElements(mode, nbindices, type, (GLvoid*)data);
		}
		
	}
	
	~GLVertexArray() {
		clearVertexArray(this);
		delete[] m_buffer;
	};
	
private:
	
	OpenGLRenderer * m_renderer;
	Vertex * m_buffer;
	
};

#define GLMapRangeVertexBuffer GLVertexArray
#define GLMapVertexBuffer GLVertexArray
#define GLShadowVertexBuffer GLVertexArray

#endif // ARX_GRAPHICS_OPENGL_GLVERTEXARRAY_H
