#pragma once

#include <exception>
#include <initializer_list>
#include <functional>

namespace MySTL
{
	template<typename T>
	class MyVector
	{
	public:
		class exception : public std::runtime_error
		{
		private:
		public:
			exception()
				:
				exception("Vector exception")
			{}
			exception(const char* message)
				:
				std::runtime_error(message)
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
		};
		class bad_iterator : public exception
		{
		public:
			bad_iterator()
				:
				exception("Bad iterator")
			{}
			bad_iterator(const char* msg)
				:
				exception(msg)
			{}
		};
	public:
		//With the help of Zachariah the Magnificent to make it work for standard algorithms like std::sort()
		class iterator 
		{
		public:
			using value_type = T;
			using reference = T&;
			using pointer = T*;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::random_access_iterator_tag;
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
			};
		protected:
			MyVector<T>* vec;
			pointer Ptr;
		protected:
			iterator(MyVector<T>* pVec, pointer Ptr)
				:
				vec(pVec),
				Ptr(Ptr)
			{}
		public:
			reference operator*() const
			{
				return *Ptr;
			}
			pointer operator->() const
			{
				return Ptr;
			}

			iterator& operator++()
			{
				if (++Ptr > &vec->data[vec->v_size])
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
				if (--Ptr < &vec->data[0])
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
				Ptr += n;
				if (Ptr > &vec->data[vec->v_size])
					throw out_of_bounds("Tried to advance iterator past the end");
			}
			void operator-=(size_t n)
			{
				Ptr -= n;
				if (Ptr < &vec->data[0])
					throw out_of_bounds("Tried to reduce iterator below the beginning");
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
			difference_type operator-(iterator other) const
			{
				int r = Ptr - other.Ptr;
				return r * ((r > 0) - (r < 0)); //calculate absolute value
			}
			
			reference operator[](size_t n) const
			{
				return *(Ptr + n);
			}

			bool operator<(const iterator& other) const
			{
				if (vec == other.vec)
				{
					return Ptr < other.Ptr;
				}
				else
					throw bad_iterator_compare("Tried to compare iterators of different vectors");
			}
			bool operator>(const iterator& other) const
			{
				if (vec == other.vec)
				{
					return Ptr > other.Ptr;
				}
				else
					throw bad_iterator_compare("Tried to compare iterators of different vectors");
			}
			bool operator<=(const iterator& other) const
			{
				if (vec == other.vec)
				{
					return Ptr <= other.Ptr;
				}
				else
					throw bad_iterator_compare("Tried to compare iterators of different vectors");
			}
			bool operator>=(const iterator& other) const
			{
				if (vec == other.vec)
				{
					return Ptr >= other.Ptr;
				}
				else
					throw bad_iterator_compare("Tried to compare iterators of different vectors");
			}
			bool operator==(const iterator& other) const
			{
				if (vec == other.vec)
				{
					return Ptr == other.Ptr;
				}
				else
					throw bad_iterator_compare("Tried to compare iterators of different vectors");
			}
			bool operator!=(const iterator& other) const
			{
				if (vec == other.vec)
				{
					return Ptr != other.Ptr;
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
			const iterator::reference operator*() const
			{
				return iterator::operator*();
			}
			const iterator::pointer operator->() const
			{
				return iterator::operator->();
			}

			const iterator::reference operator[](size_t index) const
			{
				return iterator::operator[](index);
			}
		};
		class reverse_iterator : public iterator
		{
		private:
			friend class MyVector;
		protected:
			reverse_iterator(MyVector<T>* vec, iterator::pointer Ptr)
				:
				iterator(vec, Ptr)
			{}
		public:
			reverse_iterator& operator++()
			{
				if (--iterator::Ptr < &iterator::vec->data[-1])
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
				if (++iterator::Ptr >= &iterator::vec->data[iterator::vec->v_size])
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
				iterator::Ptr -= n;
				if (iterator::Ptr < &iterator::vec->data[-1])
					throw out_of_bounds("Tried to advance reverse_iterator past the end");
			}
			void operator-=(size_t n)
			{
				iterator::Ptr += n;
				if (iterator::Ptr >= &iterator::vec->data[iterator::vec->v_size])
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
			iterator::difference_type operator-(iterator other) const
			{
				int r = iterator::Ptr - other.Ptr;
				return r * ((r > 0) - (r < 0)); //calculate absolute value
			}

			bool operator<(const reverse_iterator& other) const
			{
				if (iterator::vec == other.vec)
				{
					return iterator::Ptr > other.Ptr;
				}
				else
					throw iterator::bad_iterator_compare("Tried to compare reverse_iterators of different vectors");
			}
			bool operator>(const reverse_iterator& other) const
			{
				if (iterator::vec == other.vec)
				{
					return iterator::Ptr < other.Ptr;
				}
				else
					throw iterator::bad_iterator_compare("Tried to compare reverse_iterators of different vectors");
			}
			bool operator<=(const reverse_iterator& other) const
			{
				if (iterator::vec == other.vec)
				{
					return iterator::Ptr >= other.Ptr;
				}
				else
					throw iterator::bad_iterator_compare("Tried to compare reverse_iterators of different vectors");
			}
			bool operator>=(const reverse_iterator& other) const
			{
				if (iterator::vec == other.vec)
				{
					return iterator::Ptr <= other.Ptr;
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
			reverse_const_iterator(const MyVector<T>* vec, iterator::pointer Ptr)
				:
				reverse_iterator(const_cast<MyVector<T>*>(vec), Ptr)
			{}
		public:
			const iterator::reference operator*() const
			{
				return reverse_iterator::operator*();
			}
			const iterator::pointer operator->() const
			{
				return reverse_iterator::operator->();
			}

			const iterator::reference operator[](size_t index) const
			{
				return reverse_iterator::operator[](index);
			}
		};
	private:
		friend class iterator;

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
		MyVector(size_t v_size, const T& val = T())
			:
			v_size(0),
			v_capacity(v_size)
		{
			data = new T[v_capacity];
			insert(begin(), v_size, val);
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

		template<class Iter>
		MyVector(Iter firstIt, Iter lastIt)
			:
			v_size(lastIt - firstIt),
			v_capacity(lastIt - firstIt)
		{
			data = new T[v_capacity];
			int i = 0;
			for (auto it = firstIt; it != lastIt; ++it, i++)
			{
				data[i] = *it;
			}
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
		MyVector<T>& operator=(MyVector<T>&& donor)  noexcept
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
		size_t maxSize() const
		{
			return size_t(-1);
		}
		size_t capacity() const
		{
			return v_capacity;
		}
		T* getData()
		{
			return data;
		}
		const T* getData() const
		{
			return data;
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

		T& at(size_t n)
		{
			return (*this)[n];
		}
		const T& at(size_t n) const
		{
			return (*this)[n];
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
					v_size = n;
				}
				else
				{
					reallocate(calculateGrowth(n));
					for (size_t i = v_size; i < n; i++)
					{
						data[i] = val;
					}
					v_size = n;
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
			return iterator(this, &data[0]);
		}
		const_iterator cbegin() const
		{
			return const_iterator(this, &data[0]);
		}
		reverse_iterator rbegin()
		{
			return reverse_iterator(this, &data[v_size - 1]);
		}
		reverse_const_iterator crbegin() const
		{
			return reverse_const_iterator(this, &data[v_size - 1]);
		}
		iterator end()
		{
			return iterator(this, &data[v_size]);
		}
		const_iterator cend() const
		{
			return const_iterator(this, &data[v_size]);
		}
		reverse_iterator rend()
		{
			return reverse_iterator(this, &data[-1]);
		}
		reverse_const_iterator crend() const
		{
			return reverse_const_iterator(this, &data[-1]);
		}

		void clear()
		{
			delete[] data;
			data = new T[v_capacity];
			v_size = 0;
		}
		bool empty() const
		{
			return v_size == 0;
		}

		void assign(size_t n, const T& val)
		{
			clear();
			insert(begin(), n, val);
		}

		iterator insert(iterator position, const T& val)
		{
			if (position.vec != this)
				throw bad_iterator("Tried to pass iterator from different vector");
			T copy = val;
			return insert(position, std::move(copy));
		}
		iterator insert(iterator position, T&& val)
		{
			if (position.vec != this)
				throw bad_iterator("Tried to pass iterator from different vector");
			if (v_capacity <= v_size)
			{
				size_t newCapacity = calculateGrowth(v_capacity + 1);
				T* newData = new T[newCapacity];
				size_t i = 0;
				for (; &data[i] != position.Ptr; i++)
				{
					newData[i] = std::move(data[i]);
				}
				newData[i++] = std::move(val);
				iterator ret(this, &newData[i - 1]);
				v_size++;
				for (; i < v_size; i++)
				{
					newData[i] = std::move(data[i - 1]);
				}
				delete[] data;
				data = newData;
				v_capacity = newCapacity;
				return ret;
			}
			else
			{
				v_size++;
				for (auto it = end() - 1; it > position; --it)
				{
					*it = std::move(*(it - 1));
				}
				*position = std::move(val);
				return position;
			}
		}
		iterator insert(iterator position, size_t n, const T& val)
		{
			for (size_t i = 0; i < n; i++)
			{
				position = insert(position, val);
			}
			return position;
		}
		iterator insert(iterator position, std::initializer_list<T> list)
		{
			for (auto it = list.end() - 1; it > list.begin() - 1; --it)
			{
				position = insert(position, *it);
			}
			return position;
		}
		template<class InputIt>
		iterator insert(iterator position, InputIt firstIt, InputIt lastIt) //hope it works
		{
			if (firstIt != lastIt)
			{
				MyVector<T> newVec;
				newVec.reserve(calculateGrowth(v_capacity + (lastIt - firstIt)));
				newVec.resize(v_size + (lastIt - firstIt));
				auto it = newVec.begin(), tIt = begin();
				for (; tIt != position; ++it, ++tIt)
				{
					*it = std::move(*tIt);
				}
				position = it;
				for (auto iit = firstIt; iit < lastIt; ++iit, ++it)
				{
					*it = *iit;
				}
				for (auto stop = end(); tIt != stop; ++it, ++tIt)
				{
					*it = std::move(*tIt);
				}
				*this = std::move(newVec);
				return position;
			}
			else
			{
				position = insert(position, *firstIt);
				return position;
			}
		}

		template<typename... args>
		T& emplace_back(args&&... vals)
		{
			return *emplace(end(), std::move(vals...));
		}
		template<typename... args>
		iterator emplace(iterator position, args&&... vals)
		{
			return insert(position, T(std::move(vals...)));
		}


		void push_back(const T& val)
		{
			insert(end(), val);
		}
		void push_back(T&& val)
		{
			insert(end(), std::move(val));
		}

		iterator erase(iterator position)
		{
			if (position == end())
				throw out_of_bounds("Tried to erase element out of bounds");
			for (auto it = position, stop = end() - 1; it < stop; ++it)
			{
				*it = std::move(*(it + 1));
			}
			v_size--;
			return position;
		}
		iterator erase(iterator firstIt, iterator lastIt)
		{
			if (firstIt < begin() || lastIt > end())
				throw out_of_bounds("Tried to pass iterators out of bounds");
			size_t dist = lastIt - firstIt;
			for (auto it = lastIt, stop = end(); it < stop; ++it)
			{
				*(it - dist) = std::move(*it);
			}
			v_size -= dist;
			return firstIt;
		}
		void pop_back()
		{
			erase(end() - 1);
		}
		void pop_front()
		{
			erase(begin());
		}

		/*//////////////////////////////////////////*/
		/*////Extra Methods (not in std::vector)////*/
		/*//////////////////////////////////////////*/

		//execute the lambda taking each element as parameter
		void forEach(std::function<void(T&)> lambda)
		{
			for (auto& e : *this)
			{
				lambda(e);
			}
		}
		template<class Iter>
		void forEach(std::function<void(T&)> lambda, Iter firstIt, Iter lastIt)
		{
			for (auto it = firstIt; it < lastIt; ++it)
			{
				lambda(*it);
			}
		}

		//bad sort function using bubble sort
		template<class Iter>
		void sort(Iter firstIt, Iter lastIt)
		{
			static_assert(std::is_base_of<iterator, Iter>::value, "Iterator must be of type MyVector<T>::iterator");
			if (firstIt.vec != this || lastIt.vec != this)
				throw bad_iterator("Iterator must be pointing to this");
			for (auto it = firstIt, stop = (lastIt - 1); it < stop; ++it)
			{
				for (auto itt = firstIt; itt < (lastIt - (firstIt - it) - 1); ++itt)
				{
					if (*itt > *(itt + 1))
					{
						T temp = *itt;
						*itt = *(itt + 1);
						*(itt + 1) = temp;
					}
				}
			}
		}
		template<class Iter>
		void sort(Iter firstIt, Iter lastIt, std::function<bool(const T&, const T&)> Comp)
		{
			static_assert(std::is_base_of<iterator, Iter>::value, "Iterator must be of type MyVector<T>::iterator");
			if (firstIt.vec != this || lastIt.vec != this)
				throw bad_iterator("Iterator must be pointing to this");
			for (auto it = firstIt, stop = (lastIt - 1); it < stop; ++it)
			{
				for (auto itt = firstIt; itt < (lastIt - (firstIt - it) - 1); ++itt)
				{
					if (Pred(*itt, *(itt + 1)))
					{
						T temp = *itt;
						*itt = *(itt + 1);
						*(itt + 1) = temp;
					}
				}
			}
		}
		void sort()
		{
			sort(begin(), end());
		}
		void rsort()
		{
			sort(rbegin(), rend());
		}
		void sort(std::function<bool(const T&, const T&)> Comp)
		{
			sort(begin(), end(), Comp);
		}

		//get subVec
		template<class Iter>
		MyVector<T> subVec(Iter firstIt, Iter lastIt)
		{
			static_assert(std::is_base_of<iterator, Iter>::value, "Iterator must be of type MyVector<T>::iterator");
			if (firstIt.vec != this || lastIt.vec != this)
				throw bad_iterator("Iterator must be pointing to this");
			MyVector<T> mvec;
			mvec.reserve(lastIt - firstIt);
			for (auto it = firstIt; it < lastIt; ++it)
			{
				mvec.emplace_back(*it);
			}
			return mvec;
		}

		//adds two MyVectors together
		MyVector<T> operator+(const MyVector<T>& other) const
		{
			MyVector<T> ret;
			ret.reserve(v_size + other.v_size);
			ret.insert(ret.end(), cbegin(), cend());
			ret.insert(ret.end(), other.cbegin(), other.cend());
			return ret;
		}

		//check if the vector contains the element by using the == operator
		bool contains(const T& val)
		{
			for (auto& e : *this)
			{
				if (e == val)
					return true;
			}
			return false;
		}

		//return how often the vector contains an item using the == operator
		size_t count(const T& val)
		{
			size_t ret = 0;
			for (auto& e : *this)
			{
				if (e == val)
					ret++;
			}
			return ret;
		}

		//split the vector at the first occurence of the element (none of the returned values contains val)
		std::pair<MyVector<T>, MyVector<T>> split(const T& val)
		{
			size_t i = 0;
			for (; i < v_size; i++)
			{
				if ((*this)[i] == val)
					break;
			}
			MyVector<T> vec1;
			vec1.reserve(i);
			vec1.insert(vec1.end(), begin(), begin() + i);
			MyVector<T> vec2;
			vec2.reserve(v_size - i - 1);
			vec2.insert(vec2.end(), begin() + (i + 1), end());
			return { vec1, vec2 };
		}

		//reverses the element order
		void reverse()
		{
			MyVector<T> temp(*this);
			size_t i = 0;
			for (auto it = rbegin(), stop = rend(); it < stop; ++it, i++)
			{
				temp[i] = *it;
			}
			*this = std::move(temp);
		}
	private:
		void reallocate(size_t capacity)
		{
			if (v_size > capacity)
				throw bad_alloc("Internally tried to reallocate to less space then items");
			T* temp = new T[capacity];
			for (size_t i = 0; i < v_size; i++)
			{
				temp[i] = std::move(data[i]);
			}
			delete[] data;
			data = temp;
			v_capacity = capacity;
		}
		size_t calculateGrowth(size_t newCapacity)
		{
			if (v_capacity > maxSize() - v_capacity / 2)
				return maxSize();

			const size_t newSize = v_capacity + v_capacity / 2;

			if (newSize < newCapacity)
				return newCapacity;

			return newSize;
		}
	};
}