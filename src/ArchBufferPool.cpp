#include "ArchBufferPool.hpp"

namespace DarkDescent
{
	ArchBuffer::ArchBuffer(std::size_t count, std::size_t archSize):
		buffer_(nullptr),
		gameObjectBuffer_(static_cast<GameObjectHandle*>(_aligned_malloc(sizeof(GameObjectHandle)* count, 0x40))),
		ptr_(0)
	{
		if (archSize > 0)
		{
			buffer_ = static_cast<char*>(_aligned_malloc(archSize * count, 0x40));
			memset(buffer_, 0, archSize * count);
		}
		memset(gameObjectBuffer_, 0, sizeof(GameObjectHandle) * count);
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
		buffers_.emplace_back(new ArchBuffer(bufferCapacity_, archSize_));
	}

	Entity ArchBufferPool::alloc()
	{
		const std::size_t bufferIndex = ptr_;
		std::size_t index = buffers_[bufferIndex]->ptr_++;
		if (bufferIndex >= bufferCapacity_)
			addBuffer();
		return Entity(bufferIndex, index);
	}

	bool ArchBufferPool::free(const Entity& entity)
	{
		// TODO
		return true;
	}

	char* ArchBufferPool::getRaw(const Entity& entity) const
	{
		return buffers_[entity.bufferIndex]->buffer_ + (entity.index * archSize_);
	}

	GameObjectHandle* ArchBufferPool::getGameObject(const Entity& entity)
	{
		return &buffers_[entity.bufferIndex]->gameObjectBuffer_[entity.index];
	}
}