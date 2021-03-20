#pragma once

#include <exception>
#include <initializer_list>
#include <functional>

namespace MySTL
{
	template<typename T>
	class MyForwardList
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
	private:
		struct Node
		{
			T data;
			Node* next;
			Node(Node* next = nullptr, const T& data = T())
				:
				next(next),
				data(data)
			{}
			~Node()
			{
				delete next;
			}
		};
	public:
		class iterator
		{
		public:
			using value_type = T;
			using reference = T&;
			using pointer = T*;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::random_access_iterator_tag;
		private:
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
		private:
			friend class MyForwardList;

			MyForwardList<T>* list;
			Node* node;
		protected:
			iterator(MyForwardList<T>* list, Node* node)
				:
				list(list),
				node(node)
			{}
		public:
			reference operator*() const
			{
				if (node == list->head)
					throw out_of_bounds("Tried to dereference element before the beginning of the list");
				return node->data;
			}
			pointer operator->() const
			{
				if (node == list->head)
					throw out_of_bounds("Tried to access element before the beginning of the list");
				return &node->data;
			}

			iterator& operator++()
			{
				if (node == list->tail)
					throw out_of_bounds("Tried to advance iterator past the end of the list");
				node = node->next;
				return *this;
			}
			iterator& operator++(int)
			{
				iterator result(*this);
				node = node->next;
				return result;
			}
			bool operator==(const iterator& other) const
			{
				if (list == other.list)
					return node == other.node;
				else
					throw bad_iterator_compare("Tried to compare iterators of different lists");
			}
			bool operator!=(const iterator& other) const
			{
				if (list == other.list)
					return node != other.node;
				else
					throw bad_iterator_compare("Tried to compare iterators of different lists");
			}
		};
		class const_iterator : public iterator
		{
		private:
			friend class MyForwardList;
		protected:
			const_iterator(const MyForwardList<T>* list, Node* node)
				:
				iterator(const_cast<MyForwardList<T>*>(list), node)
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
		};
	private:
		friend class iterator;

		Node* head; //one before the first element
		Node* tail; //one after the last element
	public:
		MyForwardList()
		{
			tail = new Node(nullptr);
			head = new Node(tail);
		}
		MyForwardList(const MyForwardList<T>& copy)
		{
			tail = new Node(nullptr);
			head = new Node(tail);
			iterator mIt = before_begin();
			for (auto it = copy.cbegin(), stop = copy.cend(); it != stop; ++it, ++mIt)
			{
				mIt.node->next = new Node(tail, *it);
			}
		}
		MyForwardList(MyForwardList<T>&& donor) noexcept //needs testing
		{
			head = std::move(donor.head);
			tail = std::move(donor.tail);
			donor.head = nullptr;
			donor.tail = nullptr;
		}
		~MyForwardList()
		{
			delete head;
		}

		MyForwardList(std::initializer_list<T> list)
		{
			tail = new Node(nullptr);
			head = new Node(tail);
			iterator mIt = before_begin();
			for (auto it = list.begin(), stop = list.end(); it != stop; ++it, ++mIt)
			{
				mIt.node->next = new Node(tail, *it);
			}
		}
		MyForwardList(size_t size, const T& val)
		{
			tail = new Node(nullptr);
			head = new Node(tail);
			iterator it = before_begin();
			for (size_t i = 0; i < size; ++i, ++it)
			{
				it.node->next = new Node(tail, val);
			}
		}

		MyForwardList<T>& operator=(const MyForwardList<T>& copy)
		{
			if (&copy != this)
			{
				delete head;

				tail = new Node(nullptr);
				head = new Node(tail);
				iterator mIt = before_begin();
				for (auto it = copy.cbegin(), stop = copy.cend(); it != stop; ++it, ++mIt)
				{
					mIt.node->next = new Node(tail, *it);
				}
			}

			return *this;
		}
		MyForwardList<T>& operator=(MyForwardList<T>&& donor) noexcept
		{
			if (&donor != this)
			{
				delete head;

				head = std::move(donor.head);
				tail = std::move(donor.tail);
				donor.head = nullptr;
				donor.tail = nullptr;
			}
			return *this;
		}

		MyForwardList<T>& operator=(std::initializer_list<T> list)
		{
			delete head;

			tail = new Node(nullptr);
			head = new Node(tail);
			iterator mIt = before_begin();
			for (auto it = list.begin(), stop = list.end(); it != stop; ++it, ++mIt)
			{
				mIt.node->next = new Node(tail, *it);
			}
			
			return *this;
		}

		void assign(std::initializer_list<T> list)
		{
			*this = list;
		}
		void assign(size_t size, const T& val)
		{
			clear();
			for (size_t i = 0; i < size; i++)
			{
				push_front(val);
			}
		}
		template<class Iter>
		void assign(Iter firstIt, Iter lastIt)
		{
			clear();
			for (auto it = firstIt; it != lastIt; ++it)
			{
				push_front(*it);
			}
		}

		void resize(size_t n, const T& val = T())
		{
			if (n == 0)
			{
				clear();
				return;
			}
			size_t i = 1;
			iterator it = begin();
			for (; i < n; ++it, i++)
			{
				if (it.node->next == tail)
					break;
			}
			if (i == n)
			{
				erase_after(it, end());
				return;
			}
			else if (it.node->next == tail)
			{
				for (; i < n; i++)
				{
					it = insert_after(it, val);
				}
			}
		}

		size_t max_size() const
		{
			return size_t(-1);
		}
		bool empty() const
		{
			return head->next == tail;
		}

		T& front()
		{
			return head->next->data;
		}
		const T& front() const
		{
			return head->next->data;
		}

		void clear()
		{
			delete head;
			tail = new Node(nullptr);
			head = new Node(tail);
		}
		void swap(MyForwardList<T>& other)
		{
			Node* tempHead = head, *tempTail = tail;
			
			head = other.head;
			tail = other.tail;

			other.head = tempHead;
			other.tail = tempTail;
		}

		iterator before_begin()
		{
			return iterator(this, head);
		}
		iterator begin()
		{
			return iterator(this, head->next);
		}
		iterator end()
		{
			return iterator(this, tail);
		}
		const_iterator cbefore_begin() const
		{
			return const_iterator(this, head);
		}
		const_iterator cbegin() const
		{
			return const_iterator(this, head->next);
		}
		const_iterator cend() const
		{
			return const_iterator(this, tail);
		}

		iterator insert_after(iterator position, const T& val)
		{
			if (position.node == tail)
				throw out_of_bounds("Tried to insert after the end");
			position.node->next = new Node(position.node->next, val);
			return ++position;
		}
		iterator insert_after(iterator position, T&& val)
		{
			if (position.node == tail)
				throw out_of_bounds("Tried to insert after the end");
			position.node->next = new Node(position.node->next, std::move(val));
			return ++position;
		}
		iterator insert_after(iterator position, size_t size, const T& val)
		{
			for (size_t i = 0; i < size; i++)
			{
				insert_after(position, val);
			}
			return ++position;
		}
		iterator insert_after(iterator position, std::initializer_list<T> list)
		{
			for (auto it = list.begin(), stop = list.end(); it != stop; ++it)
			{
				position = insert_after(position, *it);
			}
			return position;
		}
		template<class Iter>
		iterator insert_after(iterator position, Iter firstIt, Iter lastIt)
		{
			for (auto it = firstIt, stop = lastIt; it != stop; ++it)
			{
				position = insert_after(position, *it);
			}
			return position;
		}

		iterator emplace_after(iterator position, T&& val)
		{
			insert_after(position, val);
		}
		void emplace_front(T&& val)
		{
			head->next = new Node(head->next, std::move(val));
		}

		void push_front(const T& val)
		{
			head->next = new Node(head->next, val);
		}
		void push_front(T&& val)
		{
			emplace_front(std::move(val));
		}

		void pop_front()
		{
			erase_after(before_begin());
		}

		iterator erase_after(iterator position)
		{
			Node* toErase = position.node->next;
			if (toErase == nullptr)
				throw out_of_bounds("Tried to delete past the end of the list");
			position.node->next = toErase->next;
			toErase->next = nullptr;
			delete toErase;
			return ++position;
		}
		iterator erase_after(iterator firstIt, iterator lastIt)
		{
			iterator test = firstIt;
			if (++test == lastIt)
				return lastIt;
			Node* moveNode = firstIt.node;
			iterator it = firstIt;
			for (auto next = ++firstIt; next != lastIt; ++it, ++next)
			{}
			it.node->next = nullptr;
			delete firstIt.node;
			moveNode->next = lastIt.node;
			return iterator(this, moveNode);
		}

		void reverse()
		{
			MyForwardList<T> newList;
			for (auto& e : *this)
				newList.emplace_front(std::move(e));
			*this = newList;
		}
		//bad bubble sort
		void sort()
		{
			for (auto it = begin(), stop = end(); it != stop; ++it)
			{
				for (auto itt = begin(), jtt = ++begin(); jtt != stop; ++itt, ++jtt)
				{
					if (*itt > *jtt)
					{
						T temp = *itt;
						*itt = *jtt;
						*jtt = temp;
					}
				}
			}
		}
		void sort(std::function<bool(const T&, const T&)> Pred)
		{
			for (auto it = begin(), stop = end(); it != stop; ++it)
			{
				for (auto itt = begin(), jtt = ++begin(); jtt != stop; ++itt, ++jtt)
				{
					if (Pred(*itt, *jtt))
					{
						T temp = *itt;
						*itt = *jtt;
						*jtt = temp;
					}
				}
			}
		}
	};
}