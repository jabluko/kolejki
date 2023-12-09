#pragma once

#include <tuple>
#include "utility.h"

namespace plib
{
	template<typename T>
	class list
	{
	protected:
		class node;
		using iterator = node*;

		/*
		template<typename T>
		class iterator
		{
		private:
			friend class list;
		protected:
			typedef node<T> node;

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
		*/
		


		node *bgn;
		node *end_dummy;
	public:
		list();
		list(iterator bgn, iterator end);
		
		iterator insert(const iterator& pos, const T& value);
		iterator insert(const iterator& pos, iterator& value);
		iterator insert(const iterator& pos, list& value);
		iterator erase(const iterator& pos);

		iterator push_back(const T& value);
		iterator push_front(const T& value);

		iterator push_back(list value);
		iterator push_front(list value);

		void pop_back();
		void pop_front();
		

		void reverse();
	
	protected:

		class node : std::array<iterator, 2>
		{
		protected:
			T value;
			node *next[2];

		};

		bool way_forward()
		{
			return (bgn->next[0] != nullptr);
		}

		iterator insert(const iterator& pos, const T &value)
		{
			node* newNode = new node;
			newNode->value = value;
			insert(pos, newNode)
		}
		iterator insert(const iterator& pos, iterator& value)
		{
			bool forwards = way_forward();
			bool backwards = !way_forward();
		
			pos->next[backwards]->next[forwards] = value;

			value->next[forwards] = pos;
			value->next[backwards] = pos->next[backwards]
		}

        template <typename T>
        inline iterator insert(iterator pos, list &value)
        {
            
        }
    };
    template <typename T>
    inline list<T>::iterator list<T>::push_back(const T &value)
    {
		node* newNode = new node(value);

    }
    template <typename T>
    inline void list<T>::pop_front()
    {
    }
}