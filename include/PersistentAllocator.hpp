#pragma once

#include "pch.hpp"
#include <stdlib.h>

namespace DarkDescent
{
	template<typename T>
	class PersistentAllocator
	{
		struct Index
		{
			Index(std::size_t buffer, std::size_t index):
				buffer(buffer),
				index(index)
			{ }

			std::size_t buffer = 0;
			std::size_t index = 0;
		};
	public:
		struct Handle
		{
			Handle(Index&& index, T* data):
				index(index),
				data(data)
			{ }

			Handle():
				index(0),
				data(nullptr)
			{ }

			Index index;
			T* data;

			bool hasValue() const { return data != nullptr; }

			T* operator->()
			{
				return data;
			}
		};
	private:
		struct Buffer
		{
		public:
			Buffer(std::size_t capacity)
			{
				data_ = static_cast<T*>(_aligned_malloc(sizeof(T) * capacity, 0x80));
			}

			~Buffer()
			{
				_aligned_free(data_);
			}

		private:
			T* data_;

			friend class PersistentAllocator<T>;
		};

	public:
		explicit PersistentAllocator(std::size_t bufferCapacity = 1024):
			bufferCapacity_(bufferCapacity),
			lastBuffer_(0),
			buffers_()
		{
			addBuffer();
		}

		PersistentAllocator(const PersistentAllocator&) = delete;
		PersistentAllocator(PersistentAllocator&&) = delete;
		~PersistentAllocator()
		{
			for (auto& buf : buffers_)
			{
				delete buf;
			}
		}

	private:
		Index getNextIndex()
		{
			const std::size_t l = freeStack_.size();

			std::size_t bufferIndex;
			std::size_t index;

			if (l > 0)
			{
				auto& a = freeStack_.at(l - 1);
				bufferIndex = a.buffer;
				index = a.index;
				freeStack_.pop_back();
			}
			else
			{
				bufferIndex = lastBuffer_;
				index = lastIndex_++;

				if (lastIndex_ >= bufferCapacity_)
					addBuffer();
			}

			return Index(bufferIndex, index);
		}

	public:
		template<typename... Args>
		Handle emplace(Args&&... args)
		{
			Index index = getNextIndex();
			T* data = new (&buffers_[index.buffer]->data_[index.index]) T(std::forward<Args>(args)...);
			return Handle(std::move(index), data);
		}

		void free(Index&& index)
		{
			freeStack_.emplace_back(std::move(index));
		}

		void free(Handle& handle)
		{
			freeStack_.emplace_back(std::move(handle.index));
		}

	private:
		void addBuffer()
		{
			lastBuffer_ = buffers_.size();
			lastIndex_ = 0;
			buffers_.emplace_back(new Buffer(bufferCapacity_));
		}

		std::size_t bufferCapacity_;
		std::size_t lastBuffer_;
		std::size_t lastIndex_;
		std::vector<Buffer*> buffers_;
		std::vector<Index> freeStack_;
	};
}