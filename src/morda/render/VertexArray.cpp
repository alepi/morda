#include "VertexArray.hpp"
#include "../exception.hpp"

#include <utki/debug.hpp>

using namespace morda;

VertexArray::VertexArray(decltype(buffers)&& buffers, std::shared_ptr<morda::IndexBuffer> indices, Mode_e mode) :
		buffers(buffers),
		indices(std::move(indices)),
		mode(mode)
{
	if(this->buffers.size() == 0){
		throw morda::Exc("no vertex buffers passed to vertex array");
	}
	ASSERT(this->buffers.front())
	auto s = this->buffers.front()->size;
	for(auto& b : this->buffers){
		if(b->size != s){
			throw morda::Exc("vertex buffers of different size passed in to vertex array, should all be of the same size");
		}
	}
}
