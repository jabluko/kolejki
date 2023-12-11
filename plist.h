#pragma once

#include <cstddef>
#include <memory>
#include <initializer_list>
#include <iterator>
#include "utility.h"

namespace plib
{
	template<class value_type, class allocator_type = std::allocator<value_type>>
	class list
	{
		struct node;
	public:

		using size_type = size_t;
		using diffrence_type = std::ptrdiff_t;

		using reference = value_type&;
		using const_reference = const reference;

		using pointer = std::allocator_traits<allocator_type>::pointer;
		using const_pointer = std::allocator_traits<allocator_type>::const_pointer;
		
		class iterator;
		class const_iterator;

		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		allocator_type allocator;

		node* _before_first;
		node* _past_last;
		size_type _size;

		list();

		template<typename otherallocator_type>
		list(const list<value_type, otherallocator_type>&);

		template<typename inputIt>
		list(const inputIt&, const inputIt&);

		list(const std::initializer_list<value_type>&);
		list(list&&);

		list& operator=(const list&);
		list& operator=(list&&);

		size_t size() const;

		iterator begin();
		const_iterator begin() const;
		const_iterator cbegin() const;
			
		iterator end();
		const_iterator end() const;
		const_iterator cend() const;

		reverse_iterator rbegin();
		const_reverse_iterator rbegin() const;
		const_reverse_iterator crbegin() const;

		reverse_iterator rend();
		const_reverse_iterator rend() const;
		const_reverse_iterator crend() const;
		
		template<typename ...Args>
		iterator emplace(const const_iterator& pos, Args... args);
		iterator insert(const const_iterator& pos, const value_type& value);
		
		template<typename _InputIterator>
		iterator insert(const const_iterator& pos, _InputIterator first, const _InputIterator& second);

		iterator insert(const const_iterator& pos, list&& value);
		iterator erase(const const_iterator& pos);

		iterator push_back(const value_type& value);
		iterator push_front(const value_type& value);

		//iterator push_back(list value);
		//iterator push_front(list value);

		void pop_back();
		void pop_front();
			
		void reverse();

private:
		struct node
		{
		private:
			friend class list;
			friend class iterator;
		public:
			value_type _value;
			node* _next[2];
		};
		template<typename ...Args>
		node* make_node(node *const previous = nullptr, node *const next = nullptr, Args&&... args); 

public:
		class iterator: public std::iterator<std::bidirectional_iterator_tag, value_type>
		{
		protected:
			friend class list;
			node* _previous;
			node* _current;
		public:
			iterator() = default;
			iterator(const iterator&) = default;
			iterator(iterator&&) = default;
			iterator(node *const, node *const);
				
			iterator& operator=(const iterator&) = default;
			iterator& operator=(iterator&&) = default;

			iterator get_next() const;
			iterator get_previous() const;

			iterator& operator++();
			iterator operator++(int);
			iterator& operator--();
			iterator operator--(int);
			reference operator*() const;
			pointer operator->() const;

			bool operator==(const iterator&) const;
			bool operator!=(const iterator&) const;

			~iterator() = default;
		};

		class const_iterator: public std::iterator<std::bidirectional_iterator_tag, const value_type>
		{
		protected:
			friend class list;
			const_iterator operator<=>(const const_iterator&) const;
			node* _previous;
			node* _current;
		public:
			const_iterator() = default;
			const_iterator(const const_iterator&) = default;
			const_iterator(const_iterator&&) = default;
			const_iterator(const iterator&);
			const_iterator(node *const, node *const);

			const_iterator& operator=(const const_iterator&) = default;
			const_iterator& operator=(const_iterator&&) = default;

			const_iterator get_next() const;
			const_iterator get_previous() const;

			const_iterator& operator++();
			const_iterator operator++(int);
			const_iterator& operator--();
			const_iterator operator--(int);
			const_reference operator*() const;
			const_pointer operator->() const;

			bool operator==(const const_iterator&) const;
			bool operator!=(const const_iterator&) const;

			~const_iterator() = default;
		};
	};

    template <class value_type, class allocator_type>
    inline list<value_type, allocator_type>::const_iterator list<value_type, allocator_type>::const_iterator::operator<=>(const const_iterator &next) const
    {
		if(_current)
		{	
			if(_current->_next[0] != _previous)
				_current->_next[0] = next._current;
			else
				_current->_next[1] = next._current;
		}

		if(next._current)
		{
			if(next._current->_next[0] == next._previous)
				next._current->_next[0] = _current;
			else
				next._current->_next[1] = _current;
		}
		return {_current, next._current};
    }

    template <class value_type, class allocator_type>
    inline list<value_type, allocator_type>::iterator::iterator(node *const previous, node *const current)
		: _previous{previous}, _current{current}
    { }

    template <class value_type, class allocator_type>
    inline typename list<value_type, allocator_type>::iterator list<value_type, allocator_type>::iterator::get_next() const
    { return {_current, (_previous != _current->_next[0]) ? (_current->_next[0]) : (_current->_next[1])}; }

    template <class value_type, class allocator_type>
    inline typename list<value_type, allocator_type>::iterator list<value_type, allocator_type>::iterator::get_previous() const
    { return {(_current != _previous->_next[0]) ? (_previous->_next[0]) : (_previous->_next[1]), _previous}; }

    template <class value_type, class allocator_type>
    inline typename list<value_type, allocator_type>::iterator &list<value_type, allocator_type>::iterator::operator++()
    { return (*this) = this->get_next(); }

	template<class value_type, class allocator_type>
	inline typename list<value_type, allocator_type>::iterator list<value_type, allocator_type>::iterator::operator++(int)
	{
		auto copy = *this;
		++(*this);
		return copy;
	}
	
	template<class value_type, class allocator_type>
	inline typename list<value_type, allocator_type>::iterator& list<value_type, allocator_type>::iterator::operator--()
	{ return (*this) = this->get_previous(); }

	template<class value_type, class allocator_type>
	inline typename list<value_type, allocator_type>::iterator list<value_type, allocator_type>::iterator::operator--(int)
	{
		auto copy = *this;
		++(*this);
		return copy;
	}

	template<class value_type, class allocator_type>
	inline typename list<value_type, allocator_type>::reference list<value_type, allocator_type>::iterator::operator*() const
	{ return this->_current->_value; }

	template<class value_type, class allocator_type>
	inline typename list<value_type, allocator_type>::pointer list<value_type, allocator_type>::iterator::operator->() const
	{ return &_current->_value; }

    template <class value_type, class allocator_type>
    inline bool list<value_type, allocator_type>::iterator::operator==(const typename list<value_type, allocator_type>::iterator& other) const
    { return _current == other._current; }

    template <class value_type, class allocator_type>
    inline bool list<value_type, allocator_type>::iterator::operator!=(const typename list<value_type, allocator_type>::iterator& other) const
    { return _current != other._current; }

    template <class value_type, class allocator_type>
    inline list<value_type, allocator_type>::const_iterator::const_iterator(const iterator& other)
		:_previous{other._previous}, _current{other._current}
    {
    }

    template <class value_type, class allocator_type>
    inline list<value_type, allocator_type>::const_iterator::const_iterator(node *const previous, node *const current)
        : _previous{previous}, _current{current}
    { }

    template <class value_type, class allocator_type>
    inline typename list<value_type, allocator_type>::const_iterator list<value_type, allocator_type>::const_iterator::get_next() const
    { return {_current, (_previous != _current->_next[0]) ? (_current->_next[0]) : (_current->_next[1])}; }

    template <class value_type, class allocator_type>
    inline typename list<value_type, allocator_type>::const_iterator list<value_type, allocator_type>::const_iterator::get_previous() const
    { return {(_current != _previous->_next[0]) ? (_previous->_next[0]) : (_previous->_next[1]), _previous}; }

    template <class value_type, class allocator_type>
    inline typename list<value_type, allocator_type>::const_iterator& list<value_type, allocator_type>::const_iterator::operator++()
    { return (*this) = this->get_next(); }

    template <class value_type, class allocator_type>
    inline typename list<value_type, allocator_type>::const_iterator list<value_type, allocator_type>::const_iterator::operator++(int)
    {
        auto copy = *this;
		++(*this);
		return copy;
    }

    template <class value_type, class allocator_type>
    inline typename list<value_type, allocator_type>::const_iterator& list<value_type, allocator_type>::const_iterator::operator--()
    { return (*this) = this->get_previous(); }

    template <class value_type, class allocator_type>
    inline typename list<value_type, allocator_type>::const_iterator list<value_type, allocator_type>::const_iterator::operator--(int)
    {
        auto copy = *this;
		--(*this);
		return copy;
    }

    template <class value_type, class allocator_type>
    inline typename list<value_type, allocator_type>::const_reference list<value_type, allocator_type>::const_iterator::operator*() const
    { return this->_current->_value; }

	template<class value_type, class allocator_type>
    inline typename list<value_type, allocator_type>::const_pointer list<value_type, allocator_type>::const_iterator::operator->() const
    { return &(this->_current->_value); }

    template <class value_type, class allocator_type>
    inline bool list<value_type, allocator_type>::const_iterator::operator==(const typename list<value_type, allocator_type>::const_iterator& other) const
    { return _current == other._current; }

    template <class value_type, class allocator_type>
    inline bool list<value_type, allocator_type>::const_iterator::operator!=(const typename list<value_type, allocator_type>::const_iterator& other) const
    { return _current != other._current; }

	template<class value_type, class allocator_type>
	inline list<value_type, allocator_type>::list(): _size(0)
	{
		node* buf = new node[2];
		(this->_before_first) = buf;
		(this->_past_last) = buf + 1;

		this->_before_first->_next[0] = nullptr;
		this->_before_first->_next[1] = _past_last;

		this->_past_last->_next[0] = _before_first;
		this->_past_last->_next[1] = nullptr;
	}

    template <class value_type, class allocator_type>
    template <typename... Args>
    inline typename list<value_type, allocator_type>::node* list<value_type, allocator_type>::make_node(node *const previous, node *const next, Args &&...args)
    { return new node({._value = (*allocator.allocate(1) = value_type(args...)), ._next = {previous, next}}); }

    template <class value_type, class allocator_type>
    inline list<value_type, allocator_type>::list(const std::initializer_list<value_type>&  it): list(it.begin(), it.end())
    { }

    template <class value_type, class allocator_type>
    inline list<value_type, allocator_type>::list(list &&l) : _before_first{l._before_first}, _past_last{l._past_last}
    { }

    template<class value_type, class allocator_type>
	inline typename list<value_type, allocator_type>::iterator list<value_type, allocator_type>::begin()
	{ return (typename list<value_type, allocator_type>::iterator{nullptr, _before_first}).get_next(); }

	template<class value_type, class allocator_type>
    inline typename list<value_type, allocator_type>::const_iterator list<value_type, allocator_type>::begin() const
    { return (typename list<value_type, allocator_type>::const_iterator{nullptr, _before_first}).get_next(); }

	template<class value_type, class allocator_type>
    inline typename list<value_type, allocator_type>::const_iterator list<value_type, allocator_type>::cbegin() const
    { return (typename list<value_type, allocator_type>::const_iterator{nullptr, _before_first}).get_next(); }

	template<class value_type, class allocator_type>
    inline typename list<value_type, allocator_type>::iterator list<value_type, allocator_type>::end()
    { return (typename list<value_type, allocator_type>::iterator{_past_last, nullptr}).get_previous(); }

	template<class value_type, class allocator_type>
    inline typename list<value_type, allocator_type>::const_iterator list<value_type, allocator_type>::end() const
    { return (typename list<value_type, allocator_type>::const_iterator{_past_last, nullptr}).get_previous(); }

	template<class value_type, class allocator_type>
    inline typename list<value_type, allocator_type>::const_iterator list<value_type, allocator_type>::cend() const
    { return (typename list<value_type, allocator_type>::const_iterator{_past_last, nullptr}).get_previous(); }

    template<class value_type, class allocator_type>
	inline typename list<value_type, allocator_type>::reverse_iterator list<value_type, allocator_type>::rbegin()
	{ return typename list<value_type, allocator_type>::reverse_iterator(end()); }
    template<class value_type, class allocator_type>
	inline typename list<value_type, allocator_type>::const_reverse_iterator list<value_type, allocator_type>::rbegin() const
	{ return typename list<value_type, allocator_type>::const_reverse_iterator(end()); }
    template<class value_type, class allocator_type>
	inline typename list<value_type, allocator_type>::const_reverse_iterator list<value_type, allocator_type>::crbegin() const
	{ return typename list<value_type, allocator_type>::const_reverse_iterator(cend()); }

    template<class value_type, class allocator_type>
	inline typename list<value_type, allocator_type>::reverse_iterator list<value_type, allocator_type>::rend()
	{ return typename list<value_type, allocator_type>::reverse_iterator(begin()); }
    template<class value_type, class allocator_type>
	inline typename list<value_type, allocator_type>::const_reverse_iterator list<value_type, allocator_type>::rend() const
	{ return typename list<value_type, allocator_type>::const_reverse_iterator(begin()); }
    template<class value_type, class allocator_type>
	inline typename list<value_type, allocator_type>::const_reverse_iterator list<value_type, allocator_type>::crend() const
	{ return typename list<value_type, allocator_type>::const_reverse_iterator(cbegin()); }

    template <class value_type, class allocator_type>
    template <typename inputIt>
    inline list<value_type, allocator_type>::list(const inputIt& bgn, const inputIt& end)
    {
		diffrence_type ssize= std::distance(bgn, end);
		if constexpr (std::is_signed_v<diffrence_type>)
			assert(ssize >= 0);

		_size = (size_type)(ssize);

		node** buf = new node*[_size + 2];

		inputIt it = bgn;
		for(size_type i = 1; i <= _size; ++i)
			buf[i] = make_node(nullptr, nullptr, *it);
		for(size_type i = 0; i < _size + 2; ++i)
		{
			buf[i]->_next[0] = (i != 0) ? buf[i - 1] : nullptr;
			buf[i]->_next[1] = (i != _size + 1) ? buf[i + 1] : nullptr;
		}
		_before_first = buf[0];
		_past_last = buf[_size + 1];
    }

    template <class value_type, class allocator_type>
    template <typename... Args>
    inline typename list<value_type, allocator_type>::iterator list<value_type, allocator_type>::emplace(const const_iterator &pos, Args... args)
    {
		++_size;
		list<value_type, allocator_type>::iterator new_node(nullptr, make_node(nullptr, nullptr, args...));

		pos.get_previous() <=> new_node <=> pos; 
		return new_node;
    }

	template<class value_type, class allocator_type>
	inline typename list<value_type, allocator_type>::iterator list<value_type, allocator_type>::insert
		(const typename list<value_type, allocator_type>::const_iterator& pos, const value_type &value)
	{ return emplace(pos, value); }

	template<class value_type, class allocator_type>
	inline typename list<value_type, allocator_type>::iterator list<value_type, allocator_type>::push_back(const value_type &value)
	{ return insert(end(), value); }

	template<class value_type, class allocator_type>
	inline typename list<value_type, allocator_type>::iterator list<value_type, allocator_type>::push_front(const value_type &value)
	{ return insert(begin(), value); }


}