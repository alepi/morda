#pragma once

#include <utki/Buf.hpp>

#include <morda/render/index_buffer.hpp>

#include "OpenGLES2Buffer.hpp"

namespace mordaren{

class OpenGLES2IndexBuffer : public morda::index_buffer, public OpenGLES2Buffer{
public:
	const GLenum elementType;
	const GLsizei elementsCount;
	
	OpenGLES2IndexBuffer(const utki::span<std::uint16_t> indices);
	
	OpenGLES2IndexBuffer(const OpenGLES2IndexBuffer&) = delete;
	OpenGLES2IndexBuffer& operator=(const OpenGLES2IndexBuffer&) = delete;
	
	
private:

};

}
