#pragma once

#include <cstddef>
#include <memory>
#include <initializer_list>
#include <iterator>
#include "utility.h"

namespace plib
{
	template<class T>
	class list
	{
	public:
		struct node;
		class iterator;
		class const_iterator;

		class iterator
		{
		protected:
			friend class list;
			node* _current;
			bool _direction;
		public:
			using iterator_tag = std::bidirectional_iterator_tag;
			using difference_type = ptrdiff_t;
			using value_type = T;

			using reference = T&;
			using const_reference = const T&;

			using pointer = T*;
			using const_pointer = const T*;

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
			node* _current;
			bool _direction;
		public:
			using iterator_tag = std::bidirectional_iterator_tag;
			using difference_type = ptrdiff_t;
			using value_type = const T;

			using reference = const T&;

			using pointer = const T*;

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
			reference operator*() const;
			pointer operator->() const;

			bool operator==(const const_iterator&) const;
			bool operator!=(const const_iterator&) const;

			~const_iterator() = default;
		};

		using value_type = T;
		using size_type = size_t;
		using difference_type = std::ptrdiff_t;

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

		list(const list<T>&);

		template<typename inputIt>
		list(const inputIt&, const inputIt&);

		list(const std::initializer_list<T>&);
		list(list&&);

		list& operator=(const list&);
		list& operator=(list&&);

		size_t size() const;
		bool empty() const;

		iterator begin();
		const_iterator begin() const;
		const_iterator cbegin() const;

		reverse_iterator rbegin();
		const_reverse_iterator rbegin() const;
		const_reverse_iterator crbegin() const;

		reference front();
		const_reference front() const;
			
		iterator end();
		const_iterator end() const;
		const_iterator cend() const;

		reverse_iterator rend();
		const_reverse_iterator rend() const;
		const_reverse_iterator crend() const;

		reference back();
		const_reference back() const;
		
		template<typename ...Args>
		iterator emplace(const const_iterator& pos, Args&&... args);
		iterator insert(const const_iterator& pos, const T& value);
		
		template<typename _InputIterator>
		iterator insert(const const_iterator& pos, _InputIterator first, const _InputIterator& second);
		iterator insert(const const_iterator& pos, list&& value);
		iterator insert(const const_iterator& pos, list& value);

		iterator erase(const const_iterator& pos);
		iterator pull_out(const const_iterator& pos);

		template<typename ...Args>
		iterator emplace_back(Args&&... args);
		template<typename ...Args>
		iterator emplace_front(Args&&... args);

		iterator push_back(const T& value);
		iterator push_front(const T& value);

		iterator push_back(list& value);
		iterator push_back(list&& value);

		iterator push_front(list& value);
		iterator push_front(list&& value);

		value_type pop_back();
		value_type pop_front();
			
		void reverse();

		iterator direct(const const_iterator&, const const_iterator&);

		~list();

public:
		struct node
		{
		private:
			friend class list;
			friend class iterator;
		public:
			T _value;
			node* _next[2];
		};
		template<typename ...Args>
		node* make_node(node *const previous = nullptr, node *const next = nullptr, Args&&... args);
		void destroy_node(node* to_delete);

		template<typename it1_t, typename it2_t>
		const it2_t& link(const it1_t&, const it2_t&);

public:
	};


    template <class T>
    template <typename... Args>
    inline typename list<T>::node *list<T>::make_node(node *const previous, node *const next, Args &&...args)
    { return new node({._value = T(args...), ._next = {previous, next}}); }

    template <class T>
    inline void list<T>::destroy_node(node *to_delete)
    { delete to_delete; }

    template <class T>
	template<typename it1_t, typename it2_t>
    inline const it2_t& list<T>::link(const it1_t& a, const it2_t& b)
    {
		if(a._current)
			a._current->_next[a._direction] = b._current;

		if(b._current)
			b._current->_next[!b._direction] = a._current;
		return b;
    }

    template <class T>
    inline list<T>::iterator::iterator(node *const current, bool direction)
        : _current{current}, _direction{direction}
    { }

    template <class T>
    inline typename list<T>::iterator list<T>::iterator::get_next() const
    { return { _current->_next[_direction], (_current->_next[_direction] && _current == _current->_next[_direction]->_next[0]) }; }

    template <class T>
    inline typename list<T>::iterator list<T>::iterator::get_previous() const
    { return { _current->_next[!_direction], (_current->_next[!_direction] && _current == _current->_next[!_direction]->_next[1]) }; }

    template <class T>
    inline typename list<T>::iterator &list<T>::iterator::operator++()
    { return (*this) = this->get_next(); }

	template<class T>
	inline typename list<T>::iterator list<T>::iterator::operator++(int)
	{
		auto copy = *this;
		++(*this);
		return copy;
	}
	
	template<class T>
	inline typename list<T>::iterator& list<T>::iterator::operator--()
	{ return (*this) = this->get_previous(); }

	template<class T>
	inline typename list<T>::iterator list<T>::iterator::operator--(int)
	{
		auto copy = *this;
		++(*this);
		return copy;
	}

	template<class T>
	inline typename list<T>::iterator::reference list<T>::iterator::operator*() const
	{ return this->_current->_value; }

	template<class T>
	inline typename list<T>::iterator::pointer list<T>::iterator::operator->() const
	{ return &_current->_value; }

    template <class T>
    inline bool list<T>::iterator::operator==(const typename list<T>::iterator& other) const
    { return _current == other._current; }

    template <class T>
    inline bool list<T>::iterator::operator!=(const typename list<T>::iterator& other) const
    { return _current != other._current; }

    template <class T>
    inline list<T>::const_iterator::const_iterator(const iterator& other)
		:_current{other._current}, _direction{other._direction}
    { }

    template <class T>
    inline list<T>::const_iterator::const_iterator(node *const current, bool direction)
        : _current{current}, _direction{direction}
    { }

    template <class T>
    inline typename list<T>::const_iterator list<T>::const_iterator::get_next() const
    { return { _current->_next[_direction], (_current->_next[_direction] && _current == _current->_next[_direction]->_next[0]) }; }

    template <class T>
    inline typename list<T>::const_iterator list<T>::const_iterator::get_previous() const
    { return { _current->_next[!_direction], (_current->_next[!_direction] && _current == _current->_next[!_direction]->_next[1]) }; }

    template <class T>
    inline typename list<T>::const_iterator& list<T>::const_iterator::operator++()
    { return (*this) = this->get_next(); }

    template <class T>
    inline typename list<T>::const_iterator list<T>::const_iterator::operator++(int)
    {
        auto copy = *this;
		++(*this);
		return copy;
    }

    template <class T>
    inline typename list<T>::const_iterator& list<T>::const_iterator::operator--()
    { return (*this) = this->get_previous(); }

    template <class T>
    inline typename list<T>::const_iterator list<T>::const_iterator::operator--(int)
    {
        auto copy = *this;
		--(*this);
		return copy;
    }

    template <class T>
    inline typename list<T>::const_iterator::reference list<T>::const_iterator::operator*() const
    { return this->_current->_value; }

	template<class T>
    inline typename list<T>::const_iterator::pointer list<T>::const_iterator::operator->() const
    { return &(this->_current->_value); }

    template <class T>
    inline bool list<T>::const_iterator::operator==(const typename list<T>::const_iterator& other) const
    { return _current == other._current; }

    template <class T>
    inline bool list<T>::const_iterator::operator!=(const typename list<T>::const_iterator& other) const
    { return _current != other._current; }

	template<class T>
	inline list<T>::list(): _size(0)
	{
		(_before_first) = {make_node(), 1};
		(_past_last) = {make_node(), 1};

		link(_before_first, _past_last);
	}

    template <class T>
    inline list<T>::list(const std::initializer_list<T>&  it): list(it.begin(), it.end())
    { }

    template <class T>
    inline list<T>::list(list &&l) : _before_first{l._before_first}, _past_last{l._past_last}
    { }

    template<class T>
	inline typename list<T>::iterator list<T>::begin()
	{ return _before_first.get_next(); }

	template<class T>
    inline typename list<T>::const_iterator list<T>::begin() const
    { return _before_first.get_next(); }

	template<class T>
    inline typename list<T>::const_iterator list<T>::cbegin() const
    { return _before_first.get_next(); }

    template<class T>
	inline typename list<T>::reverse_iterator list<T>::rbegin()
	{ return typename list<T>::reverse_iterator(end()); }
	
    template<class T>
	inline typename list<T>::const_reverse_iterator list<T>::rbegin() const
	{ return typename list<T>::const_reverse_iterator(end()); }

    template<class T>
	inline typename list<T>::const_reverse_iterator list<T>::crbegin() const
	{ return typename list<T>::const_reverse_iterator(cend()); }

    template<class T>
	inline typename list<T>::reference list<T>::front()
	{ return *begin(); }

    template<class T>
	inline typename list<T>::const_reference list<T>::front() const
	{ return *begin(); }

	template<class T>
    inline typename list<T>::iterator list<T>::end()
    { return _past_last; }

	template<class T>
    inline typename list<T>::const_iterator list<T>::end() const
    { return _past_last; }

	template<class T>
    inline typename list<T>::const_iterator list<T>::cend() const
    { return end(); }

    template<class T>
	inline typename list<T>::reverse_iterator list<T>::rend()
	{ return typename list<T>::reverse_iterator(begin()); }

    template<class T>
	inline typename list<T>::const_reverse_iterator list<T>::rend() const
	{ return typename list<T>::const_reverse_iterator(begin()); }

    template<class T>
	inline typename list<T>::const_reverse_iterator list<T>::crend() const
	{ return typename list<T>::const_reverse_iterator(cbegin()); }

    template<class T>
	inline typename list<T>::reference list<T>::back()
	{ return *(end().get_previous()); }

    template<class T>
	inline typename list<T>::const_reference list<T>::back() const
	{ return *(end().get_previous()); }

    template <class T>
    template <typename inputIt>
    inline list<T>::list(const inputIt& bgn, const inputIt& end): list()
    {
		for(inputIt it = bgn; it != end; ++it)
			push_back(*it);
    }

    template <class T>
    inline typename list<T>::size_type list<T>::size() const
    { return _size; }
    template <class T>
    inline bool list<T>::empty() const
    { return _size == 0; }

    template <class T>
    template <typename... Args>
    inline typename list<T>::iterator list<T>::emplace(const const_iterator &pos, Args&&... args)
    {
		++_size;
		list<T>::iterator new_node(make_node(nullptr, nullptr, args...), pos._direction);

		link(pos.get_previous(), new_node);
		link(new_node, pos);
		
		return new_node;
    }

    template<class T>
	inline typename list<T>::iterator list<T>::insert
		(const typename list<T>::const_iterator& pos, const T &value)
	{ return emplace(pos, value); }

    template <class T>
    inline typename list<T>::iterator list<T>::insert(const const_iterator &pos, list &&other)
    {
		if(!other.empty())
		{
			pos.get_previous() <=> other.begin();
			other.end().get_previous() <=> pos;
		}
		
		return {pos._current, pos._direction};
    }

    template <class T>
    inline typename list<T>::iterator list<T>::insert(const const_iterator &pos, list& other)
    {
		if(!other.empty())
		{
			link(pos.get_previous(), other.begin());
			link(other.end().get_previous(), pos);
			link(other._before_first, other._past_last);
		}
		
		return {pos._current, pos._direction};
    }

    template <class T>
    inline typename list<T>::iterator list<T>::erase(const const_iterator &pos)
    {
		auto copy = link(pos.get_previous(), pos.get_next());
        destroy_node(pos._current);
		return {copy._current, copy._direction};
    }

    template <class T>
    inline typename list<T>::iterator list<T>::pull_out(const const_iterator &pos)
    {
		auto copy = link(pos.get_previous(), pos.get_next());
		pos._current->_next[0] = nullptr;
		pos._current->_next[1] = nullptr;
		return copy;
    }

    template <class T>
    template <typename... Args>
    inline typename list<T>::iterator list<T>::emplace_back(Args &&...args)
    { return emplace(cend(), args...); }

    template <class T>
    inline typename list<T>::iterator list<T>::push_back(const T &value)
    { return insert(cend(), value); }

    template <class T>
    inline typename list<T>::iterator list<T>::push_back(list &value)
    { return insert(cend(), value); }
    template <class T>
    inline typename list<T>::iterator list<T>::push_back(list &&value)
    { return insert(cend(), value); }

    template <class T>
    template <typename... Args>
    inline typename list<T>::iterator list<T>::emplace_front(Args &&...args)
    { return emplace(cbegin(), args...); }

	template<class T>
	inline typename list<T>::iterator list<T>::push_front(const T &value)
	{ return insert(cbegin(), value); }

	template<class T>
	inline typename list<T>::iterator list<T>::push_front(list &value)
	{ return insert(cbegin(), value); }

	template<class T>
	inline typename list<T>::iterator list<T>::push_front(list&& value)
	{ return insert(cbegin(), value); }


    template <class T>
    inline typename list<T>::value_type list<T>::pop_back()
    { 
		auto copy = back();
		erase(cend().get_previous());
		return copy;
	}

	template <class T>
    inline typename list<T>::value_type list<T>::pop_front()
    { 
		auto copy = front();
		erase(cbegin());
		return copy;
	}


	template<class T>
	inline void list<T>::reverse()
	{ return _before_first._direction ^= 1, _past_last._direction ^= 1, std::swap(_before_first, _past_last); }

    template <class T>
    inline typename list<T>::iterator list<T>::direct(const typename list<T>::const_iterator& it, const typename list<T>::const_iterator& direction)
    {
        const_iterator it1 = {it._current, it._direction};
		const_iterator it2 = {it._current, !it._direction};

		while(it1._current && it1 != direction && it2._current && it2 != direction)  
			++it1, ++it2;
		
		while(it1._current && it1 != direction)
			++it1;

		while(it2._current && it2 != direction)
			++it2;

		if(it1 == direction)
			return {it._current, it1._direction};
		if(it2 == direction)
			return {it._current, it2._direction};

		return {nullptr, 0};
    }

    template <class T>
    inline list<T>::~list()
    {
		while(!empty())
			pop_back();
	}
}