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
		class bad_iterator : public exception
		{
		public:
			bad_iterator()
				:
				exception("Bad iterator")
			{}
			bad_iterator(const char* message)
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
				clear();
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
				clear();
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
			clear();
			Node* current = head;
			for (auto it = list.begin(), stop = list.end(); it != stop; ++it)
			{
				Node* node = new Node(current, tail, *it);
				current->next = node;
				tail->prev = node;
				current = node;
			}
			v_size = list.size();
			return *this;
		}

		void assign(size_t size, const T& val)
		{
			clear();
			Node* current = head;
			for (size_t i = 0; i < size; ++i)
			{
				Node* node = new Node(current, tail, val);
				current->next = node;
				tail->prev = node;
				current = node;
			}
			v_size = size;
		}
		template<class Iter>
		void assign(Iter firstIt, Iter lastIt)
		{
			clear();
			Node* current = head;
			size_t count = 0;
			for (auto it = firstIt; it != lastIt; ++it)
			{
				Node* node = new Node(current, tail, *it);
				current->next = node;
				tail->prev = node;
				current = node;
				count++;
			}
			v_size = count;
		}
		void assign(std::initializer_list<T> list)
		{
			clear();
			Node* current = head;
			for (auto it = list.begin(), stop = list.end(); it != stop; ++it)
			{
				Node* node = new Node(current, tail, *it);
				current->next = node;
				tail->prev = node;
				current = node;
			}
			v_size = list.size();
		}

		void swap(MyList<T>& list)
		{
			Node* tempHead = head;
			Node* tempTail = tail;

			head = list.head;
			tail = list.tail;

			list.head = tempHead;
			list.tail = tempTail;
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

		void resize(size_t size, const T& val = T())
		{
			if (v_size == size)
				return;
			else if (v_size > size)
			{
				iterator it = begin();
				for (size_t i = 0; i < size; i++, ++it)
				{}
				Node* toDelete = it.node;
				it.node->prev->next = tail;
				tail->prev->next = nullptr;
				tail->prev = it.node->prev;
				_safeDeleteAllChildren(toDelete);
				v_size = size;
			}
			else if (v_size < size)
			{
				for (size_t i = v_size; i < size; i++)
				{
					Node* node = new Node(tail->prev, tail, val);
					tail->prev->next = node;
					tail->prev = node;
				}
				v_size = size;
			}
		}

		void clear()
		{
			tail->prev->next = nullptr;
			_safeDeleteAllChildren(head->next);
			head->next = tail;
			v_size = 0;
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

		iterator insert(iterator position, const T& val)
		{
			_validateIterator(position);
			Node* old = position.node;
			Node* node = new Node(old->prev, old, val);
			old->prev->next = node;
			old->prev = node;
			v_size++;
			return iterator(this, node);
		}
		iterator insert(iterator position, T&& val)
		{
			_validateIterator(position);
			Node* old = position.node;
			Node* node = new Node(old->prev, old, std::move(val));
			old->prev->next = node;
			old->prev = node;
			v_size++;
			return iterator(this, node);
		}
		iterator insert(iterator position, size_t count, const T& val)
		{
			_validateIterator(position);
			for (size_t i = 0; i < count; i++)
			{
				position = insert(position, val);
			}
			v_size += count;
			return position;
		}
		iterator insert(iterator position, std::initializer_list<T> list)
		{
			_validateIterator(position);
			iterator ret = insert(position, *(list.begin()));
			for (auto it = list.begin() + 1, stop = list.end(); it != stop; ++it)
			{
				insert(position, *it);
			}
			v_size += list.size();
			return ret;
		}
		template<class Iter>
		iterator insert(iterator position, Iter firstIt, Iter lastIt)
		{
			_validateIterator(position);
			size_t count = 0;
			iterator ret = insert(position, *firstIt);;
			for (auto it = ++firstIt; it != lastIt; ++it)
			{
				insert(position, *it);
				count++;
			}
			v_size += count;
			return ret;
		}

		template<typename... args>
		iterator emplace(iterator position, args&&... vals)
		{
			return insert(position, T(std::move(vals...)));
		}
		template<typename... args>
		void emplace_back(args&&... vals)
		{
			insert(end(), std::move(vals...));
		}
		template<typename... args>
		void emplace_front(args&&... vals)
		{
			insert(begin(), std::move(vals...));
		}

		void push_back(const T& val)
		{
			insert(end(), val);
		}
		void push_back(T&& val)
		{
			insert(end(), std::move(val));
		}
		void push_front(const T& val)
		{
			insert(begin(), val);
		}
		void push_front(T&& val)
		{
			insert(begin(), std::move(val));
		}

		void pop_back()
		{
			_safeDelete(tail->prev);
		}
		void pop_front()
		{
			_safeDelete(head->next);
		}

		void erase(iterator position)
		{
			if (position.node == head || position.node == tail)
				throw out_of_bounds("Tried to delete element out of bounds");
			_validateIterator(position);
			_safeDelete(position.node);
		}
		iterator erase(iterator firstIt, iterator lastIt)
		{
			_validateIterator(firstIt);
			_validateIterator(lastIt);
			firstIt.node->prev->next = lastIt.node;
			lastIt.node->prev->next = nullptr;
			lastIt.node->prev = firstIt.node->prev;
			_safeDeleteAllChildren(firstIt.node);
			return lastIt;
		}

		void splice(iterator position, MyList<T>& list)
		{
			if (list.empty() || &list == this)
				return;
			_validateIterator(position);
			list.tail->prev->next = position.node;
			list.head->next->prev = position.node->prev;
			position.node->prev->next = list.head->next;
			position.node->prev = list.tail->prev;
			list.head->next = list.tail;
			list.tail->prev = list.head;
			v_size += list.v_size;
			list.v_size = 0;
		}
		void splice(iterator position, MyList<T>&& list)
		{
			splice(position, list);
		}
		void splice(iterator position, MyList<T>& list, iterator from)
		{
			_validateIterator(position);
			_validateIterator(from, &list);
			from.node->next->prev = from.node->prev;
			from.node->prev->next = from.node->next;
			position.node->prev->next = from.node;
			from.node->prev = position.node->prev;
			position.node->prev = from.node;
			from.node->next = position.node;
		}
		void splice(iterator position, MyList<T>&& list, iterator from)
		{
			splice(position, list, from);
		}
		void splice(iterator position, MyList<T>& list, iterator firstIt, iterator lastIt)
		{
			_validateIterator(position);
			_validateIterator(firstIt, &list);
			_validateIterator(lastIt, &list);
			position.node->prev->next = firstIt.node;
			firstIt.node->prev->next = lastIt.node;
			firstIt.node->prev = position.node->prev;
			position.node->prev = lastIt.node->prev;
			lastIt.node->prev->next = position.node;
			lastIt.node->prev = firstIt.node->prev;
		}
		void splice(iterator position, MyList<T>&& list, iterator firstIt, iterator lastIt)
		{
			splice(position, list, firstIt, lastIt);
		}

		void remove(const T& val)
		{
			Node* current = head->next;
			while( current != tail)
			{
				if (current->data == val)
				{
					Node* del = current;
					current = current->next;
					_safeDelete(del);
				}
				else
					current = current->next;
			}
		}
		void remove_if(std::function<bool(T val)> Comp)
		{
			Node* current = head->next;
			while ( current != tail)
			{
				if (Comp(current->data))
				{
					Node* del = current;
					current = current->next;
					_safeDelete(del);
				}
				else
					current = current->next;
			}
		}

		void unique()
		{
			Node* current = head->next;
			while (current != tail)
			{
				if (current->next->data == current->data)
				{
					_safeDelete(current->next);
				}
				else
				{
					current = current->next;
				}
			}
		}
		void unique(std::function<bool(T val)> Comp)
		{
			Node* current = head->next;
			while (current != tail)
			{
				if (Comp(current->next->data, current->data))
				{
					_safeDelete(current->next);
				}
				else
				{
					current = current->next;
				}
			}
		}
	private:
		void _safeDelete(Node* node)
		{
			node->prev->next = node->next;
			node->next->prev = node->prev;
			delete node;
			v_size--;
		}
		void _safeDeleteAllChildren(Node* node) //until now leaves the parents next and the childs prev dangling
		{
			size_t count = 0;
			while (node->next != nullptr)
			{
				Node* temp = node->next;
				node->next = temp->next;
				delete temp;
				count++;
			}
			delete node;
			v_size -= ++count;
		}
		void _validateIterator(const iterator& it)
		{
			if (it.list != this)
				throw bad_iterator("Tried to pass iterator of other list");
		}
		void _validateIterator(const iterator& it, MyList<T>* list)
		{
			if (it.list != list)
				throw bad_iterator("Tried to pass iterator from wrong list");
		}
	};
}