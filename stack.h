//
// Created by haykm on 16-Jul-22.
//
#ifndef __STACK_H__
#define __STACK_H__

#include <cstddef>
#include <algorithm>
#include <cassert>
#include <stdexcept>

namespace own
{
    template<typename T>
    void destroy(T* p_ptr) {
        p_ptr->~T();
    }

    template<typename FwdIt>
    void destroy(FwdIt begin, FwdIt end) {
        while(begin != end) {
            destroy(begin);
        }
        ++begin;
    }

    template<typename T1, typename T2>
    void construct(T1* p_ptr, const T2& obj) {
        new (p_ptr) T1{obj};
    }


    template<typename T>
    class StackImpl {
    protected:
        StackImpl(std::size_t capacity = 0) : m_ptr {static_cast<T*>
                                                     ( !capacity ?
                                                       nullptr :
                                                       operator new(sizeof(T) * capacity))},
                                              m_capacity{capacity},
                                              m_size{0}
        {}

        ~StackImpl() {
            own::destroy(m_ptr, m_ptr + m_capacity);
            operator delete(m_ptr);
        }

        void swap(StackImpl<T>& other) throw() {
            std::swap(m_ptr, other.m_ptr);
            std::swap(m_capacity, other.m_capacity);
            std::swap(m_size, other.m_size);
        }

    protected:
        T* m_ptr;
        std::size_t m_capacity;
        std::size_t m_size;

    private:
        StackImpl(const StackImpl<T>&) = delete;
        StackImpl& operator=(const StackImpl<T>&) = delete;
    };

    template<typename T>
    class Stack : private StackImpl<T> {
    public:
        Stack(std::size_t size = 0) : StackImpl<T>(size)
        {}

        ~Stack()
        {}

        Stack(const Stack<T>& rhs) : StackImpl<T>(rhs.m_size) {
            while(this->m_size < rhs.m_size) {
                own::construct(this->m_ptr + this->m_size, rhs.m_ptr[this->m_size]);
                ++(this->m_size);
            }
        }

        Stack<T>& operator=(Stack<T> rhs) {
            swap(rhs);
            return *this;
        }

        constexpr size_t size() const noexcept {
            return this->m_size;
        }

        constexpr bool empty() const noexcept {
            return this->m_size == 0;
        }

        void push(const T& val) {
            if(full()) {
                Stack<T> tmpStack(2*this->m_size + 1);
                while(tmpStack.size() < this->m_size) {
                    tmpStack.push(this->m_ptr[tmpStack.size()]);
                }
                tmpStack.push(val);
                swap(tmpStack);
            }
            else {
                construct(this->m_ptr[this->m_size++], val);
            }
        }

        T& top() {
            if(empty()) {
                throw std::logic_error("Empty stack!");
            }
            return this->m_ptr[this->m_size - 1];
        }

        const T& top() const {
            if(empty()) {
                throw std::logic_error("Empty stack!");
            }
            return this->m_ptr[this->m_size - 1];
        }
    private:
        constexpr bool full() const noexcept {
            return this->m_size == this->m_capacity;
        }
    };
}

#endif //__STACK_H__
