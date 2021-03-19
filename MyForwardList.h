#pragma once

#include <initializer_list>

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
			delete head;

			tail = new Node(nullptr);
			head = new Node(tail);
			iterator mIt = before_begin();
			for (auto it = copy.cbegin(), stop = copy.cend(); it != stop; ++it, ++mIt)
			{
				mIt.node->next = new Node(tail, *it);
			}
		}
		MyForwardList<T>& operator=(MyForwardList<T>&& donor) noexcept
		{
			delete head;

			head = std::move(donor.head);
			tail = std::move(donor.tail);
			donor.head = nullptr;
			donor.tail = nullptr;
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
		}

		size_t max_size() const
		{
			return size_t(-1);
		}
		bool empty() const
		{
			return head->next == tail;
		}

		void clear()
		{
			delete head;
			tail = new Node(nullptr);
			head = new Node(tail);
		}
		void swap(MyForwardList<T>& other)
		{
			Node* tempHead = head, tempTail = tail;
			
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
	};
}