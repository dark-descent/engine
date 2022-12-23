#pragma once

#include "pch.hpp"
#include "Entity.hpp"
#include "GameObject.hpp"

namespace DarkDescent
{
	class ArchBuffer
	{
	public:
		ArchBuffer(std::size_t count, std::size_t archSize);
		ArchBuffer(const ArchBuffer&) = delete;
		ArchBuffer(ArchBuffer&&) = delete;
		~ArchBuffer();

	private:
		char* buffer_;
		GameObjectHandle* gameObjectBuffer_;
		std::size_t ptr_;

		friend class ArchBufferPool;
	};

	class ArchBufferPool
	{
	public:
		ArchBufferPool(std::size_t archSize, std::size_t bufferCapacity);

		Entity alloc();
		bool free(const Entity& entity);
		char* getRaw(const Entity& entity) const;
		GameObjectHandle* getGameObject(const Entity& entity);

	private:
		void addBuffer();

	private:
		std::size_t archSize_;
		std::size_t bufferCapacity_;
		std::size_t bufferSize_;
		std::size_t ptr_;
		std::vector<ArchBuffer*> buffers_;
		
		friend class Arch;
	};
}