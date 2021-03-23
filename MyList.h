#pragma once

#include <stdexcept>
#include <initializer_list>
#include <functional>

namespace MySTL
{
	template<typename T>
	class MyList
	{
	public:
		class exception : public std::runtime_error
		{
		public:
			exception()
				:
				exception("List exception")
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
			out_of_bounds(const char* message)
				:
				exception(message)
			{}
		};
	private:
		struct Node
		{
			Node* prev;
			Node* next;
			T data;
			Node(Node* prev, Node* next, T data = T())
				:
				prev(prev),
				next(next),
				data(data)
			{}
		};
	public:
		class iterator
		{
		public:
			using value_type = T;
			using reference = T&;
			using pointer = T*;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::bidirectional_iterator_tag;
		public:
			class bad_iterator_compare : public exception
			{
			public:
				bad_iterator_compare()
					:
					exception("Tried to compare false iterators")
				{}
				bad_iterator_compare(const char* message)
					:
					exception(message)
				{}
			};
		private:
			friend class MyList;

			MyList<T>* list;
			Node* node;
		protected:
			iterator(MyList<T>* list, Node* node)
				:
				list(list),
				node(node)
			{}
		public:
			reference operator*() const
			{
				if (node == list->head || node == list->tail)
					throw out_of_bounds("Tried to dereference invalid element");
				return node->data;
			}
			pointer operator->() const
			{
				if (node == list->head)
					throw out_of_bounds("Tried to access invalid element");
				return &node->data;
			}

			iterator& operator++()
			{
				if (node == list->tail)
					throw out_of_bounds("Tried to increment iterator out of bounds");
				node = node->next;
				return *this;
			}
			iterator operator++(int)
			{
				iterator result(*this);
				if (node == list->tail)
					throw out_of_bounds("Tried to increment iterator out of bounds");
				node = node->next;
				return result;
			}
			iterator& operator--()
			{
				if (node->prev == list->head)
					throw out_of_bounds("Tried to decrement iterator out of bounds");
				node = node->prev;
				return *this;
			}
			iterator operator--(int)
			{
				iterator result(*this);
				if (node->prev == list->head)
					throw out_of_bounds("Tried to decrement iterator out of bounds");
				node = node->prev;
				return result;
			}

			bool operator==(const iterator& other) const
			{
				if (list == other.list)
					return node == other.node;
				else
					throw bad_iterator_compare("Tried to compare iterators from different lists");
			}
			bool operator!=(const iterator& other) const
			{
				if (list == other.list)
					return node != other.node;
				else
					throw bad_iterator_compare("Tried to compare iterators from different lists");
			}
		};
		class const_iterator : public iterator
		{
		private:
			friend class MyList;
		protected:
			const_iterator(const MyList<T>* list, Node* node)
				:
				iterator(const_cast<MyList<T>*>(list), node)
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
		class reverse_iterator : public iterator
		{
		private:
			friend class MyList;
		protected:
			reverse_iterator(MyList<T>* list, Node* node)
				:
				iterator(list, node)
			{}
		public:
			reverse_iterator& operator++()
			{
				if (iterator::node == iterator::list->head)
					throw out_of_bounds("Tried to increment reverse iterator past the reverse end");
				iterator::node = iterator::node->prev;
				return *this;
			}
			reverse_iterator operator++(int)
			{
				reverse_iterator result(*this);
				if (iterator::node == iterator::list->head)
					throw out_of_bounds("Tried to increment reverse iterator past the reverse end");
				iterator::node = iterator::node->prev;
				return result;
			}
			reverse_iterator& operator--()
			{
				if (iterator::node->next == iterator::list->tail)
					throw out_of_bounds("Tried to decrement reverse iterator past the reverse begin");
				iterator::node = iterator::node->next;
				return *this;
			}
			reverse_iterator operator--(int)
			{
				reverse_iterator result(*this);
				if (iterator::node->next == iterator::list->tail)
					throw out_of_bounds("Tried to decrement reverse iterator past the reverse begin");
				iterator::node = iterator::node->next;
				return result;
			}
		};
		class const_reverse_iterator : public reverse_iterator
		{
		private:
			friend class MyList;
		protected:
			const_reverse_iterator(const MyList<T>* list, Node* node)
				:
				reverse_iterator(const_cast<MyList<T>*>(list), node)
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
		};
	private:
		friend class iterator;

		Node* head; //one element before the first
		Node* tail; //one element after the last
		size_t v_size;
	public:
		MyList()
			:
			head(new Node(nullptr, nullptr)),
			tail(new Node(nullptr, nullptr)),
			v_size(0)
		{
			head->next = tail;
			tail->prev = head;
		}
		MyList(const MyList<T>& copy) noexcept
			:
			head(new Node(nullptr, nullptr)),
			tail(new Node(nullptr, nullptr)),
			v_size(copy.v_size)
		{
			head->next = tail;
			tail->prev = head;
			Node* current = head;
			for (auto it = copy.cbegin(), stop = copy.cend(); it != stop; ++it)
			{
				Node* node = new Node(current, tail, *it);
				current->next = node;
				tail->prev = node;
				current = node;
			}
		}
		MyList(MyList<T>&& donor) noexcept
			:
			head(donor.head),
			tail(donor.tail),
			v_size(donor.v_size)
		{
			donor.head = new Node(nullptr, nullptr);
			donor.tail = new Node(head, nullptr);
			donor.head->next = donor.tail;
		}
		~MyList()
		{
			_safeDeleteAllChildren(head);
		}

		template<class Iter>
		MyList(Iter firstIt, Iter lastIt)
			:
			head(new Node(nullptr, nullptr)),
			tail(new Node(nullptr, nullptr)),
			v_size(lastIt - firstIt)
		{
			head->next = tail;
			tail->prev = head;
			Node* current = head;
			for (auto it = firstIt; it != lastIt; ++it)
			{
				Node* node = new Node(current, tail, *it);
				current->next = node;
				tail->prev = node;
				current = node;
			}
		}
		MyList(size_t size, const T& val = T())
			:
			head(new Node(nullptr, nullptr)),
			tail(new Node(nullptr, nullptr)),
			v_size(size)
		{
			head->next = tail;
			tail->prev = head;
			Node* current = head;
			for (size_t i = 0; i < size; ++i)
			{
				Node* node = new Node(current, tail, val);
				current->next = node;
				tail->prev = node;
				current = node;
			}
		}
		MyList(std::initializer_list<T> list)
			:
			head(new Node(nullptr, nullptr)),
			tail(new Node(nullptr, nullptr)),
			v_size(list.size())
		{
			head->next = tail;
			tail->prev = head;
			Node* current = head;
			for (auto it = list.begin(), stop = list.end(); it != stop; ++it)
			{
				Node* node = new Node(current, tail, *it);
				current->next = node;
				tail->prev = node;
				current = node;
			}
		}

		MyList<T>& operator=(const MyList<T>& copy) noexcept
		{
			if (this != &copy)
			{
				tail->prev->next = nullptr;
				_safeDeleteAllChildren(head->next);
				head->next = tail;
				v_size = copy.v_size;
				Node* current = head;
				for (auto it = copy.cbegin(), stop = copy.cend(); it != stop; ++it)
				{
					Node* node = new Node(current, tail, *it);
					current->next = node;
					tail->prev = node;
					current = node;
				}
			}

			return *this;
		}
		MyList<T>& operator=(MyList<T>&& donor) noexcept
		{
			if (this != &donor)
			{
				tail->prev->next = nullptr;
				_safeDeleteAllChildren(head->next);
				head->next = tail;
				v_size = donor.v_size;
				head = donor.head;
				tail = donor.tail;
				donor.head = new Node(nullptr, nullptr);
				donor.tail = new Node(head, nullptr);
				donor.head->next = donor.tail;
			}

			return *this;
		}

		MyList<T>& operator=(std::initializer_list<T> list) noexcept
		{
			tail->prev->next = nullptr;
			_safeDeleteAllChildren(head->next);
			head->next = tail;
			Node* current = head;
			for (auto it = list.begin(), stop = list.end(); it != stop; ++it)
			{
				Node* node = new Node(current, tail, *it);
				current->next = node;
				tail->prev = node;
				current = node;
			}
			return *this;
		}

		size_t size() const
		{
			return v_size;
		}
		size_t max_size() const
		{
			return size_t(-1);
		}
		bool empty() const
		{
			return v_size == 0;
		}

		T& front()
		{
			if (empty())
				throw exception("Cannot access element in empty list");
			return head->next->data;
		}
		const T& front() const
		{
			if (empty())
				throw exception("Cannot access element in empty list");
			return head->next->data;
		}
		T& back()
		{
			if (empty())
				throw exception("Cannot access element in empty list");
			return tail->prev->data;
		}
		const T& back() const
		{
			if (empty())
				throw exception("Cannot access element in empty list");
			return tail->prev->data;
		}

		iterator begin()
		{
			return iterator(this, head->next);
		}
		const_iterator cbegin() const
		{
			return const_iterator(this, head->next);
		}
		reverse_iterator rbegin()
		{
			return reverse_iterator(this, tail->prev);
		}
		const_reverse_iterator crbegin() const
		{
			return const_reverse_iterator(this, tail->prev);
		}
		iterator end()
		{
			return iterator(this, tail);
		}
		const_iterator cend() const
		{
			return const_iterator(this, tail);
		}
		reverse_iterator rend()
		{
			return reverse_iterator(this, head);
		}
		const_reverse_iterator crend() const
		{
			return const_reverse_iterator(this, head);
		}

	private:
		void _safeDelete(Node* node)
		{
			node->prev->next = node->next;
			node->next->prev = node->prev;
			delete node;
		}
		void _safeDeleteAllChildren(Node* node) //until now leaves the parents next and the childs prev dangling
		{
			while (node->next != nullptr)
			{
				Node* temp = node->next;
				node->next = temp->next;
				delete temp;
			}
			delete node;
		}
	};
}