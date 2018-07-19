#include <bits/stdc++.h>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

template <typename T>
class circular_buffer {
    std::vector<T> elements_;
    ssize_t curr_head_;
public:
    circular_buffer()
        : elements_()
    {
        curr_head_ = 0;
    }
    void shift_left()
    {
        curr_head_ ++;
    }
    void push_back(const T& item)
    {
        elements_.push_back(item);
    }

    struct iterator {
        circular_buffer<T>& core_buffer_;
        ssize_t curr_position_;
        iterator(circular_buffer<T>& cb)
            : core_buffer_(cb)
        {
            curr_position_ = cb.curr_head_;
        }
        iterator(const iterator& right)
            : core_buffer_(right.core_buffer_), curr_position_(right.curr_position_)
        {}
        ~iterator() {};
        T& operator* ()
        {
            if (curr_position_ >= core_buffer_.elements_.size()) {
                assert(false);
            }
            return core_buffer_.elements_[curr_position_];
        }
        /*
        T* operator-> ()
        {
        }
        */
        const T& operator() ()
        {
            if (curr_position_ >= core_buffer_.elements_.size()) {
                assert(false);
            }
            return core_buffer_.elements_[curr_position_];
        }
        //prefix operator++: is called when caller does ++it;
        iterator& operator++ ()
        {
            curr_position_ ++;
            if (curr_position_ >= core_buffer_.elements_.size()) {
                curr_position_ = 0;
            }
            if (curr_position_ == core_buffer_.curr_head_) {
                //reached the end
                curr_position_ = -1;
            }
            return (*this);
        }
#if 1
        //postfix operator++: is called when caller does it++;
        void operator++ (int)
        {
            this->operator++();
            return;
        }
#endif
        bool operator==(const iterator& right)
        {
            return (this->curr_position_ == right.curr_position_);
        }
        bool operator!=(const iterator& right)
        {
            return !(this->operator==(right));
        }
    };

    circular_buffer<T>::iterator begin()
    {
        return circular_buffer<T>::iterator(*this); 
    }

    circular_buffer<T>::iterator end()
    {
        circular_buffer<T>::iterator it(*this);
        it.curr_position_ = -1;
        return it;
    }
    friend circular_buffer<T>::iterator;
};

int main()
{
    circular_buffer<int> myBuf;
    myBuf.push_back(10);
    myBuf.push_back(12);
    circular_buffer<int>::iterator it = myBuf.begin();
    while (it != myBuf.end()) {
        (*it) = (*it) + 20;
        ++it;
    }
    auto itt = myBuf.begin();
    while (itt != myBuf.end()) {
        cout << "Element: " << itt() << endl;
        itt++;
    }

    for (int a: myBuf) {
        cout << "Element: " << a << endl;
    }
    return 0;
}

