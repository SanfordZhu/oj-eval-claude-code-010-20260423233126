#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "exceptions.hpp"
#include "algorithm.hpp"

#include <cstddef>

namespace sjtu {
/**
 * a data container like std::list
 * allocate random memory addresses for data and they are doubly-linked in a list.
 */
template<typename T>
class list {
protected:
    class node {
    public:
        T *data;
        node *prev;
        node *next;

        node() : data(nullptr), prev(nullptr), next(nullptr) {}
        explicit node(const T &value) : data(new T(value)), prev(nullptr), next(nullptr) {}
        ~node() {
            if (data != nullptr) {
                delete data;
            }
        }
    };

protected:
    node *head;
    node *tail;
    size_t cnt;

    /**
     * insert node cur before node pos
     * return the inserted node cur
     */
    node *insert(node *pos, node *cur) {
        cur->prev = pos->prev;
        cur->next = pos;
        pos->prev->next = cur;
        pos->prev = cur;
        return cur;
    }

    /**
     * remove node pos from list (no need to delete the node)
     * return the removed node pos
     */
    node *erase(node *pos) {
        pos->prev->next = pos->next;
        pos->next->prev = pos->prev;
        return pos;
    }

public:
    class const_iterator;
    class iterator {
    private:
        node *ptr;
        const list *container;

    public:
        iterator() : ptr(nullptr), container(nullptr) {}
        iterator(node *p, const list *c) : ptr(p), container(c) {}

        /**
         * iter++
         */
        iterator operator++(int) {
            if (ptr == nullptr || ptr->next == nullptr) {
                throw invalid_iterator();
            }
            iterator tmp = *this;
            ptr = ptr->next;
            return tmp;
        }

        /**
         * ++iter
         */
        iterator & operator++() {
            if (ptr == nullptr || ptr->next == nullptr) {
                throw invalid_iterator();
            }
            ptr = ptr->next;
            return *this;
        }

        /**
         * iter--
         */
        iterator operator--(int) {
            if (ptr == nullptr || ptr->prev == nullptr || ptr->prev == container->head) {
                throw invalid_iterator();
            }
            iterator tmp = *this;
            ptr = ptr->prev;
            return tmp;
        }

        /**
         * --iter
         */
        iterator & operator--() {
            if (ptr == nullptr || ptr->prev == nullptr || ptr->prev == container->head) {
                throw invalid_iterator();
            }
            ptr = ptr->prev;
            return *this;
        }

        /**
         * *it
         * remember to throw if iterator is invalid
         */
        T & operator *() const {
            if (ptr == nullptr || ptr->data == nullptr) {
                throw invalid_iterator();
            }
            return *(ptr->data);
        }

        /**
         * it->field
         * remember to throw if iterator is invalid
         */
        T * operator ->() const {
            if (ptr == nullptr || ptr->data == nullptr) {
                throw invalid_iterator();
            }
            return ptr->data;
        }

        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }
        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }

        friend class list;
        friend class const_iterator;
    };

    /**
     * has same function as iterator, just for a const object.
     * should be able to construct from an iterator.
     */
    class const_iterator {
    private:
        node *ptr;
        const list *container;

    public:
        const_iterator() : ptr(nullptr), container(nullptr) {}
        const_iterator(node *p, const list *c) : ptr(p), container(c) {}
        const_iterator(const iterator &it) : ptr(it.ptr), container(it.container) {}

        const_iterator operator++(int) {
            if (ptr == nullptr || ptr->next == nullptr) {
                throw invalid_iterator();
            }
            const_iterator tmp = *this;
            ptr = ptr->next;
            return tmp;
        }

        const_iterator & operator++() {
            if (ptr == nullptr || ptr->next == nullptr) {
                throw invalid_iterator();
            }
            ptr = ptr->next;
            return *this;
        }

        const_iterator operator--(int) {
            if (ptr == nullptr || ptr->prev == nullptr || ptr->prev == container->head) {
                throw invalid_iterator();
            }
            const_iterator tmp = *this;
            ptr = ptr->prev;
            return tmp;
        }

        const_iterator & operator--() {
            if (ptr == nullptr || ptr->prev == nullptr || ptr->prev == container->head) {
                throw invalid_iterator();
            }
            ptr = ptr->prev;
            return *this;
        }

        const T & operator *() const {
            if (ptr == nullptr || ptr->data == nullptr) {
                throw invalid_iterator();
            }
            return *(ptr->data);
        }

        const T * operator ->() const {
            if (ptr == nullptr || ptr->data == nullptr) {
                throw invalid_iterator();
            }
            return ptr->data;
        }

        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }
        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }

        friend class list;
        friend class iterator;
    };

    /**
     * Constructs
     * Atleast two: default constructor, copy constructor
     */
    list() {
        head = new node();
        tail = new node();
        head->next = tail;
        tail->prev = head;
        cnt = 0;
    }

    list(const list &other) {
        head = new node();
        tail = new node();
        head->next = tail;
        tail->prev = head;
        cnt = 0;
        for (node *p = other.head->next; p != other.tail; p = p->next) {
            insert(tail, new node(*(p->data)));
            cnt++;
        }
    }

    /**
     * Destructor
     */
    virtual ~list() {
        clear();
        delete head;
        delete tail;
    }

    /**
     * Assignment operator
     */
    list &operator=(const list &other) {
        if (this == &other) {
            return *this;
        }
        clear();
        for (node *p = other.head->next; p != other.tail; p = p->next) {
            insert(tail, new node(*(p->data)));
            cnt++;
        }
        return *this;
    }

    /**
     * access the first / last element
     * throw container_is_empty when the container is empty.
     */
    const T & front() const {
        if (empty()) {
            throw container_is_empty();
        }
        return *(head->next->data);
    }

    const T & back() const {
        if (empty()) {
            throw container_is_empty();
        }
        return *(tail->prev->data);
    }

    /**
     * returns an iterator to the beginning.
     */
    iterator begin() {
        return iterator(head->next, this);
    }

    const_iterator cbegin() const {
        return const_iterator(head->next, this);
    }

    /**
     * returns an iterator to the end.
     */
    iterator end() {
        return iterator(tail, this);
    }

    const_iterator cend() const {
        return const_iterator(tail, this);
    }

    /**
     * checks whether the container is empty.
     */
    virtual bool empty() const {
        return cnt == 0;
    }

    /**
     * returns the number of elements
     */
    virtual size_t size() const {
        return cnt;
    }

    /**
     * clears the contents
     */
    virtual void clear() {
        node *p = head->next;
        while (p != tail) {
            node *tmp = p;
            p = p->next;
            erase(tmp);
            delete tmp;
        }
        head->next = tail;
        tail->prev = head;
        cnt = 0;
    }

    /**
     * insert value before pos (pos may be the end() iterator)
     * return an iterator pointing to the inserted value
     * throw if the iterator is invalid
     */
    virtual iterator insert(iterator pos, const T &value) {
        if (pos.container != this) {
            throw invalid_iterator();
        }
        node *new_node = new node(value);
        insert(pos.ptr, new_node);
        cnt++;
        return iterator(new_node, this);
    }

    /**
     * remove the element at pos (the end() iterator is invalid)
     * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
     * throw if the container is empty, the iterator is invalid
     */
    virtual iterator erase(iterator pos) {
        if (empty() || pos.container != this || pos.ptr == head || pos.ptr == tail || pos.ptr == nullptr) {
            throw invalid_iterator();
        }
        node *next_node = pos.ptr->next;
        erase(pos.ptr);
        delete pos.ptr;
        cnt--;
        return iterator(next_node, this);
    }

    /**
     * adds an element to the end
     */
    void push_back(const T &value) {
        insert(end(), value);
    }

    /**
     * removes the last element
     * throw when the container is empty.
     */
    void pop_back() {
        if (empty()) {
            throw container_is_empty();
        }
        erase(iterator(tail->prev, this));
    }

    /**
     * inserts an element to the beginning.
     */
    void push_front(const T &value) {
        insert(begin(), value);
    }

    /**
     * removes the first element.
     * throw when the container is empty.
     */
    void pop_front() {
        if (empty()) {
            throw container_is_empty();
        }
        erase(begin());
    }

    /**
     * sort the values in ascending order with operator< of T
     */
    void sort() {
        if (size() <= 1) {
            return;
        }
        T **arr = new T*[size()];
        node *p = head->next;
        for (size_t i = 0; i < size(); i++) {
            arr[i] = p->data;
            p = p->next;
        }
        sjtu::sort<T*>(arr, arr + size(), [](T* a, T* b) { return *a < *b; });
        p = head->next;
        for (size_t i = 0; i < size(); i++) {
            p->data = arr[i];
            p = p->next;
        }
        delete[] arr;
    }

    /**
     * merge two sorted lists into one (both in ascending order)
     * compare with operator< of T
     * container other becomes empty after the operation
     * for equivalent elements in the two lists, the elements from *this shall always precede the elements from other
     * the order of equivalent elements of *this and other does not change.
     * no elements are copied or moved
     */
    void merge(list &other) {
        if (this == &other || other.empty()) {
            return;
        }
        node *p1 = head->next;
        node *p2 = other.head->next;
        node *curr_p1;
        node *curr_p2;

        while (p1 != tail && p2 != other.tail) {
            if (*(p2->data) < *(p1->data)) {
                curr_p2 = p2;
                p2 = p2->next;
                other.erase(curr_p2);
                insert(p1, curr_p2);
                cnt++;
                other.cnt--;
            } else {
                p1 = p1->next;
            }
        }
        while (p2 != other.tail) {
            curr_p2 = p2;
            p2 = p2->next;
            other.erase(curr_p2);
            insert(tail, curr_p2);
            cnt++;
            other.cnt--;
        }
    }

    /**
     * reverse the order of the elements
     * no elements are copied or moved
     */
    void reverse() {
        if (size() <= 1) {
            return;
        }
        node *curr = head;
        node *temp;
        while (curr != nullptr) {
            temp = curr->next;
            curr->next = curr->prev;
            curr->prev = temp;
            curr = temp;
        }
        temp = head;
        head = tail;
        tail = temp;
    }

    /**
     * remove all consecutive duplicate elements from the container
     * only the first element in each group of equal elements is left
     * use operator== of T to compare the elements.
     */
    void unique() {
        if (size() <= 1) {
            return;
        }
        node *p = head->next;
        while (p != tail && p->next != tail) {
            if (*(p->data) == *(p->next->data)) {
                node *temp = p->next;
                erase(temp);
                delete temp;
                cnt--;
            } else {
                p = p->next;
            }
        }
    }
};

}

#endif //SJTU_LIST_HPP
