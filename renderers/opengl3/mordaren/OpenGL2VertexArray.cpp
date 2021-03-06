#include "OpenGL2VertexArray.hpp"

#include "OpenGL2_util.hpp"
#include "OpenGL2VertexBuffer.hpp"
#include "OpenGL2IndexBuffer.hpp"

namespace{
GLuint createGLVertexArray(){
	GLuint ret;
	glGenVertexArrays(1, &ret);
	assertOpenGLNoError();
	return ret;
}
}

OpenGL2VertexArray::OpenGL2VertexArray(std::vector<std::shared_ptr<morda::VertexBuffer>>&& buffers, std::shared_ptr<morda::IndexBuffer> indices, Mode_e mode) :
		morda::VertexArray(std::move(buffers), std::move(indices), mode),
		arr(createGLVertexArray())
{
	glBindVertexArray(this->arr);
	assertOpenGLNoError();
	
	for(unsigned i = 0; i != this->buffers.size(); ++i){
		ASSERT(dynamic_cast<OpenGL2VertexBuffer*>(this->buffers[i].operator->()))
		auto& vbo = static_cast<OpenGL2VertexBuffer&>(*this->buffers[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo.buffer);
		assertOpenGLNoError();
		
//		TRACE(<< "vbo.numComponents = " << vbo.numComponents << " vbo.type = " << vbo.type << std::endl)
		
		glVertexAttribPointer(i, vbo.numComponents, vbo.type, GL_FALSE, 0, nullptr);
		assertOpenGLNoError();
		
		glEnableVertexAttribArray(i);
		assertOpenGLNoError();
	}
	
	{
		ASSERT(dynamic_cast<OpenGL2IndexBuffer*>(this->indices.operator->()))
		auto& ivbo = static_cast<OpenGL2IndexBuffer&>(*this->indices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ivbo.buffer);
		assertOpenGLNoError();
	}
	
	//unbind VAO to make sure it won't be modified by another VAO definition
	glBindVertexArray(0);
	assertOpenGLNoError();
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	assertOpenGLNoError();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	assertOpenGLNoError();
}

OpenGL2VertexArray::~OpenGL2VertexArray()noexcept{
	glBindVertexArray(0);
	assertOpenGLNoError();
	glDeleteVertexArrays(1, &this->arr);
	assertOpenGLNoError();
}
