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
		class bad_alloc : public exception
		{
		public:
			bad_alloc()
				:
				exception("Tried to compare one or two invalid iterators")
			{}
			bad_alloc(const char* msg)
				:
				exception(msg)
			{}
			bad_alloc(const char* msg, int i)
				:
				exception(msg, i)
			{}
			bad_alloc(const bad_alloc& copy)
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
		protected:
			MyVector<T>* vec;
			size_t index;
		protected:
			iterator(MyVector<T>* pVec, size_t index)
				:
				vec(pVec),
				index(index)
			{}
		public:
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
		};
		class const_iterator : public iterator
		{
			friend class MyVector;
		protected:
			const_iterator(const MyVector<T>* vec, size_t index)
				:
				iterator(const_cast<MyVector<T>*>(vec), index)
			{}
		public:
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
		class reverse_iterator : public iterator
		{
		private:
			friend class MyVector;
		protected:
			reverse_iterator(MyVector<T>* vec, size_t index)
				:
				iterator(vec, index)
			{}
		public:
			reverse_iterator& operator++()
			{
				if (iterator::index-- == -1)
					throw out_of_bounds("Tried to increment reverse_iterator past the end");
				return *this;
			}
			reverse_iterator operator++(int)
			{
				reverse_iterator result(*this);
				++(*this);
				return result;
			}
			reverse_iterator& operator--()
			{
				if (++iterator::index >= iterator::vec->v_size)
					throw out_of_bounds("Tried to decrement reverse_iterator below the beginning");
				return *this;
			}
			reverse_iterator operator--(int)
			{
				reverse_iterator result(*this);
				--(*this);
				return result;
			}

			void operator+=(size_t n)
			{
				if ((int)iterator::index - n < -1)
					throw out_of_bounds("Tried to advance reverse_iterator past the end");
				iterator::index -= n;
			}
			void operator-=(size_t n)
			{
				iterator::index += n;
				if (iterator::index >= iterator::vec->v_size)
					throw out_of_bounds("Tried to reduce reverse_iterator below the beginning");
			}
			reverse_iterator operator+(size_t n) const
			{
				reverse_iterator result(*this);
				result += n;
				return result;
			}
			reverse_iterator operator-(size_t n) const
			{
				reverse_iterator result(*this);
				result -= n;
				return result;
			}
			size_t operator-(iterator other) const
			{
				int r = (int)iterator::index - (int)other.index;
				return r * ((r > 0) - (r < 0)); //get the absolute value;
			}

			bool operator<(const reverse_iterator& other) const
			{
				if (iterator::vec == other.vec)
				{
					if (iterator::index == -1 || other.index == -1)
					{
						return (iterator::index + 1) > (other.index + 1);
					}
					return iterator::index > other.index;
				}
				else
					throw iterator::bad_iterator_compare("Tried to compare reverse_iterators of different vectors");
			}
			bool operator>(const reverse_iterator& other) const
			{
				if (iterator::vec == other.vec)
				{
					if (iterator::index == -1 || other.index == -1)
					{
						return (iterator::index + 1) < (other.index + 1);
					}
					return iterator::index < other.index;
				}
				else
					throw iterator::bad_iterator_compare("Tried to compare reverse_iterators of different vectors");
			}
			bool operator<=(const reverse_iterator& other) const
			{
				if (iterator::vec == other.vec)
				{
					if (iterator::index == -1 || other.index == -1)
					{
						return (iterator::index + 1) >= (other.index + 1);
					}
					return iterator::index >= other.index;
				}
				else
					throw iterator::bad_iterator_compare("Tried to compare reverse_iterators of different vectors");
			}
			bool operator>=(const reverse_iterator& other) const
			{
				if (iterator::vec == other.vec)
				{
					if (iterator::index == -1 || other.index == -1)
					{
						return (iterator::index + 1) <= (other.index + 1);
					}
					return iterator::index <= other.index;
				}
				else
					throw iterator::bad_iterator_compare("Tried to compare reverse_iterators of different vectors");
			}
		};
		class reverse_const_iterator : public reverse_iterator
		{
		private:
			friend class MyVector;
		protected:
			reverse_const_iterator(const MyVector<T>* vec, size_t index)
				:
				reverse_iterator(const_cast<MyVector<T>*>(vec), index)
			{}
		public:
			const T& operator*() const
			{
				return reverse_iterator::operator*();
			}
			const T* operator->() const
			{
				return reverse_iterator::operator->();
			}

			const T& operator[](size_t index) const
			{
				return reverse_iterator::operator[](index);
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
					reallocate(v_capacity);
					for (size_t i = v_size; i < n; i++)
					{
						data[i] = val;
					}
				}
			}
		}
		void reserve(size_t capacity)
		{
			if (v_capacity < capacity)
			{
				reallocate(capacity);
			}
		}
		void shrink_to_fit()
		{
			if (v_capacity > v_size)
			{
				reallocate(v_size);
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
		reverse_iterator rbegin()
		{
			return reverse_iterator(this, v_size - 1);
		}
		reverse_const_iterator crbegin()
		{
			return reverse_const_iterator(this, v_size - 1);
		}
		iterator end()
		{
			return iterator(this, v_size);
		}
		const_iterator cend() const
		{
			return const_iterator(this, v_size);
		}
		reverse_iterator rend()
		{
			return reverse_iterator(this, -1);
		}
		reverse_const_iterator crend()
		{
			return reverse_const_iterator(this, -1);
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

		iterator insert(iterator position, const T& val)
		{
			if (v_capacity <= v_size)
			{
				reallocate(v_capacity + 3);
			}
			v_size++;
			for (auto it = end() - 1; it > position; --it)
			{
				*it = *(it - 1);
			}
			*position = val;
			return position;
		}
		iterator insert(iterator position, size_t n, const T& val)
		{
			for (size_t i = 0; i < n; i++)
			{
				insert(position, val);
			}
			return position;
		}
		iterator insert(iterator position, std::initializer_list<T> list)
		{
			for (auto it = list.end() - 1; it > list.begin() - 1; --it)
			{
				insert(position, *it);
			}
			return position;
		}
		template<class InputIt>
		iterator insert(iterator position, InputIt firstIt, InputIt lastIt) //does not yet work on self
		{
			reallocate(v_capacity + (lastIt - firstIt));
			for (auto it = lastIt - 1; it > firstIt; --it)
			{
				insert(position, *it);
			}
			insert(position, *firstIt);
			return position;
		}
	private:
		void reallocate(size_t capacity)
		{
			if (v_size > capacity)
				throw bad_alloc("Internally tried to reallocate to less space then items");
			T* temp = new T[capacity];
			for (size_t i = 0; i < v_size; i++)
			{
				temp[i] = data[i];
			}
			delete[] data;
			data = temp;
			v_capacity = capacity;
		}
	};
}