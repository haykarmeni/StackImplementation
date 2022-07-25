#ifndef __STACK_H__
#define __STACK_H__

#include <cstddef>
#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <initializer_list>

namespace my
{
    template<typename T>
    void destroy(T* p_ptr) {
        p_ptr->~T();
    }

    template<typename FwdIt>
    void destroy(FwdIt begin, FwdIt end) {
        while(begin != end) {
            destroy(begin);
            ++begin;
        }
    }

    template<typename T1, typename T2>
    void construct(T1* p_ptr, const T2& obj) {
        new (p_ptr) T1{obj};
    }

    template<typename T1, typename T2>
    void construct(T1* p_ptr, T2&& obj) {
        new (p_ptr) T1{std::move(obj)};
    }


    template<typename T>
    class StackImpl {
    public:
        using value_type         = T;
        using reference          = T&;
        using const_reference    = const T&;
        using rval_reference     = T&&;
        using pointer_type       = T*;
        using const_pointer_type = const T*;
        using size_type          = std::size_t;
    public:
        StackImpl(size_type capacity = 0) : m_ptr {static_cast<pointer_type>
                                                     ( !capacity ?
                                                       nullptr :
                                                       operator new(sizeof(value_type) * capacity))},
                                            m_capacity{capacity},
                                            m_size{0}
        {}

        ~StackImpl() {
            my::destroy(m_ptr, m_ptr + m_size);
            operator delete(m_ptr);
        }

        void swap(StackImpl<value_type>& other) throw() {
            std::swap(m_ptr, other.m_ptr);
            std::swap(m_capacity, other.m_capacity);
            std::swap(m_size, other.m_size);
        }
    public:
        pointer_type m_ptr;
        size_type m_capacity;
        size_type m_size;

    private:
        StackImpl(const StackImpl<value_type>&) = delete;
        StackImpl& operator=(const StackImpl<value_type>&) = delete;
    };

    template<typename T>
    class Stack {
    public:
        using value_type         = typename my::StackImpl<T>::value_type;
        using reference          = typename my::StackImpl<T>::reference ;
        using const_reference    = typename my::StackImpl<T>::const_reference;
        using rval_reference     = typename my::StackImpl<T>::rval_reference;
        using pointer_type       = typename my::StackImpl<T>::pointer_type ;
        using const_pointer_type = typename my::StackImpl<T>::const_pointer_type;
        using size_type          = typename my::StackImpl<T>::size_type;
        using implementation     = my::StackImpl<T>;
    public:
        Stack(size_type capacity = 0) : m_impl(capacity)
        {}

        ~Stack()
        {}

        Stack(const Stack<value_type>& rhs) : m_impl(rhs.m_impl.m_size) {
            while(m_impl.m_size < m_impl.m_capacity) {
                my::construct(m_impl.m_ptr + m_impl.m_size, rhs.m_impl.m_ptr[m_impl.m_size]);
                ++m_impl.m_size;
            }
        }

        Stack(Stack<value_type>&& rhs) : m_impl(rhs.m_impl.m_size) {
            pointer_type tmp = m_impl.m_ptr;
            m_impl.m_ptr = std::move(rhs.m_impl.m_ptr);
            rhs.m_impl.m_ptr = nullptr;
            m_impl.m_size = rhs.m_impl.m_size;
            destroy(tmp, tmp + m_impl.m_size);
        }

        explicit Stack(const std::initializer_list<value_type>& rhs) : m_impl(rhs.size()) {
            while(m_impl.m_size < m_impl.m_capacity) {
                my::construct(m_impl.m_ptr + m_impl.m_size, *(rhs.begin() + m_impl.m_size));
                ++m_impl.m_size;
            }
        }

        Stack<value_type>& operator=(const Stack<value_type>& rhs) {
            Stack<value_type> tmp{ rhs };
            m_impl.swap(tmp.m_impl);
            return *this;
        }

        Stack<value_type>& operator=(Stack<value_type>&& rhs) {
            destroy(m_impl.m_ptr, m_impl.m_ptr + m_impl.m_size);
            m_impl.m_ptr = std::move(rhs.m_impl.m_ptr);
            rhs.m_impl.m_ptr = nullptr;
            m_impl.m_capacity =  rhs.m_impl.m_capacity;
            m_impl.m_size =  rhs.m_impl.m_size;
            return *this;
        }

        [[nodiscard]] constexpr size_type size() const noexcept {
            return m_impl.m_size;
        }

        [[nodiscard]] constexpr size_type capacity() const noexcept {
            return m_impl.m_capacity;
        }

        [[nodiscard]] constexpr bool empty() const noexcept {
            return m_impl.m_size == 0;
        }

        void push(const_reference val) {
            if(full()) {
                Stack<value_type> tmpStack(2 * m_impl.m_size + 1);
                while(tmpStack.size() < m_impl.m_size) {
                    tmpStack.push(m_impl.m_ptr[tmpStack.size()]);
                }
                tmpStack.push(val);
                m_impl.swap(tmpStack.m_impl);
            }
            else {
                construct(m_impl.m_ptr + m_impl.m_size++, val);
            }
        }

        void push(rval_reference val) {
            if(full()) {
                Stack<value_type> tmpStack(2 * m_impl.m_size + 1);
                while(tmpStack.size() < m_impl.m_size) {
                    tmpStack.push(m_impl.m_ptr[tmpStack.size()]);
                }
                tmpStack.push(std::move(val));
                m_impl.swap(tmpStack.m_impl);
            }
            else {
                construct(m_impl.m_ptr + m_impl.m_size++, std::move(val));
            }
        }

        [[nodiscard]] T& top() {
            if(empty()) {
                throw std::logic_error("Empty stack!");
            }
            return m_impl.m_ptr[m_impl.m_size - 1];
        }

        [[nodiscard]] const T& top() const {
            if(empty()) {
                throw std::logic_error("Empty stack!");
            }
            return m_impl.m_ptr[m_impl.m_size - 1];
        }

        void pop() {
            if(empty()) {
                throw std::underflow_error("Pop from empty stack!");
            }
            --m_impl.m_size;
            destroy(m_impl.m_ptr + m_impl.m_size);
        }
    private:
        constexpr bool full() const noexcept {
            return m_impl.m_size == m_impl.m_capacity;
        }
    private:
        implementation m_impl;
    };
}

#endif //__STACK_H__
