#pragma once

#include <tuple>
#include "utility.h"

namespace plib
{
	template<typename T>
	class _list_node
	{
	protected:
		typedef _list_node<T> node;

		T _value;
		node *_xor_prev_next;
	public:
		_list_node(const T& value, node *const prev = nullptr, node *const next = nullptr);

		node* get_next(node *const prev) const;
		void change_next(node *const old_next, node *const new_next);

		node* get_prev(node *const next) const;
		void change_prev(node *const old_prev, node *const new_prev);

	};


	template<typename T>
	class _list_iterator
	{
	private:
		friend class list;
	protected:
		typedef _list_node<T> node;
		typedef _list_iterator<T> iterator;

		node *_current;
		node *_previous;
		void link_current(node *const value);
	public:
		iterator operator++();
		iterator operator++(int);
		iterator operator--();
		iterator operator--(int);
		T& operator*();
		T* operator->();


	};

	template<typename T>
	class list
	{
	protected:
		typedef _list_node<T> node;
		typedef _list_iterator<T> iterator;
		typedef list<T> list;

		node *bgn_dummy;
		node *end_dummy;
	public:
		list();

		void push_back(const T& value);
		void push_front(const T& value);

		void push_back(list value);
		void push_front(list value);

		void pop_back();
		void pop_front();

		iterator insert(iterator pos, const T &value);
		iterator insert(iterator pos, list& value);
		iterator erase(iterator pos);

		void reverse();

	};

    template <typename T>
    _list_node<T>::_list_node(const T &value, node *const prev, node *const next) : _value{value}, _xor_prev_next{xored(prev, next)}
    { }
	
    template <typename T>
    _list_node<T>::node *_list_node<T>::get_next(_list_node<T>::node *const prev) const
    {
		return xored(_xor_prev_next, prev);
    }

    template <typename T>
    void _list_node<T>::change_next(node *const old_next, node *const new_next)
    {
        selfxor(_xor_prev_next, old_next);
        selfxor(_xor_prev_next, new_next);
    }

	template <typename T>
    _list_node<T>::node *_list_node<T>::get_prev(_list_node<T>::node *const next) const
    {
        return xored(_xor_prev_next, next);
    }

	template <typename T>
    void _list_node<T>::change_prev(node *const old_prev, node *const new_prev)
    {
		selfxor(_xor_prev_next, old_prev);
		selfxor(_xor_prev_next, new_prev);
    }



    template <typename T>
    _list_iterator<T>::iterator _list_iterator<T>::operator++()
    {
        std::tie(_previous, _current) = std::tuple{_current, _current->get_next(_previous)};
        return *this;
    }

	template <typename T>
    _list_iterator<T>::iterator _list_iterator<T>::operator++(int)
    {
		auto copy = *this;
		++(*this);
        return copy;
    }
        


    /*template <typename T>
    void _list_iterator<T>::iterator::linkAfter(_list_iterator<T>::node* const _node)
    {
		assert(_node != nullptr);
		assert(_current != nullptr);
		
		node* old_next = _next;
		node* old_prev = _current->get_prev(_next);

		_current->change_next(_next, _node);
		*_node = node(_current, old_next);
		
		if(old_next != nullptr)
			old_next->change_prev(_current, _node);

    }*/

    template <typename T>
    void _list_iterator<T>::link_current(node *const value)
    {
		value->change_next(nullptr, _current);
		_current->change_prev(_previous, value);
		_previous = value;
    }
	
    template <typename T>
    T &_list_iterator<T>::iterator::operator*()
    {
        return _current->_value;
    }

    template <typename T>
    T *_list_iterator<T>::operator->()
    {
		return &_current->_value;
    }

    template <typename T>
    void list<T>::push_back(const T &value)
    {
		insert(end_dummy, value);
    }
    template <typename T>
    list<T>::iterator list<T>::insert(list<T>::iterator pos, const T &value)
    {
		node* newNode = new node{value};



        return ++pos;
    }



}