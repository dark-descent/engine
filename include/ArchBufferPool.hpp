#pragma once

#include "pch.hpp"
#include "Entity.hpp"
#include "GameObject.hpp"

namespace DarkDescent
{
	class ArchBuffer
	{
	public:
		struct Data
		{
			char* data;

			template<typename T>
			T& get(const std::size_t offset) const noexcept
			{
				return *reinterpret_cast<T*>(&data[offset]);
			}
		};

		ArchBuffer(std::size_t count, std::size_t archSize);
		ArchBuffer(const ArchBuffer&) = delete;
		ArchBuffer(ArchBuffer&&) = delete;
		~ArchBuffer();

		template<typename Callback>
		void iterate(Callback callback)
		{
			for(std::size_t i = 0; i < ptr_; i++)
				callback(Data{ &buffer_[(i * archSize_)] });
		}

	private:
		char* buffer_;
		GameObjectHandle* gameObjectBuffer_;
		std::size_t ptr_;
		std::size_t archSize_;

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

		std::size_t getCount() const;

		inline const std::vector<ArchBuffer*>& buffers() const noexcept { return buffers_; }

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