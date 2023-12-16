#pragma once

#include <cstddef>
#include <memory>
#include <initializer_list>
#include <iterator>
#include <cassert>
#include <iostream>

namespace plib
{
    template <class T>
    class list
    {
    private:
        struct node;

    public:
        using value_type = T;
        using size_type = size_t;
        using difference_type = std::ptrdiff_t;

        using reference = value_type &;
        using const_reference = const value_type &;

        using pointer = value_type *;
        using const_pointer = const value_type *;

        class iterator;
        class const_iterator;

        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        template<class U>
        friend void swap(list<U>& l1, list<U>& l2);

        list();
        list(const list&);
        list(list&&);

        template <typename inputIt>
        list(const inputIt&, const inputIt&);

        list(const std::initializer_list<T>&);

        list& operator=(list);
        list& operator=(const std::initializer_list<T>&);

        size_t size() const;
        bool empty() const;

        iterator begin();
        const_iterator begin() const;
        const_iterator cbegin();
        const_iterator cbegin() const;

        reverse_iterator rbegin();
        const_reverse_iterator rbegin() const;
        const_reverse_iterator crbegin();
        const_reverse_iterator crbegin() const;

        iterator end();
        const_iterator end() const;
        const_iterator cend();
        const_iterator cend() const;

        reverse_iterator rend();
        const_reverse_iterator rend() const;
        const_reverse_iterator crend();
        const_reverse_iterator crend() const;

        reference front();
        const_reference front() const;

        reference back();
        const_reference back() const;

        /**
         * @brief constructs an element in-place, before the given position.
         *
         * @tparam Args types of arguments the constructor takes.
         * @param pos iterator before which to insert the element.
         * @param args list of arguments for the constructor.
         * @return iterator to the inserted element.
         */
        template <typename... Args>
        iterator emplace(const const_iterator &pos, Args&&... args);
        template <typename... Args>
        iterator emplace_back(Args&&... args);
        template <typename... Args>
        iterator emplace_front(Args&&... args);

        /**
         * @brief inserts a copied value before the given position.
         *
         * @param pos iterator before which to insert the element.
         * @param value to be inserted.
         * @return iterator to the inserted element.
         */
        iterator insert(const const_iterator& pos, const T& value);

        /**
         * @brief inserts all elements of a given list before the specified position.
         *
         * @param pos iterator before which to insert the list.
         * @param l list to be inserted.
         * @return iterator to the last inserted element or pos if the provided list is empty.
         */
        iterator insert(const const_iterator& pos, const list& l);

        /**
         * @brief inserts all elements of a given std::initializer_list before the specified position.
         *
         * @param pos iterator before which to insert the list.
         * @param il std::initializer_list to be inserted.
         * @return iterator to the firs
        if(_before_first == nullptr)
            return;
        t inserted element or pos if the provided list is empty.
         */
        iterator insert(const const_iterator& pos, const std::initializer_list<value_type>& il);

        /**
         * @brief Inserts elements from a range before the given position.
         *
         * @tparam _InputIt type of iterator for the input range.
         * @param pos iterator before which to insert the elements.
         * @param first begin iterator of the input range.
         * @param second end iterator of the input range.
         * @return iterator to the last inserted element or pos if the provided range is empty.
         */
        template <typename _InputIt>
        iterator insert(const const_iterator& pos, const _InputIt& first, const _InputIt& second);

        /**
         * @brief pushed the given argument to the back of the list using a specialized insert function.
         *
         * @tparam to_push type of element to push.
         * @param value argument to be pushed.
         * @return iterator to the inserted element.
         */
        template <class to_push>
        iterator push_back(const to_push& value);

        /**
         * @brief pushed the given argument to the front of the list using a specialized insert function.
         *
         * @tparam to_push type of element to push.
         * @param value argument to be pushed.
         * @return iterator to the inserted element.
         */
        template <class to_push>
        iterator push_front(const to_push& value);

        /**
         * @brief merges another list before the specified position.
         *
         * @param pos iterator before which to merge the other list.
         * @param other list to be merged.
         * @return iterator pointing to the first element,
         *  which came from other in the merged list, or pos if other was empty
         */
        iterator merge(const const_iterator& pos, list& other);
        iterator merge_back(list& other);
        iterator merge_front(list& other);

        /**
         * @brief erases the element at the specified position.
         *
         * @param pos iterator pointing to the element to be erased.
         * @return iterator pointing to the element following the erased element.
         */
        iterator erase(const const_iterator& pos);

        /**
         * @brief removes the element at the specified position and returns it.
         *
         * @param pos iterator pointing to the element to be removed.
         * @return value of the removed element.
         */
        iterator pull_out(const const_iterator& pos);

        value_type pop_back();
        value_type pop_front();

        void reverse();
        void clear();

        ~list();

        class iterator
        {
        protected:
            friend class list;
            node *_current;
            bool _direction;
            bool _const_linking;
            iterator(node *const currnet, bool direction, bool const_linking);
            iterator(const const_iterator &other);

        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type = ptrdiff_t;
            using value_type = T;

            using reference = T &;
            using const_reference = const T &;

            using pointer = T *;
            using const_pointer = const T *;

            iterator() = default;
            iterator(const iterator &) = default;
            iterator(iterator &&) = default;

            iterator &operator=(const iterator &) = default;
            iterator &operator=(iterator &&) = default;

            iterator &operator++();
            iterator operator++(int);
            iterator &operator--();
            iterator operator--(int);
            reference operator*() const;
            pointer operator->() const;

            bool operator==(const iterator &) const;
            bool operator!=(const iterator &) const;

            ~iterator() = default;



            friend bool has_next(const iterator& it)
            { return it._current && it._current->_next[it._direction]; }

            friend bool has_prev(const iterator& it)
            { return it._current && it._current->_next[!it._direction]; }

            friend iterator next(const iterator& it)
            {
                assert(has_next(it));
                return  {
                            it._current->_next[it._direction],
                            it._current == it._current->_next[it._direction]->_next[0],
                            it._const_linking
                        };
            }

            friend iterator prev(const iterator& it)
            {
                assert(has_prev(it));
                return  {
                        it._current->_next[!it._direction],
                        it._current == it._current->_next[!it._direction]->_next[1],
                        it._const_linking
                        };
            }

            /**
             * @brief directs one iterator to face another
             * 
             * @param it iterator to be directed at dst
             * @param dst 
             * @return iterator pointing to it, but incrementing it will lead to dst
             */
            friend iterator direct
                (const iterator &it, const iterator &dst)
            {
                iterator it1{it._current, 0, it._const_linking};
                iterator it2{it._current, 1, it._const_linking};

                while (has_next(it1) && it1 != dst && has_next(it2) && it2 != dst)
                    ++it1, ++it2;
                
                while (has_next(it1) && it1 != dst)
                    ++it1;

                while (has_next(it2) && it2 != dst)
                    ++it2;

                if (it1 == dst)
                    return {it._current, 0, it._const_linking};
                if (it2 == dst)
                    return {it._current, 1, it._const_linking};
                else
                    return {nullptr, 0, 1};
            }
        };

        class const_iterator
        {
        protected:
            friend class list;
            node *_current;
            bool _direction;
            bool _const_linking;
            const_iterator(node *const current, bool direction, bool const_linking);

        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type = ptrdiff_t;
            using value_type = const T;

            using reference = const T &;
            using pointer = const T *;

            const_iterator() = default;
            const_iterator(const const_iterator &) = default;
            const_iterator(const_iterator &&) = default;
            const_iterator(const iterator &);

            const_iterator &operator=(const const_iterator &) = default;
            const_iterator &operator=(const_iterator &&) = default;

            const_iterator &operator++();
            const_iterator operator++(int);
            const_iterator &operator--();
            const_iterator operator--(int);
            reference operator*() const;
            pointer operator->() const;

            bool operator==(const const_iterator &) const;
            bool operator!=(const const_iterator &) const;

            ~const_iterator() = default;

            friend bool has_next(const const_iterator& it)
            { return it._current && it._current->_next[it._direction]; }

            friend bool has_prev(const const_iterator& it)
            { return it._current && it._current->_next[!it._direction]; }

            friend const_iterator next(const const_iterator& it)
            {
                assert(has_next(it));
                return  {
                            it._current->_next[it._direction],
                            it._current == it._current->_next[it._direction]->_next[0],
                            it._const_linking
                        };
            }

            friend const_iterator prev(const const_iterator& it)
            {
                assert(has_prev(it));
                return  {
                        it._current->_next[!it._direction],
                        it._current == it._current->_next[!it._direction]->_next[1],
                        it._const_linking
                        };
            }


            /**
             * @brief directs one const_iterator to face another
             * 
             * @param it const_iterator to be directed at dst
             * @param dst 
             * @return const_iterator pointing at start but incrementing wich will lead to dst
             */
            friend const_iterator direct
                (const const_iterator &it, const const_iterator &dst)
            {
                const_iterator it1 = {it._current, 0, it._const_linking};
                const_iterator it2 = {it._current, 1, it._const_linking};

                while (has_next(it1) && it1 != dst && has_next(it2) && it2 != dst)
                    ++it1, ++it2;
                
                while (has_next(it1) && it1 != dst)
                    ++it1;

                while (has_next(it2) && it2 != dst)
                    ++it2;

                if (it1 == dst)
                    return {it._current, 0, it._const_linking};
                if (it2 == dst)
                    return {it._current, 1, it._const_linking};
                else
                    return {nullptr, 0, 1};
            }
        };

    private:
        node *_before_first;
        node *_past_last;
        bool _direction;

        iterator before_begin();
        const_iterator before_begin() const;
        const_iterator cbefore_begin();
        const_iterator cbefore_begin() const;

        struct node
        {
        private:
            friend class list;
            friend class iterator;

        public:
            T _value;
            node *_next[2];
        };
        template <typename... Args>
        node *make_node(node *const previous = nullptr, node *const next = nullptr, Args &&...args);
        void destroy_node(node *to_delete);

        template <typename it1_t, typename it2_t>
        const it2_t &link(const it1_t &, const it2_t &);
    };

    template <class T>
    void swap(list<T>& l1, list<T>& l2)
    {
        using std::swap;
        swap(l1._before_first, l2._before_first);
        swap(l1._past_last, l2._past_last);
        swap(l1._direction, l2._direction);
    }


    template <class T>
    template <typename... Args>
    inline typename list<T>::node *list<T>::make_node
        (node *const previous, node *const next, Args &&...args)
    {
        return new node{
            T(args...),      //_value
            {previous, next} //_next
        };
    }

    template <class T>
    inline void list<T>::destroy_node(node *to_delete)
    { delete to_delete; }

    template <class T>
    template <typename it1_t, typename it2_t>
    inline const it2_t &list<T>::link(const it1_t &a, const it2_t &b)
    {
        assert(a._const_linking == 0 && b._const_linking == 0);
        a._current->_next[a._direction] = b._current;
        b._current->_next[!b._direction] = a._current;
        return b;
    }


    template <class T>
    inline list<T>::iterator::iterator(node *const current, bool direction, bool const_linking)
        : _current{current}, _direction{direction}, _const_linking{const_linking}
    { }

    template <class T>
    inline list<T>::iterator::iterator(const const_iterator &other)
        : _current{other._current}, _direction{other._direction}, _const_linking{other._const_linking}
    { }

    template <class T>
    inline typename list<T>::iterator &list<T>::iterator::operator++()
    { return (*this) = next(*this); }

    template <class T>
    inline typename list<T>::iterator list<T>::iterator::operator++(int)
    {
        auto copy = *this;
        ++(*this);
        return copy;
    }

    template <class T>
    inline typename list<T>::iterator &list<T>::iterator::operator--()
    { return (*this) = prev(*this); }

    template <class T>
    inline typename list<T>::iterator list<T>::iterator::operator--(int)
    {
        auto copy = *this;
        ++(*this);
        return copy;
    }

    template <class T>
    inline typename list<T>::iterator::reference list<T>::iterator::operator*() const
    { return this->_current->_value; }

    template <class T>
    inline typename list<T>::iterator::pointer list<T>::iterator::operator->() const
    { return &_current->_value; }

    template <class T>
    inline bool list<T>::iterator::operator==(const typename list<T>::iterator &other) const
    { return _current == other._current; }

    template <class T>
    inline bool list<T>::iterator::operator!=(const typename list<T>::iterator &other) const
    { return _current != other._current; }


    template <class T>
    inline list<T>::const_iterator::const_iterator(const iterator &other)
        : _current{other._current}, _direction{other._direction}, _const_linking{other._const_linking}
    { }

    template <class T>
    inline list<T>::const_iterator::const_iterator(node *const current, bool direction, bool const_linking)
        : _current{current}, _direction{direction}, _const_linking{const_linking}
    { }

    template <class T>
    inline typename list<T>::const_iterator &list<T>::const_iterator::operator++()
    { return (*this) = next(*this); }

    template <class T>
    inline typename list<T>::const_iterator list<T>::const_iterator::operator++(int)
    {
        auto copy = *this;
        ++(*this);
        return copy;
    }

    template <class T>
    inline typename list<T>::const_iterator &list<T>::const_iterator::operator--()
    { return (*this) = prev(*this); }

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

    template <class T>
    inline typename list<T>::const_iterator::pointer list<T>::const_iterator::operator->() const
    { return &(this->_current->_value); }

    template <class T>
    inline bool list<T>::const_iterator::operator==(const list<T>::const_iterator &other) const
    { return _current == other._current; }

    template <class T>
    inline bool list<T>::const_iterator::operator!=(const list<T>::const_iterator &other) const
    { return _current != other._current; }


    template <class T>
    inline list<T>::list()
        : _before_first(make_node()), _past_last(make_node()), _direction(1)
    { link(before_begin(), end()); }

    template <class T>
    inline list<T>::list(const list<T>& other) : list(other.cbegin(), other.cend())
    { }

    template <class T>
    template <typename inputIt>
    inline list<T>::list(const inputIt &bgn, const inputIt &end) : list()
    {
        for (inputIt it = bgn; it != end; ++it)
            push_back(*it);
    }

    template <class T>
    inline list<T>::list(const std::initializer_list<T> &it) : list(it.begin(), it.end())
    { }

    template <class T>
    inline list<T>::list(list &&other)
        :list()
    { swap(*this, other); }

    template <class T>
    inline list<T>& list<T>::operator=(list<T> other)
    {
        swap(*this, other);
        return *this;
    }

    template <class T>
    inline list<T>& list<T>::operator=(const std::initializer_list<T>& il)
    { return (*this) = plib::list<T>(il); }

    template <class T>
    inline typename list<T>::iterator list<T>::before_begin()
    { return {_before_first, _direction, 0}; }

    template <class T>
    inline typename list<T>::const_iterator list<T>::before_begin() const
    { return {_before_first, _direction, 1}; }

    template <class T>
    inline typename list<T>::const_iterator list<T>::cbefore_begin()
    { return before_begin(); }

    template <class T>
    inline typename list<T>::const_iterator list<T>::cbefore_begin() const
    { return before_begin(); }

    template <class T>
    inline typename list<T>::iterator list<T>::begin()
    { return next(before_begin()); }

    template <class T>
    inline typename list<T>::const_iterator list<T>::begin() const
    { return next(before_begin()); }

    template <class T>
    inline typename list<T>::const_iterator list<T>::cbegin()
    { return next(cbefore_begin()); }

    template <class T>
    inline typename list<T>::const_iterator list<T>::cbegin() const
    { return next(cbefore_begin()); }

    template <class T>
    inline typename list<T>::reverse_iterator list<T>::rbegin()
    { return typename list<T>::reverse_iterator(end()); }

    template <class T>
    inline typename list<T>::const_reverse_iterator list<T>::rbegin() const
    { return typename list<T>::const_reverse_iterator(end()); }

    template <class T>
    inline typename list<T>::const_reverse_iterator list<T>::crbegin()
    { return typename list<T>::const_reverse_iterator(cend()); }

    template <class T>
    inline typename list<T>::const_reverse_iterator list<T>::crbegin() const
    { return typename list<T>::const_reverse_iterator(cend()); }

    template <class T>
    inline typename list<T>::iterator list<T>::end()
    { return {_past_last, _direction, 0}; }

    template <class T>
    inline typename list<T>::const_iterator list<T>::end() const
    { return {_past_last, _direction, 1}; }

    template <class T>
    inline typename list<T>::const_iterator list<T>::cend()
    { return end(); }

    template <class T>
    inline typename list<T>::const_iterator list<T>::cend() const
    { return end(); }

    template <class T>
    inline typename list<T>::reverse_iterator list<T>::rend()
    { return begin(); }

    template <class T>
    inline typename list<T>::const_reverse_iterator list<T>::rend() const
    { return typename list<T>::const_reverse_iterator(begin()); }

    template <class T>
    inline typename list<T>::const_reverse_iterator list<T>::crend()
    { return typename list<T>::const_reverse_iterator(cbegin()); }

    template <class T>
    inline typename list<T>::const_reverse_iterator list<T>::crend() const
    { return typename list<T>::const_reverse_iterator(cbegin()); }

    template <class T>
    inline typename list<T>::reference list<T>::front()
    { return *begin(); }

    template <class T>
    inline typename list<T>::const_reference list<T>::front() const
    { return *cbegin(); }

    template <class T>
    inline typename list<T>::reference list<T>::back()
    { return *(prev(end())); }

    template <class T>
    inline typename list<T>::const_reference list<T>::back() const
    { return *(prev(cend())); }

    template <class T>
    inline typename list<T>::size_type list<T>::size() const
    { return std::distance(cbegin(), cend()); }

    template <class T>
    inline bool list<T>::empty() const
    { return cbegin() == cend(); }

    template <class T>
    template <typename... Args>
    inline typename list<T>::iterator list<T>::emplace
        (const const_iterator &pos, Args &&...args)
    {
        list<T>::iterator new_node(make_node(nullptr, nullptr, args...), pos._direction, 0);

        link(prev(pos), new_node);
        link(new_node, pos);

        return new_node;
    }

    template <class T>
    inline typename list<T>::iterator list<T>::insert
        (const list<T>::const_iterator &pos, const T &value)
    { return emplace(pos, value); }

    template <class T>
    inline typename list<T>::iterator list<T>::insert
        (const list<T>::const_iterator &pos, const list<T> &other)
    { return insert(pos, other.begin(), other.end()); }

    template <class T>
    inline typename list<T>::iterator list<T>::insert
        (const const_iterator &pos, const std::initializer_list<value_type> &il)
    { return insert(pos, il.begin(), il.end()); }

    template <class T>
    template <typename _InputIt>
    inline typename list<T>::iterator list<T>::insert
        (const const_iterator &pos, const _InputIt &first, const _InputIt &second)
    {
        const_iterator itl = pos;
        _InputIt input_it = first;

        while (input_it != second)
            insert((++itl), *(input_it++));

        return itl;
    }

    template <class T>
    inline typename list<T>::iterator list<T>::merge
        (const const_iterator &pos, list &other)
    {
        auto copy = prev(pos);
        if (!other.empty())
        {
            link(prev(pos), other.begin());
            link(prev(other.end()), pos);
            link(other.before_begin(), other.end());
        }

        return next(copy);
    }

    template <class T>
    inline typename list<T>::iterator list<T>::merge_back(list &other)
    { return merge(cend(), other); }

    template <class T>
    inline typename list<T>::iterator list<T>::merge_front(list &other)
    { return merge(cbegin(), other); }

    template <class T>
    inline typename list<T>::iterator list<T>::erase(const const_iterator &pos)
    {
        auto copy = next(pos);
        link(prev(pos), next(pos));
        destroy_node(pos._current);
        return copy;
    }

    template <class T>
    inline typename list<T>::iterator list<T>::pull_out(const const_iterator &pos)
    {
        auto copy = link(prev(pos), next(pos));
        pos._current->_next[0] = nullptr;
        pos._current->_next[1] = nullptr;
        return copy;
    }

    template <class T>
    template <class to_push>
    inline typename list<T>::iterator list<T>::push_back(const to_push &value)
    { return insert(cend(), value); }

    template <class T>
    template <class to_push>
    inline typename list<T>::iterator list<T>::push_front(const to_push &value)
    { return insert(cbegin(), value); }

    template <class T>
    template <typename... Args>
    inline typename list<T>::iterator list<T>::emplace_front(Args &&...args)
    { return emplace(cbegin(), args...); }

    template <class T>
    template <typename... Args>
    inline typename list<T>::iterator list<T>::emplace_back(Args &&...args)
    { return emplace(cend(), args...); }

    template <class T>
    inline typename list<T>::value_type list<T>::pop_back()
    {
        auto copy = back();
        erase(prev(cend()));
        return copy;
    }

    template <class T>
    inline typename list<T>::value_type list<T>::pop_front()
    {
        auto copy = front();
        erase(cbegin());
        return copy;
    }

    template <class T>
    inline void list<T>::reverse()
    {
        _direction ^= 1;
        std::swap(_before_first, _past_last);
    }

    template <class T>
    inline list<T>::~list()
    {
        clear();
        destroy_node(_before_first);
        destroy_node(_past_last);
    }

    template <class T>
    inline void list<T>::clear()
    {
        while (!empty())
            pop_back();
    }
}
