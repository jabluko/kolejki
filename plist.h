#pragma once

#include <cstddef>
#include <memory>
#include "utility.h"

namespace plib
{
	template<class T, class Allocator = std::allocator<T>>
	class list
	{
	protected:
		struct node
		{
		private:
			friend class list;
			friend class iterator;
		protected:
			T value;
			node* _next[2];
			node*& next(bool x);
		};
		

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

		class iterator
		{
		private:
			friend class list;
			//constexpr operator node*();
			constexpr void operator<=>(const iterator&) const;
		public:
			node* _current;
			bool _direction;
			constexpr iterator() = default;
			constexpr iterator(const iterator&) = default;
			constexpr iterator(iterator&&) = default;
			constexpr iterator(pointer&&, bool = 0);
			
			constexpr iterator& operator=(const iterator&) = default;
			constexpr iterator& operator=(iterator&&) = default;

			constexpr iterator operator++();
			constexpr iterator operator++(int);
			constexpr iterator operator--();
			constexpr iterator operator--(int);
			constexpr reference operator*() const;
			constexpr pointer operator->() const;

			constexpr ~iterator() = default;
		};

		class const_iterator: std::iterator<std::bidirectional_iterator_tag, const node>
		{
		public:
			node* _current;
			bool _direction;
			constexpr const_iterator() = default;
			constexpr const_iterator(const const_iterator&) = default;
			constexpr const_iterator(const_iterator&&) = default;
			constexpr const_iterator(const_pointer&&, bool = 0);

			constexpr const_iterator& operator=(const const_iterator&) = default;
			constexpr const_iterator& operator=(const_iterator&&) = default;

			constexpr const_iterator operator++();
			constexpr const_iterator operator++(int);
			constexpr const_iterator operator--();
			constexpr const_iterator operator--(int);
			constexpr const_reference operator*() const;// override;
			constexpr const_pointer operator->() const;// override;

			constexpr ~iterator() = default;
		};


		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		node* _bgn;
		node* _end;
		size_type _size;

		list();
		list(const list&);
		list(list&&);
		constexpr list(iterator bgn, iterator end);

		list& operator=(const list&);
		list& operator=(list&&);

		constexpr size_t size() const;
		constexpr bool rotation() const;


		constexpr iterator begin();
		constexpr const_iterator begin() const;
		
		constexpr iterator end();
		constexpr const_iterator end() const;
		
		iterator insert(const const_iterator& pos, const T& value);
		//constexpr iterator insert(const iterator& pos, node& value);
		constexpr iterator insert(const const_iterator& pos, const const_iterator& value);
		constexpr iterator insert(const const_iterator& pos, list&& value);
		iterator erase(const const_iterator& pos);

		constexpr iterator push_back(const T& value);
		constexpr iterator push_front(const T& value);

		constexpr iterator push_back(list value);
		constexpr iterator push_front(list value);

		constexpr void pop_back();
		constexpr void pop_front();
		
		constexpr void reverse();
	};

    /*template <typename T>
    inline constexpr list<T, Allocator>::iterator::iterator(pointer _current, bool _direction): current{_current}, direction{_direction}
    {}*/
    template <class T, class Allocator>
    inline list<T, Allocator>::node*& list<T, Allocator>::node::next(bool x)
    { return this->next(x); }

	/*
	template<class T, class Allocator>
	inline constexpr list<T, Allocator>::iterator::operator node*()
	{ return this->current; }
	*/

    template <class T, class Allocator>
    inline constexpr void list<T, Allocator>::iterator::operator<=>(const iterator &_next) const
    {
        if(this->_current)
			this->_current->next(this->_direction) = _next._current;
		if(_next._current)
			_next._current->next(!this->_direction) = this->_current;
    }

    template <class T, class Allocator>
    inline constexpr typename list<T, Allocator>::iterator list<T, Allocator>::iterator::operator++()
    {
		_current = (*_current)[_direction];
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
	inline constexpr typename list<T, Allocator>::iterator list<T, Allocator>::iterator::operator--()
	{
		_current = (*_current)[!_direction];
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
	{ return this->_current->value; }

	template<class T, class Allocator>
	inline constexpr typename list<T, Allocator>::pointer list<T, Allocator>::iterator::operator->() const
	{ return &_current->value; }

	template<class T, class Allocator>
    inline constexpr typename list<T, Allocator>::const_reference list<T, Allocator>::const_iterator::operator*() const
    { return this->current->value; }

	template<class T, class Allocator>
    inline constexpr typename list<T, Allocator>::const_pointer list<T, Allocator>::const_iterator::operator->() const
    { return this->current; }

	template<class T, class Allocator>
    inline constexpr size_t list<T, Allocator>::size() const
    { return size; }

	template<class T, class Allocator>
    inline constexpr bool list<T, Allocator>::rotation() const
    { return (_bgn->next(0) != nullptr); }

	template<class T, class Allocator>
	inline list<T, Allocator>::list(): _size(0)
	{
		(this->_end) = new node;
		this->_end->next(0) = nullptr;
		this->_end->next(1) = nullptr;
		_bgn = (this->_end);
	}

	template<class T, class Allocator>
    inline list<T, Allocator>::list(const list<T, Allocator>& to_copy): _size(to_copy.size())
    {
		if(std::addressof(to_copy) == this)
			return;
		node* tab = new node[to_copy.size() + 1];
		list<T, Allocator>::const_iterator it = to_copy.begin();

		for(size_t i = 1; i <= to_copy.size(); ++i, ++it)
		{
			tab[i].value = it->value;
			tab[i][0] = i != 0 ? &tab[i - 1] : nullptr;
			tab[i][1] = i != to_copy.size() ? &tab[i + 1] : nullptr;
		}
    }

	template<class T, class Allocator>
    inline list<T, Allocator>::list(list&& l): _bgn{l._bgn}, _end{l._end}
    {}

	template<class T, class Allocator>
	inline constexpr list<T, Allocator>::list(iterator bgn, iterator end)
	{
		
	}

	template<class T, class Allocator>
	inline constexpr typename list<T, Allocator>::iterator list<T, Allocator>::begin()
	{ return {_bgn, rotation()}; }

	template<class T, class Allocator>
    inline constexpr typename list<T, Allocator>::const_iterator list<T, Allocator>::begin() const
    { return {_bgn, rotation()}; }

	template<class T, class Allocator>
    inline constexpr typename list<T, Allocator>::iterator list<T, Allocator>::end()
    { return {_end, rotation() }; }

	template<class T, class Allocator>
    inline constexpr typename list<T, Allocator>::const_iterator list<T, Allocator>::end() const
    { return {_end, rotation()}; }


	template<class T, class Allocator>
	inline typename list<T, Allocator>::iterator list<T, Allocator>::insert(const typename list<T, Allocator>::const_iterator& pos, const T &value)
	{
		list<T, Allocator>::const_iterator new_node = {new node(value), rotation()};
		return insert(pos, new_node);
	}

	template<class T, class Allocator>
	inline constexpr typename list<T, Allocator>::iterator list<T, Allocator>::insert
		(const typename list<T, Allocator>::const_iterator& pos, const typename list<T, Allocator>::const_iterator& value)
	{
		auto copy = pos;
		(--copy) <=> value;
		value <=> pos;
		return pos;
	}
	
	

	template<class T, class Allocator>
	inline constexpr typename list<T, Allocator>::iterator list<T, Allocator>::push_back(const T &value)
	{ return insert(end(), value); }

	template<class T, class Allocator>
	inline constexpr typename list<T, Allocator>::iterator list<T, Allocator>::push_front(const T &value)
	{ return insert(begin(), value); }


}