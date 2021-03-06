#pragma once

#include <exception>
#include <initializer_list>

namespace MySTL
{
	template<typename T>
	class MyVector
	{
	public:
		class exception : public std::exception
		{
		public:
			exception()
				:
				std::exception("Vector exception")
			{}
			exception(const char* msg)
				:
				std::exception(msg)
			{}
			exception(const char* msg, int i)
				:
				std::exception(msg, i)
			{}
			exception(const exception& copy) noexcept
				:
				std::exception(copy)
			{}
		};
		class out_of_bounds : public exception
		{
		public:
			out_of_bounds()
				:
				exception("Tried to access element out of bounds")
			{}
			out_of_bounds(const char* msg)
				:
				exception(msg)
			{}
			out_of_bounds(const char* msg, int i)
				:
				exception(msg, i)
			{}
			out_of_bounds(const out_of_bounds& copy) noexcept
				:
				exception(copy)
			{}
		};
	public:
		class iterator
		{
		private:
			friend class MyVector;

			class bad_iterator_compare : public exception
			{
			public:
				bad_iterator_compare()
					:
					exception("Tried to compare one or two invalid iterators")
				{}
				bad_iterator_compare(const char* msg)
					:
					exception(msg)
				{}
				bad_iterator_compare(const char* msg, int i)
					:
					exception(msg, i)
				{}
				bad_iterator_compare(const bad_iterator_compare& copy)
					:
					exception(copy)
				{}
			};
		public:
			iterator(MyVector<T>* pVec, size_t index)
				:
				vec(pVec),
				index(index)
			{
				if (index > vec->v_size)
					throw out_of_bounds("Tried to construct invalid iterator");
			}

			T& operator*() const
			{
				return (*vec)[index];
			}
			T* operator->() const
			{
				return &(*vec)[index];
			}

			iterator& operator++()
			{
				if (++index > vec->v_size)
					throw out_of_bounds("Tried to increment iterator past the end");
				return *this;
			}
			iterator operator++(int)
			{
				iterator result(*this);	
				++(*this);
				return result;
			}
			iterator& operator--()
			{
				if (index-- <= 0)
					throw out_of_bounds("Tried to decrement iterator below the beginning");
				return *this;
			}
			iterator operator--(int)
			{
				iterator result(*this);
				--(*this);
				return result;
			}

			void operator+=(size_t n)
			{
				index += n;
				if (index > vec->v_size)
					throw out_of_bounds("Tried to advance iterator past the end");
			}
			void operator-=(size_t n)
			{
				if ((int)index - n < 0)
					throw out_of_bounds("Tried to reduce iterator below the beginning");
				index -= n;
			}
			iterator operator+(size_t n) const
			{
				iterator result(*this);
				result += n;
				return result;
			}
			iterator operator-(size_t n) const
			{
				iterator result(*this);
				result -= n;
				return result;
			}
			size_t operator-(iterator other) const
			{
				int r = (int)index - (int)other.index;
				return r * ((r > 0) - (r < 0)); //get the absolute value;
			}
			
			T& operator[](size_t n) const
			{
				return (*vec)[index + n];
			}

			bool operator<(const iterator& other) const
			{
				if (vec == other.vec)
				{
					return index < other.index;
				}
				else
					throw bad_iterator_compare("Tried to compare iterators of different vectors");
			}
			bool operator>(const iterator& other) const
			{
				if (vec == other.vec)
				{
					return index > other.index;
				}
				else
					throw bad_iterator_compare("Tried to compare iterators of different vectors");
			}
			bool operator<=(const iterator& other) const
			{
				if (vec == other.vec)
				{
					return index <= other.index;
				}
				else
					throw bad_iterator_compare("Tried to compare iterators of different vectors");
			}
			bool operator>=(const iterator& other) const
			{
				if (vec == other.vec)
				{
					return index >= other.index;
				}
				else
					throw bad_iterator_compare("Tried to compare iterators of different vectors");
			}
			bool operator==(const iterator& other) const
			{
				if (vec == other.vec)
				{
					return index == other.index;
				}
				else
					throw bad_iterator_compare("Tried to compare iterators of different vectors");
			}
			bool operator!=(const iterator& other) const
			{
				if (vec == other.vec)
				{
					return index != other.index;
				}
				else
					throw bad_iterator_compare("Tried to compare iterators of different vectors");
			}
		protected:
			MyVector<T>* vec;
			size_t index;
		};
		class const_iterator : public iterator
		{
			friend class MyVector;
		public:
			const_iterator(const MyVector<T>* vec, size_t index)
				:
				iterator(const_cast<MyVector<T>*>(vec), index)
			{}

			const T& operator*() const
			{
				return iterator::operator*();
			}
			const T* operator->() const
			{
				return iterator::operator->();
			}

			const T& operator[](size_t index) const
			{
				return iterator::operator[](index);
			}
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
				throw out_of_bounds();
			return data[index];
		}
		const T& operator[](size_t index) const
		{
			if (index >= v_size)
				throw out_of_bounds();
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
				throw out_of_bounds("Tried to access element in empty vector");
			return data[0];
		}
		const T& front() const
		{
			if (v_size <= 0)
				throw out_of_bounds("Tried to access element in empty vector");
			return data[0];
		}
		T& back()
		{
			if (v_size <= 0)
				throw out_of_bounds("Tried to access element in empty vector");
			return data[v_size - 1];
		}
		const T& back() const
		{
			if (v_size <= 0)
				throw out_of_bounds("Tried to access element in empty vector");
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

		void resize(size_t n, const T& val = T())
		{
			if (n < v_size)
			{
				v_size = n;
			}
			else if (n > v_size)
			{
				if (v_capacity >= n)
				{
					for (size_t i = v_size; i < n; i++)
					{
						data[i] = val;
					}
				}
				else
				{
					v_capacity = n + 5;
					T* temp = new T[v_capacity];
					for (size_t i = 0; i < v_size; i++)
					{
						temp[i] = data[i];
					}
					for (size_t i = v_size; i < n; i++)
					{
						temp[i] = val;
					}
					delete[] data;
					data = temp;
				}
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

		iterator begin()
		{
			return iterator(this, 0);
		}
		const_iterator cbegin() const
		{
			return const_iterator(this, 0);
		}
		iterator end()
		{
			return iterator(this, v_size);
		}
		const_iterator cend() const
		{
			return const_iterator(this, v_size);
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
		void insert(iterator position, const T& val)
		{
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