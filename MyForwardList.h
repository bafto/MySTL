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
				exception("ForwardList exception")
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
		class bad_iterator : public exception
		{
		public:
			bad_iterator()
				:
				exception("Tried to use bad iterator")
			{}
			bad_iterator(const char* msg)
				:
				exception(msg)
			{}
		};
	private:
		struct Node
		{
			T data;
			Node* next;
			Node(Node* next, const T& data = T())
				:
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
			using iterator_category = std::forward_iterator_tag;
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
				if (node == list->head || node == list->tail)
					throw out_of_bounds("Tried to dereference invalid element");
				return node->data;
			}
			pointer operator->() const
			{
				if (node == list->head || node == list->tail)
					throw out_of_bounds("Tried to access invalid element");
				return &node->data;
			}

			iterator& operator++()
			{
				if (node == list->tail)
					throw out_of_bounds("Tried to advance iterator past the end of the list");
				node = node->next;
				return *this;
			}
			iterator operator++(int)
			{
				iterator result(*this);
				if (node == list->tail)
					throw out_of_bounds("Tried to advance iterator past the end of the list");
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

		Node* head; //one element before the actual beginning
		Node* tail; //one element after the actual end
	public:
		MyForwardList()
			:
			tail(new Node(nullptr)),
			head(new Node(tail))
		{}
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
			_safeDeleteAllChildren(head);
		}

		template<class Iter>
		MyForwardList(Iter firstIt, Iter lastIt)
		{
			tail = new Node(nullptr);
			head = new Node(tail);
			iterator mIt = before_begin();
			for (auto it = firstIt; it != lastIt; ++it, ++mIt)
			{
				mIt.node->next = new Node(tail, *it);
			}
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
		MyForwardList(size_t size, const T& val = T())
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
				_safeDeleteAllChildren(head);

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
				_safeDeleteAllChildren(head);

				head = std::move(donor.head);
				tail = std::move(donor.tail);
				donor.head = nullptr;
				donor.tail = nullptr;
			}
			return *this;
		}

		MyForwardList<T>& operator=(std::initializer_list<T> list)
		{
			_safeDeleteAllChildren(head);

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
			_safeDeleteAllChildren(head);
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
			_validateIteratorPtr(position, this);
			if (position.node == tail)
				throw out_of_bounds("Tried to insert after the end");
			position.node->next = new Node(position.node->next, val);
			return ++position;
		}
		iterator insert_after(iterator position, T&& val)
		{
			_validateIteratorPtr(position, this);
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
			_validateIteratorPtr(position, this);

			for (auto it = list.begin(), stop = list.end(); it != stop; ++it)
			{
				position = insert_after(position, *it);
			}
			return position;
		}
		template<class Iter>
		iterator insert_after(iterator position, Iter firstIt, Iter lastIt)
		{
			_validateIteratorPtr(position);

			for (auto it = firstIt, stop = lastIt; it != stop; ++it)
			{
				position = insert_after(position, *it);
			}
			return position;
		}

		template<typename... args>
		iterator emplace_after(iterator position, args&&... vals)
		{
			return insert_after(position, T(vals...));
		}
		template<typename... args>
		void emplace_front(args&&... vals)
		{
			head->next = new Node(head->next, std::move(vals...));
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
			_validateIteratorPtr(position, this);
			if (position.node->next == nullptr)
				throw out_of_bounds("Tried to delete past the end of the list");
			position.node->next = _safeDelete(position.node->next);
			return ++position;
		}
		iterator erase_after(iterator firstIt, iterator lastIt)
		{
			_validateIteratorPtr(firstIt, this);
			_validateIteratorPtr(lastIt, this);
			if (lastIt.node == tail)
				throw out_of_bounds("Tried to erase_after end iterator");

			iterator test = firstIt;
			if (++test == lastIt)
				return lastIt;
			Node* moveNode = firstIt.node;
			iterator it = firstIt;
			for (auto next = ++firstIt; next != lastIt; ++it, ++next)
			{}
			it.node->next = nullptr;
			_safeDeleteAllChildren(firstIt.node);
			moveNode->next = lastIt.node;
			return iterator(this, moveNode);
		}

		//reverse the order
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
		void sort(std::function<bool(const T&, const T&)> Comp)
		{
			for (auto it = begin(), stop = end(); it != stop; ++it)
			{
				for (auto itt = begin(), jtt = ++begin(); jtt != stop; ++itt, ++jtt)
				{
					if (!Comp(*itt, *jtt))
					{
						T temp = *itt;
						*itt = *jtt;
						*jtt = temp;
					}
				}
			}
		}
		//mega big brain algorithm only partly from me
		void merge(MyForwardList<T>& fwdlst)
		{
			if (empty() && !fwdlst.empty())
			{
				head = fwdlst.head;
				tail = fwdlst.tail;
				fwdlst.tail = new Node(nullptr);
				fwdlst.head->next = fwdlst.tail;
				return;
			}
			else if (fwdlst.empty())
				return;

			sort();
			fwdlst.sort();

			Node* mergedHead = nullptr;
			Node* head1 = head->next, * head2 = fwdlst.head->next;
			if (head1->data <= head2->data)
			{
				mergedHead = head1;
				head1->next = head1->next;
			}
			else
			{
				mergedHead = head2;
				head2 = head2->next;
			}

			Node* mergedTail = mergedHead;

			while (head1 != tail && head2 != fwdlst.tail)
			{
				Node* temp = nullptr;
				if (head1->data <= head2->data)
				{
					temp = head1;
					head1 = head1->next;
				}
				else
				{
					temp = head2;
					head2 = head2->next;
				}
				mergedTail->next = temp;
				mergedTail = temp;
			}

			if (head1 != tail) {
				mergedTail->next = head1;
			}
			else if (head2 != fwdlst.tail) {
				mergedTail->next = head2;
			}

			head->next = mergedHead;
			fwdlst.head->next = fwdlst.tail;
		}
		void merge(MyForwardList<T>&& fwdlst)
		{
			merge(fwdlst);
		}
		void merge(MyForwardList<T>& fwdlst, std::function<bool(const T&, const T&)> Comp)
		{
			if (empty() && !fwdlst.empty())
			{
				head = fwdlst.head;
				tail = fwdlst.tail;
				fwdlst.tail = new Node(nullptr);
				fwdlst.head->next = fwdlst.tail;
				return;
			}
			else if (fwdlst.empty())
				return;

			sort(Comp);
			fwdlst.sort(Comp);

			Node* mergedHead = nullptr;
			Node* head1 = head->next, * head2 = fwdlst.head->next;
			if (Comp(head1->data, head2->data))
			{
				mergedHead = head1;
				head1->next = head1->next;
			}
			else
			{
				mergedHead = head2;
				head2 = head2->next;
			}

			Node* mergedTail = mergedHead;

			while (head1 != tail && head2 != fwdlst.tail)
			{
				Node* temp = nullptr;
				if (Comp(head1->data, head2->data))
				{
					temp = head1;
					head1 = head1->next;
				}
				else
				{
					temp = head2;
					head2 = head2->next;
				}
				mergedTail->next = temp;
				mergedTail = temp;
			}

			if (head1 != tail) {
				mergedTail->next = head1;
			}
			else if (head2 != fwdlst.tail) {
				mergedTail->next = head2;
			}

			head->next = mergedHead;
			fwdlst.head->next = fwdlst.tail;
		}
		void merge(MyForwardList<T>&& fwdlst, std::function<bool(const T&, const T&)> Comp)
		{
			merge(fwdlst, Comp);
		}
		//remove specific values
		void remove(const T& val)
		{
			for (auto it = before_begin(), stop = end(); it.node->next != stop.node;)
			{
				Node* node = it.node, *next = it.node->next;
				if (next->data == val)
					node->next = _safeDelete(next);
				else
					++it;
			}
		}
		void remove_if(std::function<bool(const T&)> Comp)
		{
			for (auto it = before_begin(), stop = end(); it.node->next != stop.node;)
			{
				Node* node = it.node, * next = it.node->next;
				if (Comp(next->data))
					node->next = _safeDelete(next);
				else
					++it;
			}
		}
		//why is this called splice_after? uh, nvm
		void splice_after(iterator position, MyForwardList<T>& fwdlst) //in work
		{
			_validateIteratorPtr(position, this);
			if (position.node == tail)
				throw out_of_bounds("Tried to insert after end of list");
			if (fwdlst.empty())
				return;
			else if (empty())
			{
				swap(fwdlst);
				return;
			}

			iterator it = fwdlst.before_begin();
			for(auto stop = fwdlst.end(); it.node->next != stop.node; ++it)
			{}
			it.node->next = position.node->next;
			position.node->next = fwdlst.head->next;
			fwdlst.head->next = fwdlst.tail;
		}
		void splice_after(iterator position, MyForwardList<T>&& fwdlst)
		{
			splice_after(position, fwdlst);
		}
		void splice_after(iterator position, MyForwardList<T>& fwdlst, iterator from)
		{
			_validateIteratorPtr(position, this);
			_validateIteratorPtr(from, &fwdlst);
			_validateIteratorRange(from, fwdlst.before_begin(), fwdlst.end());

			iterator it = fwdlst.before_begin();
			for(; it.node->next != from.node; ++it)
			{}
			it.node->next = from.node->next;
			from.node->next = position.node->next;
			position.node->next = from.node;
		}
		void splice_after(iterator position, MyForwardList<T>&& fwdlst, iterator from)
		{
			splice_after(position, fwdlst, from);
		}
		void splice_after(iterator position, MyForwardList<T>& fwdlst, iterator firstIt, iterator lastIt)
		{
			if (position.node == tail)
				throw out_of_bounds("Tried to use end iterator");
			_validateIteratorPtr(position, this);
			_validateIteratorPtr(firstIt, &fwdlst);
			_validateIteratorPtr(lastIt, &fwdlst);

			iterator test = firstIt;
			if (firstIt == lastIt)
				return;
			else if (++test == lastIt)
				return;

			iterator it = firstIt;
			for(; it.node->next != lastIt.node; ++it)
			{}
			it.node->next = position.node->next;
			position.node->next = firstIt.node->next;
			firstIt.node->next = lastIt.node;
		}
		void splice_after(iterator position, MyForwardList<T>&& fwdlst, iterator firstIt, iterator lastIt)
		{
			splice_after(position, fwdlst, firstIt, lastIt);
		}
		void unique()
		{
			for (auto it = begin(), itt = ++begin(), stop = end(); itt != stop;)
			{
				if (it.node->next->data == *it)
				{
					it.node->next = _safeDelete(it.node->next);
					itt.node = it.node->next;
				}
				else
				{
					++it;
					++itt;
				}
			}
		}
		void unique(std::function<bool(const T&, const T&)> Comp)
		{
			for (auto it = begin(), itt = ++begin(), stop = end(); itt != stop;)
			{
				if (Comp(*itt, *it))
				{
					it.node->next = _safeDelete(it.node->next);
					itt.node = it.node->next;
				}
				else
				{
					++it;
					++itt;
				}
			}
		}
	private:
		Node* _safeDelete(Node* node)
		{
			Node* next = node->next;
			delete node;
			return next;
		}
		void _safeDeleteAllChildren(Node* node)
		{
			Node* current = node;
			do
			{
				Node* next = current->next;
				delete current;
				current = next;
			} while (current != nullptr);
		}
		//check if the it points to list
		void _validateIteratorPtr(iterator& it, MyForwardList<T>* list)
		{
			if (it.list != list)
				throw bad_iterator("Tried to use iterator from wrong list");
		}
		//check if iterator is between firstIt and lastIt
		void _validateIteratorRange(iterator& it, iterator firstIt = before_begin(), iterator lastIt = end())
		{
			if (it.list != firstIt.list)
				throw bad_iterator("Tried to use iterator from wrong list");
			else if (it == firstIt || it == lastIt)
				throw out_of_bounds("Tried to use iterator out of bounds");
		}

	};
}