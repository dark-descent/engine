#pragma once

#include "pch.hpp"
#include "PersistentAllocator.hpp"

namespace DarkDescent
{
	template<typename T, std::size_t Capacity>
	class PersistentVector
	{
	private:
		struct Buffer
		{
			std::optional<T> buffer[Capacity];
		};

		std::size_t getNextIndexAndIncrement()
		{
			std::size_t index;
			if (freeList_.size() > 0)
			{
				index = freeList_.top();
				freeList_.pop();
			}
			else
			{
				index = top_++;
				if (top_ >= buffers_.size() * Capacity)
				{
					buffers_.emplace_back(new Buffer());
				}
			}
			return index;
		}

	public:
		PersistentVector():
			buffers_({ new Buffer() }),
			freeList_(),
			top_(0)
		{

		}

		std::size_t getNextIndex()
		{
			if (freeList_.size() > 0)
				return freeList_.top();
			return top_;
		}

		template<typename... Args>
		T& emplace_back(Args&&... args)
		{
			std::size_t index = getNextIndexAndIncrement();
			std::size_t bufferIndex = index / Capacity;
			index = index % Capacity;
			std::optional<T>& maybe = buffers_.at(bufferIndex)->buffer[index];
			return maybe.emplace(std::forward<Args>(args)...);
		}

		void resize(std::size_t capacity)
		{
			for (std::size_t i = buffers_.size(), l = (capacity / Capacity) + 1; i < l; i++)
				buffers_.emplace_back(new Buffer());
		}

		template<typename... Args>
		T& emplace_at(std::size_t index, Args&&... args)
		{
			resize(index);
			std::size_t bufferIndex = index / Capacity;
			std::size_t index2 = index % Capacity;
			std::optional<T>& maybe = buffers_.at(bufferIndex)->buffer[index2];
			if (index > top_)
				top_ = index + 1;
			return maybe.emplace(std::forward<Args>(args)...);
		}

		T& push_back(T&& item)
		{
			std::size_t index = getNextIndexAndIncrement();
			std::size_t bufferIndex = index / Capacity;
			index = index % Capacity;
			std::optional<T>& maybe = buffers_.at(bufferIndex)->buffer[index];
			return maybe.emplace(std::forward<T>(item));
		}

		template<typename... Ts>
		T& push_back(Ts&&... items)
		{
			int i = 0;

			T* first = nullptr;

			([ & ]
			{
				T* item = std::addressof(push_back(std::forward<T>(items)));
				if(first == nullptr)
					first = item;
			} (), ...);

			assert(first != nullptr);
			return *first;
		}

		T& at(std::size_t index) const
		{
			std::size_t bufferIndex = index / Capacity;
			index = index % Capacity;
			std::optional<T>& maybe = buffers_.at(bufferIndex)->buffer[index];
			if (!maybe.has_value())
				throw "PersistentVector slot is empty!";
			return maybe.value();
		}

		const std::optional<T>& maybe_at(std::size_t index)
		{
			std::size_t bufferIndex = index / Capacity;
			index = index % Capacity;
			return buffers_.at(bufferIndex)->buffer[index];
		}

		void erase_at(std::size_t index)
		{
			if (index == top_)
			{
				top_--;
			}
			else
			{
				freeList_.push(index);
			}
			std::size_t bufferIndex = index / Capacity;
			index = index % Capacity;
			std::optional<T>& maybe = buffers_.at(bufferIndex)->buffer[index];
			maybe.reset();
		}

		template<typename Callback>
		void foreach(Callback callback)
		{
			for (std::size_t b = 0; b < buffers_.size(); b++)
			{
				Buffer& buf = *buffers_.at(b);
				for (std::size_t i = 0; i < Capacity; i++)
				{
					const std::size_t index = (Capacity * b) + i;
					if (index >= top_)
						break;

					if (buf.buffer[i].has_value())
						callback(buf.buffer[i].value(), index);
				}
			}
		}

		void clear()
		{
			for (auto& buf : buffers_)
				delete buf;
			buffers_.clear();
			buffers_.emplace_back(new Buffer());
			top_ = 0;
		}

		std::vector<Buffer*> buffers_;
		std::stack<std::size_t> freeList_;
		std::size_t top_;
	};
};