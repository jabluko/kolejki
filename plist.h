#pragma once

#include <cstddef>
#include <memory>
#include <initializer_list>
#include <iterator>
#include "utility.h"

namespace plib
{
	template<class value_type>
	class list
	{
		struct node;
	public:

		using size_type = size_t;
		using diffrence_type = std::ptrdiff_t;

		using reference = value_type&;
		using const_reference = const value_type&;

		using pointer = value_type*;
		using const_pointer = const value_type*;
		
		class iterator;
		class const_iterator;

		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		iterator _before_first;
		iterator _past_last;
		size_type _size;

		list();

		list(const list<value_type>&);

		template<typename inputIt>
		list(const inputIt&, const inputIt&);

		list(const std::initializer_list<value_type>&);
		list(list&&);

		list& operator=(const list&);
		list& operator=(list&&);

		size_t size() const;
		bool empty() const;

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
		iterator erase(const iterator& pos);

		iterator push_back(const value_type& value);
		iterator push_front(const value_type& value);

		//iterator push_back(list value);
		//iterator push_front(list value);

		void pop_back();
		void pop_front();
			
		void reverse();

		~list();

private:
		struct node
		{
		private:
			friend class list;
			friend class iterator;
		public:
			value_type _value;
			std::array<node*, 2> _next;
		};
		template<typename ...Args>
		node* make_node(node *const previous = nullptr, node *const next = nullptr, Args&&... args);
		void destroy_node(node* to_delete);

public:
		class iterator
		{
		protected:
			friend class list;
			node* _current;
			bool _direction;
			const iterator& operator<=>(const iterator&) const;
		public:
			using iterator_tag = std::bidirectional_iterator_tag;

			using reference = value_type&;
			using const_reference = const value_type&;

			using pointer = value_type*;
			using const_pointer = const value_type*;

			iterator() = default;
			iterator(const iterator&) = default;
			iterator(iterator&&) = default;
			iterator(node *const, bool);
				
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

		class const_iterator
		{
		protected:
			friend class list;
			const const_iterator& operator<=>(const const_iterator&) const;
			node* _current;
			bool _direction;
		public:
			using iterator_tag = std::bidirectional_iterator_tag;

			//using value_type = const value_type;

			using reference = value_type&;
			using const_reference = const value_type&;

			using pointer = value_type*;
			using const_pointer = const value_type*;

			const_iterator() = default;
			const_iterator(const const_iterator&) = default;
			const_iterator(const_iterator&&) = default;
			const_iterator(const iterator&);
			const_iterator(node *const, bool);

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

    template <class value_type>
    inline const typename list<value_type>::iterator& list<value_type>::iterator::operator<=>(const iterator &next) const
    {
		if(_current)
			_current->_next[_direction] = next._current;

		if(next._current)
			next._current->_next[!next._direction] = this->_current;
		return next;
    }

    template <class value_type>
    inline list<value_type>::iterator::iterator(node *const current, bool direction)
		: _current{current}, _direction{direction}
    { }

    template <class value_type>
    inline typename list<value_type>::iterator list<value_type>::iterator::get_next() const
    { return { _current->_next[_direction], (_current == _current->_next[_direction]->_next[0]) }; }

    template <class value_type>
    inline typename list<value_type>::iterator list<value_type>::iterator::get_previous() const
    { return { _current->_next[!_direction], (_current == _current->_next[!_direction]->_next[1]) }; }

    template <class value_type>
    inline typename list<value_type>::iterator &list<value_type>::iterator::operator++()
    { return (*this) = this->get_next(); }

	template<class value_type>
	inline typename list<value_type>::iterator list<value_type>::iterator::operator++(int)
	{
		auto copy = *this;
		++(*this);
		return copy;
	}
	
	template<class value_type>
	inline typename list<value_type>::iterator& list<value_type>::iterator::operator--()
	{ return (*this) = this->get_previous(); }

	template<class value_type>
	inline typename list<value_type>::iterator list<value_type>::iterator::operator--(int)
	{
		auto copy = *this;
		++(*this);
		return copy;
	}

	template<class value_type>
	inline typename list<value_type>::iterator::reference list<value_type>::iterator::operator*() const
	{ return this->_current->_value; }

	template<class value_type>
	inline typename list<value_type>::iterator::pointer list<value_type>::iterator::operator->() const
	{ return &_current->_value; }

    template <class value_type>
    inline bool list<value_type>::iterator::operator==(const typename list<value_type>::iterator& other) const
    { return _current == other._current; }

    template <class value_type>
    inline bool list<value_type>::iterator::operator!=(const typename list<value_type>::iterator& other) const
    { return _current != other._current; }

    template <class value_type>
    inline list<value_type>::const_iterator::const_iterator(const iterator& other)
		:_current{other._current}, _direction{other._direction}
    {
    }

    template <class value_type>
    inline list<value_type>::const_iterator::const_iterator(node *const current, bool direction)
        : _current{current}, _direction{direction}
    { }

	template <class value_type>
    inline const typename list<value_type>::const_iterator& list<value_type>::const_iterator::operator<=>(const const_iterator &next) const
    {
		if(_current)
			_current->_next[_direction] = next._current;

		if(next._current)
			next._current->_next[!next._direction] = this->_current;
		return next;
    }

    template <class value_type>
    inline typename list<value_type>::const_iterator list<value_type>::const_iterator::get_next() const
    { return { _current->_next[_direction], (_current == _current->_next[_direction]->_next[0]) }; }

    template <class value_type>
    inline typename list<value_type>::const_iterator list<value_type>::const_iterator::get_previous() const
    { return { _current->_next[!_direction], (_current == _current->_next[!_direction]->_next[1]) }; }

    template <class value_type>
    inline typename list<value_type>::const_iterator& list<value_type>::const_iterator::operator++()
    { return (*this) = this->get_next(); }

    template <class value_type>
    inline typename list<value_type>::const_iterator list<value_type>::const_iterator::operator++(int)
    {
        auto copy = *this;
		++(*this);
		return copy;
    }

    template <class value_type>
    inline typename list<value_type>::const_iterator& list<value_type>::const_iterator::operator--()
    { return (*this) = this->get_previous(); }

    template <class value_type>
    inline typename list<value_type>::const_iterator list<value_type>::const_iterator::operator--(int)
    {
        auto copy = *this;
		--(*this);
		return copy;
    }

    template <class value_type>
    inline typename list<value_type>::const_reference list<value_type>::const_iterator::operator*() const
    { return this->_current->_value; }

	template<class value_type>
    inline typename list<value_type>::const_pointer list<value_type>::const_iterator::operator->() const
    { return &(this->_current->_value); }

    template <class value_type>
    inline bool list<value_type>::const_iterator::operator==(const typename list<value_type>::const_iterator& other) const
    { return _current == other._current; }

    template <class value_type>
    inline bool list<value_type>::const_iterator::operator!=(const typename list<value_type>::const_iterator& other) const
    { return _current != other._current; }

	template<class value_type>
	inline list<value_type>::list(): _size(0)
	{
		(_before_first) = {make_node(), 1};
		(_past_last) = {make_node(), 1};

		_before_first._current->_next = std::array<node*, 2>{nullptr, _past_last};
		_past_last._current->_next = std::array<node*, 2>{_before_first, nullptr};
	}

    template <class value_type>
    template <typename... Args>
    inline typename list<value_type>::node* list<value_type>::make_node(node *const previous, node *const next, Args &&...args)
    { return new node({._value = value_type(args...), ._next = {previous, next}}); }

    template <class value_type>
    inline void list<value_type>::destroy_node(node *to_delete)
    { delete to_delete; }

    template <class value_type>
    inline list<value_type>::list(const std::initializer_list<value_type>&  it): list(it.begin(), it.end())
    { }

    template <class value_type>
    inline list<value_type>::list(list &&l) : _before_first{l._before_first}, _past_last{l._past_last}
    { }

    template<class value_type>
	inline typename list<value_type>::iterator list<value_type>::begin()
	{ return _before_first.get_next(); }

	template<class value_type>
    inline typename list<value_type>::const_iterator list<value_type>::begin() const
    { return _before_first.get_next(); }

	template<class value_type>
    inline typename list<value_type>::const_iterator list<value_type>::cbegin() const
    { return _before_first.get_next(); }

	template<class value_type>
    inline typename list<value_type>::iterator list<value_type>::end()
    { return _past_last.get_previous(); }

	template<class value_type>
    inline typename list<value_type>::const_iterator list<value_type>::end() const
    { return _past_last.get_previous(); }

	template<class value_type>
    inline typename list<value_type>::const_iterator list<value_type>::cend() const
    { return end(); }

    template<class value_type>
	inline typename list<value_type>::reverse_iterator list<value_type>::rbegin()
	{ return typename list<value_type>::reverse_iterator(end()); }
    template<class value_type>
	inline typename list<value_type>::const_reverse_iterator list<value_type>::rbegin() const
	{ return typename list<value_type>::const_reverse_iterator(end()); }
    template<class value_type>
	inline typename list<value_type>::const_reverse_iterator list<value_type>::crbegin() const
	{ return typename list<value_type>::const_reverse_iterator(cend()); }

    template<class value_type>
	inline typename list<value_type>::reverse_iterator list<value_type>::rend()
	{ return typename list<value_type>::reverse_iterator(begin()); }
    template<class value_type>
	inline typename list<value_type>::const_reverse_iterator list<value_type>::rend() const
	{ return typename list<value_type>::const_reverse_iterator(begin()); }
    template<class value_type>
	inline typename list<value_type>::const_reverse_iterator list<value_type>::crend() const
	{ return typename list<value_type>::const_reverse_iterator(cbegin()); }

    template <class value_type>
    template <typename inputIt>
    inline list<value_type>::list(const inputIt& bgn, const inputIt& end): list()
    {
		for(inputIt it = bgn; it != end; ++it)
			push_back(*it);
    }

    template <class value_type>
    inline typename list<value_type>::size_type list<value_type>::size() const
    { return _size; }
    template <class value_type>
    inline bool list<value_type>::empty() const
    { return _size == 0; }

    template <class value_type>
    template <typename... Args>
    inline typename list<value_type>::iterator list<value_type>::emplace(const const_iterator &pos, Args... args)
    {
		++_size;
		list<value_type>::iterator new_node(nullptr, make_node(nullptr, nullptr, args...));

		pos.get_previous() <=> new_node <=> pos;
		return new_node;
    }

	template<class value_type>
	inline typename list<value_type>::iterator list<value_type>::insert
		(const typename list<value_type>::const_iterator& pos, const value_type &value)
	{ return emplace(pos, value); }

    template <class value_type>
    inline typename list<value_type>::iterator list<value_type>::erase(const iterator &pos)
    {
        destroy_node(pos._current);
		return pos.get_previous() <=> pos.get_next();
    }

    template <class value_type>
    inline typename list<value_type>::iterator list<value_type>::push_back(const value_type &value)
    { return insert(end(), value); }

	template<class value_type>
	inline typename list<value_type>::iterator list<value_type>::push_front(const value_type &value)
	{ return insert(begin(), value); }

	template <class value_type>
    inline void list<value_type>::pop_back()
    { erase(end().get_previous()); }

	template <class value_type>
    inline void list<value_type>::pop_front()
    { erase(begin()); }


	template<class value_type>
	inline void list<value_type>::reverse()
	{ return _before_first._direction ^= 1, _past_last._direction ^= 1, std::swap(_before_first, _past_last); }

    template <class value_type>
    inline list<value_type>::~list()
    {
		while(!empty())
			pop_back();
	}
}