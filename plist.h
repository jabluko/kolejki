#pragma once

#include <cstddef>
#include <memory>
#include "utility.h"

namespace plib
{
	template<typename T>
	class list
	{
	protected:
		struct node : protected std::array<node*, 2>
		{
		private:
			friend class list;
			friend class iterator;
		protected:
			T value; 
		};
		
		node* bgn;
		node* end_dummy;
		size_t _size;

	public:
		class iterator : std::iterator<std::bidirectional_iterator_tag, node>
		{
			using pointer = node*;
			using reference = node&;
		private:
			friend class list;

		protected:
			pointer current;
			bool direction;

		public:
			constexpr iterator() = default;
			constexpr iterator(const iterator&) = default;
			constexpr iterator(iterator&&) = default;

			constexpr iterator& operator=(const iterator&) = default;
			constexpr iterator& operator=(iterator&&) = default;

			constexpr iterator operator++();
			constexpr iterator operator++(int);
			constexpr iterator operator--();
			constexpr iterator operator--(int);
			constexpr reference operator*() const;
			constexpr pointer operator->() const;


		};

		class const_iterator : public iterator
		{
			using const_pointer = const node*;
			using const_reference = const node&;
		public:
			constexpr const_reference operator*() const override;
			constexpr const_pointer operator->() const override;
		};

		
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
		
		iterator insert(const iterator& pos, const T& value);
		constexpr iterator insert(const iterator& pos, iterator& value);
		constexpr iterator insert(const iterator& pos, list&& value);
		iterator erase(iterator& pos);

		constexpr iterator push_back(const T& value);
		constexpr iterator push_front(const T& value);

		constexpr iterator push_back(list value);
		constexpr iterator push_front(list value);

		constexpr void pop_back();
		constexpr void pop_front();
		

		constexpr void reverse();
	};

    /*template <typename T>
    inline constexpr list<T>::iterator::iterator(pointer _current, bool _direction): current{_current}, direction{_direction}
    {}*/

    template <typename T>
    inline constexpr typename list<T>::iterator list<T>::iterator::operator++()
    {
		current = (*current)[direction];
		return *this;
	}

	template <typename T>
	inline constexpr typename list<T>::iterator list<T>::iterator::operator++(int)
	{
		auto copy = *this;
		++(*this);
		return copy;
	}
	
	template <typename T>
	inline constexpr typename list<T>::iterator list<T>::iterator::operator--()
	{
		current = (*current)[!direction];
		return *this;
	}

	template <typename T>
	inline constexpr typename list<T>::iterator list<T>::iterator::operator--(int)
	{
		auto copy = *this;
		++(*this);
		return copy;
	}

	template <typename T>
	inline constexpr typename list<T>::iterator::reference list<T>::iterator::operator*() const
	{ return this->current->value; }

	template <typename T>
	inline constexpr typename list<T>::iterator::pointer list<T>::iterator::operator->() const
	{ return current; }


    template <typename T>
    inline constexpr typename list<T>::const_iterator::const_reference list<T>::const_iterator::operator*() const
    { return this->current->value; }

    template <typename T>
    inline constexpr typename list<T>::const_iterator::const_pointer list<T>::const_iterator::operator->() const
    { return this->current; }

    template <typename T>
    inline constexpr size_t list<T>::size() const
    {
        return size;
    }

    template <typename T>
    inline constexpr bool list<T>::rotation() const
    { return ((*bgn)[0] != nullptr); }

	template <typename T>
	inline list<T>::list(): _size(0)
	{
		end_dummy = new node;
		(*end_dummy)[0] = nullptr;
		(*end_dummy)[1] = nullptr;
		bgn = end_dummy;
	}

    template <typename T>
    inline list<T>::list(const list<T>& to_copy): _size(to_copy.size())
    {
		if(std::addressof(to_copy) == this)
			return;
		node* tab = new node[to_copy.size() + 1];
		list<T>::const_iterator it = to_copy.begin();

		for(size_t i = 1; i <= to_copy.size(); ++i, ++it)
		{
			tab[i].value = it->value;
			tab[i][0] = i != 0 ? &tab[i - 1] : nullptr;
			tab[i][1] = i != to_copy.size() ? &tab[i + 1] : nullptr;
		}
    }

    template <typename T>
    inline list<T>::list(list&& l): bgn{l.bgn}, end_dummy{l.end_dummy}
    {}

    template <typename T>
	inline constexpr typename list<T>::iterator list<T>::begin()
	{ return typename list<T>::iterator{ .current = bgn, .direction = rotation()}; }

    template <typename T>
    inline constexpr typename list<T>::const_iterator list<T>::begin() const
    { return {bgn, rotation()}; }

    template <typename T>
    inline constexpr typename list<T>::iterator list<T>::end()
    { return { end_dummy, rotation() }; }

    template <typename T>
    inline constexpr typename list<T>::const_iterator list<T>::end() const
    { return {end_dummy, rotation()}; }


    template<typename T>
	inline typename list<T>::iterator list<T>::insert(const typename list<T>::iterator& pos, const T &value)
	{
		typename list<T>::node* new_node = new node;
		new_node->value = value;
		insert(pos, {new_node, 0});
	}
	
	template<typename T>
	inline constexpr typename list<T>::iterator list<T>::insert(const typename list<T>::iterator& pos, typename list<T>::iterator& value)
	{
		bool forwards = rotation();
		bool backwards = !rotation();
	
		pos->next[backwards]->next[forwards] = value;
		value->next[forwards] = pos;
		value->next[backwards] = pos->next[backwards];
	}

	template <typename T>
	inline constexpr typename list<T>::iterator list<T>::push_back(const T &value)
	{ return insert(end(), value); }

	template <typename T>
	inline constexpr typename list<T>::iterator list<T>::push_front(const T &value)
	{ return insert(begin(), value); }

}