#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <assert.h>

template <typename T>
struct heap_interface {
    virtual void add(T elem) = 0;
    virtual T pop() = 0; // removes the top element from the heap and returns it
    virtual T top() const = 0; // returns the top element
    virtual size_t size() const = 0; // current size, i.e. number of elements in the heap
};

template <typename T>
class min_heap : public heap_interface<T> {
public:
    min_heap() {}
    ~min_heap() {}
    void add(T elem)
    {
        pq_.push(elem);
    }
    T pop()
    {
        T elem = top();
        pq_.pop();
        return elem;
    }
    T top() const
    {
        return pq_.top();
    }
    size_t size() const
    {
        return pq_.size();
    }
private:
    std::priority_queue<T, std::vector<T>, std::greater<T> > pq_;
};


/* My quick and dirty implementation of a min-heap. O(1) for push() but O(n) for pop() */
template <typename T>
class min_heap2 : public heap_interface<T> {
public:
    min_heap2() : min_idx_(-1) {}
    ~min_heap2() {}
    void add(T elem)
    {
        elements_.push_back(elem);
        if (min_idx_ == -1) {
            min_idx_ = 0;
        } else {
            if (elements_[min_idx_] > elem) {
                min_idx_ = (int)elements_.size()-1;
            }
        }
    }

    T pop() 
    { 
        T ret_elem = top();
        elements_.erase(elements_.begin()+min_idx_);
        min_idx_ = -1;
        if (elements_.size() == 0) {
            return ret_elem;
        }
        typename std::vector<T>::iterator it = elements_.begin();
        T minval = elements_[0];
        min_idx_ = 0;
        for (int i=0; it != elements_.end(); it++, i++) {
            if (minval > (*it)) {
                minval = (*it);
                min_idx_ = i;
            }
        }
        return ret_elem;
    }
    T top() const 
    { 
        return elements_[min_idx_]; 
    }
    size_t size() const { return elements_.size(); }
protected:
    std::vector<T> elements_;
    int min_idx_;
};

template <typename T>
class max_heap : public heap_interface<T> {
public:
    max_heap(){} //constructor
    ~max_heap(){} //destructor
    void add(T elem){ inverse_heap_.add(-elem); }
    T pop() { T tmp = inverse_heap_.pop(); return (-tmp); } 
    T top() const { T tmp = inverse_heap_.top(); return (-tmp); }
    size_t size() const { return inverse_heap_.size(); }
protected:
    min_heap<T> inverse_heap_;
};

class median_maintainer {
public:
    median_maintainer();
    ~median_maintainer();
    void add(int elem);
    int get_median() const;
    size_t size() const { return curr_size_; }

protected:
    max_heap<long> left_;
    min_heap<long> right_;
    int top_;
    size_t curr_size_;
};

median_maintainer::median_maintainer()
    : top_(0), curr_size_(0)
{}

median_maintainer::~median_maintainer()
{}

void median_maintainer::add(int elem)
{
    if (curr_size_ == 0) {
        top_ = elem;
        curr_size_ ++;
        return;
    }
    if (left_.size() >= right_.size()) {
        // insertion has to be in the right 
        if (elem > top_) {
            right_.add(elem);
        } else {
            right_.add(top_);
            left_.add(elem);
            top_ = left_.pop();
        }
        size_t diff_height = right_.size() - left_.size();
        assert(diff_height == 0 || diff_height == 1);
    } else {
        // insertion has to be in the left 
        if (elem < top_) {
            left_.add(elem);
        } else {
            left_.add(top_);
            right_.add(elem);
            top_ = right_.pop();
        }
        assert(left_.size() == right_.size());
    }
    curr_size_ ++;
}

int median_maintainer::get_median() const
{
    return top_;
}

int main(int argc, char *argv[])
{
    std::ifstream infile(argv[1]);
    int a_num;
    median_maintainer medianizer;
    max_heap<int> heap;
    int answer = 0;
    while (infile >> a_num) {
        medianizer.add(a_num);
        //std::cout << "Added " << a_num << ". Current top is " << heap.top() << std::endl;
        std::cout << "Added " << a_num << ". Current median is " << medianizer.get_median() << std::endl;
        answer += medianizer.get_median();
    }
    while (heap.size() > 0) {
        std::cout << "top returned " << heap.top() << std::endl;
        heap.pop();
    }

    std::cout << "Number of numbers = " << medianizer.size() << std::endl;
    std::cout << "Answer: " << answer%10000 << std::endl;
    return 0;
}

