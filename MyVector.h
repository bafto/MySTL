#pragma once

#include <exception>
#include <initializer_list>

namespace MySTL
{
	template<typename T>
	class MyVector
	{
	public:
		class Iterator
		{

		};
	private:
		size_t v_size;
		size_t v_capacity;
		T* data;
	public:
		MyVector()
			:
			v_size(0),
			v_capacity(0),
			data(nullptr)
		{}
		~MyVector()
		{
			delete[] data;
		}
		MyVector(size_t v_size)
			:
			v_size(v_size),
			v_capacity(v_size)
		{
			data = new T[v_capacity];
		}
		MyVector(const MyVector<T>& copy) noexcept
			:
			v_size(copy.v_size),
			v_capacity(copy.v_capacity)
		{
			data = new T[v_capacity];
			for (size_t i = 0; i < v_size; i++)
			{
				data[i] = copy.data[i];
			}
		}
		MyVector(MyVector<T>&& donor) noexcept
			:
			v_size(donor.v_size),
			v_capacity(donor.v_capacity)
		{
			data = donor.data;
			donor.data = nullptr;
		}

		MyVector(std::initializer_list<T> list)
			:
			v_size(list.size()),
			v_capacity(v_size)
		{
			data = new T[v_capacity];
			for (size_t i = 0; i < v_size; i++)
			{
				data[i] = *(list.begin() + i);
			}
		}

		MyVector<T>& operator=(const MyVector<T>& copy) noexcept
		{
			if (&copy != this)
			{
				v_size = copy.v_size;
				v_capacity = copy.v_capacity;
				delete[] data;
				data = new T[v_capacity];
				for (size_t i = 0; i < v_size; i++)
				{
					data[i] = copy.data[i];
				}
			}
			return *this;
		}
		MyVector<T>& operator=(MyVector<T>&& donor) noexcept
		{
			if (&donor != this)
			{
				v_size = donor.v_size;
				v_capacity = donor.v_capacity;
				delete[] data;
				data = donor.data;
				donor.data = nullptr;
			}
			return *this;
		}

		MyVector<T>& operator=(std::initializer_list<T> list)
		{
			v_size = list.size();
			v_capacity = v_size;
			delete[] data;
			data = new T[v_capacity];
			for (size_t i = 0; i < v_size; i++)
			{
				data[i] = *(list.begin() + i);
			}
			return *this;
		}

		T& operator[](size_t index)
		{
			if (index >= v_size)
				throw std::out_of_range ("Index was out of bounds");
			return data[index];
		}
		const T& operator[](size_t index) const
		{
			if (index >= v_size)
				throw std::out_of_range ("Index was out of bounds");
			return data[index];
		}

		size_t size() const
		{
			return v_size;
		}
		size_t capacity() const
		{
			return v_capacity;
		}

		T& front()
		{
			if (v_size <= 0)
				throw std::out_of_range("Tried to access element in empty vector");
			return data[0];
		}
		const T& front() const
		{
			if (v_size <= 0)
				throw std::out_of_range("Tried to access element in empty vector");
			return data[0];
		}
		T& back()
		{
			if (v_size <= 0)
				throw std::out_of_range("Tried to access element in empty vector");
			return data[v_size - 1];
		}
		const T& back() const
		{
			if (v_size <= 0)
				throw std::out_of_range("Tried to access element in empty vector");
			return data[v_size - 1];
		}

		void swap(MyVector<T>& other)
		{
			int tempSize = v_size;
			int tempCapacity = v_capacity;
			T* tempData = data;

			v_size = other.v_size;
			v_capacity = other.v_capacity;
			data = other.data;

			other.v_size = tempSize;
			other.v_capacity = tempCapacity;
			other.data = tempData;
		}

		void resize(size_t n, const T& type = T())
		{
			if (n < v_size)
			{
				v_size = n;
			}
			else if (n > v_size)
			{
				for(size_t i = v_size; )//finish
			}
		}
		void reserve(size_t capacity)
		{
			if (v_capacity < capacity)
			{
				T* temp = new T[capacity];
				for (size_t i = 0; i < v_size; i++)
				{
					temp[i] = data[i];
				}
				delete[] data;
				data = temp;
			}
		}
		void shrink_to_fit()
		{
			if (v_capacity > v_size)
			{
				T* temp = new T[v_size];
				for (size_t i = 0; i < v_size; i++)
				{
					temp[i] = data[i];
				}
				delete[] data;
				data = temp;
				v_capacity = v_size;
			}
		}

		void clear()
		{
			delete[] data;
			data = nullptr;
			v_size = 0;
			v_capacity = 0;
		}
		bool empty() const
		{
			return v_size == 0;
		}

		//remake with iterators
		void insert(size_t position, const T& val)
		{
			if (position > v_size)
				throw std::out_of_range("Tried to insert at an invalid position");
			if (v_size >= v_capacity)
			{
				v_capacity += 4;
				v_size++;
				T* temp = new T[v_capacity];
				for (size_t i = 0; i < position; i++)
				{
					temp[i] = data[i];
				}
				temp[position] = val;
				for (size_t i = position; i < v_size; i++)
				{
					temp[i + 1] = data[i];
				}
				delete[] data;
				data = temp;
			}
			else
			{
				for (size_t i = v_size; i > position; i--)
				{
					data[i] = data[i - 1];
				}
				data[position] = val;
				v_size++;
			}
		}
		//remake with iterators
		void insert(size_t position, size_t n, const T& val)
		{
			for (size_t i = 0; i < n; i++)
			{
				insert(position, val);
			}
		}
		//remake with iterators!!!!!!!!!
		void insert(size_t position, T* firstIt, T* lastIt)
		{
			for (T* it = lastIt - 1; it >= firstIt; --it)
			{
				insert(position, *it);
			}
		}
	};
}