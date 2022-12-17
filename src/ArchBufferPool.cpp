#include "ArchBufferPool.hpp"

namespace DarkDescent
{
	ArchBuffer::ArchBuffer(std::size_t capacity):
		buffer_(static_cast<char*>(_aligned_malloc(capacity, 0x40))),
		ptr_(0)
	{

	}

	ArchBuffer::~ArchBuffer()
	{
		_aligned_free(buffer_);
	}

	ArchBufferPool::ArchBufferPool(std::size_t archSize, std::size_t bufferCapacity):
		archSize_(archSize),
		bufferCapacity_(bufferCapacity),
		bufferSize_(bufferCapacity* archSize),
		ptr_(0),
		buffers_()
	{
		addBuffer();
	}
	
	void ArchBufferPool::addBuffer()
	{
		ptr_ = buffers_.size();
		buffers_.emplace_back(bufferSize_);
	}

	Entity ArchBufferPool::alloc()
	{
		const std::size_t bufferIndex = ptr_;
		std::size_t index = buffers_[bufferIndex]->ptr_++;
		if(bufferIndex > bufferCapacity_)
			addBuffer();
		return Entity(bufferIndex, index);
	}

	bool ArchBufferPool::free(const Entity& entity)
	{
		// TODO
		return true;
	}

	char* ArchBufferPool::getRaw(const Entity& entity)
	{
		return buffers_[entity.bufferIndex]->buffer_ + (entity.index * archSize_);
	}
}