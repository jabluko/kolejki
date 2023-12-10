#pragma once

#include <cstddef>
#include <memory>
#include <initializer_list>
#include "utility.h"

namespace plib
{
	template<class T, class Allocator = std::allocator<T>>
	class list
	{
	public:
		using value_type = T;
		using allocator_type = Allocator;
		using size_type = size_t;
		using diffrence_type = std::ptrdiff_t;

		using reference = value_type&;
		using const_reference = const reference;

		using pointer = std::allocator_traits<Allocator>::pointer;
		using const_pointer = std::allocator_traits<Allocator>::const_pointer;
		
		class iterator;
		class const_iterator;
	protected:
		struct node;
	public:
	


		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		node* _before_first;
		node* _past_last;
		size_type _size;

		list();

		template<typename otherAllocator>
		list(const list<T, otherAllocator>&);

		template<typename container = std::initializer_list<value_type>>
		list(const container&);
		list(list&&);
		constexpr list(iterator bgn, iterator end);

		list& operator=(const list&);
		list& operator=(list&&);

		constexpr size_t size() const;
		constexpr bool forward() const;

		constexpr iterator begin();
		constexpr const_iterator begin() const;
		constexpr const_iterator cbegin() const;
		
		constexpr iterator end();
		constexpr const_iterator end() const;
		constexpr const_iterator cend() const;

		constexpr reverse_iterator rbegin();
		constexpr const_reverse_iterator rbegin() const;
		constexpr const_reverse_iterator crbegin() const;

		constexpr reverse_iterator rend();
		constexpr const_reverse_iterator rend() const;
		constexpr const_reverse_iterator crend() const;
		
		iterator insert(const const_iterator& pos, const T& value);
		
		template<typename _InputIterator>
		constexpr iterator insert(const const_iterator& pos, _InputIterator first, const _InputIterator& second);

		constexpr iterator insert(const const_iterator& pos, list&& value);
		iterator erase(const const_iterator& pos);

		constexpr iterator push_back(const T& value);
		constexpr iterator push_front(const T& value);

		constexpr iterator push_back(list value);
		constexpr iterator push_front(list value);

		constexpr void pop_back();
		constexpr void pop_front();
		
		constexpr void reverse();

protected:
		struct node
		{
		private:
			friend class list;
			friend class iterator;
		public:
			value_type _value;
			node* _next[2];

			node(value_type value = value_type(), node* left = nullptr, node* right = nullptr, bool forward = 0);
		};
public:
		class iterator: public std::iterator<std::bidirectional_iterator_tag, value_type>
		{
		protected:
			friend class list;
			node* _current;
			bool _direction;
		public:
			constexpr iterator() = default;
			constexpr iterator(const iterator&) = default;
			constexpr iterator(iterator&&) = default;
			constexpr iterator(node*, bool = 0);
			
			constexpr iterator& operator=(const iterator&) = default;
			constexpr iterator& operator=(iterator&&) = default;

			constexpr iterator& operator++();
			constexpr iterator operator++(int);
			constexpr iterator& operator--();
			constexpr iterator operator--(int);
			constexpr reference operator*() const;
			constexpr pointer operator->() const;

			constexpr bool operator==(const iterator&) const;
			constexpr bool operator!=(const iterator&) const;

			constexpr ~iterator() = default;
		};

		class const_iterator: public std::iterator<std::bidirectional_iterator_tag, const value_type>
		{
		protected:
			friend class list;
			constexpr const const_iterator& operator<=>(const const_iterator&) const;
			node* _current;
			bool _direction;
		public:
			constexpr const_iterator() = default;
			constexpr const_iterator(const const_iterator&) = default;
			constexpr const_iterator(const_iterator&&) = default;
			constexpr const_iterator(const iterator&);
			constexpr const_iterator(node* const, bool = 0);

			constexpr const_iterator& operator=(const const_iterator&) = default;
			constexpr const_iterator& operator=(const_iterator&&) = default;

			constexpr const_iterator& operator++();
			constexpr const_iterator operator++(int);
			constexpr const_iterator& operator--();
			constexpr const_iterator operator--(int);
			constexpr const_reference operator*() const;
			constexpr const_pointer operator->() const;

			constexpr bool operator==(const const_iterator&) const;
			constexpr bool operator!=(const const_iterator&) const;

			constexpr ~const_iterator() = default;
		};
	};

    template <class T, class Allocator>
    inline list<T, Allocator>::node::node(value_type value, node *left, node *right, bool forward): _value{value}
    {
		_next[forward] = left;
		_next[!forward] = right;
    }

    template <class T, class Allocator>
    inline constexpr const list<T, Allocator>::const_iterator& list<T, Allocator>::const_iterator::operator<=>(const const_iterator &_next) const
    {
        if(this->_current)
			this->_current->_next[this->_direction] = _next._current;
		if(_next._current)
			_next._current->_next[!this->_direction] = this->_current;
		return _next;
    }

    template <class T, class Allocator>
    inline constexpr list<T, Allocator>::iterator::iterator(node* current, bool direction): _current{current}, _direction{direction}
    {}

    template <class T, class Allocator>
    inline constexpr typename list<T, Allocator>::iterator& list<T, Allocator>::iterator::operator++()
    {
		_current = _current->_next[_direction];
		return *this;
	}

	template<class T, class Allocator>
	inline constexpr typename list<T, Allocator>::iterator list<T, Allocator>::iterator::operator++(int)
	{
		auto copy = *this;
		++(*this);
		return copy;
	}
	

	template<class T, class Allocator>
	inline constexpr typename list<T, Allocator>::iterator& list<T, Allocator>::iterator::operator--()
	{
		_current = _current->_next[!_direction];
		return *this;
	}

	template<class T, class Allocator>
	inline constexpr typename list<T, Allocator>::iterator list<T, Allocator>::iterator::operator--(int)
	{
		auto copy = *this;
		++(*this);
		return copy;
	}

	template<class T, class Allocator>
	inline constexpr typename list<T, Allocator>::reference list<T, Allocator>::iterator::operator*() const
	{ return this->_current->_value; }

	template<class T, class Allocator>
	inline constexpr typename list<T, Allocator>::pointer list<T, Allocator>::iterator::operator->() const
	{ return &_current->_value; }

    template <class T, class Allocator>
    inline constexpr bool list<T, Allocator>::iterator::operator==(const typename list<T, Allocator>::iterator& other) const
    {
        return _current == other._current;
    }

    template <class T, class Allocator>
    inline constexpr bool list<T, Allocator>::iterator::operator!=(const typename list<T, Allocator>::iterator& other) const
    {
        return _current != other._current;
    }

    template <class T, class Allocator>
    inline constexpr list<T, Allocator>::const_iterator::const_iterator(const iterator &other) : _current{other._current}, _direction{other._direction}
    { }

    template <class T, class Allocator>
    inline constexpr list<T, Allocator>::const_iterator::const_iterator(node *const current, bool direction): _current{current}, _direction{direction}
    { }

    template <class T, class Allocator>
    inline constexpr typename list<T, Allocator>::const_iterator& list<T, Allocator>::const_iterator::operator++()
    {
        _current = _current->_next[_direction];
		return *this;
    }

    template <class T, class Allocator>
    inline constexpr typename list<T, Allocator>::const_iterator list<T, Allocator>::const_iterator::operator++(int)
    {
        auto copy = *this;
		++(*this);
		return copy;
    }

    template <class T, class Allocator>
    inline constexpr typename list<T, Allocator>::const_iterator& list<T, Allocator>::const_iterator::operator--()
    {
        _current = _current->_next[!_direction];
		return *this;
    }

    template <class T, class Allocator>
    inline constexpr typename list<T, Allocator>::const_iterator list<T, Allocator>::const_iterator::operator--(int)
    {
        auto copy = *this;
		--(*this);
		return copy;
    }

    template <class T, class Allocator>
    inline constexpr typename list<T, Allocator>::const_reference list<T, Allocator>::const_iterator::operator*() const
    { return this->_current->_value; }

	template<class T, class Allocator>
    inline constexpr typename list<T, Allocator>::const_pointer list<T, Allocator>::const_iterator::operator->() const
    { return &(this->_current->_value); }

    template <class T, class Allocator>
    inline constexpr bool list<T, Allocator>::const_iterator::operator==(const typename list<T, Allocator>::const_iterator& other) const
    {
        return _current == other._current;
    }

    template <class T, class Allocator>
    inline constexpr bool list<T, Allocator>::const_iterator::operator!=(const typename list<T, Allocator>::const_iterator& other) const
    {
        return _current != other._current;
    }

    template<class T, class Allocator>
    inline constexpr size_t list<T, Allocator>::size() const
    { return _size; }

	template<class T, class Allocator>
    inline constexpr bool list<T, Allocator>::forward() const
    { return (_before_first->_next[1] != nullptr); }

	template<class T, class Allocator>
	inline list<T, Allocator>::list(): _size(0)
	{
		node* buf = new node[2];
		(this->_before_first) = buf;
		(this->_past_last) = buf + 1;

		this->_before_first->_next[0] = nullptr;
		this->_before_first->_next[1] = _past_last;

		this->_past_last->_next[0] = _before_first;
		this->_past_last->_next[1] = nullptr;
	}

    template <class T, class Allocator>
    template <typename container>
    inline list<T, Allocator>::list(const container &to_copy) : _size(to_copy.size())
    {
		node* tab = new node[_size + 2];

		_before_first = tab;
		_before_first->_next[0] = nullptr;
		_before_first->_next[1] = tab + 1;

		_past_last = tab + _size + 1;
		_past_last->_next[0] = tab + _size;
		_past_last->_next[1] = nullptr;

		size_t i = 1;
		for(const auto& x : to_copy)
		{
			tab[i]._value = x;
			tab[i]._next[0] = tab + i - 1;
			tab[i]._next[1] = tab + i + 1;
			++i;
		}
    }

	template<class T, class Allocator>
    inline list<T, Allocator>::list(list&& l): _before_first{l._before_first}, _past_last{l._past_last}
    {}

    template<class T, class Allocator>
	inline constexpr typename list<T, Allocator>::iterator list<T, Allocator>::begin()
	{ return {_before_first->_next[forward()], forward()}; }

	template<class T, class Allocator>
    inline constexpr typename list<T, Allocator>::const_iterator list<T, Allocator>::begin() const
    { return typename list<T, Allocator>::const_iterator(_before_first->_next[forward()], forward()); }

	template<class T, class Allocator>
    inline constexpr typename list<T, Allocator>::const_iterator list<T, Allocator>::cbegin() const
    { return typename list<T, Allocator>::const_iterator(_before_first->_next[forward()], forward()); }

	template<class T, class Allocator>
    inline constexpr typename list<T, Allocator>::iterator list<T, Allocator>::end()
    { return typename list<T, Allocator>::iterator(_past_last, forward()); }

	template<class T, class Allocator>
    inline constexpr typename list<T, Allocator>::const_iterator list<T, Allocator>::end() const
    { return typename list<T, Allocator>::const_iterator(_past_last, forward()); }

	template<class T, class Allocator>
    inline constexpr typename list<T, Allocator>::const_iterator list<T, Allocator>::cend() const
    { return typename list<T, Allocator>::const_iterator(_past_last, forward()); }

    template<class T, class Allocator>
	inline constexpr typename list<T, Allocator>::reverse_iterator list<T, Allocator>::rbegin()
	{ return typename list<T, Allocator>::reverse_iterator(end()); }
    template<class T, class Allocator>
	inline constexpr typename list<T, Allocator>::const_reverse_iterator list<T, Allocator>::rbegin() const
	{ return typename list<T, Allocator>::const_reverse_iterator(end()); }
    template<class T, class Allocator>
	inline constexpr typename list<T, Allocator>::const_reverse_iterator list<T, Allocator>::crbegin() const
	{ return typename list<T, Allocator>::const_reverse_iterator(cend()); }

    template<class T, class Allocator>
	inline constexpr typename list<T, Allocator>::reverse_iterator list<T, Allocator>::rend()
	{ return typename list<T, Allocator>::reverse_iterator(begin()); }
    template<class T, class Allocator>
	inline constexpr typename list<T, Allocator>::const_reverse_iterator list<T, Allocator>::rend() const
	{ return typename list<T, Allocator>::const_reverse_iterator(begin()); }
    template<class T, class Allocator>
	inline constexpr typename list<T, Allocator>::const_reverse_iterator list<T, Allocator>::crend() const
	{ return typename list<T, Allocator>::const_reverse_iterator(cbegin()); }

	template<class T, class Allocator>
	inline typename list<T, Allocator>::iterator list<T, Allocator>::insert(const typename list<T, Allocator>::const_iterator& pos, const T &value)
	{
		++_size;
		list<T, Allocator>::iterator new_node(new node(value), pos._direction);

		auto prev = pos;
		--prev;

		prev <=> new_node <=> pos; 
		return new_node;
	}	

	template<class T, class Allocator>
	inline constexpr typename list<T, Allocator>::iterator list<T, Allocator>::push_back(const T &value)
	{ return insert(end(), value); }

	template<class T, class Allocator>
	inline constexpr typename list<T, Allocator>::iterator list<T, Allocator>::push_front(const T &value)
	{ return insert(begin(), value); }


}